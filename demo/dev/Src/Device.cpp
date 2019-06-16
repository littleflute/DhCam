// Device.cpp: implementation of the CDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Device.h"
#include "ChildFrm.h"
#include "mainfrm.h"

#define NEARLYINFINITE 10000

// initialize static members
long CDevice::s_cGrabsActive = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDevice::CDevice(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame) : 
m_nNumber(nNumber), 
m_nIndex(nIndex),
m_hWndNotify(hWndNotify), 
m_ppBuffers(NULL),
m_pBitmap(NULL),
m_pCurrentBuffer(NULL),
m_pNextBuffer(NULL),
m_fBitmapValid(false),
m_SensorSize(0,0),
m_ImageSize(400,300),//(1392,1040),//1410Test ///must have default value
m_Origin(0,0),
m_ColorCode(COLOR_MONO8),

m_DisplayMode(1),
m_Resolution((HV_VIDEO_MODE)VIDEO_MODE0),
m_pInfo(NULL),
m_pShutter(NULL),
m_pGain(NULL),
m_pSnapMode(NULL),
m_pTriggerPolarity(NULL),
m_pStrobePolarity(NULL),
m_pADCLevel(NULL),
m_pBlackLevelEnable(NULL),
m_pBlank(NULL),
m_pBlackLevel(NULL),
m_fContinuousGrabActive(false),
m_fSingleGrabActive(false),
m_fSuspendGrab(false),
m_fScalable(true),
m_nClockFreq(24000000),
m_pChildFrame(NULL),
//ccd

m_StrobeOnOff(1),
m_nTestImage(0),
m_bRaw8or12(0),	
m_nDataMode(0),	//数据格式选项
m_bLeBe(1),
m_bTriggerDelayOnOff(0),
m_nTriggerDelayValue(1),
m_TriggerDelayUnit(0),
m_nSelectMode(1),	//xupxPD1420模式选择
m_nTransfersDelayValue(0),
m_nFilterTimeUnit(0),
m_nFilterTime(10),
m_nOutPutIO_0(3),
m_nOutPutIO_1(1),
m_nOutPutIO_2(0),
m_nOutPutIO_0_POL(0),
m_nOutPutIO_1_POL(0),
m_nOutPutIO_2_POL(0),
m_nTriggerSource(0),
m_LutPath(""),

m_PacketSize(4096),
m_FrameFrozen(0),
m_Brightness(16),
m_BlueGain(0),
m_RedGain(0),
m_WB_OnOffInq(0),
m_WB_OnOff(0),
m_WB_AutoInq(0),
m_WB_AutoManual(0),
m_WB_OnePush(0),
m_WB_OnePushInq(0),
m_R_Value(0),
m_B_Value(0),
m_G_Value(0),

//1410
m_AntiFilterInq(0),
m_AntiFilterState(0),
m_AntiFilterSource(0),

m_GreenGain(60),
m_bShutterAuto(0),			//快门自动模式
m_bShutterAutoInq(0),
m_bGainAuto(0),				//增益自动模式
m_bGainAutoInq(0),
m_nPacketDelay(0),			//包间延迟
m_nDigitalGainItem(0),		//数字增益 0-1 1-2 2-4 3-8 4-16 1410GC
m_nConnectSpeed(1),		//1000 MB
m_nHeartInterval(1500),		//心跳间隔
m_nHeartExpiration(3000),		//心跳期满
m_bInit(FALSE),
m_flagSeparateGain(0),
m_flagAutoGain(0),
m_flagWB(0),
m_flagDigitalGain(0),
m_bDigitalGainInq(0),
m_flagPassive(0),
m_flagLUT(0),
m_flagAutoShutter(0),
m_flagAutoStrobe(0),
m_flagMultiTrigger(0),
m_MultiTriggerOnOffInq(0),
m_flagTrigger(0),
m_flagTriggerDelay(0),
m_flagTriggerDelayUnit(0),
m_TriggerDelayStateDescrip(0),
m_TriggerDelayValDescrip(0),
m_flagAntiFlicker(0),
m_flagACPhaseAdjust(0),
m_flagGlobalBlk(0),
m_ACPhaseAdjust(0),
m_bStrobeAuto(0),
m_bStrobeAutoInq(0),
m_bGetPresence(FALSE),
//m_nColorCode(COLOR_RAW8),
//ccd.
m_bOnProcessing(FALSE),
m_PassiveTransBufNum(0),
m_PassiveTransOnOff(0),
m_MainFrame(MainFrame)
{
	// 2009.06.30
	ZeroMemory(&m_resFMT7Descriptor.ColorMode, sizeof(m_resFMT7Descriptor.ColorMode));
		
	DWORD* pNew=m_dwLut;
   
    BYTE byteV=0;
	BYTE *pBt=NULL;
/*	
	for(int i=0;i<256;i++)
	{
		*pNew=0;
		pBt=(BYTE*)pNew;
		pBt[3]=byteV++;
		pNew++;
	}
	for( i=256;i<4*1024;i++)
	{
		*pNew=0;
		pBt=(BYTE*)pNew;
		pBt[3]=255;
		pNew++;
	}
*/

    /*
	double dTemp = 0.0;
	for(int i = 0; i < 4096; i++)		
    {
		*pNew=0;
		pBt=(BYTE*)pNew;
        dTemp = ((double)i) / ((double)(4095));
		
        dTemp = pow(dTemp, 0.4546); 
		
        pBt[3] = (DWORD)(dTemp * 255); 
		pNew++;
		
    }*/
    DWORD dwV=0x00;
	for(int i=0;i<256;i++)      //初始化查找表数据为4-11
	{
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV<<24;
		*pNew++=dwV++<<24;
	}

	resPIOPresence.Port0 = 0;
	resPIOPresence.Port1 = 0;
	resPIOPresence.Port2 = 0;

	m_OutputIO_Desc.Channel0.SourceSelect.StroboscopeInq = 0;
	m_OutputIO_Desc.Channel1.SourceSelect.StroboscopeInq = 0;
	m_OutputIO_Desc.Channel2.SourceSelect.StroboscopeInq = 0;
}

CDevice::~CDevice()
{
	try
	{
		// If we are still grabbing, cancel the grabbing
		if ( IsGrabActive() )
		{
			GrabCancel();
		}
	}
	catch ( HVBaseException& e)
	{
		m_MainFrame.ReportError(e);
	}
	
	// Free memory buffers 
	ReleaseBuffers();

	if ( m_pBitmap != NULL )
		delete m_pBitmap;

	Deactivate();
}	




//------------------------------------------------------------------------------
// CDevice::Activate()
// Author: 
//------------------------------------------------------------------------------
/**
* Open the device driver and do some initialization
* 
* \return void
*
*/
//------------------------------------------------------------------------------
void CDevice::Activate(bool bInit)
{
	{
		if ( ! IsOpen() )
		{
			CHVBase::Open(m_nNumber, m_hWndNotify);
			
			//add by xupx 2009.04.02
			try
			{
				Set_Default_AOI();
				CPropertyBagPtr ptrBag = HVDevice::CRegistryPropertyBag::Open(APPLICATION_KEY + m_pInfo->DeviceName() + "\\Device");
				RestoreConfiguration(ptrBag);
			}
			catch ( HVBaseException& e)
			{
				if ( e.Error() != STATUS_INTERNAL_ERROR )
					m_MainFrame.ReportError(e);
			}
			
			if ( bInit)
				InitializeDevice();
			
			// Refresh the cached information and notify clients of changes
			Refresh();
			
		}
	}
}



//------------------------------------------------------------------------------
// void CDevice::Deactivate()
// Author: 
//------------------------------------------------------------------------------
/**
* Close the device driver and do some cleanup
*
* \return void
*
*/
//------------------------------------------------------------------------------
void CDevice::Deactivate()
{
	if ( IsOpen() ){
		CHVBase::Close();
	}
}



