#ifndef MY_SONY_SDK_H
#define MY_SONY_SDK_H

#include "CRSDK\CameraRemote_SDK.h"
#include "CRSDK\CrDeviceProperty.h"
#include "CRSDK\CrTypes.h"
#include "CRSDK\CrCommandData.h"
#include "CRSDK\CrError.h"
#include "CRSDK\ICrCameraObjectInfo.h"
#include "CRSDK\CrDefines.h"
#include "CRSDK\CrImageDataBlock.h"
#include "CRSDK\IDeviceCallback.h"

#include "ConnectionInfo.h"
#include "MessageDefine.h"
#include "CameraDevice.h"
#include "PropertyValueTable.h"
#include "Text.h"

#include <iostream>

// function declaration
bool Init();
void Terminate();
void Disconnect(SCRSDK::CrDeviceHandle);
bool ConnectCamera(SCRSDK::ICrCameraObjectInfo *pcamera);
SCRSDK::ICrCameraObjectInfo* CreateUSBObject();

/*
class MyDeviceCallback : public SCRSDK::IDeviceCallback {
    void OnConnected(SCRSDK::DeviceConnectionVersioin version) override;
    void OnDisconnected(CrInt32u error) override;
    void OnPropertyChanged() override;
    void OnPropertyChangedCodes(CrInt32u num, CrInt32u* codes) override;
    void OnLvPropertyChanged() override;
    void OnLvPropertyChangedCodes(CrInt32u num, CrInt32u* codes) override;
    void OnCompleteDownload(CrChar* filename, CrInt32u type) override ;
    void OnNotifyContentsTransfer(CrInt32u notify, SCRSDK::CrContentHandle handle, CrChar* filename) override;
    void OnWarning(CrInt32u warning) override ;
    void OnWarningExt(CrInt32u warning, CrInt32 param1, CrInt32 param2, CrInt32 param3) override;
    void OnError(CrInt32u error) override;
};
*/
#endif // MY_SONY_SDK_H