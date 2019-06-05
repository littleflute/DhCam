// Device.h: interface for the CDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICE_H__A6B47285_FBD5_4E5C_8939_56369F355777__INCLUDED_)
#define AFX_DEVICE_H__A6B47285_FBD5_4E5C_8939_56369F355777__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define WM_ERROR WM_USER + 42   // user defined message, sent by the acquisistion thread to the main frame if 
								// an error occured in the thread's run method

class CChildFrame;
class CMainFrame;
struct CImageInfo;
struct CImageData;
struct CConvertImageInfo;
struct CConvertImageData;

#include "image.h"
#include "resource.h"
#include "math.h"
#include "HVBase.h"
#include "..\inc\HVDef.h"

const unsigned long TIMERID = 0xa7a3;
const unsigned long TIMERINTERVAL = 1000;

typedef enum tagReadFunID{	

		Get_OUTPUT_IO_0			= 21,
		Get_OUTPUT_IO_1			= 22,
		Get_OUTPUT_IO_2			= 23,
		ColorModeInq		= 80,
//		ColorModeInq_Mono8		= 83,
		ColorModeInq_Raw10		= 84,
		
}ReadFunID;		// add by xupx 2008.06.30

class CDevice : public CHVBase
{
public:
	CDevice(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	
public:	
	virtual ~CDevice();
	/// open the device driver and do some initialization
	void Activate(bool bInit = true);
	/// close the device driver and do some cleanup
	void Deactivate();
	/// Tell the camera object which is its associated MDI child window
	void SetMDIChild(CChildFrame* pChild);
	
	/// Grab single frame
	void GrabSingle();
	/// "Grab" image from file
	void GrabFromFile(CString FileName);
	/// Activate continuous grabbing
	void GrabContinuous();
	/// Cancel continuous grabbing
	void GrabCancel();
	/// Perform a white balance
	void PerformWhiteBalance();
	/// Perform a Region white balance
	void PerformRegionWhiteBalance();
	/// Are we currently grabbing ?
	bool IsGrabActive() { return m_fContinuousGrabActive || m_fSingleGrabActive; }
	bool IsContinuousGrabActive() { return m_fContinuousGrabActive; }
	bool IsSingleGrabActive() { return m_fSingleGrabActive; }
	
	/// Get current acquisition information
	void GetImageInfo(CImageInfo &info);
	void GetImageData(CImageData &data);
	/*===============ImageInfo v1.1  0409==================*/
	void GetImageInfo(CConvertImageInfo *pImageInfo);
	void GetImageData(CConvertImageData *pImageData);
	
	/// Retrieve actual frame rate
	void GetFrameRate(double& displayed); 
	
	bool XD_Get_FPS(double& fps,bool b=0);

	HVSTATUS  RegisterSnapMode();

	/// Return the bitmap to display  ( The image view will use this function in its OnPaint method)
	CHVBitmap* GetBitmap() 
	{ 
		return m_fBitmapValid ? m_pBitmap : NULL;
	}
	
	/// Return the current color code
	HV_COLOR_CODE	GetColorCode()
	{ 
		return m_ColorCode; 
	}
	
	/// Get source colorCode from camera. add by xupx 2009.02.20
	int	GetFMT7Mode(HV_VIDEO_MODE mode, HV_FMT7_MODE_FUNC_ID id, HV_RES_GET_FMT7_MODE *pRes) 
	{ 
		HVAPI_CONTROL_PARAMETER  p;
		HHV hhv=m_hDevice;
		HVSTATUS status;
		
		int	nValue = 0;	
		
		HV_ARG_GET_FMT7_MODE ArgFeature;
		HV_RES_GET_FMT7_MODE ResFeature;
		p.pInBuf = &ArgFeature;
		p.dwInBufSize = sizeof(ArgFeature);	
		p.pOutBuf = &ResFeature;
		p.dwOutBufSize = sizeof(ResFeature);
		p.pBytesRet = NULL;		
		p.code = ORD_GET_FMT7_MODE;
		
		ArgFeature.VideoMode	 = mode;	//VIDEO_MODE0;  by xupx 2009.10.18
		switch(id)
		{
		case FMT7_MODE_FUNC_ID_COLOR_CODING:
			ArgFeature.FuncId		 = FMT7_MODE_FUNC_ID_COLOR_CODING;
			status = HVCommand(hhv,CMD_HVAPI_CONTROL,&p,0);
			nValue = ResFeature.ColorMode.Code;
			break;
		case FMT7_MODE_FUNC_ID_AOI:
			ArgFeature.FuncId		 = FMT7_MODE_FUNC_ID_AOI;
			status = HVCommand(hhv,CMD_HVAPI_CONTROL,&p,0);
			pRes->AOI = ResFeature.AOI;
			break;
		}
		
		return nValue;

	}
	
	/// Inform the camera object that it is to be reconfigured (e.g. by the properties dialog or by changing the AOI)
	/// An active grabbed will be suspended.
	bool ReconfigurationRequest();
	
	/// Inform the camera object that the reconfiguration is done. A suspended grab will be resumed.
	void ConfigurationChanged();
	
	void Set_Default_AOI();
	/// SetAOI
	void SetAOI(CRect AOI);
	/// Maximize the AOI
	void MaximizeAOI();
	/// Parametrize Device 
	void ParametrizeDevice(HV_VIDEO_MODE mode, HV_COLOR_CODE code, CPoint position, CSize size);
	/// Has the camera a scalable AOI ?
	bool IsScalable() {return m_fScalable;}
	
	/// Set Video mode
	void SetVideoMode(HV_VIDEO_MODE mode);
	void UpdateImageSize(HV_VIDEO_MODE mode);
	
	/// Set Bayer to RGB conversion mode ( 0: no conversion, 1 .. 4 enable conversion)
	void SetDisplayMode( int mode );
	/// Get Bayer to RGB conversion mode ( 0: no conversion, 1 .. 4 enable conversion)
	int GetDisplayMode() { return m_DisplayMode; }
	
	void SaveConfiguration(const CPropertyBagPtr ptrBag);
	/// save the current configuration to a file
	void SaveConfiguration(const CString fileName) { SaveConfiguration(CIniFilePropertyBag::Create(fileName, "HVDevice")); }
	/// restore configuration from a CPropertyBag
	void RestoreConfiguration(const CPropertyBagPtr ptrBag);
	/// restore configuration from a file
	void RestoreConfiguration(const CString fileName) {   RestoreConfiguration(CIniFilePropertyBag::Open(fileName, "HVDevice"));}

	//get and set the frequence of the clock
	void SetClockFreq(long lFreq){m_nClockFreq = lFreq;}
	long GetClockFreq(){return m_nClockFreq;}

	
	HV_VIDEO_MODE  GetVideoMode()	//分辨率大小 mode0，mode1... add by xupx 2008.0.09
	{
		HVAPI_CONTROL_PARAMETER  p;
		HVSTATUS status;
		CString str;
		int	nValue;
		
		HV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE ResFeature;
		p.pOutBuf = &ResFeature;
		p.dwOutBufSize = sizeof(ResFeature);
		p.pBytesRet = NULL;		
		p.code = ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE;
		
		status = HVCommand(m_hDevice, CMD_HVAPI_CONTROL,&p,0);
		nValue = ResFeature.VideoMode;
		
		if ( ! HV_SUCCESS(status))
			return (HV_VIDEO_MODE)0;		// 2009.07.30 因为现在GIGE相机不支持此读取，为了避免报错所以返回0.
//			throw HVBaseException(status, "HVDAILT.dll : HVCommand (GetVideoMode)");
		
		return (HV_VIDEO_MODE)nValue;
	}

	int  GetParameter(int Id, int subID)		//add by xupx 2008.09.09
	{
		HVAPI_CONTROL_PARAMETER  p;
		HVSTATUS status = STATUS_INTERNAL_ERROR;
		CString str;
		int	nValue;
		
		HV_ARG_GET_FEATURE GetFeature;
		HV_RES_GET_FEATURE ResFeature;
		p.pInBuf = &GetFeature;
		p.dwInBufSize = sizeof(GetFeature);	
		p.pOutBuf = &ResFeature;
		p.dwOutBufSize = sizeof(ResFeature);
		p.pBytesRet = NULL;		
		p.code = ORD_GET_FEATURE;

		switch(Id) {
		case IDS_SNAPMODE:		//采集模式			
			{
				GetFeature.FeatureId = FEATURE_ID_TRIGGER;
				status = HVCommand(m_hDevice,CMD_HVAPI_CONTROL,&p,0);
				/*********** modified by xupx 2009.10.30 **********/
				if (ResFeature.Trigger.OnOff)
				{
					nValue = ResFeature.Trigger.Mode +1 ;
				}
				else
					nValue = 0;
			}
			break;
		case IDS_TRIGGERPOLARITY://外触发信号极性
			{
				GetFeature.FeatureId = FEATURE_ID_TRIGGER;
				status = HVCommand(m_hDevice,CMD_HVAPI_CONTROL,&p,0);
				nValue = ResFeature.Trigger.Polarity;
			}
			break;
		case IDS_STROBEPOLARITY:
			{
				HV_ARG_GET_STROBE GetStrobeFeature;
				HV_RES_GET_STROBE ResStrobeFeature;
				p.pInBuf = &GetStrobeFeature;
				p.dwInBufSize = sizeof(GetStrobeFeature);
				p.pOutBuf = &ResStrobeFeature;
				p.dwOutBufSize = sizeof(ResStrobeFeature);
				p.pBytesRet = NULL;		
				p.code = ORD_GET_STROBE;

				GetStrobeFeature.StrobeId = STROBE0;
				status = HVCommand(m_hDevice,CMD_HVAPI_CONTROL,&p,0);
				if(subID == 0)	//读取OnOff状态
					nValue = ResStrobeFeature.OnOff;
				else			//读取闪光灯极性
					nValue = ResStrobeFeature.SignalPolarity;
				break;
			}
		default:
			throw HVBaseException(status, "GetParameter invalidate parameter");
			break;
		}
		return nValue;
	}
	
    void SetPacketSize(int value) {
		
		HV_CMD_PACKET_SIZE pz;
		pz.dwSize=value;
		HVSTATUS status = HVCommand(m_hDevice,CMD_SET_BYTE_PER_PACKET,&pz,0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVCommand");
		
    }

    void GetPacketSize(int* CurrentValue, int* Max, int* Min) {
        //::HV1394GetPacketInfo(m_hDevice, Max, Min, CurrentValue);
    }

    void Set_Brightness() {
		HVSTATUS status = HVADCControl(m_hDevice, ADC_BRIGHTNESS, m_Brightness);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVADCControl");
    }

    void SetShutterUnit(int value) {
		HVSTATUS status = HVAECControl(m_hDevice, AEC_SHUTTER_UNIT, (SHUTTER_UNIT_VALUE)value);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVAECControl");
    }

	void Set_WB_Red(int r){
		if(!IS_READABLE_CAM(m_pInfo->DeviceType()))
		{
			if(r < 0 || r > 63)		r = 46;
		}

		HVSTATUS status = HVAGCControl(m_hDevice, AGC_GAIN_RED_CHANNEL, r);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVAGCControl");

	}
	void Set_WB_Blue(int b){
		
		if(!IS_READABLE_CAM(m_pInfo->DeviceType()))
		{
			if(b < 0 || b > 63)		b = 60;
		}

		HVSTATUS status = HVAGCControl(m_hDevice, AGC_GAIN_BLUE_CHANNEL, b);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVAGCControl");

	}
	void Set_WB_Green(int b){

		if(!IS_READABLE_CAM(m_pInfo->DeviceType()))
		{
			if(b < 0 || b > 63)		b = 32;
		}
		HVSTATUS status = HVAGCControl(m_hDevice, AGC_GAIN_GREEN_CHANNEL1, b);
		status = HVAGCControl(m_hDevice, AGC_GAIN_GREEN_CHANNEL2, b);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVAGCControl");
		
//		HVAPI_CONTROL_PARAMETER  p;
//		p.code = ORD_SET_ADV_FEATURE;
//		
//		HV_ARG_SET_ADV_FEATURE ArgSetAdvFeature;
//		ArgSetAdvFeature.AdvFeatureId = ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN;
//		ArgSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GR_VALUE; 
//		ArgSetAdvFeature.AdvFeature.PixelSeparate.GrValue = b;
//
//		p.pInBuf = &ArgSetAdvFeature;
//		p.dwInBufSize = sizeof(ArgSetAdvFeature);
//		
//		HVSTATUS status = HVCommand(m_hDevice, CMD_HVAPI_CONTROL,&p,0);
//		ArgSetAdvFeature.AdvFeature.PixelSeparate.GbValue = b;
//		ArgSetAdvFeature.AdvFeatureItemId = ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GB_VALUE; 
//		status = HVCommand(m_hDevice, CMD_HVAPI_CONTROL,&p,0);
//		if ( ! HV_SUCCESS(status))
//			throw HVBaseException(status, "HVDAILT.dll : HVAGCControl");
		
	}

	void LUT_Def()
	{
		HHV hhv = GetHandle();
		DWORD	 m_dwLut[4*1024] = {0};
		DWORD *p1 = m_dwLut;
		int n=0;
		for(int i=0; i<1024; i++)
		{
			n = 4 *i;
			if(i > 255)
			{
				(DWORD&)m_dwLut[n]	= 0x000000FF;
				(DWORD&)m_dwLut[n+1]	= 0x000000FF;
				(DWORD&)m_dwLut[n+2]	= 0x000000FF;
				(DWORD&)m_dwLut[n+3]	= 0x000000FF;
			}
			else
			{
				(WORD&)m_dwLut[n]	= 0x00000000 | i;
				(WORD&)m_dwLut[n+1]	= 0x00000000 | i;
				(WORD&)m_dwLut[n+2]	= 0x00000000 | i;
				(WORD&)m_dwLut[n+3]	= 0x00000000 | i;
			}
		}
		
		/************ 加载 LUT ***************/
		if(IS_GIGE(m_pInfo->DeviceType()) || IS_GM_CAM(m_pInfo->DeviceType()))
		{
            LoadLut(m_dwLut, 16*1024,0);
			return;
		}

		HVSTATUS status;	
		HVAPI_CONTROL_PARAMETER  p;	
		HV_ARG_SET_ADV_LUT setFeature;
		setFeature.dwOffset = 0;
		setFeature.nLength = 16*1024;
		setFeature.pBuffer = m_dwLut;
		
		p.pInBuf = &setFeature;
		p.dwInBufSize = sizeof(setFeature);
		p.code = ORD_SET_ADV_LUT;		
		
		status = HVCommand(hhv, CMD_HVAPI_CONTROL, &p, 0);
	}
    void LoadLut(DWORD* ptr,int nLong,int nOffset) {
   //     HV1394LoadLUT(m_hDevice, ptr, nLong);
		HV_CMD_LUT_DWENTRIES lut;
		lut.dwEntries=nLong;
		lut.dwOffset=nOffset;
		lut.pBuffer=ptr;
		HVSTATUS status = HVCommand(m_hDevice,CMD_SET_LUT_DWENTRIES,&lut,0);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVCommand");
    }
    void SetGain(int value) {
		HVSTATUS status = HVAGCControl(m_hDevice, AGC_GAIN, value);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVAGCControl");
    }

    CSize GetImageSize() {
        return m_ImageSize;
    }

	
    void SoftTrigger() {
        HVSTATUS status = ::HVTriggerShot(m_hDevice);
		if ( ! HV_SUCCESS(status))
			throw HVBaseException(status, "HVDAILT.dll : HVTriggerShot");
    }
	
    void Set_Frame_Frozen(int index) {
		m_FrameFrozen=index;
		if(index==1){			
			HVSTATUS status = HVCommand(m_hDevice,CMD_FRAME_SHOT,0,0);
			if ( ! HV_SUCCESS(status))
				throw HVBaseException(status, "HVDAILT.dll : HVCommand");
		}
		if(index==0){			
			HVSTATUS status = HVCommand(m_hDevice,CMD_FRAME_SHOT_FINISH,0,0);
			if ( ! HV_SUCCESS(status))
				throw HVBaseException(status, "HVDAILT.dll : HVCommand");
		}
		
    }

	struct TRIGGERSOURCE {
		int Source0Inq;
		int Source1Inq;
		int Source2Inq;
		int Source3Inq;
	};
	TRIGGERSOURCE m_TriggerSourceInq;

	struct  RANGEVAL{
		int maxV;
		int minV;
	};

	RANGEVAL	m_GainRange;
	RANGEVAL	m_RGainRange;
	RANGEVAL	m_ShutterRange;
	RANGEVAL	m_SeperateGain_Range;
	RANGEVAL	m_RBRange;
	RANGEVAL	m_G1G2Range;
	RANGEVAL	m_WB_ValueRange;
	RANGEVAL	m_BrightnessRange;
	RANGEVAL	m_TriggerDelayRange;
	RANGEVAL	m_MultiTriggerNumRange;
	RANGEVAL	m_MultiTriggerTimeRange;
	RANGEVAL	m_PacketDelayRange;
	RANGEVAL	m_ExpectDataRateRange;
	RANGEVAL	m_ACPhaseAdjustRange;

	// 功能查询
	int				m_flagSeparateGain;
	int				m_flagAutoGain;
	int				m_flagWB;
	int				m_flagPassive;
	int				m_flagTrigger;
	int				m_flagTriggerDelay;
	int				m_flagTriggerDelayUnit;

	int				m_flagFilterUnit;
	int				m_flagFilterTime;

	int				m_flagDigitalGain;
	int				m_flagLUT;
	int				m_flagAutoShutter;
	int				m_flagAutoStrobe;
	int				m_flagMultiTrigger;
	int				m_flagAntiFlicker;
	int				m_flagACPhaseAdjust;
	int				m_flagGlobalBlk;
	//-------
	ULONG    m_FrameFrozen;
	ULONG    m_StrobeOnOff;
    ULONG    m_nTestImage;
	BOOL     m_bTriggerDelayOnOff;
	ULONG    m_nTriggerDelayValue;
	int		 m_TriggerDelayUnit;
// SV1410GC 2000GC
	ULONG	 m_nBeginParam;		//启动参数组  by xupx 0520
	ULONG	 m_nSaveParam;		//保存参数    xupx 0520
	ULONG	 m_nLoadParam;		//加载参数    xupx 0520
	
	ULONG	 m_nPacket1OverTime;	//第一个包超时 xupx1410GC
	ULONG	 m_nPacketDelay;		//包间延迟
	int		 m_nConnectSpeed; 
	ULONG	 m_nDigitalGainItem;		//xupx1410GC	0521
	int		 m_bDigitalGainInq;
	ULONG	 m_nHeartInterval;		//心跳间隔
	int		 m_nDataMode;			//数据格式选择 Raw8、Raw10……
	ULONG	 m_nHeartExpiration;
	int		 m_nSelectMode;
	int		 m_bShutterAuto;		//快门自动模式
	int		 m_bShutterAutoInq;		//查询自动功能是否可用
	int		 m_bGainAuto;
	int		 m_bGainAutoInq;		//查询自动功能是否可用
	int		 m_bStrobeAuto;
	int		 m_bStrobeAutoInq;
	int		 m_FilterUnitInq;
	int		 m_FilterTimeInq;
	int		 m_AntiFilterInq;

	int		 m_PacketDelay;
	int		 m_ExpectDataRate;
	int		 m_DelayResult;
	int		 m_PassiveTransOnOff;
	int		 m_PassiveTransBufNum;

	HV_COLOR_CODE	m_ColorCode;
	map<CString, int> m_ColorCodeMap;
	map<HV_COLOR_CODE, int> m_ColorCodeIndex;

	BOOL	 m_bInit;				//已经打开过一次相机 xupx0710
	BOOL	 m_bGetPresence;
	
	HV_RES_QUERY_FEATURE_PRESENCE		resPresence;
	HV_RES_QUERY_ADV_FEATURE_PRESENCE	resAdvPresence;
	HV_RES_QUERY_AUTO_FUNC_PRESENCE		resAutoPresence;
	HV_RES_QUERY_ADV_OPT_FUNC_PRESENCE	resAdvFunPresence;
	HV_RES_QUERY_PIO_OUTPUT_PORT_PRESENCE resPIOPresence;

	HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR		m_res_Output_Descriptor;
	HV_RES_QUERY_FEATURE_DESCRIPTOR				m_resFeatureDes;

	struct PIO_OUTPUT_DESC {
		HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR Channel0;
		HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR Channel1;
		HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR Channel2;
	} m_OutputIO_Desc;
	
	// 用于查询所支持的ColorMode
	HV_RES_QUERY_FMT7_MODE_DESCRIPTOR	m_resFMT7Descriptor; 
	// Version info add by xupx0710
	CString			m_strNodeID;
	CString			m_ModeName;
	CString			m_VendorName;
	CString			m_strFPGAVer;
	CString			m_strHWVer;
	CString			m_strSWVer;
	CString			m_strAPIVer;
	CString			m_strDailtVer;
	CString			m_strIP;
	CString			m_strMAC;
	CString			m_strHostIP;
	CString			m_strHostMAC;
	CString			m_strColorMode;
	int				m_TriggerNum;
	int				m_TriggerTime;
	int				m_MultiTriggerOnOffInq;
	int				m_TriggerDelayStateDescrip;
	int				m_TriggerDelayValDescrip;
	int				m_CurrentModeIdx;

	HVTYPE			m_CamType;
	HVTYPE			m_HVTYPE;
	int				m_HardwareID;

	ULONG    m_nTransfersDelayValue;
	ULONG    m_nFilterTime;
	ULONG    m_nFilterTimeUnit;
	ULONG    m_nTriggerSource;
	BOOL     m_bRaw8or12;
	BOOL     m_bLeBe;
	CString  m_LutPath;  //保存LUT文件绝对路径
	//---------------
    ULONG    m_nOutPutIO_0;
	ULONG    m_nOutPutIO_0_POL;
	ULONG    m_nOutPutIO_1;
	ULONG    m_nOutPutIO_1_POL;
	ULONG    m_nOutPutIO_2;
	ULONG    m_nOutPutIO_2_POL;
//---------------
	unsigned long m_ShutterSpeed;
	int			  m_ShutterUnit;
    ULONG    m_PacketSize;
    ULONG    m_Brightness;
	ULONG	 m_Gain;
    ULONG    m_BlueGain;
    ULONG    m_RedGain;
	ULONG	 m_GreenGain;		//1410GC
	// WB白平衡
	int		 m_WB_OnOff;
	int		 m_WB_AutoManual;
	int		 m_WB_OnePush;
	int		 m_WB_OnOffInq;
	int		 m_WB_AutoInq;
	int		 m_WB_OnePushInq;
	int		 m_R_Value;
	int		 m_B_Value;
	int		 m_G_Value;
	

	int		 m_CurrentSnapMode;
	int		 m_CurrentTriggerPolarity;
	int		 m_CurrentStrobePolarity;
	int		 m_AntiFilterState;
	int		 m_AntiFilterSource;
	int		 m_ACPhaseAdjust;


    int      m_bFirstImage;   // Does this is the first snap image?
	DWORD	 m_dwLut[4*1024];

	DWORD   m_dwImageSize;	// add by xupx 2009.07.07
	BOOL m_bOnProcessing;
	/// Supply information about the camera and the driver
	class CInfo
	{
	protected:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange

	public:
		CInfo(CDevice *pDevice) :
		  m_pDevice(pDevice)
		{}
		CString VendorName() 
		{
			//return "Daheng Imavision";
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_VENDOR,lpTemp,MAX_PATH);		  
			CString str;
			str.Format("%s", lpTemp);
			return str;
		} 
		int DeviceNumber() {return m_pDevice->m_nNumber;}
		CString NodeId() 
		{
			char info[256] = {0};
			int size = 0;
			CString str;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_SERIESNUM, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_SERIESNUM, info, &size);
			str.Format("%s",info);
			return str;
		}
//PD1420
		CString ColorMode()
		{
			//char mode[32];
			int mode1=0;
			CString str;
			int size = 0;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_BAYER_LAYOUT, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_BAYER_LAYOUT, &mode1, &size);
			//str.Format("%s",mode);
			if(mode1 == 4)
				str.Format("RGGB");
			if(mode1 == 1)
				str.Format("GBRG");
			if(mode1 == 2)
				str.Format("GRBG");
			if(mode1 == 3)
				str.Format("BGGR");

			return str;
		}