//------------------------------------------------------------------------------
// CDevice::InitializeDevice()
// Author: 
//------------------------------------------------------------------------------
/**
*  Setup our CDevice object and initialze camera.
*  The display thread and the acquisition thread will be created
*
* \return void
*
*/
//------------------------------------------------------------------------------
void CDevice::InitializeDevice()
{
	m_HVTYPE		= m_pInfo->DeviceHVTYPE();	
	if(IS_PD_CAM(m_pInfo->DeviceType()))	//PD1420 add by xupx 2008.06.26
	{
		m_Resolution = GetVideoMode();
		m_ColorCode = (HV_COLOR_CODE)GetFMT7Mode(m_Resolution, FMT7_MODE_FUNC_ID_COLOR_CODING, NULL); 
//		SetResolution(m_Resolution);
		m_SensorSize = Get_Sensor_Size(m_Resolution);
		m_ImageSize = m_SensorSize;			//使显示窗口与sensor窗口一致，防止注册表被改写
		
	}
	// add by xupx for SV1410GC 2008.06.27 用于设置为最大分辨率
	if(m_pInfo->DeviceType()== DRVIFGIGETYPE ||  m_pInfo->DeviceType() == DRVIF1394TYPE)		
	{
		/************ add by xupx 2009.10.18**********/
		if (IS_HV_CAM(m_HVTYPE))
		{
			m_Resolution = GetVideoMode();
			SetResolution(m_Resolution);
			m_SensorSize = Get_Sensor_Size(m_Resolution);
			HV_RES_GET_FMT7_MODE res;
			GetFMT7Mode(m_Resolution, FMT7_MODE_FUNC_ID_AOI, &res);
			m_Origin.x = res.AOI.Left;
			m_Origin.y = res.AOI.Top;
			m_ImageSize.cx = res.AOI.Width;
			m_ImageSize.cy = res.AOI.Height;
			SetOutputWindow(m_Origin.x, m_Origin.y, m_ImageSize.cx, m_ImageSize.cy);
			return;
		}
		/****************************/
 		m_SensorSize= Get_Sensor_Size(VIDEO_MODE0);		//xupxTest0731
		if(IS_GIGE(m_pInfo->DeviceType()) || IS_GM_CAM(m_pInfo->DeviceType()))
		{
			m_Resolution = GetVideoMode();			// 2009.11.05
			m_ColorCode = (HV_COLOR_CODE)GetFMT7Mode(m_Resolution, FMT7_MODE_FUNC_ID_COLOR_CODING, NULL);			// add by xupx 2009.02.26
		}
		m_ImageSize = m_SensorSize;
	}
	else		//普通1394相机
	{
		SetResolution(m_Resolution);
		m_SensorSize = Get_Sensor_Size(m_Resolution);
// 		m_ImageSize = m_SensorSize;
		
		SetOutputWindow(m_Origin.x, m_Origin.y, m_ImageSize.cx, m_ImageSize.cy);
	}
	// 初始化时最大的方法
	//MaximizeAOI();

}



std::list<CString> CDevice::DeviceModeList()
{
	std::list<CString> DeviceModes;
	BYTE buffer[256] = {0};
	int size = 0;

	GetDeviceInfo(DESC_RESOLUTION, NULL, &size);
	GetDeviceInfo(DESC_RESOLUTION, buffer, &size);
	int count = size /  (2 * sizeof(DWORD));
	DeviceModes.clear();
	CString strDesc;
	for (int i = 0; i < count; ++i)
	{
//		strDesc.Format("mode %d ", i);
		strDesc.Format("mode %d (%d * %d)", 
			i, *((DWORD *)buffer + 2 * i), *((DWORD *)buffer + 2 * i + 1));
		DeviceModes.push_back(strDesc);
	}
	
	return DeviceModes;
}



CDevice::Idx2Mode_t CDevice::DeviceModeMap()
{
	CDevice::Idx2Mode_t DeviceModes;
	BYTE buffer[256] = {0};
	int size = 0;
	GetDeviceInfo(DESC_RESOLUTION, NULL, &size);
	int count = size /  (2 * sizeof(DWORD));
	DeviceModes.clear();
	for (int i = 0; i < count; ++i)
		DeviceModes.insert(Idx2Mode_t::value_type(i, (HV_VIDEO_MODE)i));
	
	return DeviceModes;
}



CSize CDevice::Get_Sensor_Size(HV_VIDEO_MODE mode) 
{
	CSize sz;
	BYTE buffer[256] = {0};
	int size = 0;
	GetDeviceInfo(DESC_RESOLUTION, NULL, &size);
	GetDeviceInfo(DESC_RESOLUTION, buffer, &size);
	int count = size /  (2 * sizeof(DWORD));
	if ( mode >= count )
		throw HVBaseException(STATUS_PARAMETER_INVALID, "CDevice::SensorSize()");
	sz.cx = *((DWORD *)buffer + 2 * mode);
	sz.cy = *((DWORD *)buffer + 2 * mode + 1);
	
	return sz;
}


//------------------------------------------------------------------------------
// void CDevice::SetMDIChild(CChildFrame* pChild)
// Author: 
//------------------------------------------------------------------------------
/**
* Tell the Device object which is its assoicated MDI child window
*
* \param     pChild
* \return    void
*
*/
//------------------------------------------------------------------------------
void CDevice::SetMDIChild(CChildFrame* pChild)
{ 
	m_pChildFrame = pChild; 
	if ( pChild != NULL )
	{
//		if(m_pInfo->DeviceType() == SV1410GCTYPE)
//		{
//			m_SensorSize.cx = 1392;
//			m_SensorSize.cy = 1040;//= Get_Sensor_Size(0);//xupxTest0606
//		}
		// Inform the the image view about the current video format and AOI dimensions
		m_pChildFrame->m_View.ConfigurationChanged(m_Resolution, m_SensorSize, m_ImageSize, m_Origin); 
	}
}


//------------------------------------------------------------------------------
// void CDevice::GrabSingle()
// Author: 
//------------------------------------------------------------------------------
/**
* Grab single frame
*
* \return    void
*
*/
//------------------------------------------------------------------------------

void CDevice::GrabSingle()
{
	assert ( !m_fContinuousGrabActive );
	assert ( m_pChildFrame != NULL );
	
	m_fSuspendGrab		= false;
	m_pNextBuffer		= NULL;
	m_pCurrentBuffer	= NULL;
	m_fBitmapValid		= false;

	// Setup associated image window
	m_pChildFrame->m_View.ConfigurationChanged(m_Resolution, m_SensorSize, m_ImageSize, m_Origin); 
  
	CreateNewBuffers(1); // old buffers will be released
	
	m_fSingleGrabActive = true;  // the display thread will reset this flag and will free the resources

	// Notify plugins // modified by xupx 2008.07.08
	CPlugin *pPlugin = NULL;
	for ( std::vector<CPlugin *>::iterator it = (m_pChildFrame->m_PluginManager.m_PluginsSet.begin()); 
	it != m_pChildFrame->m_PluginManager.m_PluginsSet.end(); ++ it )
	{ 
		pPlugin = *it;
		
		if(pPlugin->GetSpecVersion() == 0x00010000 )
		{
			CImageInfo info = {0};
			GetImageInfo(info);
			m_pChildFrame->m_PluginManager.OnStartGrabbing(pPlugin, info);
		}
		else
		{
			CConvertImageInfo  *pImageInfo = new CConvertImageInfo;			// v1.1 0409
			GetImageInfo(pImageInfo);
			m_pChildFrame->m_PluginManager.OnStartGrabbing(pPlugin, pImageInfo);
			delete pImageInfo;
			
		}
	}


	try
	{
		BYTE *ppBuffer[1];
		ppBuffer[0] = *m_ppBuffers[0];
		// Grab Image
		SnapShot(ppBuffer, 1);  // issue one shot command
	}
	catch(HVBaseException &e)
	{
		m_fSingleGrabActive = false;
		throw e;
	}
	
	m_pNextBuffer = m_ppBuffers[0];
	
	ShowFrame();
		
	// Notify plugins
	for ( std::vector<CPlugin *>::iterator it1 = (m_pChildFrame->m_PluginManager.m_PluginsSet.begin()); 
	it1 != m_pChildFrame->m_PluginManager.m_PluginsSet.end(); ++ it1 )
	{
		if(pPlugin->GetSpecVersion() == 0x00010000 )
		{
			CImageData data = {0};
			GetImageData(data);	
			CImageInfo info = {0};
			GetImageInfo(info);
			
			m_pChildFrame->m_PluginManager.OnStopGrabbing(pPlugin, info, data);
		}
		else
		{
			CConvertImageData *pImageData = new CConvertImageData;
			GetImageData(pImageData);	
			CConvertImageInfo  *pImageInfo = new CConvertImageInfo;			// v1.1 0409
			GetImageInfo(pImageInfo);
			
			m_pChildFrame->m_PluginManager.OnStopGrabbing(pPlugin, pImageInfo, pImageData);
			delete pImageData;
			delete pImageInfo;
		}
	}

}



//------------------------------------------------------------------------------
// void CDevice::GrabFromFile(CString FileName)
// Author: 
//------------------------------------------------------------------------------
/**
* Grab image from file
*
* \param     FileName
* \return    void
*
* 
*/
//------------------------------------------------------------------------------

