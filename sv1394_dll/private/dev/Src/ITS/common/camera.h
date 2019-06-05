//File: camera.h
#ifndef __1394CAMERA_H__
#define __1394CAMERA_H__
/////////////////////////////////////////////////////////////////////////////////
//snap mode
typedef struct tagHV_ARG_SET_SNAP_MODE
{
    HV_SNAP_MODE SnapMode;
} HV_ARG_SET_SNAP_MODE;


typedef struct _QBUFFER
{
    OVERLAPPED				overLapped;
    PISOCH_ATTACH_BUFFERS   pIsochAttachBuffers;
    PUCHAR					pDataBuf;
    PUCHAR					pFrameStart;
    int						index;
    UINT                    indexAtch;
    struct _QBUFFER		    *pNextBuffer;
} QBUFFER, *PQBUFFER;


//
// In any time, the specific camera can have one of the three status:
// idle, single grabing, or continuous grabing. To change camera
// status, just change the m_Camera_Status in CCamera object, be sure
// do some wait action carefully.
//

typedef enum tagCameraStatus
{
    idle               = 0,
    SingleGrabing      = 1,
    ContinuousGrabing  = 2
} CameraStatus;

typedef struct _BUS_RESET_THREAD_PARAMS
{
    PSTR                szDeviceName;
    BOOL                bKill;
    HANDLE              hEvent;
    HANDLE              hThread;
    ULONG               ThreadId;
    CCamera				*pMyCam;
} BUS_RESET_THREAD_PARAMS, *PBUS_RESET_THREAD_PARAMS;

typedef struct _SnapShotCell
{
    CCamera* pCamera;
    BYTE** ppBuffer;
    int nSum;
    HANDLE hEvent;
    HANDLE hThread;
} SnapShotCell;

//---------------------------------------------------------------
#define  ORD_NOTIFY_REGESTER	 0x10000000
/// 该结构
typedef struct tagHV_NOTIFY_INFO
{
    int		nMsgType;	//通知消息类型
    void	*pMsgParam; //通知消息所携带的参数
} HV_NOTIFY_INFO;

/// NOTIFY 回调函数的声明
typedef int (CALLBACK* HV_NOTIFY_PROC)(HV_NOTIFY_INFO *pNotifyInfo);

///Argument for the ORD_NOTIFY_REGESTER function
typedef struct tagHV_ARG_NOTIFY_REGESTER
{
    HV_NOTIFY_PROC		ftn;
    HV_NOTIFY_INFO		inf;
} HV_ARG_NOTIFY_REGESTER;

//---------------------------------------------------------------

class sharelib_hvapi;

#define MAX_FRAME_COUNT 0xFFFF

class CCamera
{
public:
    CCamera();
    ~CCamera();

    BOOL OpenCamera(TCHAR* lpFullCameraName);

public:
    UINT  m_CameraType     : 2;  // CMOS camera is 1, CCD camera is 2, if 0, mean unknow type.
    UINT  m_videoMode      : 8;
    USHORT							m_Width;
    USHORT							m_Height;

    HVSTATUS ALL_Ctrl(HV_CONTROL_CODE code,
                      void *pInBuffer, DWORD nInBufferSize,
                      void *pOutBuffer, DWORD nOutBufferSize,
                      DWORD *pBytesReturned);

    DWORD AcquisitionThreadProc(void* pParameter);

    DWORD DisplayThreadProc(void* pParameter);

//	DWORD HeartBeatThreadProc(void* pParameter);	

    DWORD SnapShot(BYTE** ppBuffer, int num);

    void StartBusResetThread();
    void StopBusResetThread();

    // Raw register I/O
    HVSTATUS WriteQuadlet(DWORD address, DWORD data);
    HVSTATUS ReadQuadlet(DWORD address, DWORD *pData);

private:
    BOOL StartSnap();
    HVTYPE _GetMyType();
    HVSTATUS local_Get_Resolution(void *pContext,int *pSize);
    HVSTATUS local_Get_SN(void *pContext,int *pSize);
    HVSTATUS local_Get_Sensor_ID(void *pContext,int *pSize);
    HVSTATUS local_Get_Hardware_Ver(void *pContext,int *pSize);
    HVSTATUS local_Get_Device_Name(void *pContext,int *pSize);
    HVSTATUS local_Get_Sys_Ver(void *pContext,int *pSize);
    HVSTATUS local_Get_Device_Drv_Ver(void *pContext,int *pSize);
    HVSTATUS local_Get_FPGA_Ver(void *pContext,int *pSize);
    HVSTATUS local_Get_Vender_Name(void *pContext,int *pSize);
    HVSTATUS local_Get_Bayer_Layout(void *pContext,int *pSize);
    HVSTATUS local_Get_Device_Type(void *pContext,int *pSize);

