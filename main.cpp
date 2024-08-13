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

        std::cout << "Attempting to initialize SDK" <<std::endl;
        bool ret = Init();
        if(!ret){
            std::cout << "Initialization failed" << std::endl;
            std::cout << "Retry." << std::endl;
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

    void enumerateCamera(){
        std::cout << "Enumerating cameras..." << std::endl;
        ICrEnumCameraObjectInfo* pEnum = nullptr;
        CrError err = EnumCameraObjects(&pEnum);
        if (pEnum == 0) {
            std::cerr << "error: " << err << std::endl;
            std::cout << "No cameras detected, exiting..." << std::endl;
            std::cout << "Connect a camera and retry." << std::endl;
            Release();
            std::exit(EXIT_FAILURE);
        }

        CrInt32u count = pEnum->GetCount();
        std::cout << count << " cameras detected" <<std::endl;
        for(CrInt32u n = 0; n < count; n++){
            const ICrCameraObjectInfo *pobj = pEnum->GetCameraObjectInfo(n);
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
    handle.enumerateCamera();
}