void CDevice::GrabFromFile(CString FileName)
{
	assert ( ! IsGrabActive() );
	assert ( m_pChildFrame != NULL );
	
	// release image buffers and current bitmap
	ReleaseBuffers();
	if ( m_pBitmap != NULL )
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
		m_fBitmapValid = false;
	}
	
	// Load bitmap from file
	m_pBitmap = new CHVBitmap(FileName);
	if ( m_pBitmap != NULL )
	{
		m_fBitmapValid = true;

		//HVBaseUtility::FormatRGB24ToBW24(*m_pBitmap,*m_pBitmap, m_pBitmap->GetSize().cx,m_pBitmap->GetSize().cy);
		// inform the child view 
		m_pChildFrame->m_View.ConfigurationChanged((HV_VIDEO_MODE)-1, m_pBitmap->GetSensorSize(),m_pBitmap->GetSize(), m_pBitmap->GetOrigin());
	}
}



//------------------------------------------------------------------------------
// void CDevice::GrabContinuous()
// Author: 
//------------------------------------------------------------------------------
/**
* Activate continuous grabbing
*
* \return void 
*
*/
//------------------------------------------------------------------------------
void CDevice::GrabContinuous()
{
	assert ( ! m_fContinuousGrabActive );
	assert ( m_pChildFrame != NULL );
	
	// Allocate bandwidth, enable Device and queue in buffers, ISO_ENABLE=true
	PrepareContinuousGrab();
	
	m_fContinuousGrabActive = true;  // signals that we are grabbing

	// Notify plugins
	// modified by xupx 2008.07.08
	CPlugin *pPlugin = NULL;
	for ( std::vector<CPlugin *>::iterator it = (m_pChildFrame->m_PluginManager.m_PluginsSet.begin()); 
	it != m_pChildFrame->m_PluginManager.m_PluginsSet.end(); ++ it )
	{ 
		pPlugin = *it;
		if(pPlugin->GetSpecVersion() == 0x00010000 )
		{
			CImageInfo info = {0};
			GetImageInfo(info);
			m_pChildFrame->m_PluginManager.OnStartGrabbing(pPlugin, info);
		}
		else
		{
			CConvertImageInfo  *pImageInfo = new CConvertImageInfo;			// v1.1 0409
			GetImageInfo(pImageInfo);
			m_pChildFrame->m_PluginManager.OnStartGrabbing(pPlugin, pImageInfo);
			delete pImageInfo;
		}
	}
	// book keeping about number of active grabs
	s_cGrabsActive ++;
	if ( s_cGrabsActive == 1 )
	{   // Start timer for the frame rate display
		m_MainFrame.SetTimer(TIMERID, TIMERINTERVAL);
	}
	
	// start watches to measure fps
	m_DisplayWatch.Start();
	
	// Switch on ISO_ENABLE  -> Device will send continuously data
	//	ContinuousShot = true;
}




//------------------------------------------------------------------------------
// void CDevice::GrabCancel()
// Author: 
//------------------------------------------------------------------------------
/**
* Cancel continuous grabbing.
*  * kill timer to display framerates ( if the last grabbing instance is to be cancelled )
*  * suspend the display thread ( to prevent it from accessing image buffers )
*  * cancel pending i/o requests
*
* \return void
*/
//------------------------------------------------------------------------------

void CDevice::GrabCancel()
{
	DWORD error = 0;
	
	if ( m_fContinuousGrabActive )
	{
		assert(! m_fSingleGrabActive);
		s_cGrabsActive --;  // bookkeeping of numbers of running continuous grabs
		// if last grab is canceled we have to kill the timer used to display frame rates
		if ( s_cGrabsActive == 0 )
			m_MainFrame.KillTimer(TIMERID);
		m_fContinuousGrabActive = false;  // this will signal the thread not to enque further buffers 
		error = CancelContinuousGrab();
		if(!m_MainFrame.m_bToClose)		m_MainFrame.m_DeviceManager.XD_Show_All_FPS();
	}
	else
	{
		assert(m_fSingleGrabActive);
		assert(! m_fContinuousGrabActive);
		m_fSingleGrabActive = false;
	}
	
	m_CritSect.Lock();
	m_pNextBuffer = NULL;
	m_CritSect.Unlock();

	m_MainFrame.UpdateUI();
    
//	ATLASSERT( m_pCurrentBuffer != NULL);
	// modified by xupx 2008.07.08
	CPlugin *pPlugin = NULL;
	for ( std::vector<CPlugin *>::iterator it = (m_pChildFrame->m_PluginManager.m_PluginsSet.begin()); 
	it != m_pChildFrame->m_PluginManager.m_PluginsSet.end(); ++ it )
	{ 
		pPlugin = *it;
		if ( m_pCurrentBuffer && (pPlugin->GetSpecVersion() == 0x00010000)) 
		{
			CImageInfo info = {0};	
			GetImageInfo(info);		//
			CImageData data = {0};
			data.pRawBuffer		= *m_pCurrentBuffer;
			assert( m_pBitmap != NULL);
			data.pImageBuffer	= *m_pBitmap;
			m_pChildFrame->m_PluginManager.OnStopGrabbing(pPlugin, info, data);
		}
		else if( m_pCurrentBuffer && (pPlugin->GetSpecVersion() == 0x00010001))	//v1.1	0409
		{
			CConvertImageInfo *pImageInfo =NULL;
			CConvertImageData *pImageData = NULL;
			try{
				pImageInfo = new CConvertImageInfo;	
				pImageData = new CConvertImageData;		
				GetImageInfo(pImageInfo);		//
				pImageData->pCidSrce	= *m_pCurrentBuffer;
				assert( m_pBitmap != NULL);
				pImageData->pCidDest	= *m_pBitmap;
				ATLTRACE("HVPerf: Plugin OnStopGrabbing 1");
				m_pChildFrame->m_PluginManager.OnStopGrabbing(pPlugin, pImageInfo,pImageData);
				ATLTRACE("HVPerf: Plugin OnStopGrabbing 2");
				delete pImageInfo;
				delete pImageData;
			}
			catch (HVBaseException& e) {	//xupxTest0724
				if(pImageInfo != NULL)
					delete pImageInfo;
				if(pImageData != NULL)
					delete pImageData;
				throw e;
			}
		}
	}
}




//------------------------------------------------------------------------------
// void CDevice::GetFrameRate(double& acquired, double& displayed)
// Author: 
//------------------------------------------------------------------------------
/**
*  Retrieve actual frame rate
*
* \param     acquired
* \param     displayed
* \return    
*
*/
//------------------------------------------------------------------------------

void CDevice::GetFrameRate(double& displayed)
{
	assert ( m_fContinuousGrabActive );
	
	double avg = m_DisplayAvg.Avg();
	displayed = avg == 0 ? 0.0 : 1.0 / avg;
	m_DisplayAvg.Reset();
}


bool CDevice::XD_Get_FPS(double& fps,bool b)
{
	
//	ATLTRACE("XD_Get_FPS()\n");
	if ( !m_fContinuousGrabActive ) 
	{
//		ATLTRACE("XD_Get_FPS: m_fContinuousGrabActive=0\n");
		return false;
	}
	else{
//		ATLTRACE("XD_Get_FPS: m_fContinuousGrabActive=1\n");
		double avg = m_DisplayAvg.Avg();
		fps = avg == 0 ? 0.0 : 1.0 / avg;
		if(this!=m_MainFrame.m_DeviceManager.GetCurrentDevice())
			m_DisplayAvg.Reset();
		return true;
	}
}

//inline 
void CDevice::GetImageInfo(CImageInfo &info)
{
	if ( ( m_ColorCode == COLOR_MONO8 && ! m_DisplayMode ) || m_ColorCode == COLOR_RGB8 ){
		info.ImageBitDepth = info.ImageBitDepthReal = 8;
	}
	else{
		info.ImageBitDepth = info.ImageBitDepthReal = 24;
	}

	info.ImageFormat	= (IMAGEFORMAT)m_ColorCode;
	info.ImageWidth		= m_ImageSize.cx;
	info.ImageHeight	= m_ImageSize.cy;
	info.ImageSizeBytes = info.ImageWidth * info.ImageHeight * info.ImageBitDepth / 8;
}

const int ColorCode2BitWidth[] = 
{

	      8, 16, 16, 24, 24, 16, 48, 16,     48,  8, 16,  0,  0,  0,  0,  0,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
         16, 16, 12, 16, 16, 12, 16, 16,     12, 16, 16, 12,  0,  0,  0,  0,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24,  8, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		
};