    HVSTATUS Get_Unique_ID(TCHAR* szDeviceName,PLARGE_INTEGER pliUniqueID);
    HVSTATUS Get_SYS_Version(char* pSzDrvVer);
    HVSTATUS Raw8or12(void *pInBuffer);
    HVSTATUS SetOutPutIO_0(void *pInBuffer);
    HVSTATUS SetOutPutIO_1(void *pInBuffer);
    HVSTATUS SetOutPutIO_2(void *pInBuffer);
    HVSTATUS SetOutPutIO_0_POL(void *pInBuffer);
    HVSTATUS SetOutPutIO_1_POL(void *pInBuffer);
    HVSTATUS SetOutPutIO_2_POL(void *pInBuffer);
    HVSTATUS SetTriggerSource(void *pInBuffer);
    HVSTATUS TriggerDelayOnOff(void *pInBuffer);
    HVSTATUS TriggerDelayValue(void *pInBuffer);
    HVSTATUS TestImage(void *pInBuffer);
    HVSTATUS TransfersDelayValue(DWORD dwV);

    HVSTATUS ShutterUnit(DWORD dwV);
    HVSTATUS FilterTimeUnit(DWORD dwV);
    HVSTATUS FilterTimeValue(DWORD dwV);
    HVSTATUS LE_BE_Mode(DWORD dwV);

    HVSTATUS TriggerShot();
    BOOL CheckPassword();


    static DWORD WINAPI BusResetThread(LPVOID  lpParameter);

    TCHAR  m_pName[256];
    HANDLE m_hCamera;		//2006.2.20,XYB,注意，必须放在此处，VerifyHandle(handle)才OK。不能放在前面。

    PBUS_RESET_THREAD_PARAMS    m_BusResetThreadParams;
    //
    // Buffer management.
    //
    PQBUFFER	m_pFirstBuffer;
    PQBUFFER	m_pCurrentBuffer;
    PQBUFFER	m_pNextBuffer;
    PQBUFFER	m_pBlockBuffer; //2007.1.15

    DWORD m_dwCameraType;

    UINT  m_videoFormat    : 8;
    UINT  m_videoFrameRate : 8;

    UINT  m_bfps           : 1;
    UINT  m_bStopDisplay   : 1;
	BOOL  m_bStopHeartBeat;

    // Camera feature masks.
    DWORD m_dwFormatMask;
    DWORD m_dwModeMask[8];
    DWORD m_dwRateMask[7][8];

    // Resources limit.
    int   m_maxBytes;
    int   m_maxBufferSize;
    ULONG m_maxSpeed;

    // Isochronous resources.
    HANDLE m_hBandwidth;
    HANDLE m_hResource;
    LONG   m_lChannel;

    CameraStatus m_Camera_Status;

    LONG							m_n8or12;

    int								m_nNumErr;        //Added by HYL 2007.1.10
    //BOOL							m_bDeviceRemove;  //Added by HYL 2007.1.10
    //BOOL							m_bBlockWait;     //Added by HYL 2007.1.15

    LONG							m_lLostNum;//Added by HYL 2007.1.15
    UINT                            m_AttachNum;//ydTest...
    UINT                            m_BroadNum;//ydTest...

    UCHAR				*m_pData;// Store raw bayer data.

    HV_NOTIFY_PROC		m_lpNotifyFun;

    HV_SNAP_PROC		m_lpSnapProc;
    void				*m_pSnapParam;

    BYTE				**m_ppSnapBuffer;
    DWORD				m_BufferNum;


    // Critical section to synchronize access to m_pNextBuffer
    CRITICAL_SECTION	  m_CritSect;
    // Event.
    HANDLE m_hNewBufferAvailable;
    HANDLE m_hCancelRequest;
    HANDLE m_hTerminate;


    HANDLE m_hBlockEvent;


    // Event trace two thread over.
    HANDLE m_hDisplayOver;
    HANDLE m_hGrabOver;
	HANDLE m_hHeartBeatOver;

    // Two thread.
    HANDLE m_hGrabThread;
    HANDLE m_hDisplayThread;
	HANDLE m_hHeartBeatThread;