		CString DeviceTypePD1420()
		{
			char type[32] = {0};
			int size = 0;
			
			m_pDevice->GetDeviceInfo(DESC_DEVICE_TYPE, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_TYPE, type, &size);
			
			return type;					
		}


//PD1420end
		CString FPGAVersion1410GC() 
		{
			char info[256] = {0};
			int size = 0;
			CString str;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_FPGA_VERSION, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_FPGA_VERSION, info, &size);
			str.Format("%s",info);			
			return str;
		}

		CString HardwareVersion1410GC() 
		{
			char info[256] = {0};
			int size = 0;
			CString str;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_HARDWARE_VERSION, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_HARDWARE_VERSION, info, &size);

			str.Format("%s",info);	
			
			return str;			
		}


		int HardwareVersionID()
		{
			char info[256] = {0};
			int size = 0;
			CString str;
			int nValue;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_HARDWARE_VERSION, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_HARDWARE_VERSION, info, &size);

			int a, b, c, d;

			sscanf(info, "%d.%d.%d.%d", &a, &b, &c, &d);

			nValue = a*1000000 + b*10000 + c*100 + d;

			return nValue;

		}

		
		CString HardwareVersion() 
		{
			char info[256] = {0};
			int size = 0;
			int nVer = 0;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_HARDWARE_VERSION, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_HARDWARE_VERSION, &nVer, &size);
			//itoa(nVer, info, 10);
				
			//
            // CCD camera changed source.
            //
			if (IS_CCD_CAMERA(m_pDevice->m_pInfo->DeviceType()) || m_pDevice->m_pInfo->DeviceType() == DRVIF1394TYPE)
			{
				DWORD dwFPGAVer;
				m_pDevice->GetDeviceInfo(DESC_DEVICE_FPGA_VERSION, &dwFPGAVer, &size);
				int major = (nVer >> 16);
				int minor1 = (nVer & 0xFF00) >> 8;
				int minor2 = (nVer & 0xFF);
				sprintf(info, "%d.%d%d;%d", major, minor1, minor2,dwFPGAVer);
			}
			else if(IS_CMOS_1394_CAM(m_pDevice->m_pInfo->DeviceType()))
			{				
				int major = (nVer & 0xFF)/100;
				int minor1 = (nVer & 0xFF)%100/10; 
				int minor2 = (nVer & 0xFF)%10;
				
				sprintf(info, "%d.%d%d", major, minor1, minor2);
			}
			return info;
		}

		virtual CString DeviceName() =0;

		virtual CString ModelName() 
		{
			char info[256] = {0};
			int size = 0;
			CString str;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_NAME, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_NAME, info, &size);
			str.Format("%s",info);
			return str;
		}
		virtual HVTYPE DeviceHVTYPE() // = 0;
		{
			int deviceID;
			int size = 0;
			CString str;
			m_pDevice->GetDeviceInfo(DESC_DEVICE_TYPE, NULL, &size);
			m_pDevice->GetDeviceInfo(DESC_DEVICE_TYPE, &deviceID, &size);
			return (HVTYPE)deviceID;
		}

		virtual HVTYPE DeviceType() = 0;
		
	} ; ///< Device information object
	friend class CInfo;
	
	CInfo *m_pInfo;
	
	class CFormat{
		IMPLEMENT_THIS( CDevice, Format ); ///< This() returns pointer to outer class
	public:	
		HV_COLOR_CODE ColorMode() {return This()->m_ColorCode;}
		HV_VIDEO_MODE Resolution() {return This()->m_Resolution;}
		CSize ImageSize() {return This()->m_ImageSize;}
		CPoint ImagePosition() {return This()->m_Origin;}
		CSize SensorSize() {return This()->m_SensorSize;}
	} Format;
	friend class CFormat;
	
	
	/// Inquire, get and set all scalar properties.
	class CScalarProperty
	{
	protected:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		/// Different modes for CSR 
		enum Mode_t
		{
			mOnOff,
			mAutoMode,
			mAbsControl
		};
		
	public:
		
		CScalarProperty( bool fAbsControl, unsigned long Value, CDevice* pDevice ) :
			  m_fAbsControl(fAbsControl),
			  m_RawValue(0),
			  m_AbsValue(0),
			  m_pDevice(pDevice)
		  {	
			  if (m_fAbsControl)
				  m_AbsValue = Value;
			  else
				  m_RawValue = Value;
		  }
		  
		  virtual bool IsSupported() = 0;
		  virtual bool IsAbsControl() {return m_fAbsControl;}
		  
		  virtual unsigned long Min() = 0;
		  virtual unsigned long Max() = 0;
		  virtual unsigned long Value()
		  { 
			  if (m_fAbsControl)
				  return m_AbsValue;
			  else
				  return m_RawValue;
		  }
		  virtual void Refresh()
		  {
			  Set(Value());
		  }
		  
		  virtual void Set(unsigned long Value, void *pContext = NULL) = 0;
		  
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				if (m_fAbsControl)
					ptrBag->WriteLong("Value", m_AbsValue);
				else
					ptrBag->WriteLong("Value", m_RawValue);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				if (m_fAbsControl)
					m_AbsValue = ptrBag->ReadLong("Value");
				else
					m_RawValue = ptrBag->ReadLong("Value"); 
			}
		}
		
		friend class CDevice;
		
	protected:
		unsigned long m_RawValue;
		unsigned long m_AbsValue;
		const bool	  m_fAbsControl;
	};  // CScalarProperty
	
	class CGain : public CScalarProperty
	{
	public:
		CGain( unsigned long Value, CDevice* pDevice ) :
		  CScalarProperty(false, Value, pDevice)
		  {}	
		  
		  virtual void Set(unsigned long Value, void *pContext = NULL)
		  {
			  if (Value > Max() || Value < Min())
				  throw HVBaseException(STATUS_PARAMETER_INVALID, "CGain::Set()");
			  /*
			  
			  			  for (int Channel = RED_CHANNEL; Channel <= BLUE_CHANNEL; ++Channel )
			  			  {
			  				  m_pDevice->AGCControl(Channel, Value);
			  			  }*/
			  
			  m_pDevice->SetGain(Value);
			  m_RawValue = Value;
		  }
		  
	private:
		
	};
	friend class CGain;
	CScalarProperty *m_pGain;             ///< Gain feature
	
	class CShutter : public CScalarProperty
	{
	public:
		class CShutterUnit 
		{
		public:
			CShutterUnit (TUnit value = UNIT_MILLISECOND) 
			 :	m_Value(value)
			{
			}

			void Set (TUnit value) { m_Value = value; }
			TUnit Value() { return m_Value; }
			long Ratio() { return pow(1000, UNIT_SECOND - m_Value); }

			void Save(const CPropertyBagPtr ptrBag)
			{
				ptrBag->WriteLong("Unit", m_Value);
			}
			
			void Restore(const CPropertyBagPtr ptrBag)
			{
				m_Value = (TUnit)ptrBag->ReadLong("Unit");
			}

		private:
			TUnit m_Value;
		};

		CShutter( unsigned long Value, CDevice* pDevice ) :
		  CScalarProperty(true, Value, pDevice)
		  {}
		  
		  virtual void Set(unsigned long Value, void *pContext = NULL)
		  {
			  HV_AEC_CONTROL_CONTEXT* pAECContext=(HV_AEC_CONTROL_CONTEXT*)pContext;
			  /**** add by xupx 2008.08.11****/
			  HVTYPE type = m_pDevice->m_pInfo->DeviceType();
			  if(type == HV3000UCTYPE || type == HV3100FCTYPE)
			  {
				  m_pDevice->AECControl_HV3(pAECContext->ctrl, Value);
			  }
			  else
				  m_pDevice->AECControl(pAECContext->ctrl, Value);
			  /**** end****/
			  
			  m_RawValue = Value;
		  }
		  virtual unsigned long Raw() { return m_RawValue;}
			
		  virtual unsigned long Value()
		  { 
			  ULONG ulRet;
			  if (m_fAbsControl)
				  ulRet = m_AbsValue;
			  else
				  ulRet = m_RawValue;

			  ulRet = ulRet<Min()?Min():ulRet;
			  ulRet = ulRet>Max()?Max():ulRet;
			  return ulRet;
		  }

		protected:
		  void Save(const CPropertyBagPtr ptrBag)
		  {
				if ( IsSupported() )
				{
					m_Unit.Save(ptrBag);
					CScalarProperty::Save(ptrBag);
				}
		  }
			
		  void Restore(const CPropertyBagPtr ptrBag)
		  {
				if ( IsSupported() )
				{
					m_Unit.Restore(ptrBag);
					CScalarProperty::Restore(ptrBag);
				}
		  }

		  friend class CDevice;

	public:
		CShutterUnit m_Unit;
	};
	friend class CShutter;
	CShutter *m_pShutter;				  ///< Shutter feature

	class CBlackLevel : public CScalarProperty
	{
	public:
		CBlackLevel( unsigned long Value, CDevice* pDevice ) :
		  CScalarProperty(false, Value, pDevice)
		  {}	
		  
		  virtual void Set(unsigned long Value, void *pContext = NULL)
		  {
			  if ((long)Value > (long)Max() || (long)Value < (long)Min())
				  throw HVBaseException(STATUS_PARAMETER_INVALID, "CBlackLevel::Set()");
			  
			  for (int Channel = ADC_BLKLEVEL_CAL_REDCHANNEL; Channel <= ADC_BLKLEVEL_CAL_BLUECHANNEL; ++Channel )
			  {
				  m_pDevice->ADCControl(Channel, Value);
			  }
			  m_RawValue = Value;
		  }
		  
	private:
		
	};
	friend class CBlackLevel;
	CScalarProperty *m_pBlackLevel;       ///< BlackLevel feature

	class CSnapMode
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CSnapMode(int Value, CDevice* pDevice ) :
			m_SnapMode(Value),
			m_pDevice(pDevice)
		{}
		
		virtual bool IsSupported() = 0;
		int Value() {return m_SnapMode;}
		void Set(int Value)
		{
			BOOL b = FALSE;
			HVTYPE type = m_pDevice->m_pInfo->DeviceType();
			//----add by xupx for HV3100FC 2008.08.10----
			if(type == HV3000UCTYPE || type == HV3100FCTYPE )
			{
				if (m_pDevice->IsContinuousGrabActive())
				{
					m_pDevice->GrabCancel();
					b = TRUE;
				}

				m_pDevice->SetSnapMode((HV_SNAP_MODE)Value);

				if(b)
					m_pDevice->GrabContinuous();
			}
			//---end by xupx----
			else
			{
				m_pDevice->SetSnapMode((HV_SNAP_MODE)Value);
			}

			m_SnapMode = Value; 
		}
		
		virtual void Refresh()
		{
			Set(Value());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("Value", m_SnapMode);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_SnapMode = ptrBag->ReadLong("Value");
			}
		}
		
	protected:
		int m_SnapMode;
		friend class CDevice;
	};
	friend class CSnapMode;
	
	CSnapMode *m_pSnapMode;
	

	class CTriggerPolarity
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CTriggerPolarity(int Value, CDevice* pDevice ) :
			m_TriggerPolarity(Value),
			m_pDevice(pDevice)
		{}
		
		virtual bool IsSupported() = 0;
		int Value() {return m_TriggerPolarity;}
		void Set(int Value)
		{
			m_pDevice->SetTriggerPolarity((HV_POLARITY)Value);
			m_TriggerPolarity = Value; 
		}
		
		virtual void Refresh()
		{
			Set(Value());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("Value", m_TriggerPolarity);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_TriggerPolarity = ptrBag->ReadLong("Value");
			}
		}
		
	protected:
		int m_TriggerPolarity;
		friend class CDevice;
	};
	friend class CTriggerPolarity;
	
	CTriggerPolarity *m_pTriggerPolarity;
	

	class CStrobePolarity
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CStrobePolarity(int Value, CDevice* pDevice ) :
		m_StrobePolarity(Value),
			m_pDevice(pDevice)
		{}
		
		virtual bool IsSupported() = 0;
		int Value() {return m_StrobePolarity;}
		void Set(int Value)
		{
			m_pDevice->SetStrobePolarity((HV_POLARITY)Value);
			m_StrobePolarity = Value; 
		}
		
		virtual void Refresh()
		{
			Set(Value());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("Value", m_StrobePolarity);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_StrobePolarity = ptrBag->ReadLong("Value");
			}
		}
		
	protected:
		int m_StrobePolarity;
		friend class CDevice;
	};
	friend class CStrobePolarity;
	
	CStrobePolarity *m_pStrobePolarity;
	

	class CADCLevel
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CADCLevel(int Value, CDevice* pDevice ) :
			m_ADCLevel(Value),
			m_pDevice(pDevice)
		{}
		
		virtual bool IsSupported() = 0;
		int Value() {return m_ADCLevel;}
		void Set(int Value)
		{
			m_pDevice->ADCControl(ADC_BITS, (HV_ADC_LEVEL)Value);
			m_ADCLevel = Value; 
		}
		
		virtual void Refresh()
		{
			Set(Value());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("Value", m_ADCLevel);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_ADCLevel = ptrBag->ReadLong("Value");
			}
		}
		
	protected:
		int m_ADCLevel;
		friend class CDevice;
	};
	friend class CADCLevel;	

	CADCLevel *m_pADCLevel;


	class CSnapSpeed
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CSnapSpeed(int Value, CDevice* pDevice) :
			m_SnapSpeed(Value),
			m_pDevice(pDevice)
		{}
		
		virtual bool IsSupported() = 0;
		int Value() {return m_SnapSpeed;}
		void Set(int Value)
		{
			if(!IsSupported()) return;
			m_pDevice->SetSnapSpeed( (HV_SNAP_SPEED)Value);
			m_pDevice->SetClockFreq(Value == HIGH_SPEED? 24000000:12000000);
			m_pDevice->m_pShutter->Refresh();
			m_SnapSpeed = Value; 
		}
		
		virtual void Refresh()
		{
			Set(Value());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("Value", m_SnapSpeed);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_SnapSpeed = ptrBag->ReadLong("Value");
			}
		}
		
	protected:
		int m_SnapSpeed;
		friend class CDevice;
	};
	friend class CSnapSpeed;	
 
	
	class CBlackLevelEnable
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CBlackLevelEnable(int Value, CDevice* pDevice ) :
			m_BlackLevelEnable(Value),
			m_pDevice(pDevice)
		{}
		
		virtual bool IsSupported() = 0;
		int Value() {return m_BlackLevelEnable;}
		void Set(int Value)
		{
			m_pDevice->ADCControl(ADC_BLKLEVEL_CAL, Value);
			m_BlackLevelEnable = Value; 
		}
		
		virtual void Refresh()
		{
			Set(Value());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("Value", m_BlackLevelEnable);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_BlackLevelEnable = ptrBag->ReadLong("Value");
			}
		}
		
	protected:
		int m_BlackLevelEnable;
		friend class CDevice;
	};
	friend class CBlackLevelEnable;	
	
	CBlackLevelEnable *m_pBlackLevelEnable;

	class CBlank
	{
	public:
		CDevice * const m_pDevice;            ///< Backpointer for data exchange
		
		CBlank( int hor, int vert, CDevice* pDevice ) :
			m_Hor(hor),
			m_Vert(vert),
			m_pDevice(pDevice)
		  {}	
		  
		virtual bool IsSupported() = 0;
		int Hor() {return m_Hor;}
		int Vert() {return m_Vert;}

	private:
		int GetInfo(int index) 
		{
			int size = sizeof(DWORD) * 4;
			DWORD buffer[4] = {0};
			m_pDevice->GetDeviceInfo(DESC_DEVICE_BLANKSIZE, buffer, &size);
			return buffer[index];
		}

	public:
		int HorMin() { return GetInfo(0);}
		int VertMin() { return GetInfo(1);}
		int HorMax() { return  GetInfo(2);}
		int VertMax() { return GetInfo(3);}

		virtual void SetHor(int value)
		{
			m_pDevice->SetBlank(value, m_Vert);
			m_pDevice->m_pShutter->Refresh();
			m_Hor = value; 
		}
		virtual void SetVert(int value)
		{
			m_pDevice->SetBlank(m_Hor, value);
			m_pDevice->m_pShutter->Refresh();
			m_Vert = value; 
		}
		virtual void Refresh()
		{
			SetHor(Hor());
			SetVert(Vert());
		}
		
	protected:
		void Save(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				ptrBag->WriteLong("HorValue", m_Hor);
				ptrBag->WriteLong("VertValue", m_Vert);
			}
		}
		
		void Restore(const CPropertyBagPtr ptrBag)
		{
			if ( IsSupported() )
			{
				m_Hor = ptrBag->ReadLong("HorValue");
				m_Vert = ptrBag->ReadLong("VertValue");
			}
		}
		  
	protected:
		int m_Hor;
		int m_Vert;
		friend class CDevice;
	};
	friend class CBlank;

	CBlank *m_pBlank;

	//
	typedef map<int, HV_VIDEO_MODE>         Idx2Mode_t;
	Idx2Mode_t DeviceModeMap();
	std::list<CString> DeviceModeList();
	
	protected:
		CSize Get_Sensor_Size(HV_VIDEO_MODE mode);	
		
	private:
		// hide copy constructor and assignment operator
		CDevice( const CDevice& );
		CDevice& operator=( const CDevice& );
		
		/// Initialize Device 
		void InitializeDevice(); 
		/// To be called before a continuous grab is started
		void PrepareContinuousGrab();
		/// Allocate n new buffers
		void CreateNewBuffers(unsigned long n);
		/// Release Buffers
		void ReleaseBuffers();
		/// Tell the display thread to suspend processing of buffers
		DWORD SuspendDisplayThread();
		/// Callback. It is called if an error occured in one of the threads
		void OnContinuousGrabError(DWORD err);
		/// Cancel an active continuous grab
		DWORD CancelContinuousGrab();
		
		// Refresh the cached information and notify clients about changes
		void Refresh();
		void ShowFrame();
		/// Continuous snap callback procedure
		static int CALLBACK SnapCallbackProc(HV_SNAP_INFO *pInfo);
		static int CALLBACK SnapJPEGCallback(HV_CALLBACK_INFO *pInfo);  ///English:    ///Chinese: 回调函数声明
		
		
	protected:
		/// Device number
		int		            m_nNumber;
		//////////////////////////////////////////////////////////////////////////
		int					m_nIndex;
		/// Window handle to receive device notifications
		HWND                m_hWndNotify;
		
		/// number of buffers we use for image acquisition
		int                 m_cBuffers;       
		/// array of buffers to be filled by the DMA
		CHVBitmap**			m_ppBuffers;    
		/// bitmap to display. 
		CHVBitmap*			m_pBitmap; 
		/// 
		CHVBitmap*			m_pCurrentBuffer;
		/// the next buffer to be processed by the display thread
		CHVBitmap*		    m_pNextBuffer;	
		/// Critical section to synchronize access to m_pNextBuffer
		CCriticalSection	  m_CritSect;
		/// Size of max
		CSize               m_SensorSize;
		/// Size of AOI
		CSize               m_ImageSize;
		/// Origin of AOI
		CPoint              m_Origin;
		/// Current color mode