const int ColorCode2DataDepth[] = 
{
		  8, 16, 16, 24, 24, 16, 48, 16,     48,  8, 16, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 10, 10, 10, 10, 10, 10, 12, 12,     12, 12, 12, 12,  0,  0,  0,  0,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 8/*jpeg*/, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
		 24, 24, 24, 24, 24, 24, 24, 24,     24, 24, 24, 24, 24, 24, 24, 24,
};
void CDevice::GetImageInfo(CConvertImageInfo  *pImageInfo)		//v1.1  0409 申请的空间有问题
{
	pImageInfo->ciiSize = sizeof(CConvertImageInfo);
	pImageInfo->ciiSrce.iV11Size = pImageInfo->ciiDest.iV11Size = sizeof(CImageV11Info);
	
	pImageInfo->ciiSrce.iV11Width = pImageInfo->ciiDest.iV11Width = m_ImageSize.cx;
	pImageInfo->ciiSrce.iV11Height= pImageInfo->ciiDest.iV11Height= m_ImageSize.cy;
	
	
	int nSrcWid = pImageInfo->ciiSrce.iV11Width ;
	int nSrcHei = pImageInfo->ciiSrce.iV11Height;
	int nDesWid = pImageInfo->ciiDest.iV11Width ;
	int nDesHei = pImageInfo->ciiDest.iV11Height;
	
	//modify by yans 
	pImageInfo->ciiSrce.iV11ColorCode = (HV_COLOR_CODE)m_ColorCode;
	pImageInfo->ciiSrce.iV11BitCount = HVBaseUtility::BitsPerPixel(m_ColorCode);//ColorCode2BitWidth[m_ColorCode]; 2008.12.24
	pImageInfo->ciiSrce.iV11PixelNumber = nSrcWid * nSrcHei;
	
	if(IS_READABLE_CAM(m_pInfo->DeviceType()))	//1410 v1.1 xupx0612
	{
		//m_nColorCode是HV_COLOR_CODE类型,
		//在加载和更改属性栏选项时，此参数被赋值

		pImageInfo->ciiSrce.iV11DataDepth = ColorCode2DataDepth[m_ColorCode];
	}
	else
	{
		if ( m_bRaw8or12 )	//是否选择了12位Raw数据格式
			pImageInfo->ciiSrce.iV11DataDepth = 12;
		else
			pImageInfo->ciiSrce.iV11DataDepth = 8;
	}
	if(m_ColorCode == COLOR_JPEG)		// add by xupx 2009.07.07
	{
		pImageInfo->ciiSrce.iV11SizeImage = m_dwImageSize;	
	}
	else
		pImageInfo->ciiSrce.iV11SizeImage = nSrcWid * nSrcHei * pImageInfo->ciiSrce.iV11BitCount / 8;
	
	HV_COLOR_CODE tDisplayColorCode;
	tDisplayColorCode = m_DisplayMode == 0 ? COLOR_MONO8 : COLOR_RGB8;

	pImageInfo->ciiDest.iV11ColorCode = tDisplayColorCode;//(HV_COLOR_CODE)m_ColorCode;
	pImageInfo->ciiDest.iV11BitCount = ColorCode2BitWidth[tDisplayColorCode];
	pImageInfo->ciiDest.iV11PixelNumber = nDesWid * nDesHei;
	pImageInfo->ciiDest.iV11DataDepth = pImageInfo->ciiDest.iV11BitCount;
	pImageInfo->ciiDest.iV11SizeImage = nDesWid * nDesHei * pImageInfo->ciiDest.iV11DataDepth / 8;
	//modify by yans end
	
}




inline void CDevice::GetImageData(CImageData &data)
{
	ATLASSERT( m_pBitmap != NULL);
	ATLASSERT( m_pCurrentBuffer != NULL);
	data.pRawBuffer		= *m_pCurrentBuffer;
	data.pImageBuffer	= *m_pBitmap;	
}

inline void CDevice::GetImageData(CConvertImageData *pImageData)	//v1.1
{
	ATLASSERT( m_pBitmap != NULL);
	ATLASSERT( m_pCurrentBuffer != NULL);
	pImageData->cidSize = sizeof(CConvertImageData);
	pImageData->pCidSrce = *m_pCurrentBuffer;
	pImageData->pCidDest = *m_pBitmap;	// 返回了m_pPixelData ，即位图数据  xupx
}


//------------------------------------------------------------------------------
// bool CDevice::ReconfigurationRequest()
// Author: 
//------------------------------------------------------------------------------
/**
*  To be called before reconfiguring the Device

  * Call this function before the Device is to be reparametrized. 
  * The function will suspend an active grab.
  * After configuring the Device call CDevice::ConfigurationChanged to resume the grab.
  *
  * \return bool 
  *
  * returns true if succeeded
  * 
*/
//------------------------------------------------------------------------------

bool CDevice::ReconfigurationRequest()
{
	if ( ! IsGrabActive() )
		return true;
	
	// a pending single grab will be terminated
	if ( IsSingleGrabActive() )
	{
		m_fSingleGrabActive = false;
	}
	
	m_fSuspendGrab = true;  // the threads will no longer queue in buffers
	
	if ( IsContinuousGrabActive() ){
		// cancel I/O requests 
		StopSnap();
		CloseSnap();
	}
	
	// now we are ready for a reconfiguration
	
	return true;
	
}




//------------------------------------------------------------------------------
// void CDevice::OnContinuousGrabError(DWORD err)
// Author: 
//------------------------------------------------------------------------------
/**
* Callback. It is called if an error occured in one of the threads
*
* \param     err  error code
* \return    
*
* <type Return description here>
* 
* \see       <delete line if not used>
* \todo      
*/
//------------------------------------------------------------------------------

void CDevice::OnContinuousGrabError(DWORD err)
{
	HVBaseException e(err, "Image acquisition");
	m_MainFrame.ReportError(e);
	if ( m_fContinuousGrabActive )
		GrabCancel();
}




//------------------------------------------------------------------------------
// void CDevice::CancelContinuousGrab()
// Author: 
//------------------------------------------------------------------------------
/**
* Cancel an active continuous grab. The acquisition thread will be terminated.
*
* \return   error code. 0 indicates success
*/
//------------------------------------------------------------------------------

DWORD CDevice::CancelContinuousGrab()
{
	DWORD error = 0;
	
	m_fSuspendGrab = true;  // the acquisition thread will no longer queue in buffers
	
	int iTimeOut = 10000;
	while(1)
	{
		m_CritSect.Lock();
		if(!m_bOnProcessing)
		{
			m_CritSect.Unlock();
			break;
		}
		else
		{
			m_CritSect.Unlock();
		}

		if(iTimeOut-- < 0)
		{
			m_bOnProcessing = FALSE;
		}

		MSG msg;
		::GetMessage(&msg, NULL, NULL, NULL);
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}


	try
	{
		StopSnap();
		CloseSnap();             // cancel I/O requests 
	}
	catch ( HVBaseException& e )
	{
		if ( e.Error() != STATUS_NOT_OPEN_SNAP )
			throw e;
	} 

	return error;
}





//------------------------------------------------------------------------------
// void CDevice::PrepareContinuousGrab()
// Author: 
//------------------------------------------------------------------------------
/**
* Allocate buffers
* Enqueue buffers in driver's queue
* Initialize measurment of frame rates
* Switch Device to ISO enable mode
*
* \return    
*
* void
* 
* \see       <delete line if not used>
* \todo      
*/
//------------------------------------------------------------------------------


void CDevice::PrepareContinuousGrab()
{
	m_fBitmapValid = false;
	if ( m_pChildFrame != NULL ){
		m_pChildFrame->m_View.ConfigurationChanged(m_Resolution, m_SensorSize, m_ImageSize, m_Origin); 
	}
	
	// Allocate new buffers  ( existing ones will be released )
	CreateNewBuffers(1);  
	
	m_pNextBuffer		= NULL;
	m_pCurrentBuffer	= NULL;
	
	m_DisplayAvg.Reset();
	
	m_fSuspendGrab = false; 
	
	BYTE *pBuffer[1];
	pBuffer[0] = *m_ppBuffers[0];
	try{
		OpenSnap(CDevice::SnapCallbackProc, this);
		RegisterSnapMode();
		StartSnap(pBuffer, 1);
	}
	catch ( HVBaseException& e){
		CloseSnap();
		throw e;
	}
}



//------------------------------------------------------------------------------
// void CDevice::CreateNewBuffers(unsigned long n)
// Author: 
//------------------------------------------------------------------------------
/**
* Allocate new buffers. Currently allocated buffers are freed
*
* \param     n  Number of buffers to allocate
* \return    void 
*
*/
//------------------------------------------------------------------------------

