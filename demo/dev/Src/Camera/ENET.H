// ENET.h: interface for the CENET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENET_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_ENET_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CENET : public CDevice  
{
public:
	CENET(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CENET();
	
	class CENETInfo : public CDevice::CInfo
	{
	public:
		CENETInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  

		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_ENET,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}

//		virtual CString ModelName()
//		{
//			char lpTemp[256];		  
//			LoadString(NULL,IDS_STRING_ENET,lpTemp,MAX_PATH);	
//			CString str;
//			str.Format("%s", lpTemp );
//			return str;
//		}
		virtual HVTYPE DeviceType() {return DRVIFENETTYPE;}
		  
	private:
		friend class CENET;
	};
	friend class CENETInfo;

	/// Inquire, get and set all scalar properties.
	class CENETGain  : public CDevice::CGain
	{
		CENETGain(unsigned long Value, CDevice *pDevice) :
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
		friend class CENET;
	};
	friend class CENETGain;
	
	/// Inquire, get and set all scalar properties.
	class CENETShutter  : public CDevice::CShutter
	{
		CENETShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CENET;
	};
	friend class CENETShutter;

	/// Inquire, get and set all scalar properties.
	class CENETBlackLevel  : public CDevice::CBlackLevel
	{
		CENETBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CENET;
	};
	friend class CENETBlackLevel;


	class CENETSnapMode : public CDevice::CSnapMode
	{
	public:
		CENETSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CENET;
	};
	friend class CENETSnapMode;

	class CENETTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CENETTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CENET;
	};
	friend class CENETTriggerPolarity;


	class CENETStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CENETStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CENET;
	};
	friend class CENETStrobePolarity;

	
	class CENETADCLevel : public CDevice::CADCLevel
	{
	public:
		CENETADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CENET;
	};
	friend class CENETADCLevel;
	
	class CENETSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CENETSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CENET;
	};
	friend class CENETSnapSpeed;

	class CENETBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CENETBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CENET;
	};
	friend class CENETBlackLevelEnable;

	class CENETBlank : public CDevice::CBlank
	{
	public:
		CENETBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CENET;
	};
	friend class CENETBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_ENET_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















