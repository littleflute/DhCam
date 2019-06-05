// HV.cpp: implementation of the CHVBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HVBase.h"
#include "Device.h"


/******************************************************************************/
using namespace HVDevice;


//------------------------------------------------------------------------------
// HVBaseException::HVBaseException( HVSTATUS e, CString context ) 
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \overload
* \brief Constructs a new object
*
* \param     e The error code.
* \param     context The context string, describing the circumstances of the exception.
*/
//------------------------------------------------------------------------------
HVBaseException::HVBaseException( HVSTATUS e, CString Context, va_list *pArgs)
: Exception( e, Context, true )
{
	FormatMessage(e, Context);
}



//------------------------------------------------------------------------------
// HVBaseException::HVBaseException( DWORD e, CString context ) 
// Author: Hartmut Nebelung
//------------------------------------------------------------------------------
/**
* \overload
* \brief Constructs a new object
*
* \param     e The error code.
* \param     context The context string, describing the circumstances of the exception.
*/
//------------------------------------------------------------------------------
HVBaseException::HVBaseException( DWORD e, CString Context, va_list *pArgs) 
: Exception( e, Context, true )
{
	Exception::FormatMessage(pArgs);
}



void HVBaseException::FormatMessage( HVSTATUS e, CString Context )
{
	m_Description.Format("%s", HVGetErrorString(e));
	ATLTRACE("HVGetErrorString,  HVSTATUS is: %d", e);
}




