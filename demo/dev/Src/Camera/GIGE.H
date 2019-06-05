// GIGE.h: interface for the CGIGE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GIGE_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_GIGE_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CGIGE : public CDevice  
{
public:
	CGIGE(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CGIGE();
	
	class CGIGEInfo : public CDevice::CInfo
	{
	public:
		CGIGEInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  

		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_GIGE,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}

//		virtual CString ModelName()
//		{
//			char lpTemp[256];		  
//			LoadString(NULL,IDS_STRING_GIGE,lpTemp,MAX_PATH);	
//			CString str;
//			str.Format("%s", lpTemp );
//			return str;
//		}
		virtual HVTYPE DeviceType() {return DRVIFGIGETYPE;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGEInfo;

	/// Inquire, get and set all scalar properties.
	class CGIGEGain  : public CDevice::CGain
	{
		CGIGEGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ 
			ULONG ulRet;
			ulRet = m_pDevice->m_GainRange.minV;
			return ulRet;
		}
		virtual unsigned long Max(){
			ULONG ulRet;
			ulRet = m_pDevice->m_GainRange.maxV;
			return ulRet;
		}
	
	private:
		friend class CGIGE;
	};
	friend class CGIGEGain;
	
	/// Inquire, get and set all scalar properties.
	class CGIGEShutter  : public CDevice::CShutter
	{
		CGIGEShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		

		virtual unsigned long Min(){ 
			ULONG ulRet;
			ulRet = m_pDevice->m_ShutterRange.minV;
			return ulRet;
		}
		virtual unsigned long Max(){	// add by xupx 0530
			ULONG ulRet;
			ulRet = m_pDevice->m_ShutterRange.maxV;
			return ulRet;

		}//return 3000;}
		
		virtual void Set(unsigned long Value, void *pContext = NULL)
		{
			Value = Value<Min()?Min():Value;
			Value = Value>Max()?Max():Value;

			
			HV_AEC_CONTROL_CONTEXT AECCtrl;
			AECCtrl.ctrl=AEC_SHUTTER_SPEED;

			CShutter::Set(Value,&AECCtrl);

			m_AbsValue = Value;

		}
	
	private:
		friend class CGIGE;
	};
	friend class CGIGEShutter;

	/// Inquire, get and set all scalar properties.
	class CGIGEBlackLevel  : public CDevice::CBlackLevel
	{
		CGIGEBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CGIGE;
	};
	friend class CGIGEBlackLevel;


	class CGIGESnapMode : public CDevice::CSnapMode
	{
	public:
		CGIGESnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGESnapMode;

	class CGIGETriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CGIGETriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGETriggerPolarity;


	class CGIGEStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CGIGEStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGEStrobePolarity;

	
	class CGIGEADCLevel : public CDevice::CADCLevel
	{
	public:
		CGIGEADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGEADCLevel;
	
	class CGIGESnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CGIGESnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGESnapSpeed;

	class CGIGEBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CGIGEBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGEBlackLevelEnable;

	class CGIGEBlank : public CDevice::CBlank
	{
	public:
		CGIGEBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CGIGE;
	};
	friend class CGIGEBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_GIGE_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















