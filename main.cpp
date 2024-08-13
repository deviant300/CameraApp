/*Note to the ones new to c++ do remember to change compiler settings
  Freaked me out when I took this project to my windows Laptop */

//***Camera Libraries***
#include "CameraRemote_SDK.h"
#include "CrCommandData.h"
#include "CrDefines.h"
#include "CrDeviceProperty.h"
#include "CrError.h"
#include "CrImageDataBlock.h"
#include "CrTypes.h"
#include "ICrCameraObjectInfo.h"
#include "IDeviceCallback.h"

//***General libraries***
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <string>

//defining namespace
using namespace SCRSDK;

//Creating class and defining functions
class CameraHandler : public IDeviceCallback {
    public:

    CameraHandler() : camera_connected(false), camera_handle(0) {std::cout << "Constructor called..." << std::endl;}
    ~CameraHandler() {std::cout << "Destructor called..." << std::endl;}

    void OnConnected() {
        std::cout << "Connected to camera." << std::endl;
    }

    void Initialize(){
        CrError err = Init();
        if(CR_FAILED(err)){
            std::cout << "No cameras detected,exiting..." << std::endl;
            std::cout << "Connect a camera and retry." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }
    }

    bool isCameraConnected(){
        return camera_connected;
    }

    CrDeviceHandle getCameraHandle(){
        return camera_handle;
    }

    void Connect(){
        std::cout << "Enumerating cameras...";
        ICrEnumCameraObjectInfo* camera_list = nullptr;
        auto enum_status = EnumCameraObjects(&camera_list);
        if (CR_FAILED(enum_status) || camera_list == nullptr) {
            std::cout << "No cameras detected,exiting..." << std::endl;
            std::cout << "Connect a camera and retry." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }

        auto camera_info = camera_list->GetCameraObjectInfo(0);
        if (camera_info == nullptr){
            std::cout <<"Camera detected but could not connect..." << std::endl;
            std::cout << "Reconnect camera and retry." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }

        std::cout << "Camera detected: " << camera_info << std::endl;
        const ICrCameraObjectInfo *pcamera = camera_info;
        CameraHandler *cb = new CameraHandler();
        CrDeviceHandle hDev = NULL;
        CrError err = SCRSDK::Connect(const_cast<ICrCameraObjectInfo*>(pcamera), cb, &hDev);

        CrDeviceHandle handle;
        camera_handle = handle;
        
        DeviceConnectionVersion version;
        DeviceConnectionVersion ver = version;

        if(CR_FAILED(err)){
            std::cout << "Camera not connected due to error:" << err << std::endl;
            std::cout << "Try again later." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }
    }
    
    void disConnect(){
        std::cout << "Disconnecting camera..." << std::endl;
        Disconnect(camera_handle);
        std::cout << "Releasing SDK..." << std::endl;
        Release();
        std::exit(0);
    }
    
    void CaptureImage(){
        if (!camera_connected){
            std::cout << "No camera is connected, exiting..." << std::endl;
        }

        std::cout << "clicking shutter..." << std::endl;
        CrError err = SendCommand(camera_handle, CrCommandId_Release, CrCommandParam_Down);
        if (CR_FAILED(err)){
            std::cout << "Shutter could not be clicked, error: " << err << std::endl;
        }

        std::cout << "releasing shutter..." << std::endl;
        err = SendCommand(camera_handle, CrCommandId_Release, CrCommandParam_Up);
        if (CR_FAILED(err)){
            std::cout << "Shutter could not be released, error: " << err << std::endl;
        }
    }

    void setSaveInfo(CrChar filepath){
        SetSaveInfo(camera_handle, &filepath, "", -1);
    }

    private:
    bool camera_connected;
    SCRSDK::CrDeviceHandle camera_handle;
};

int main(){
    CameraHandler handle;
    
    handle.Initialize();
    handle.Connect();
    handle.OnConnected();

    if (handle.isCameraConnected()){
        std::cout << "Proceeding" << std::endl;
    }
    else {
        std::cerr << "No camera detected exiting..." << std::endl;
        std::exit(0);
        return 0;
    }

    CrChar* filepath = "C:/Users/alank/Documents/CameraApp/Images";
    handle.setSaveInfo(*(const_cast<CrChar*>(filepath)));

    while(true){
        std::string input;

        std::cout << "Take a picture (y/n):" << std::endl;
        std::cin >> input;
        std::cout << " " << std::endl;

        if (handle.isCameraConnected() == true){
            continue;
        }
        else {
            break;
        }

        if (input == "y"){
            handle.CaptureImage();

            std::cout << "Image captured and saved to: " << filepath << std::endl;
        }
        else if (input == "n"){
            std::cout << "Disconnecting..." << std::endl;
            handle.disConnect();
            break;
        }
        else {
            std::cout << "Invalid input..." << std::endl;
            continue;
        }
    }

}