    //
    // Camera Feature Members.
    //
    CFeature m_Brightness;
    CFeature m_AutoExposure;
    CFeature m_Sharpness;
    CFeature m_WhiteBalance;
    CFeature m_Hue;
    CFeature m_Saturation;
    CFeature m_Gamma;
    CFeature m_Shutter;
    CFeature m_Gain;
    CFeature m_Iris;
    CFeature m_Focus;
    CFeature m_Temperature;
    CTrigger m_Trigger;
    CFeature m_TriggerDly;
    CFeature m_WhiteShd;
    CFeature m_FrameRate;

    CStrobe  m_Strobe;

    // 0x40 - 0x7C is reserved for other FEATURE_HI
    CFeature m_Zoom;
    CFeature m_Pan;
    CFeature m_Tilt;
    CFeature m_OpticalFilter;
    // 0x90 - 0xBC is reserved for other FEATURE_LO
    CFeature m_CaptureSize;
    CFeature m_CaptureQuality;
    CFormat7 m_Format7;


    // Query format, mode or rate.
    BOOL IsFormatAvailable(int nFormat);
    BOOL IsModeAvailable(int nFormat, int nMode);

    // Load last Settings.
    int LoadProfileSettings();
    int LoadFormat7Settings();


    // Video format/mode/rate
//	int GetVideoFormat();
    int SetVideoFormat(DWORD format);
//	int GetVideoMode();
    int SetVideoMode(DWORD mode);
//	int GetVideoFrameRate();
    int SetVideoFrameRate(DWORD rate);

    // Grab control
    DWORD GrabCancel();
    int  Enqueue(PQBUFFER pQBuffer);

    // Image Grab Control
    int PrepareContinuousGrab(int nBuffers, int Flags);
    int StopImageAcquisition();

    // Query format, mode or rate.
    BOOL IsRateAvailable(int nFormat, int nMode, int nRate);

    BOOL InitResources();
    void StopVideoStream();
    BOOL FreeResources();
    void StartVideoStream();

    BOOL InquireVideoFormats();
    BOOL InquireVideoModes();
    BOOL InquireVideoRates();


    // Camera Feature registers quick access functions.
    void InquireControlRegisters();
    void StatusControlRegisters();
    void SetShutter(UINT value);
    void SetGain(UINT value);
    void SetWhiteBalance(UINT uvalue,UINT vvalue);

    void UpdateParameters();

    DWORD AsyncBlockWrite(DWORD address, DWORD *pData,int nLength);
    DWORD AsyncBlockRead(DWORD address, DWORD *pData,int nLength);

private:

    HVSTATUS f_SET_BASIC_FUNC(void *pInBuffer);
    HVSTATUS f_GET_BASIC_FUNC(void *pInBuffer,void *pOutBuffer);
    HVSTATUS f_QUERY_BASIC_FUNC_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_BASIC_FUNC_DESCRIPTOR(void *pInBuffer, DWORD nInBufferSize,void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_SET_ADV_USER_CUSTOM(void *pInBuffer, DWORD nInBufferSize);
    HVSTATUS f_GET_ADV_USER_CUSTOM(void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize);
    HVSTATUS f_QUERY_ADV_USER_CUSTOM_DESCRIPTOR(HV_CONTROL_CODE code, void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_GET_STRING(void *pInBuffer,void *pOutBuffer);
    HVSTATUS f_QUERY_STRING_DESCRIPTOR(HV_CONTROL_CODE code, void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_STRING_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_GET_VIDEO_FORMAT_MODE_FRAME_RATE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_SET_VIDEO_FORMAT_MODE_FRAME_RATE(void *pInBuffer);
    HVSTATUS f_QUERY_FRAME_RATE_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_VIDEO_MODE_PRESENCE(HV_CONTROL_CODE code, void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_INIT_CAMERA(void *pInBuffer);
    HVSTATUS f_CLEAR_PASSWORD();
    HVSTATUS f_CHECK_PASSWORD(void *pInBuffer);
    HVSTATUS f_SET_PASSWORD(void *pInBuffer);
    HVSTATUS f_NOTIFY_REGESTER(void *pInBuffer);
    HVSTATUS f_OPEN_SNAP(void *pInBuffer);
    HVSTATUS f_START_SNAP(void *pInBuffer);
    HVSTATUS f_STOP_SNAP();
    HVSTATUS f_CLOSE_SNAP();
    HVSTATUS f_SNAP_SHOT(void *pInBuffer);
    HVSTATUS f_TRIGGER_SHOT();

    HVSTATUS f_SET_STROBE(void *pInBuffer);
    HVSTATUS f_GET_STROBE(void *pInBuffer,void *pOutBuffer);

    HVSTATUS f_SET_FEATURE(void *pInBuffer);
    HVSTATUS f_GET_FEATURE(void *pInBuffer,void *pOutBuffer);
    HVSTATUS f_QUERY_FEATURE_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_FEATURE_DESCRIPTOR(void *pInBuffer, DWORD nInBufferSize,void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_SET_FMT7_MODE(void *pInBuffer);
    HVSTATUS f_GET_FMT7_MODE(void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_FMT7_MODE_DESCRIPTOR(void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_SET_ADV_FEATURE(void *pInBuffer);
    HVSTATUS f_GET_ADV_FEATURE(void *pInBuffer,void *pOutBuffer);
    HVSTATUS f_QUERY_ADV_FEATURE_DESCRIPTOR(void *pInBuffer, DWORD nInBufferSize,void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_ADV_FEATURE_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_SET_ADV_BASIC_FUNC(void *pInBuffer);
    HVSTATUS f_QUERY_ADV_OPT_FUNC_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_SET_ADV_LUT(void *pInBuffer);

    HVSTATUS f_SET_PIO_OUTPUT_PORT(void *pInBuffer);
    HVSTATUS f_GET_PIO_OUTPUT_PORT(void *pInBuffer,void *pOutBuffer);

    HVSTATUS f_QUERY_IMAGE_INFO_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_IMAGE_INFO_DESCRIPTOR(void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_GET_IMAGE_INFO(void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS f_SET_AUTO_FUNC(void *pInBuffer);
    HVSTATUS f_GET_AUTO_FUNC(void *pInBuffer, DWORD nInBufferSize,void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_AUTO_FUNC_PRESENCE(void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);
    HVSTATUS f_QUERY_AUTO_FUNC_DESCRIPTOR(void *pInBuffer, DWORD nInBufferSize,void *pOutBuffer, DWORD nOutBufferSize,DWORD *pBytesReturned);

    HVSTATUS vv_SetAutoGainCtrl(HV_ARG_SET_AUTO_FUNC *pArgSetAutoFunc);
    HVSTATUS vv_SetAutoShutterCtrl(HV_ARG_SET_AUTO_FUNC *pArgSetAutoFunc);
    HVSTATUS vv_SetAutoFuncAOI(HV_ARG_SET_AUTO_FUNC *pArgSetAutoFunc);
    HVSTATUS vv_SetAutoStrobeScalar(DWORD dwOffset, HV_ARG_SET_AUTO_FUNC *pArgSetAutoFunc);
    HVSTATUS vv_SetAutoStrobeStat(DWORD dwOffset, HV_ARG_SET_AUTO_FUNC *pArgSetAutoFunc);

    HVSTATUS SetGainGreen_B(DWORD dwV);
    HVSTATUS SetGainGreen_R(DWORD dwV);

    DWORD m_Trigger_Source;
    UINT  m_uiShutterVal;

    //////////////////////////////////////////////////////////////////////////
    BYTE *m_pImageInfoBuf; // add by LV Xiaofeng
    DWORD m_dwImageInfoBufSize;
    CRITICAL_SECTION m_CritSectImageInfo;
    BOOL HasImageInfo();
    HVSTATUS GetImageInfoSize(DWORD *pSize);
    HVSTATUS AllocImageInfoBuf();
    void DeleteImageInfoBuf();

	//////////////////////////////////////////////////////////////////////////
	// HVAPI新功能
	sharelib_hvapi *m_pHVAPI;
	
	//////////////////////////////////////////////////////////////////////////
// 	DWORD GetRecvFrameCount();
// 	VOID  SetRecvFrameCount(DWORD dwVal);
// 	VOID  AddRecvFrameCount();
// 	VOID  ClearRecvFrameCount();
// 	CRITICAL_SECTION m_CritSectFrameCount;
// 	DWORD m_dwRecvFrameCount;
// 	BOOL HasHeartBeat();
// 	BOOL  m_bHasHeartBeat;
// 	HVSTATUS GetHeartBeatCount(DWORD *pValue);
// 	HVSTATUS SetHeartBeatCount(DWORD dwValue);
// 
// 	int SetTimer();
// 	int KillTimer();
};

/////////////////////////////////////////////////////////////////////////////////
#endif//__1394CAMERA_H__