void CDevice::CreateNewBuffers(unsigned long n)
{
	try
	{
		// Release old buffers
		ReleaseBuffers();
		
		m_ppBuffers = new CHVBitmap*[n];// m_ppBuffers = new BYTE[w*h];
		if ( m_ppBuffers == NULL )
		{
			throw HVBaseException(E_OUTOFMEMORY, "CDevice::CreateNewBuffers()");
		}
		ZeroMemory(m_ppBuffers, n * sizeof(CHVBitmap*));
		m_cBuffers = n;

		HVTYPE type = m_pInfo->DeviceType();
		if(IS_READABLE_CAM(type))	//for v1.1
		{
			/******* 2008.12.23 by xupx *****/
			m_nDataMode = m_ColorCode;
			/*********** end ***************/
		}
		else
		{
			if(m_bRaw8or12)   // 放到下面有错
			{
				m_ColorCode=COLOR_MONO16;
				
			}
			else {
				m_ColorCode = COLOR_MONO8;
			}
		}

		for ( unsigned long i = 0; i < n; i++)
		{
			m_ppBuffers[i] = new CHVBitmap(m_ImageSize, m_SensorSize, m_Origin,  m_ColorCode);
			if ( m_ppBuffers[i] == NULL )
				throw HVBaseException(E_OUTOFMEMORY, "CDevice::CreateNewBuffers()");
		}
		
		// Create new bitmap for display purposes		
		// first free an existing one
		if ( m_pBitmap != NULL )
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
			m_fBitmapValid = false;
		}
		// We will only allocate an extra bitmap to display, if the Device sends image data
		// in a format we have explicetely to convert. If no conversion is needed, no extra memory 
		// for the display bitmap will be allocated. Instead the grab functions have to ensure that 
		// m_pBitmap points to one of the buffers stored in m_ppBuffers.
		
		if ( m_DisplayMode) {	//modified by xupx 2008.07.24
			// create a new Bitmap to display
			m_pBitmap = new CHVBitmap(m_ImageSize, m_SensorSize, m_Origin, COLOR_RGB8);
		}
		else{
			m_pBitmap = new CHVBitmap(m_ImageSize, m_SensorSize, m_Origin, COLOR_MONO8);
		}
		
		m_MainFrame.m_mono8 = TRUE;
		//		m_MainFrame.m_mono8 = (GetColorCode() == Color_Mono8 );
	
		if ( m_pBitmap == NULL )
				throw HVBaseException(E_OUTOFMEMORY, "CDevice::CreateBewBuffers()");
	}

	catch (HVBaseException& e)
	{
		ReleaseBuffers();
		throw e;
	}  
}



//------------------------------------------------------------------------------
// void CDevice::ReleaseBuffers()
// Author: 
//------------------------------------------------------------------------------
/**
* Free buffers.
*
* \return  void 
*
*/
//------------------------------------------------------------------------------
void CDevice::ReleaseBuffers()
{
	if ( m_ppBuffers != NULL )
	{
		for ( int i = 0; i < m_cBuffers; i ++ )
		{
			if ( m_ppBuffers[i] != NULL && m_ppBuffers[i] != m_pBitmap )
			{
				delete m_ppBuffers[i];
			}
		}
		delete[] m_ppBuffers;
		m_ppBuffers = NULL;
		m_cBuffers = 0;
	}
}



void CDevice::ShowFrame()
{
	m_CritSect.Lock();
	m_bOnProcessing = TRUE;
	m_CritSect.Unlock();
	
	m_pCurrentBuffer = m_pNextBuffer;
	m_pNextBuffer = NULL;

	if ( m_pCurrentBuffer == NULL ) 
		return ; // no new buffer available, sleep until a new one arrives
	
	try
	{
		// modified by xupx 2008.07.08
		CPlugin *pPlugin = NULL;
		for ( std::vector<CPlugin *>::iterator it = (m_pChildFrame->m_PluginManager.m_PluginsSet.begin()); 
		it != m_pChildFrame->m_PluginManager.m_PluginsSet.end(); ++ it )
		{ 
			pPlugin = *it;	
			if(pPlugin->GetSpecVersion() == 0x00010000)
			{
				CImageInfo info = {0};
				CImageData data = {0};
				
				GetImageInfo(info);
				data.pRawBuffer		= *m_pCurrentBuffer;
				data.pImageBuffer	= *m_pBitmap;

				int w=info.ImageWidth;
				int h=info.ImageHeight;
				if(m_bRaw8or12)
				{  
					BYTE* pRBuf=(BYTE*)data.pRawBuffer;
					WORD *wpBuf=(WORD*)pRBuf;
					BYTE *pB=NULL;
					//	
					WORD v;
					
					for(int i=0;i<w*h;i++){ 
						
						pB=(BYTE*)&wpBuf[i];
						if(!m_bLeBe){
							BYTE btTemp;
							btTemp=pB[0];
							pB[0]=pB[1];
							pB[1]=btTemp;
						}
						v=wpBuf[i]&0x0fff;
						pB=(BYTE*)&m_dwLut[v];
						pRBuf[i]=pB[3];
						
					}			
				}

				m_pChildFrame->m_PluginManager.OnShowFrame(pPlugin, info, data);
			}
			else
			{
				CConvertImageInfo  *pImageInfo = new CConvertImageInfo;
				CConvertImageData  *pImageData = new CConvertImageData;
				
				GetImageInfo(pImageInfo);
				pImageData->pCidSrce = *m_pCurrentBuffer;
				pImageData->pCidDest = *m_pBitmap;
				
				m_pChildFrame->m_PluginManager.OnShowFrame(pPlugin, pImageInfo, pImageData);		//v1.1  0409
				delete pImageData;
				delete pImageInfo;
			}
		} //end for
		m_fBitmapValid = true;
		
		//::CopyMemory(m_pBitmap,m_pCurrentBuffer,1392*1040); //xdTest2:
		
		// if a single grab is active, free resources and signal the grab as finished
		if ( m_fSingleGrabActive ){
			m_fSingleGrabActive = false;  
			m_MainFrame.UpdateUI();
		}
		
		// Force Drawing of m_pBitmap
		HDC DC = m_pChildFrame->m_View.GetDC();
		m_pChildFrame->m_View.BitBlt(DC, m_pBitmap);

		m_pChildFrame->m_View.ReleaseDC(DC);
		
		if ( m_fContinuousGrabActive )
			// actualize moving average ( for fps display purposes)  
			m_DisplayAvg.Add(m_DisplayWatch.Stop(true));
		
	}  //end try

	catch (HVBaseException&  e )
	{
		PostMessage(m_MainFrame, WM_ERROR, e.Error(), (long) this); 
	}

	m_CritSect.Lock();
	m_bOnProcessing = FALSE;
	m_CritSect.Unlock();
}



//------------------------------------------------------------------------------
// void CDevice::ConfigurationChanged()
// Author: 
//------------------------------------------------------------------------------
/**
* The client informs the Device object that the reconfiguration is done. 
* A suspended grab will be resumed.
*
* \return void
*
*/
//------------------------------------------------------------------------------

void CDevice::ConfigurationChanged()
{
	if ( m_pChildFrame ){
		// modified by xupx 2008.07.08
		CPlugin *pPlugin = NULL;
		for ( std::vector<CPlugin *>::iterator it = (m_pChildFrame->m_PluginManager.m_PluginsSet.begin()); 
		it != m_pChildFrame->m_PluginManager.m_PluginsSet.end(); ++ it )
		{ 
			pPlugin = *it;
			if(pPlugin->GetSpecVersion() == 0x00010000 )
			{	
				CImageInfo info = {0};
				GetImageInfo(info);
				m_pChildFrame->m_PluginManager.OnManagerNotify(pPlugin, info);
			}
			else
			{
				CConvertImageInfo  *pImageInfo = new CConvertImageInfo;			// v1.1 0409
				GetImageInfo(pImageInfo);			
				m_pChildFrame->m_PluginManager.OnManagerNotify(pPlugin, pImageInfo);
				delete pImageInfo;
			}
		}
	}

	if ( m_fContinuousGrabActive ){
		try
		{
			PrepareContinuousGrab();
			m_fSuspendGrab = false;
			//			ContinuousShot = true;
		}
		catch ( HVBaseException& e )
		{
			m_MainFrame.ReportError(e);
			GrabCancel();
		}
	}
}


