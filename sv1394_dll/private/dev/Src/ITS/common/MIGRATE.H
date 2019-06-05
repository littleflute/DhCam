//File: MIGRATE.H

#ifndef __MIGRATE_DH1394_H__
#define __MIGRATE_DH1394_H__

#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define	MAX_CAMERA_NUM			16

//
// Old Migrate Macros.
//
#define CHANNEL_RED              0x10
#define CHANNEL_GREEN1           0x11
#define CHANNEL_GREEN2           0x12
#define CHANNEL_BLUE             0x13

    typedef HANDLE HCAMERA;
    typedef BOOL (CALLBACK* PROCESSPROC)(UCHAR *pBayerData, int nWidth, int nHeight);

    typedef struct _ProcessCell
    {
        HCAMERA     hCamera;
        PROCESSPROC lpProcessProc;
    } ProcessCell, *PProcessCell;


    typedef struct _CAMERA_1394_ARRAY
    {
        TCHAR            DeviceName[255];
    } CAMERA_1394_ARRAY, *PCAMERA_1394_ARRAY;

    typedef struct _CAMERA_1394_LIST
    {
        DWORD  CameraNum;
        CAMERA_1394_ARRAY CameraList[MAX_CAMERA_NUM];
    } CAMERA_1394_LIST, *PCAMERA_1394_LIST;

//
// Camera info, Used in GetCameraInfo.
//
    typedef struct _CAMERA_INFO
    {
        TCHAR   BusName[32];         // Camera type, as "1394".
        TCHAR   VendorName[32];  // Camera vendor name.
        TCHAR   ModelName[32];   // Camera Model name.
        TCHAR   UniqueID[32];    // Camera unique ID.
    } CAMERA_INFO, *PCAMERA_INFO;

    HCAMERA
    WINAPI
    OpenCamera
    (TCHAR* lpCamera);

    BOOL
    WINAPI
    SetVideoMode
    (HCAMERA hCamera, int nFormat, int nMode, int nRate);

    DWORD
    WINAPI
    SetSnapMode
    (HCAMERA hCamera, DWORD dwSnapMode);

    DWORD
    WINAPI
    SetResolution
    (HCAMERA hCamera, DWORD dwResolution);

    DWORD
    WINAPI
    SetOutputWindow
    (HCAMERA hCamera, int x, int y, int nWidth, int nHeight);


    BOOL
    WINAPI
    StopSnap
    (HCAMERA hCamera);

    BOOL
    WINAPI
    CloseCamera
    (HCAMERA hCamera);

    DWORD
    WINAPI
    ADCControl
    (HCAMERA hCamera, DWORD dwADCType, DWORD dwValue);

    DWORD
    WINAPI
    AGCControl
    (HCAMERA hCamera, DWORD dwAGCType, DWORD dwValue);

    DWORD
    WINAPI
    AECControl
    (HCAMERA hCamera, DWORD dwAECType, DWORD dwValue);

    DWORD
    WINAPI
    GetCameraInfo
    (HCAMERA hCamera, PCAMERA_INFO pCell);

    DWORD
    WINAPI
    GetDeviceList
    (PCAMERA_1394_LIST DeviceData);

    typedef struct tagVERSION
    {
        WORD wMajor;
        WORD wMinor;
        WORD wRevision;
        WORD wBuild;
    } VERSION, *PVERSION;
    /*
    \brief GetLibraryVersion, 获取资源文件中的版本信息
    \param hModule, exe，DLL 句柄
    \return 返回
    */
    VERSION GetLibraryVersion(HMODULE hModule);

    PCHAR GetLibraryName();

#define ToClass(handle)          ((CCamera*)handle)
#define ToHandle(pCamClass)      (HANDLE)(pCamClass)
#define VerifyHandle(handle) \
    if (*((int*)handle) != 0x5C3F5C5C)     \
        return STATUS_DEVICE_HANDLE_INVALID;

#define VerifyRetVal(retval)  \
    if (retval != STATUS_OK)  \
        return retval;

#define ToCameraType(pixel, interfacetype, colortype)  \
    ((pixel & 0xFFFF) << 16) | (interfacetype) << 8 |  \
    (colortype)

#define ToCameraTypeShort(pixel, type)  \
    ((pixel & 0xFFFF) << 16) | (type)

#ifdef __cplusplus
}
#endif

#endif // __MIGRATE_DH1394_H__