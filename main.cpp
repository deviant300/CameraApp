//***Camera Libraries***
#include "Cameraremote_SDK\include\CRSDK\CameraRemote_SDK.h"
#include "Cameraremote_SDK\include\CameraDevice.h"
#include "Cameraremote_SDK\include\Text.h"
#include "Cameraremote_SDK\include\CRSDK\CrTypes.h"


//***General libraries***
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <string>
#include <cstdint>
#include <cstdlib>

//defining namespace
using namespace SCRSDK;

//Creating class and defining functions
class CameraHandler : public IDeviceCallback {
    public:

    CameraHandler() : camera_connected(false), camera_handle(0) {std::cout << "Constructor called..." << std::endl;}
    ~CameraHandler() {cli::tout << "Destructor called..." << std::endl;}

    void Initialize(){

        std::cout << "Attempting to initialize SDK" <<std::endl;
        auto ret = Init();
        if(!ret){
            std::cout << "Initialization failed" << std::endl;
            std::cout << "Terminating..." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }

        std::cout << "Initialization failed" << std::endl;
    }

    void getSDKversion() {
        std::cout << "RemoteSampleApp v1.12.00 running..." << std::endl << std::endl;

        CrInt32u version = GetSDKVersion();
        int major = (version & 0xFF000000) >> 24;
        int minor = (version & 0x00FF0000) >> 16;
        int patch = (version & 0x0000FF00) >> 8;
        // int reserved = (version & 0x000000FF);

        std::cout << "Remote SDK version: ";
        std::cout << major << "." << minor << "." << std::setfill(char('0')) << std::setw(2) << patch << std::endl;

        std::cout << "Initialize Remote SDK...\n";
    }

    void Enumerate(){
        cli::tout << "Enumerate connected camera devices..." << std::endl;
        ICrEnumCameraObjectInfo* camera_list = nullptr;
        auto enum_status = EnumCameraObjects(&camera_list);
        if (CR_FAILED(enum_status) || camera_list == nullptr) {
            cli::tout << "No cameras detected. Connect a camera and retry." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }
        auto ncams = camera_list->GetCount();
        cli::tout << "Camera enumeration successful. " << ncams << " detected." << std::endl << std::endl;

        auto camera_info = camera_list->GetCameraObjectInfo(0);
        cli::text conn_type(camera_info->GetConnectionTypeName());
        cli::text model(camera_info->GetModel());
        cli::text id = TEXT("");
        if (TEXT("IP") == conn_type) {
            cli::NetworkInfo ni = cli::parse_ip_info(camera_info->GetId(), camera_info->GetIdSize());
            id = ni.mac_address;
        }
        else id = ((TCHAR*)camera_info->GetId());
        cli::tout << '[' <<  1 << "] " << model.data() << " (" << id.data() << ")" << std::endl;
        
    }

    private:
    bool camera_connected;
    SCRSDK::CrDeviceHandle camera_handle;
};

int main(){
    // Change global locale to native locale
    std::locale::global(std::locale(""));

    CameraHandler handle;
    
    handle.Initialize();

    handle.getSDKversion();
    handle.Enumerate();
}