//------------------------------------------------------------------------------
// void CDevice::Refresh(bool force /* = false*/)
// Author: 
//------------------------------------------------------------------------------
/**
* Refresh the cached information. If the video mode, video format etc. 
* have changed, clients will be informed
*
* \param     force if set to true, the rest will be informed even if the cached 
*            information has not been changed
* \return    
*
* <type Return description here>
* 
* \see       <delete line if not used>
* \todo      
*/
//------------------------------------------------------------------------------
void CDevice::Refresh()
{
	HVTYPE type = m_pInfo->DeviceType();
	if(IS_READABLE_CAM(type))
	{
		//deleted by xupx 2009.04.24
		m_pSnapMode->m_SnapMode	 = GetParameter(IDS_SNAPMODE, 0);	//读取采集模式,用来更新 软触发按钮状态
		m_CurrentModeIdx		 = GetVideoMode();					// 2009.11.05
		m_ColorCode		 	     = (HV_COLOR_CODE)GetFMT7Mode((HV_VIDEO_MODE)m_CurrentModeIdx, FMT7_MODE_FUNC_ID_COLOR_CODING, NULL);	//add by xupx 2009.02.26
	}

	if(IS_GIGE(type)|| IS_GM_CAM(type) || type == DRVIF1394TYPE)
	{
		m_pSnapMode->m_SnapMode	 = GetParameter(IDS_SNAPMODE, 0);	//读取采集模式,用来更新 软触发按钮状态
		m_fBitmapValid = ! m_fContinuousGrabActive && m_pBitmap !=  NULL && m_SensorSize == m_pBitmap->GetSensorSize();
		
		if ( m_pChildFrame != NULL ){
			m_pChildFrame->m_View.ConfigurationChanged(m_Resolution, m_SensorSize, m_ImageSize, m_Origin);
		}
		m_MainFrame.Refresh(this);
		return;
	}
	if(m_pInfo->DeviceType() == DRVIFENETTYPE)
	{
		m_fBitmapValid = ! m_fContinuousGrabActive && m_pBitmap !=  NULL && m_SensorSize == m_pBitmap->GetSensorSize();
		
		if ( m_pChildFrame != NULL ){
			m_pChildFrame->m_View.ConfigurationChanged(m_Resolution, m_SensorSize, m_ImageSize, m_Origin);
		}
		m_MainFrame.Refresh(this);
		return;
	}

	m_pSnapMode->Refresh();
	m_pTriggerPolarity->Refresh();
	m_pStrobePolarity->Refresh();
	m_pShutter->Refresh();
	m_pGain->Refresh();
		
	m_pBlackLevelEnable->Refresh();
	m_pBlackLevel->Refresh();
	
	
    if (!(IS_CCD_CAMERA(m_pInfo->DeviceType()))) {
		
		m_pADCLevel->Refresh();
//		m_pBlank->Refresh();
    }
	m_fBitmapValid = ! m_fContinuousGrabActive && m_pBitmap !=  NULL && m_SensorSize == m_pBitmap->GetSensorSize();

	//inform a child frame that the configuration has changed
	if ( m_pChildFrame != NULL ){
		m_pChildFrame->m_View.ConfigurationChanged(m_Resolution, m_SensorSize, m_ImageSize, m_Origin);
	}
	
	// ask the main frame to refresh it's controls
	m_MainFrame.Refresh(this);
	
}





//------------------------------------------------------------------------------
// void CDevice::SetAOI(CRect AOI)
// Author: 
//------------------------------------------------------------------------------
/**
*  Set new AOI
*
* \param     AOI

  * \return    void
  
	* \todo      
*/
//------------------------------------------------------------------------------
void CDevice::SetAOI(CRect AOI)
{	
	if ( ReconfigurationRequest() )
	{
		int l=AOI.left;
		int t=AOI.top;
		int w=AOI.Width();
		int h=AOI.Height();
		if (!IS_HV_CAM(m_HVTYPE))
		{
			switch(m_Resolution) {//修正小分辨率小窗口的Bug HYL 2006.9.11
			case VIDEO_MODE1:
				l=l*2;
				t=t*2;
				break;
			case VIDEO_MODE2:
				l=l*4;
				t=t*4;
				break;
			case VIDEO_MODE3:
				l=l*8;
				t=t*8;
				break;
			case VIDEO_MODE4:
				l=l*16;
				t=t*16;
				break;
			default:
				break;
			}
		}
		
		CHVBase::SetOutputWindow(l,t,w,h);
		
		m_Origin.x = AOI.left;
		m_Origin.y = AOI.top;
		m_ImageSize.cx = AOI.Width();
		m_ImageSize.cy = AOI.Height();
		
		///HV相机在每次更改窗口后，需要重新设置一次曝光时间 by xupx 2009.02.06
		if(IS_CMOS_1394_CAM(m_pInfo->DeviceType()))
			m_pShutter->Refresh();
		if (IS_HV_CAM(m_HVTYPE))
		{
			m_MainFrame.m_PropertyView.UpdatePacketSize(this);
		}		
		ConfigurationChanged();
	}
}


void CDevice::UpdateImageSize(HV_VIDEO_MODE mode)
{
	// 以下是用于更新窗口 2009.06.30
	m_SensorSize = Get_Sensor_Size(mode);
	if (!IS_HV_CAM(m_HVTYPE))	// 2009.10.16
	{
		m_ImageSize = m_SensorSize;
		m_Origin.x  = 0;
		m_Origin.y  = 0;
	}

	ConfigurationChanged();
}

/// Set Video mode
void CDevice::SetVideoMode(HV_VIDEO_MODE mode)
{
	if ( ReconfigurationRequest() )
	{
		CHVBase::SetResolution(mode);
		if(IS_READABLE_CAM(this->m_pInfo->DeviceType()))
			mode = GetVideoMode();
		m_Resolution = mode;
		m_SensorSize = Get_Sensor_Size(mode);
				
		m_Origin.SetPoint(0, 0);
		m_ImageSize = m_SensorSize;
		CHVBase::SetOutputWindow(m_Origin.x, m_Origin.y, m_ImageSize.cx, m_ImageSize.cy);

//		m_pShutter->Refresh();	//xupx 2008.09.23
	
		ConfigurationChanged();
		MaximizeAOI();

	}
}





//------------------------------------------------------------------------------
// void CDevice::MaximizeAOI()
// Author: 
//------------------------------------------------------------------------------
/**
*  Maximize AOI
*
* \return void
*/
//------------------------------------------------------------------------------
void CDevice::MaximizeAOI()
{
	
	SetAOI(CRect(CPoint(0, 0), Format.SensorSize()));
	Refresh();
}

void CDevice::Set_Default_AOI()
{
	m_Resolution = VIDEO_MODE0;
	m_ImageSize = Get_Sensor_Size(VIDEO_MODE0);
	m_Origin.SetPoint(0, 0);
}




//------------------------------------------------------------------------------
// void CDevice::SetBayerToRGBConversion( int mode )
// Author: 
//------------------------------------------------------------------------------
/**
* Enable or disable the Bayer to RGB conversion
*
* \param     mode
* \return    
*
* void
* 
*/
//------------------------------------------------------------------------------

void CDevice::SetDisplayMode( int mode )
{ 
	assert ( mode >= 0 && mode < 5); 
	if ( ReconfigurationRequest() )
	{
		m_DisplayMode = mode;
	}
	ConfigurationChanged();
	m_MainFrame.ConfigurationChanged(this, m_Resolution, m_ColorCode);	
}



//------------------------------------------------------------------------------
// void CDevice::ParametrizeDevice(DCSVideoMode mode, DCSColorCode code, CPoint position, CSize size, unsigned long bpp)
// Author: 
//------------------------------------------------------------------------------
/**
*  Parametrize camera ( format 7 )
*
* \param     mode
* \param     code
* \param     position
* \param     size
* \param     bpp
* \return    void
*
*/
//------------------------------------------------------------------------------
void CDevice::ParametrizeDevice(HV_VIDEO_MODE mode, HV_COLOR_CODE code, CPoint position, CSize size)
{

}



//------------------------------------------------------------------------------
// int CDevice::SnapCallback(HV_SNAP_INFO *pInfo)
// Author: 
//------------------------------------------------------------------------------
/**
* Set device output window area 
*
* \param     
*            
* \return    
*
* <type Return description here>
* 
* \see       <delete line if not used>
* \todo      
*/
//////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------

// HVCommand调用HVControl功能
HVSTATUS
__stdcall
HVCommand_HVAPIControl(
					   HHV hhv,
					   HV_CONTROL_CODE code,
					   void *pInBuf,
					   DWORD nInBufSize,
					   void *pOutBuf,
					   DWORD nOutBufSize,
					   DWORD *pBytesRet
					   )
{
    int size;
    HVAPI_CONTROL_PARAMETER param;
    param.code         = (HV_CONTROL_CODE)(code);
    param.pInBuf       = (void*)(pInBuf);
    param.dwInBufSize  = (DWORD)(nInBufSize);
    param.pOutBuf      = (void*)(pOutBuf);
    param.dwOutBufSize = (DWORD)(nOutBufSize);
    param.pBytesRet    = (DWORD*)(pBytesRet);
    size = sizeof(param);
    return HVCommand(
		(HHV)(hhv),
		(HV_COMMAND_CODE)CMD_HVAPI_CONTROL,
		(void*)&param,
		(int*)&size);
}