//		HV_COLOR_CODE		m_ColorCode;
//		HV_COLOR_CODE		m_ColorCode_Readable;
		///
		HV_VIDEO_MODE		m_Resolution;
		//////////////////////////////////////////////////////////////////////////


		/// does the bitmap match the current camera configuration?
		bool                m_fBitmapValid;
		/// flag to signal if we are currently grabbing in continuous mode
		bool                m_fContinuousGrabActive;  
		/// flag to signal if a single grab is pending
		bool                m_fSingleGrabActive;

		// if m_fSuspendGrab is true, the acquisition thread won't queue in any buffers
		bool                m_fSuspendGrab;
		/// is the display thread processing image buffer(s)?
		bool				m_fIsDisplayThreadActive;
		/// is AOI maximize 
		bool				m_fScalable;
		
		/// timer to measure the time needed to display one buffer
		CStopWatch          m_DisplayWatch;              
		/// Moving average of time needed to display one buffer
		CMovingAvg<double, 20>  m_DisplayAvg;
		/// MDI child window which will display our image data
		CChildFrame*        m_pChildFrame;
		/// Reference to main frame 
		CMainFrame&         m_MainFrame;
			
		/// shall we convert MONO8 images to RGB ( Bayer -> RGB conversion )
		int                 m_DisplayMode;

		long                m_nClockFreq; //the frequence of the clock
		
		/// 
		static long  s_cGrabsActive;
	
		//	friend class CImageView;
		friend class CMainFrame;
		friend class CChildFrame;
		friend class CDeviceManager;
		friend class CPluginManager;
};

#endif // !defined(AFX_DEVICE_H__A6B47285_FBD5_4E5C_8939_56369F355777__INCLUDED_)



