//------------------------------------------------------------------------------
// unsigned short HVBaseUtility::BitsPerPixel(DCSColorCode ColorCode)
// Author: A.Happpe
//------------------------------------------------------------------------------
/**
* \brief Looks up the pixel size of a color code
*
* \param     ColorCode
* \return    
*
* Returns the number of bits used for a pixel.
*
* \throw HVBaseException BCAM_E_UNSUPPORTED_COLOR_CODE when passing an invalid color code to the function
*
* 
* \see       DCAM
*/
//------------------------------------------------------------------------------
unsigned short HVBaseUtility::BitsPerPixel(HV_COLOR_CODE ColorCode)
{
	switch ( ColorCode )
	{
	case COLOR_MONO8			: return 8;
	case COLOR_YUV8_4_1_1		: return 12;
	case COLOR_YUV8_4_2_2		: return 16;
	case COLOR_YUV8_4_4_4		: return 24;
	case COLOR_MONO16			: return 16;
	case COLOR_RGB8				: return 24;
	case COLOR_RGB16			: return 48;

	case COLOR_RAW8				: return 8 ;  
	case COLOR_RAW16			: return 16;
	case COLOR_RAW12			: return 16;
	case COLOR_MONO12_NORMALIZED: return 16;
	case COLOR_RAW12_PACKED		: return 16;
	case COLOR_JPEG				: return 8;		// 2009.06.09
			
	default:                 //
		return 24;
//		throw HVBaseException(STATUS_PARAMETER_INVALID, "HVBaseUtility::BitsPerPixel");
	}
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHVBase::HVBaseMap_t CHVBase::s_HVBaseMap;
HWND CHVBase::s_hWndNotify = NULL;

//------------------------------------------------------------------------------
// CHVBase::CHVBase()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Constructs a camera object without opening the driver.
*
*/
//------------------------------------------------------------------------------
CHVBase::CHVBase() :
m_hDevice( INVALID_HANDLE_VALUE )
{
	m_Counter = 0;
}



//------------------------------------------------------------------------------
// CHVBase::~CHVBase()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Destructor - closes the driver
*
*/
//------------------------------------------------------------------------------
CHVBase::~CHVBase()
{
	ATLTRACE("HVPerf: CHVBase::~CHVBase()\n");
//	if (IsOpen())	//add by xupx 2008.11.04
		Close();
}




//------------------------------------------------------------------------------
// bool CHVBase::IsOpen()
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
* \brief Retrieve open status
*
* \return    
*
* True if the device is already open
* 
*/
//------------------------------------------------------------------------------
bool CHVBase::IsOpen()
{
	return IsValid();
}



//------------------------------------------------------------------------------
// void CHVBase::Open(CString DeviceName, HWND hWndNotify)
// Author: 
//------------------------------------------------------------------------------
/**
*
* \brief Opens a camera and registers a client window for device notifications
*
*  Both full device names (as delivered by the plug and play manager) as friendly device names 
*  (as returned by the CBcam::DeviceNames() method) can be used to open the camera.
*
* \param     DeviceName name of the device (either full or friendly device name)
* \param     hWndNotify handle to client window (optional)
* \throw HVBaseException The value of \c ::GetLastError() is thrown.
*/
//------------------------------------------------------------------------------
void CHVBase::Open(int nNumber, HWND hWndNotify)
{
	ATLTRACE("HVPerf: CHVBase::Open device(%d)\n", nNumber);
	
	if (IsOpen())  
		Close();
	
	assert(m_hDevice == INVALID_HANDLE_VALUE);
	
	HVSTATUS status = BeginHVDevice(nNumber, &m_hDevice);

	ATLTRACE(" HVPerf BeginHVDevice status is %d !!!!", status);
	if (HV_SUCCESS(status)){
		// add handle to DeviceMap
		s_HVBaseMap.insert(HVBaseMap_t::value_type(m_hDevice, this));
		m_nNumber = nNumber;
		// If the user wants to register an additional window => register it for handle related device notifications
		if(s_hWndNotify) s_hWndNotify = hWndNotify;
	}
	else{
		ATLTRACE(" BeginHVDevice Fail ....");
		m_hDevice = INVALID_HANDLE_VALUE;
		throw HVBaseException(status, "CHVBase::Open");
	}
}


//------------------------------------------------------------------------------
// void CHVBase::Close()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief Close the device handle 
*
* 
* \throw HVBaseException The errorcode of WaitForCompletion is returned.
* \todo      
*/
//------------------------------------------------------------------------------
void CHVBase::Close()
{ 
	HVSTATUS status = STATUS_OK;
	DWORD error = 0;
	if ( INVALID_HANDLE_VALUE != m_hDevice ) {
		ATLTRACE(" HVPerf EndHVDevice status ...");
		status = EndHVDevice(m_hDevice);
		ATLTRACE(" HVPerf EndHVDevice status is %d !!!!", status);
		s_HVBaseMap.erase(m_hDevice) ;
		m_hDevice = INVALID_HANDLE_VALUE;
	}

}



//------------------------------------------------------------------------------
// bool CHVBase::IsValid()
// Author: 
//------------------------------------------------------------------------------
/**
* \brief check whether the device is still valid.
*
* \retval  true  if it is ok
* \retval  false if it is not ok
*/
//------------------------------------------------------------------------------
bool CHVBase::IsValid()
{
	return (m_hDevice != NULL) && (m_hDevice != INVALID_HANDLE_VALUE);
}



void CHVBase::SetResolution(HV_VIDEO_MODE Mode)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetResolution()");
	
	HVSTATUS status = HVSetResolution(m_hDevice, (HV_RESOLUTION)Mode);
//	if ( ! HV_SUCCESS(status))
//		throw HVBaseException(status, "HVDAILT.dll : HVSetResolution");
}



void CHVBase::SetOutputWindow(int nLeft, int nTop, int nWidth, int nHeight)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetOutputWindow()");
	
	int l=nLeft,t=nTop;
	HVSTATUS status = HVSetOutputWindow(m_hDevice, l,t, nWidth, nHeight);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVSetOutputWindow");
}



void CHVBase::AGCControl(BYTE byParam, long lValue)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::AGCControl()");
	
	HVSTATUS status = HVAGCControl(m_hDevice, byParam, lValue);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVAGCControl");
}




void CHVBase::AECControl_HV3(BYTE byParam, long lValue)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::AECControl()");
	
	HVSTATUS status;
	if(m_hDevice&& m_Counter < 2)		//Trigger 状态 add by xupx 2008.08.11
	{
		status = STATUS_OK;
		m_Counter++;
	}
	else								//连续采集状态
		status = HVAECControl(m_hDevice, byParam, lValue);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVAECControl");
}


void CHVBase::AECControl(BYTE byParam, long lValue)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::AECControl()");
	
	HVSTATUS status = HVAECControl(m_hDevice, byParam, lValue);

	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVAECControl");
}



