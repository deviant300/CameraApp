//***To find memory leaks***
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//***Camera Libraries***
#include "main.h"


//***General libraries***
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <thread>

//defining namespace
using namespace SCRSDK;

//Creating class and defining functions
class CameraHandler : public IDeviceCallback {
    public:
    CameraHandler() : camera_connected(false), camera_handle(0) {std::cout << "Constructor called..." << std::endl;}
    ~CameraHandler() {std::cout << "Destructor called..." << std::endl;};
    
    void Initialize(){
        std::cout << "Attempting to initialize SDK" <<std::endl;
        auto ret = Init(0);
        if(!ret){
            std::cout << "Initialization failed" << std::endl;
            std::cout << "Terminating..." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }
        std::cout << "Initialization Success" << std::endl;
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

    void Connect(){
        cli::tout << "Enumerating connected camera devices..." << std::endl;
        ICrEnumCameraObjectInfo* camera_list = nullptr;
        cli::tout << "grabbing details..." << std::endl;
        auto enum_status = EnumCameraObjects(&camera_list);
        
        if (CR_FAILED(enum_status)) {
            cli::tout << "No cameras detected due to an error retry." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }else if (camera_list == nullptr){
            cli::tout << "No cameras detected retry." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
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
        std::cout << "Initiating connection" << std::endl;
        typedef std::shared_ptr<cli::CameraDevice> CameraDevicePtr;
        typedef std::vector<CameraDevicePtr> CameraDeviceList;
        CameraDeviceList cameraList; // all
        std::int32_t cameraNumUniq = 1;
        cli::tout << "Create camera SDK camera callback object.\n";
        CameraDevicePtr camera = CameraDevicePtr(new cli::CameraDevice(cameraNumUniq, camera_info));
        cameraList.push_back(camera); // add 1st
        cli::tout << "Release enumerated camera list.\n";
        std::cout << std::endl << std::endl;
        camera_list->Release();
        camera->connect(CrSdkControlMode_Remote, CrReconnecting_ON);
        camera->af_shutter();
        camera->capture_image();
        camera->set_save_info();
        camera->OnCompleteDownload(L"alank",-1);

        camera_connected = true;


    }
    
    void setSaveInfo(){
        try {
            // Get the current working directory
            std::cout << "Detecting current folder..." << std::endl;
            std::filesystem::path currentPath = std::filesystem::current_path();
            std::cout << "Current folder: " << currentPath << std::endl;
            // Define a subdirectory name
            std::string subdirectory = "Images";
            // Create a new path variable that combines the current path with the subdirectory
            std::filesystem::path newPath = currentPath / subdirectory;
            std::cout << "Image folder: " << newPath << std::endl;
            std::wstring wideFilePath = newPath.wstring();  // Convert to std::wstring
            // Now you can use wideFilePath.c_str() to get a wchar_t* pointer
            std::cout << "Setting save folder here..." << std::endl;
            std::cout << "Save folder set" << std::endl; 

            SCRSDK::SetSaveInfo(camera_handle, L"C:\\Users\\alank", L"test", -1); 
            SCRSDK::SendCommand(camera_handle, CrCommandId_Release, CrCommandParam_Down);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            SCRSDK::SendCommand(camera_handle, CrCommandId_Release, CrCommandParam_Up);

        } 
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    
    void ImageDownload(){
    
    }

    void release(){
        std::cout << "Releasing SDK resources" << std::endl;
        
        Release();
        std::cout << "SDK resources Released" << std::endl;
    }
    
    bool isConnected(){
        return camera_connected;
    }

    private:
    bool camera_connected;
    SCRSDK::CrDeviceHandle camera_handle;
    SCRSDK::DeviceConnectionVersion version;
};

int main(){
    // Change global locale to native locale
    std::locale::global(std::locale(""));
    CameraHandler handle;
    
    handle.Initialize();
    handle.getSDKversion();
    handle.Connect();

    // handle.setSaveInfo();
    std::cout << "Setting save folder..." << std::endl; 
    std::this_thread::sleep_for(std::chrono::seconds(5));


    // handle.setSaveInfo();
    // std::this_thread::sleep_for(std::chrono::seconds(5));
    // while(true){
        // printf("Connected");
    // }
    handle.release();
    std::exit(EXIT_SUCCESS);
    return 0;
}