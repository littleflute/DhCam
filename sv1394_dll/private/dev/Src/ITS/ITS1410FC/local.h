//File: local.h
#if !defined(XD_LOCAL_H)
#define XD_LOCAL_H


//freeze
typedef struct tagHV_ARG_FREEZE_FRAME
{
    int nFreeze;
} HV_ARG_FREEZE_FRAME;


//snap
HVSTATUS HVInitCamera(HHV pHandle,void *pInBuffer);

HVSTATUS
HVOpenSnap(PCAMOBJ pCamObj,
           void *pInBuffer);
HVSTATUS HVStartSnap(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVStopSnap(PCAMOBJ pCamObj);
HVSTATUS HVCloseSnap(PCAMOBJ pCamObj);
HVSTATUS HVSnapShot(PCAMOBJ pCamObj,void *pInBuffer);


//string
HVSTATUS HVGetString(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVGetCameraInfo(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);

//format¡¢mode¡¢rate
HVSTATUS HVGetVideoFormatMask(PCAMOBJ pCamObj,void *pOutBuffer);
HVSTATUS HVGetVideoModeMask(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVGetFrameRateMask(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);

//format7
HVSTATUS HVFMT7GetModeDescriptor(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVFMT7SetAOI(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVFMT7GetAOI(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVFMT7SetColorMode(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVFMT7GetColorMode(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);


//feature
HVSTATUS HVFeaturePresence(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVFeatureINQ(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVSetFeature(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVGetFeature(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);

//read or write data
HVSTATUS HVReadBlock(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVWriteBlock(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVReadQuadlet(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVWriteQuadlet(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVGetUserData(PCAMOBJ pCamObj,void *pInBuffer,void *pOutBuffer);
HVSTATUS HVSetUserData(PCAMOBJ pCamObj,void *pInBuffer);

//ADD
HVSTATUS HVGetShutterUnit(PCAMOBJ pCamObj,void *pOutBuffer);
HVSTATUS HV1394FreezeFrame(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HV1394GetUserMem(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HV1394GetLUT(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HV1394SetLUT(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVSetTriggerPolarity(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVSetStrobePolarity(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVSetBlackLevel(PCAMOBJ pCamObj,void *pInBuffer);
HVSTATUS HVSetExposureTime(PCAMOBJ pCamObj,void *pInBuffer);

// for all function of 1311...

HVSTATUS
Control(PCAMOBJ pCamObj,
        HV_CONTROL_CODE code,
        void *pInBuffer,
        DWORD nInBufferSize,
        void *pOutBuffer,
        DWORD nOutBufferSize);



#endif