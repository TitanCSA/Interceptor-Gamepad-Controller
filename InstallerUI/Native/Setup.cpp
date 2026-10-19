#define IC_SDK_INTERNAL 1
#include "Interceptor.h"
namespace Interceptor {Init_Result Restore_Controller_Hid_Filters();}
#include <SetupAPI.h>
#include <newdev.h>
#include <cfgmgr32.h>
#include <initguid.h>
#include <devguid.h>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <system_error>
#include "SetupBinding.h"

namespace {
void Check(BOOL ok,const char* operation) { if(!ok)throw std::system_error(static_cast<int>(GetLastError()),std::system_category(),operation); }
struct Devices { HDEVINFO value;~Devices(){if(value!=INVALID_HANDLE_VALUE)SetupDiDestroyDeviceInfoList(value);} };
constexpr wchar_t ControlHardware[]=L"ROOT\\InterceptorGamepadController";
constexpr wchar_t ControlInstance[]=L"ROOT\\InterceptorGamepadController\\0000";
bool IsControl(HDEVINFO set,SP_DEVINFO_DATA& d) {
    wchar_t id[MAX_DEVICE_ID_LEN]{},hardware[4096]{};DWORD type=0;
    Check(SetupDiGetDeviceInstanceIdW(set,&d,id,MAX_DEVICE_ID_LEN,nullptr),"Read control instance");
    if(_wcsnicmp(id,L"ROOT\\InterceptorGamepadController\\",_countof(ControlHardware)))return false;
    if(!SetupDiGetDeviceRegistryPropertyW(set,&d,SPDRP_HARDWAREID,&type,reinterpret_cast<BYTE*>(hardware),sizeof(hardware)-2*sizeof(wchar_t),nullptr)) {
        if(GetLastError()==ERROR_INVALID_DATA)return false;Check(FALSE,"Read control hardware ID");
    }
    if(type!=REG_MULTI_SZ)return false;
    for(const auto* p=hardware;*p;p+=wcslen(p)+1)if(!_wcsicmp(p,ControlHardware))return true;
    return false;
}
bool RemoveControlDevice(HDEVINFO set,SP_DEVINFO_DATA& d) {
    SP_REMOVEDEVICE_PARAMS remove{};remove.ClassInstallHeader.cbSize=sizeof(SP_CLASSINSTALL_HEADER);
    remove.ClassInstallHeader.InstallFunction=DIF_REMOVE;remove.Scope=DI_REMOVEDEVICE_GLOBAL;
    Check(SetupDiSetClassInstallParamsW(set,&d,&remove.ClassInstallHeader,sizeof(remove)),"Prepare root control removal");
    Check(SetupDiCallClassInstaller(DIF_REMOVE,set,&d),"Remove root control device");
    SP_DEVINSTALL_PARAMS_W params{};params.cbSize=sizeof(params);
    Check(SetupDiGetDeviceInstallParamsW(set,&d,&params),"Query root removal restart");
    return (params.Flags&(DI_NEEDREBOOT|DI_NEEDRESTART))!=0;
}
bool ConfigureControl(const wchar_t* inf,bool remove) {
    Devices scan{SetupDiGetClassDevsW(&GUID_DEVCLASS_SYSTEM,nullptr,nullptr,0)};
    Check(scan.value!=INVALID_HANDLE_VALUE,"Enumerate root control devices");
    std::vector<std::wstring> ids;
    for(DWORD index=0;;++index) {
        SP_DEVINFO_DATA d{};d.cbSize=sizeof(d);
        if(!SetupDiEnumDeviceInfo(scan.value,index,&d)){if(GetLastError()==ERROR_NO_MORE_ITEMS)break;Check(FALSE,"Enumerate root control device");}
        if(!IsControl(scan.value,d))continue;
        wchar_t id[MAX_DEVICE_ID_LEN]{};Check(SetupDiGetDeviceInstanceIdW(scan.value,&d,id,MAX_DEVICE_ID_LEN,nullptr),"Read root control ID");ids.emplace_back(id);
    }
    Devices set{SetupDiCreateDeviceInfoList(&GUID_DEVCLASS_SYSTEM,nullptr)};
    Check(set.value!=INVALID_HANDLE_VALUE,"Create root control information set");
    bool reboot=false;
    if(remove) {
        for(const auto& id:ids) {
            SP_DEVINFO_DATA d{};d.cbSize=sizeof(d);Check(SetupDiOpenDeviceInfoW(set.value,id.c_str(),nullptr,0,&d),"Open root control device");
            reboot|=RemoveControlDevice(set.value,d);
        }
        std::cout<<ids.size()<<" root control devices removed\n";return reboot;
    }
    SP_DEVINFO_DATA created{};created.cbSize=sizeof(created);bool registered=false;
    try {
        if(ids.empty()) {
            // Stable instance ID makes repeated installation idempotent. Unlike
            // DICD_GENERATE_ID, this cannot create a new device on every click.
            Check(SetupDiCreateDeviceInfoW(set.value,ControlInstance,&GUID_DEVCLASS_SYSTEM,L"Interceptor Control",nullptr,0,&created),"Create root control device");
            constexpr wchar_t hardware[]=L"ROOT\\InterceptorGamepadController\0";
            Check(SetupDiSetDeviceRegistryPropertyW(set.value,&created,SPDRP_HARDWAREID,reinterpret_cast<const BYTE*>(hardware),sizeof(hardware)),"Set root control hardware ID");
            Check(SetupDiCallClassInstaller(DIF_REGISTERDEVICE,set.value,&created),"Register root control device");registered=true;
        }
        BOOL needsRestart=FALSE;
        Check(UpdateDriverForPlugAndPlayDevicesW(nullptr,ControlHardware,inf,INSTALLFLAG_FORCE|INSTALLFLAG_NONINTERACTIVE,&needsRestart),"Bind root control driver");
        reboot=needsRestart!=FALSE;
    } catch(...) {
        if(registered)try {RemoveControlDevice(set.value,created);}catch(const std::exception& e){std::cerr<<"Root rollback failed: "<<e.what()<<'\n';}
        throw;
    }
    std::cout<<"Root control configured; reboot="<<reboot<<'\n';return reboot;
}
}
int ConfigureControlDriver(const wchar_t* inf,bool remove) {return ConfigureControl(inf,remove)?ERROR_SUCCESS_REBOOT_REQUIRED:0;}
int InstallDriver(const wchar_t* directory,bool restore,bool onlyNew,const wchar_t*,const wchar_t* controlInf) {
    bool needsReboot=false;
    if(!restore && (!onlyNew || controlInf)) {
        const auto path=onlyNew?std::wstring(controlInf):std::wstring(directory)+L"\\InterceptorGamepadControllerControl.inf";
        needsReboot=ConfigureControl(path.c_str(),false);
    }
    if(restore) {
        const auto hid=Interceptor::Restore_Controller_Hid_Filters();
        if(hid.error)throw std::system_error(hid.error,std::system_category(),"Remove HID controller filters");
        needsReboot|=hid.reboot_required;
    }
    if(restore)needsReboot|=ConfigureControl(nullptr,true);
    return needsReboot?3010:0;
}