void CHVBase::ADCControl(BYTE byParam, long lValue)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::ADCControl()");
	
	HVSTATUS status = HVADCControl(m_hDevice, byParam, lValue);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVADCControl");
}




void CHVBase::SnapShot(BYTE **ppBuffer, int nNumber)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SnapShot()");
	
//	assert(! IsBadWritePtr(pBuffer, BufferSize ) );
	
	HVSTATUS status = HVSnapShot(m_hDevice, ppBuffer, nNumber);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVSnapShot");    
}


void CHVBase::OpenSnap(HV_SNAP_PROC lpSnapFunc, void *pParam)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::OpenSnap()");
	
	HVSTATUS status = HVOpenSnap(m_hDevice, lpSnapFunc, pParam);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVOpenSnap");    
}


void CHVBase::CloseSnap()
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::CloseSnap()");
	
	HVSTATUS status = HVCloseSnap(m_hDevice);
	ATLTRACE("HVPerf CloseSnap status is %d ....", status);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVCloseSnap");    
}



void CHVBase::StartSnap(BYTE **ppBuffer, size_t BufferNumber)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::StartSnap()");
	
//	assert(! IsBadWritePtr(pBuffer, BufferSize ) );
	
	HVSTATUS status = HVStartSnap(m_hDevice, ppBuffer, BufferNumber);
	
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVStartSnap");  
}



void CHVBase::StopSnap()
{
	ATLTRACE("HVPerf StopSnap ....");
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::StopSnap()");
	
	HVSTATUS status = HVStopSnap(m_hDevice);
	ATLTRACE("HVPerf StopSnap status is %d ....", status);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVStopSnap");  
}

//HVSTATUS CHVBase::GetDeviceIPAdd( HV_RES_GET_DEVICE_IP ResGetDevIP)
//{
//	HVSTATUS status = SV1410GCControl(m_hDevice, ORD_GET_DEVICE_IP,	NULL,0,&ResGetDevIP,sizeof(ResGetDevIP),0)
//	return status;
//}


BOOL CHVBase::GetDeviceInfo(HV_DEVICE_INFO Param, void *pContext, int *pSize)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::GetDeviceInfo()");
	
	ATLTRACE( "HV_DEVICE_INFO is %d !!!!", (int)Param);
	HVSTATUS status = HVGetDeviceInfo(m_hDevice, Param, pContext, pSize);
	if(status == STATUS_PARAMETER_INVALID)
	{
		return FALSE;
	}

	else if ( ! HV_SUCCESS(status))
	{
		throw HVBaseException(status, "HVDAILT.dll : HVGetDeviceInfo"); 
	}

	return TRUE;
}



void CHVBase:: SetSnapMode(HV_SNAP_MODE mode)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetSnapMode()");
	
	HVSTATUS status = HVSetSnapMode(m_hDevice, mode);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVSetSnapMode"); 
} 

void CHVBase:: SetTriggerPolarity(HV_POLARITY polarity)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetTriggerPolarity()");
	
	HVSTATUS status = HVSetTriggerPolarity(m_hDevice, polarity);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVSetTriggerPolarity"); 
} 

void CHVBase:: SetStrobePolarity(HV_POLARITY polarity)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetStrobePolarity()");
	
	HVSTATUS status = HVSetStrobePolarity(m_hDevice, polarity);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVSetStrobePolarity"); 
} 


void CHVBase::SetSnapSpeed(HV_SNAP_SPEED speed)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetSnapSpeed()");
	
	HVSTATUS status = HVSetSnapSpeed(m_hDevice, speed);
	if ( ! HV_SUCCESS(status))		
	{
	//	throw HVBaseException(status, "HVDAILT.dll : HVSetSnapSpeed"); 
	}
} 


void CHVBase::SetBlank(int hor, int vert)
{
	if ( ! IsValid())
		throw HVBaseException(STATUS_DEVICE_HANDLE_INVALID, "CHVBase::SetBlankHor()");

	HVSTATUS status = HVSetBlanking(m_hDevice, hor, vert);
	if ( ! HV_SUCCESS(status))
		throw HVBaseException(status, "HVDAILT.dll : HVSetBlank"); 
}