HVSTATUS
__stdcall
HVRegisterSnapProcEx(
					 HHV hhv,
					 HV_CALLBACK_PROC pFun,
					 void *pUserParam
					 )
{
    HV_SNAP_INFO_EX_REGISTER SnapEx;
    memset(&SnapEx,0,sizeof(HV_SNAP_INFO_EX_REGISTER));
	
    HV_ARG_REGISTER_CALLBACK argRegCallback;
    argRegCallback.type = HV_SNAP_CALLBACK_USE_HV_SNAP_INFO_EX;
    argRegCallback.Parameter.SnapProcEx.pFun = pFun;
    argRegCallback.Parameter.SnapProcEx.pParam = pUserParam;
    argRegCallback.Parameter.SnapProcEx.pRegister = &SnapEx;
	
    return HVCommand_HVAPIControl(hhv,
		ORD_REGISTER_CALLBACK,
		&argRegCallback,
		sizeof(HV_ARG_REGISTER_CALLBACK),
		NULL,
		0,
		NULL);
}

HVSTATUS
__stdcall
HVDeregisterSnapProcEx(
					   HHV hhv,
					   HV_CALLBACK_PROC pFun
					   )
{
    HV_ARG_DEREGISTER_CALLBACK argDeregCallback;
    argDeregCallback.type = HV_SNAP_CALLBACK_USE_HV_SNAP_INFO_EX;
    argDeregCallback.Parameter.CallbackProc.pFun = pFun;
	
    return HVCommand_HVAPIControl(hhv,
		ORD_DEREGISTER_CALLBACK,
		&argDeregCallback,
		sizeof(HV_ARG_DEREGISTER_CALLBACK),
		NULL,
		0,
		NULL);
}

HVSTATUS
__stdcall
HVRegisterSnapProc(
				   HHV hhv,
				   HV_SNAP_PROC pFun,
				   void *pUserParam
				   )
{
    HV_ARG_REGISTER_CALLBACK argRegCallback;
    argRegCallback.type = HV_SNAP_CALLBACK_USE_HV_SNAP_INFO;
    argRegCallback.Parameter.SnapProc.pFun = pFun;
    argRegCallback.Parameter.SnapProc.pParam = pUserParam;
	
    return HVCommand_HVAPIControl(hhv,
		ORD_REGISTER_CALLBACK,
		&argRegCallback,
		sizeof(HV_ARG_REGISTER_CALLBACK),
		NULL,
		0,
		NULL);
}

HVSTATUS
__stdcall
HVDeregisterSnapProc(
					 HHV hhv,
					 HV_SNAP_PROC pFun
					 )
{
    HV_ARG_DEREGISTER_CALLBACK argDeregCallback;
    argDeregCallback.type = HV_SNAP_CALLBACK_USE_HV_SNAP_INFO;
    argDeregCallback.Parameter.SnapProc.pFun = pFun;
    return HVCommand_HVAPIControl(hhv,
		ORD_DEREGISTER_CALLBACK,
		&argDeregCallback,
		sizeof(HV_ARG_DEREGISTER_CALLBACK),
		NULL,
		0,
		NULL);
}


int CALLBACK CDevice::SnapCallbackProc(HV_SNAP_INFO *pInfo)
{
	CDevice *This = (CDevice *)(pInfo->pParam);
	assert(This);
	
	if ( ! This->m_fSuspendGrab )
	{
		This->m_pNextBuffer = This->m_ppBuffers[pInfo->nIndex];
		This->ShowFrame();
	}
	return 1;
}


int CALLBACK CDevice::SnapJPEGCallback(HV_CALLBACK_INFO *pInfo)  //Chinese: JPEG图像数据回调函数
{
	HWND hwnd = (HWND)(pInfo->pParam);
	CDevice *This = (CDevice *)(pInfo->pParam);
	assert(This);

	This->m_dwImageSize  = pInfo->Parameter.HvSnapInfoEx.pInfo->imageDes.imageSize;
	if ( ! This->m_fSuspendGrab )
	{
		This->m_pNextBuffer = This->m_ppBuffers[pInfo->Parameter.HvSnapInfoEx.pInfo->nImageIndex];
		This->ShowFrame();
	}	
	
	return 1;
}

HVSTATUS CDevice::RegisterSnapMode()
{
	HVSTATUS status = STATUS_OK;

	if(m_ColorCode != COLOR_JPEG)
	{
		status = HVDeregisterSnapProcEx(this->GetHandle(), SnapJPEGCallback);
		if (!HV_SUCCESS(status))
			return status;
		
		status = HVRegisterSnapProc(this->GetHandle(),SnapCallbackProc, this);
		return status;
	}
	status = HVDeregisterSnapProc(this->GetHandle(), SnapCallbackProc);
	if (!HV_SUCCESS(status))
		return status;

	//Chinese:注册JPEG回调函数
	status = HVRegisterSnapProcEx(this->GetHandle(),SnapJPEGCallback, this);
	if (!HV_SUCCESS(status)) 
	{	
		return status;
	}
	return status;
}


