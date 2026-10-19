#include "Interceptor.h"
#include <iostream>
#include <system_error>
#include <winsvc.h>

int wmain() {
    using namespace Interceptor;
    try {
        // A read-only startup check. Installation is always an explicit action.
        const auto manager=OpenSCManagerW(nullptr,nullptr,SC_MANAGER_CONNECT);
        if(!manager)throw std::system_error(GetLastError(),std::system_category());
        const auto service=OpenServiceW(manager,L"InterceptorGamepadController",SERVICE_QUERY_STATUS);
        const DWORD error=service?ERROR_SUCCESS:GetLastError();
        if(service)CloseServiceHandle(service);
        CloseServiceHandle(manager);
        if(error==ERROR_SERVICE_DOES_NOT_EXIST) {
            std::cerr<<"Interceptor Gamepad / Controller is not installed.\n"
                     <<"Run InterceptorSetup.exe or InterceptorSetupConsole.exe /install first.\n";
            return static_cast<int>(error);
        }
        if(error)throw std::system_error(error,std::system_category());
        const auto init=Init_AllDevices();
        if(!init.ready) {
            std::cerr<<"Initialization: error="<<init.error<<", restart="<<init.reboot_required<<'\n';
            return static_cast<int>(init.error?init.error:ERROR_SUCCESS_REBOOT_REQUIRED);
        }
        std::cout<<"HID, XUSB, XInputHID and GIP input is forwarded unchanged. Ctrl+C to stop.\n";
        ULONGLONG nextPrint=0;
        for(;;) {
            // The library owns the context, decodes the right family and keeps
            // each event's originating device identity for Send_Input.
            auto inputs=Get_Input_With_Timeout(INFINITE);
            for(auto& event:inputs) {
                if(auto* input=event.Get<Controller_Hid_Input>()) {
                    // Each HID control retains its usage and report identity.
                    // To change an axis: input->state.axes[n].normalized = ...;
                    (void)input;
                } else if(auto* xusb=event.Get<Controller_Xinput_Input>()) {
                    // Example field: xusb->state.left_x.
                    (void)xusb;
                } else if(auto* xih=event.Get<Controller_XinputHid_Input>()) {
                    // Descriptor-decoded controls: xih->state.axes/buttons.
                    (void)xih;
                } else if(auto* gip=event.Get<Controller_GIP_Input>()) {
                    // Example field: gip->state.left_x.
                    (void)gip;
                }
            }
            const auto result=Send_Input(inputs);
            if(!result)throw std::system_error(result.error,std::system_category(),"Send_Input");
            // Logging follows forwarding and is limited to four updates/second.
            if(GetTickCount64()>=nextPrint) {
                nextPrint=GetTickCount64()+250;
                std::cout<<inputs.size()<<" event(s), "<<result.accepted<<" forwarded\n";
            }
        }
    } catch(const std::exception& e) {
        std::cerr<<e.what()<<"\nCapture closed; physical controllers work normally.\n";
        return 1;
    }
}
