// ITS1394.h: interface for the CITS1394 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITS1394_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_ITS1394_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CITS1394 : public CDevice  
{
public:
	CITS1394(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CITS1394();
	
	class CITS1394Info : public CDevice::CInfo
	{
	public:
		CITS1394Info(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  

		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_ITS1394,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}

/*
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_ITS1394,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
*/
		virtual HVTYPE DeviceType() {return DRVIF1394TYPE;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394Info;

	/// Inquire, get and set all scalar properties.
	class CITS1394Gain  : public CDevice::CGain
	{
		CITS1394Gain(unsigned long Value, CDevice *pDevice) :
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
		friend class CITS1394;
	};
	friend class CITS1394Gain;
	
	/// Inquire, get and set all scalar properties.
	class CITS1394Shutter  : public CDevice::CShutter
	{
		CITS1394Shutter(unsigned long Value, CDevice *pDevice) :
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
		}
		
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
		friend class CITS1394;
	};
	friend class CITS1394Shutter;

	/// Inquire, get and set all scalar properties.
	class CITS1394BlackLevel  : public CDevice::CBlackLevel
	{
		CITS1394BlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CITS1394;
	};
	friend class CITS1394BlackLevel;


	class CITS1394SnapMode : public CDevice::CSnapMode
	{
	public:
		CITS1394SnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394SnapMode;

	class CITS1394TriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CITS1394TriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394TriggerPolarity;


	class CITS1394StrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CITS1394StrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394StrobePolarity;

	
	class CITS1394ADCLevel : public CDevice::CADCLevel
	{
	public:
		CITS1394ADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394ADCLevel;
	
	class CITS1394SnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CITS1394SnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394SnapSpeed;

	class CITS1394BlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CITS1394BlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394BlackLevelEnable;

	class CITS1394Blank : public CDevice::CBlank
	{
	public:
		CITS1394Blank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CITS1394;
	};
	friend class CITS1394Blank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_ITS1394_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