//------------------------------------------------------------------------------
// void CDevice::SaveConfiguration(const CPropertyBagPtr ptrBag) 
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
* Save the current settings to a property bag.
*
* A client should not use the CBcam's SaveSettings methods, because we have to store
* additional information
*
* \param     ptrBag
*/
//------------------------------------------------------------------------------
void CDevice::SaveConfiguration(const CPropertyBagPtr ptrBag) 
{
	// save video mode
	ptrBag->WriteLong("VideoMode", m_Resolution);
	ptrBag->WriteLong("ImageCx", m_ImageSize.cx);
	ptrBag->WriteLong("ImageCy", m_ImageSize.cy);
	ptrBag->WriteLong("OriginX", m_Origin.x);
	ptrBag->WriteLong("OriginY", m_Origin.y);
	
	// save snap mode settings
	if ( m_pSnapMode->IsSupported() )
		m_pSnapMode->Save(ptrBag->CreateBag("SnapMode"));

	// save trigger polarity settings
	if ( m_pTriggerPolarity->IsSupported() )
		m_pTriggerPolarity->Save(ptrBag->CreateBag("TriggerPolarity"));

	// save strobe polarity settings
	if ( m_pStrobePolarity->IsSupported() )
		m_pStrobePolarity->Save(ptrBag->CreateBag("StrobePolarity"));

	/// save adc level settings
	if ( m_pADCLevel->IsSupported() )
		m_pADCLevel->Save(ptrBag->CreateBag("ADCLevel"));

	if ( m_pBlackLevelEnable->IsSupported() )
		m_pBlackLevelEnable->Save(ptrBag->CreateBag("BlackLevelEnable"));
	
	// save scalar properties
	if ( m_pShutter->IsSupported() )
		m_pShutter->Save(ptrBag->CreateBag("Shutter"));

	if ( m_pGain->IsSupported() )
		m_pGain->Save(ptrBag->CreateBag("Gain"));

	if ( m_pBlackLevel->IsSupported() )
		m_pBlackLevel->Save(ptrBag->CreateBag("BlackLevel"));

	if ( m_pBlank->IsSupported() )
		m_pBlank->Save(ptrBag->CreateBag("Blank"));

	
    if (IS_CCD_CAMERA(m_pInfo->DeviceType())) {
		ptrBag->CreateBag("FrameFrozen")->WriteLong("Value", m_FrameFrozen);
		ptrBag->CreateBag("StrobeOnOff")->WriteLong("Value", m_StrobeOnOff);
		ptrBag->CreateBag("TestImage")->WriteLong("Value",m_nTestImage);
		ptrBag->CreateBag("TriggerDelayOnOff")->WriteLong("Value",m_bTriggerDelayOnOff);
        ptrBag->CreateBag("TriggerDelayValue")->WriteLong("Value",m_nTriggerDelayValue);
        ptrBag->CreateBag("TransDelayValue")->WriteLong("Value",m_nTransfersDelayValue);

		ptrBag->CreateBag("FilterTimeUnit")->WriteLong("Value",m_nFilterTimeUnit);
		ptrBag->CreateBag("FilterTime")->WriteLong("Value",m_nFilterTime);

		ptrBag->CreateBag("Raw8or12")->WriteLong("Value",m_bRaw8or12);
		ptrBag->CreateBag("LeBe")->WriteLong("Value",m_bLeBe);
		ptrBag->CreateBag("TriggerSource")->WriteLong("Value",m_nTriggerSource);
		ptrBag->CreateBag("LutPath")->WriteString("String",m_LutPath);

        ptrBag->CreateBag("OutPutIO_0")->WriteLong("Value",m_nOutPutIO_0);
		ptrBag->CreateBag("OutPutIO_1")->WriteLong("Value",m_nOutPutIO_1);
		ptrBag->CreateBag("OutPutIO_2")->WriteLong("Value",m_nOutPutIO_2);
		ptrBag->CreateBag("OutPutIO_0_POL")->WriteLong("Value",m_nOutPutIO_0_POL);
		ptrBag->CreateBag("OutPutIO_1_POL")->WriteLong("Value",m_nOutPutIO_1_POL);
		ptrBag->CreateBag("OutPutIO_2_POL")->WriteLong("Value",m_nOutPutIO_2_POL);
			
        ptrBag->CreateBag("PacketSize")->WriteLong("Value", m_PacketSize);
        ptrBag->CreateBag("Brightness")->WriteLong("Value", m_Brightness);
        ptrBag->CreateBag("BlueGain")->WriteLong("Value", m_BlueGain);
        ptrBag->CreateBag("RedGain")->WriteLong("Value", m_RedGain); 

    }
	
	CPropertyBagPtr ptrBcamViewerBag = ptrBag->CreateBag("HVPerf");
	ptrBcamViewerBag->WriteLong("DisplayMode", m_DisplayMode);

}
								
								
//------------------------------------------------------------------------------
// void CDevice::RestoreConfiguration(const CPropertyBagPtr ptrBag)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
* Restore the current settings from a property bag.
*
*
* \param     ptrBag
*/
//------------------------------------------------------------------------------
void CDevice::RestoreConfiguration(const CPropertyBagPtr ptrBag)
{
	if(IS_READABLE_CAM(m_pInfo->DeviceType()))		// add by xupx 2008.10.09
	{
		try
		{
			m_Resolution = GetVideoMode(); //(HVResolution) ptrBag->ReadLong("VideoMode");
			m_ImageSize.cx = ptrBag->ReadLong("ImageCx"); //1410Test
			m_ImageSize.cy = ptrBag->ReadLong("ImageCy");
			m_Origin.x = ptrBag->ReadLong("OriginX");
			m_Origin.y = ptrBag->ReadLong("OriginY");
			
			CPropertyBagPtr ptrBcamViewerBag = ptrBag->GetBag("HVPerf");
			m_DisplayMode = ptrBcamViewerBag->ReadLong("DisplayMode");
			
		}
		catch ( HVBaseException& )
		{
			m_Resolution = VIDEO_MODE0;
			m_ImageSize = Get_Sensor_Size(VIDEO_MODE0);
			m_Origin.SetPoint(0, 0);
		}
		
//		ConfigurationChanged();
		return;		//相机不需要注册表。 by xupx 2008.09.09
	}


	if ( ReconfigurationRequest() )
	{
		// restore the video mode
		try
		{
			m_Resolution = (HV_VIDEO_MODE) ptrBag->ReadLong("VideoMode");
			m_ImageSize.cx = ptrBag->ReadLong("ImageCx"); //1410Test
			m_ImageSize.cy = ptrBag->ReadLong("ImageCy");
			m_Origin.x = ptrBag->ReadLong("OriginX");
			m_Origin.y = ptrBag->ReadLong("OriginY");
		}
		catch ( HVBaseException& )
		{
			m_Resolution = VIDEO_MODE0;
			m_ImageSize = Get_Sensor_Size(VIDEO_MODE0);
			m_Origin.SetPoint(0, 0);
		}
		
		// restore snap mode settings
		if ( m_pSnapMode->IsSupported() )
			m_pSnapMode->Restore(ptrBag->GetBag("SnapMode"));

		// restore trigger polarity settings
		if ( m_pTriggerPolarity->IsSupported() )
			m_pTriggerPolarity->Restore(ptrBag->GetBag("TriggerPolarity"));

		// restore strobe polarity settings
		if ( m_pStrobePolarity->IsSupported() )
			m_pStrobePolarity->Restore(ptrBag->GetBag("StrobePolarity"));
		
		// restore ADC level settings
		if ( m_pADCLevel->IsSupported() )
			m_pADCLevel->Restore(ptrBag->GetBag("ADCLevel"));

		if ( m_pBlackLevelEnable->IsSupported() )
			m_pBlackLevelEnable->Restore(ptrBag->GetBag("BlackLevelEnable"));
		
		// restore scalar properties
		// save scalar properties
		if ( m_pShutter->IsSupported() )
			m_pShutter->Restore(ptrBag->GetBag("Shutter"));

		if ( m_pGain->IsSupported() )
			m_pGain->Restore(ptrBag->GetBag("Gain"));

		if ( m_pBlackLevel->IsSupported() )
			m_pBlackLevel->Restore(ptrBag->GetBag("BlackLevel"));

		if ( m_pBlank->IsSupported())
			m_pBlank->Restore(ptrBag->GetBag("Blank"));

        if (IS_CCD_CAMERA(m_pInfo->DeviceType())) {
			m_FrameFrozen = ptrBag->GetBag("FrameFrozen")->ReadLong("Value");   
			
			Set_Frame_Frozen(m_FrameFrozen);

            m_StrobeOnOff = ptrBag->GetBag("StrobeOnOff")->ReadLong("Value");
//---------------------------
			m_nTestImage         = ptrBag->GetBag("TestImage")->ReadLong("Value");
			m_bTriggerDelayOnOff = ptrBag->GetBag("TriggerDelayOnOff")->ReadLong("Value");
			m_nTriggerDelayValue = ptrBag->GetBag("TriggerDelayValue")->ReadLong("Value");
	        m_nTransfersDelayValue = ptrBag->GetBag("TransDelayValue")->ReadLong("Value");

	        m_nFilterTimeUnit = ptrBag->GetBag("FilterTimeUnit")->ReadLong("Value");
	        m_nFilterTime = ptrBag->GetBag("FilterTime")->ReadLong("Value");
			m_bRaw8or12          = ptrBag->GetBag("Raw8or12")->ReadLong("Value");
			m_bLeBe          = ptrBag->GetBag("LeBe")->ReadLong("Value");
			m_nTriggerSource     = ptrBag->GetBag("TriggerSource")->ReadLong("Value");
			m_nOutPutIO_0        = ptrBag->GetBag("OutPutIO_0")->ReadLong("Value");
			m_nOutPutIO_1        = ptrBag->GetBag("OutPutIO_1")->ReadLong("Value");
			m_nOutPutIO_2        = ptrBag->GetBag("OutPutIO_2")->ReadLong("Value");
            m_nOutPutIO_0_POL    = ptrBag->GetBag("OutPutIO_0_POL")->ReadLong("Value");
			m_nOutPutIO_1_POL    = ptrBag->GetBag("OutPutIO_1_POL")->ReadLong("Value");
			m_nOutPutIO_2_POL    = ptrBag->GetBag("OutPutIO_2_POL")->ReadLong("Value");
			try{
				m_LutPath        = ptrBag->GetBag("LutPath")->ReadString("String");
			}
			catch ( HVBaseException& /*e*/)
			{
			//	m_MainFrame.ReportError(e);
			}

//-------------------------
            int temp = (125*m_ImageSize.cx*m_ImageSize.cy)/
                (120*(m_ImageSize.cy + 88));
            temp = (temp + 3) & 0xfffffffc;

            m_PacketSize = ptrBag->GetBag("PacketSize")->ReadLong("Value");
            
			if(	!IS_SV1311(m_pInfo->DeviceType())&&
				!IS_SV1410(m_pInfo->DeviceType())&&
				!IS_SV400(m_pInfo->DeviceType()) )
			{
				if (m_PacketSize < temp || m_PacketSize > 4096) {
					m_PacketSize = 4096;
				}
			}
            SetPacketSize(m_PacketSize);

            m_Brightness = ptrBag->GetBag("Brightness")->ReadLong("Value");
            if (m_Brightness < 0 || m_Brightness > 1023) {
                m_Brightness = 0;
            }
            Set_Brightness();

            m_BlueGain = ptrBag->GetBag("BlueGain")->ReadLong("Value");
            m_RedGain = ptrBag->GetBag("RedGain")->ReadLong("Value");
            if (m_BlueGain < 0 || m_BlueGain > 64) {		//xupx 0513
                m_BlueGain = 28;
            }
            if (m_RedGain < 0 || m_RedGain > 64) {			//xupx 0513
                m_RedGain = 14;
            }
			Set_WB_Red(m_RedGain);
			Set_WB_Blue(m_BlueGain);
        }

		try
		{
			CPropertyBagPtr ptrBcamViewerBag = ptrBag->GetBag("HVPerf");
			m_DisplayMode = ptrBcamViewerBag->ReadLong("DisplayMode");
			
			SetBlank(ptrBcamViewerBag->ReadLong("HBlank"),ptrBcamViewerBag->ReadLong("VBlank"));
		}
		catch ( HVBaseException& )
		{
			// Additional information for bcam viewer not found.
			// Nothing to be done;
		}

		InitializeDevice();
		// Refresh the cached information and notify clients of changes
		Refresh(/*true*/);
		ConfigurationChanged();
	}
}


