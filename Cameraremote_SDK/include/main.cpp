//***To find memory leaks***
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//***Camera Libraries***
#include "main.h"
    
bool Init(){
    std::cout << "Attempting to initialize SDK" <<std::endl;
    auto ret = SCRSDK::Init(0);
    if(!ret){
        std::cout << "Initialization failed" << std::endl;
        std::cout << "Terminating..." << std::endl;
        return false;
    }
    std::cout << "Initialization Success" << std::endl;
    return true;
}

void Terminate(){
    SCRSDK::Release();
    std::exit(0);
}

void Disconnect(SCRSDK::CrDeviceHandle handle){
    SCRSDK::Disconnect(handle);
}

class MyDeviceCallback : public SCRSDK::IDeviceCallback
{

    void OnConnected(SCRSDK::DeviceConnectionVersion version) override
    {
        SCRSDK::DeviceConnectionVersion ver = version;
        std::cout << "Camera connected with version: " << version << std::endl;
        // Program can use the device handle.
    }

    void OnDisconnected(CrInt32u error) override
    {
        // Implementation of OnDisconnected
        std::cout << "Camera disconnected, error code: " << error << std::endl;
    }

    void OnPropertyChanged() override
    {
        // Implementation of OnPropertyChanged
        std::cout << "A property changed." << std::endl;
    }

    void OnPropertyChangedCodes(CrInt32u num, CrInt32u *codes) override
    {
        // Implementation of OnPropertyChangedCodes
        std::cout << "Properties changed, number of codes: " << num << std::endl;
        // Optionally, loop through the codes array and print each code
    }

    void OnLvPropertyChanged() override
    {
        // Implementation of OnLvPropertyChanged
        std::cout << "Live view property changed." << std::endl;
    }

    void OnLvPropertyChangedCodes(CrInt32u num, CrInt32u *codes) override
    {
        // Implementation of OnLvPropertyChangedCodes
        std::cout << "Live view properties changed, number of codes: " << num << std::endl;
    }

    void OnCompleteDownload(CrChar *filename, CrInt32u type) override
    {
        // Implementation of OnCompleteDownload
        std::cout << "Download complete, filename: " << filename << ", type: " << type << std::endl;
    }

    void OnNotifyContentsTransfer(CrInt32u notify, SCRSDK::CrContentHandle handle, CrChar *filename) override
    {
        // Implementation of OnNotifyContentsTransfer
        std::cout << "Contents transfer notified, filename: " << filename << std::endl;
    }

    void OnWarning(CrInt32u warning) override
    {
        // Implementation of OnWarning
        std::cout << "Warning received, code: " << warning << std::endl;
    }

    void OnWarningExt(CrInt32u warning, CrInt32 param1, CrInt32 param2, CrInt32 param3) override
    {
        // Implementation of OnWarningExt
        std::cout << "Extended warning received, code: " << warning << std::endl;
    }

    void OnError(CrInt32u error) override
    {
        // Implementation of OnError
        std::cout << "Error occurred, code: " << error << std::endl;
    }

    // additional functions required by IDeviceCallback
};

bool ConnectCamera(SCRSDK::ICrCameraObjectInfo *pcamera)
{
    MyDeviceCallback *cb = new MyDeviceCallback();
    SCRSDK::CrDeviceHandle hDev = 0;
    SCRSDK::CrError err = SCRSDK::Connect(pcamera, cb, &hDev);

    if (err == SCRSDK::CrError_None)
    {
        return hDev; // Return the handle if connection was successful
    }
    else
    {
        delete cb; // Clean up callback if connection failed
        return 0;  // Return an invalid handle to signify failure
    }
}

SCRSDK::ICrCameraObjectInfo *CreateUSBObject()
{
    CrChar serialNum[(SCRSDK::USB_SERIAL_LENGTH + 1)] = {0}; // +1 is Null-terminate
    memcpy(serialNum, L"CFB7A03380EF", sizeof(serialNum));   // <-- check what is this value for us
    SCRSDK::ICrCameraObjectInfo *pCam = nullptr;
    SCRSDK::CrError err = SCRSDK::CreateCameraObjectInfoUSBConnection(&pCam, SCRSDK::CrCameraDeviceModel_ILX_LR1, (CrInt8u *)serialNum); // ILX-LR1
    if (err == SCRSDK::CrError_None && pCam != nullptr)
    {
        std::cout << "Failed to create camera object" << std::endl;
        return pCam;
    }
    std::cout << "Created camera object" << std::endl;
    return nullptr;
}