#define IC_SDK_INTERNAL 1
#include "Interceptor.h"
#include <iostream>
#include <string>
#include <system_error>
#include <atomic>

int InstallDriver(const wchar_t* directory,bool restore,bool onlyNew=false,const wchar_t* mouseInf=nullptr,const wchar_t* controlInf=nullptr);
int ConfigureControlDriver(const wchar_t* inf,bool remove);
namespace {
bool UsbXusbBackendLoaded(const IC_CAPABILITIES& capabilities) {
    return (capabilities.controller_mask&IC_CONTROLLER_XUSB)!=0 &&
        (capabilities.xusb_profiles&4u)!=0 && (capabilities.xusb_profiles&3u)!=0;
}
int SetupPolicySelfTest() {
    struct Case {unsigned controllers,profiles;bool expected;};
    constexpr Case cases[]={{2,3,false},{2,7,true},{2,6,true},{0,7,false},{2,4,false},{2,0,false},{15,7,true}};
    for(const auto& test:cases) {
        IC_CAPABILITIES capabilities{};capabilities.controller_mask=test.controllers;capabilities.xusb_profiles=test.profiles;
        if(UsbXusbBackendLoaded(capabilities)!=test.expected)throw std::runtime_error("Installer USB backend policy test failed");
    }
    std::cout<<"Installer backend policy: 7 cases passed; no driver opened or installation performed.\n";
    return 0;
}
}
int wmain(int argc,wchar_t** argv) {
    using namespace Interceptor;
    try {
        std::wstring mode=argc>1?argv[1]:L"--help";
        if(mode==L"--self-test-setup" && argc==2)return SetupPolicySelfTest();
        if(mode==L"--require-usb-backend" && argc==2) {
            try {
                if(UsbXusbBackendLoaded(Get_Capabilities()))return 0;
            }catch(const std::system_error& error) {
                if(error.code().value()!=ERROR_INVALID_FUNCTION && error.code().value()!=ERROR_NOT_SUPPORTED &&
                    error.code().value()!=ERROR_REVISION_MISMATCH)throw;
            }
            std::cout<<"The installed package requires the USB XUSB backend, but the loaded driver does not advertise it. "
                "Restart Windows and reopen this installer before completing associations.\n";
            return ERROR_SUCCESS_REBOOT_REQUIRED;
        }
        if(mode==L"--associate-controllers"){const auto r=Init_AllDevices();return r.error?static_cast<int>(r.error):r.ready?0:ERROR_SUCCESS_REBOOT_REQUIRED;}
        if(mode==L"--install-control" && argc==3)return ConfigureControlDriver(argv[2],false);
        if(mode==L"--remove-control" && argc==2)return ConfigureControlDriver(nullptr,true);
        if(mode==L"--install" && argc==3)return InstallDriver(argv[2],false);
        if(mode==L"--associate" && (argc==4 || argc==5))return InstallDriver(argv[2],false,true,argv[3],argc==5?argv[4]:nullptr);
        if(mode==L"--init-all") {
            const bool quiet=argc==3 && std::wstring(argv[2])==L"--quiet";
            if(argc!=2 && !quiet)throw std::invalid_argument("Expected --init-all [--quiet]");
            const auto r=Init_AllDevices();
            std::cout<<"Associated="<<r.associated<<"; already associated="<<r.already_associated<<"; skipped="<<r.skipped
                <<"; restart required="<<r.reboot_required<<"; ready="<<r.ready<<"; error="<<r.error<<'\n';
            return r.error?static_cast<int>(r.error):r.ready?0:ERROR_SUCCESS_REBOOT_REQUIRED;
        }
        if(mode==L"--restore")return InstallDriver(nullptr,true);
        if(mode==L"--status") {
            auto s=Get_Status();
            std::cout<<"ABI "<<s.version<<"; keyboards="<<s.keyboard_devices<<"; mouse="<<s.mouse_devices
                     <<"; HID readers="<<s.controller_hid_devices<<"; HID queued="<<s.controller_hid_count
                     <<"; overflow resets="<<s.overflows<<"; last reset="<<s.last_reset_reason<<"\n";
            try {
                const auto c=Get_Capabilities();std::cout<<"Capabilities v"<<c.version<<"; controller mask="<<c.controller_mask
                    <<"; XUSB profiles="<<c.xusb_profiles<<"; XInputHID profiles="<<c.xinputhid_profiles<<"; GIP profiles="<<c.gip_profiles
                    <<" (compiled profiles, not connected/capturing device counts)\n";
            } catch(const std::system_error& e) {
                if(e.code().value()!=ERROR_INVALID_FUNCTION && e.code().value()!=ERROR_NOT_SUPPORTED)throw;
                std::cout<<"Capabilities endpoint unavailable; native association disabled\n";
            }
            return 0;
        }
        std::cout<<"Interceptor Gamepad / Controller setup helper\n"
                 <<"  --status | --init-all [--quiet]\n"
                 <<"  --install FOLDER | --restore\n"
                 <<"  --install-control CONTROL_INF | --remove-control\n"
                 <<"  --associate-controllers\n";
        return mode==L"--help"?0:ERROR_INVALID_PARAMETER;
    } catch(const std::system_error& e) {
        if(e.code().value()==ERROR_OPERATION_ABORTED) { std::cout<<"Capture revoked; normal input restored.\n";return 0; }
        std::cerr<<e.what()<<"\n";return 1;
    } catch(const std::exception& e) { std::cerr<<e.what()<<"\n";return 1; }
}
