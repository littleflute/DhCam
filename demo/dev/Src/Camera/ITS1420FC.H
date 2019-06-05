// ITS1420FC.h: interface for the CITS1420FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITS1420FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_ITS1420FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CITS1420FC : public CDevice  
{
public:
	CITS1420FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CITS1420FC();
	
	class CITS1420FCInfo : public CDevice::CInfo
	{
	public:
		CITS1420FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  

		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_ITS1420FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}

		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_ITS1420FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return ITS1420FCTYPE;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCInfo;

	/// Inquire, get and set all scalar properties.
	class CITS1420FCGain  : public CDevice::CGain
	{
		CITS1420FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CITS1420FCShutter  : public CDevice::CShutter
	{
		CITS1420FCShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CITS1420FC;
	};
	friend class CITS1420FCShutter;

	/// Inquire, get and set all scalar properties.
	class CITS1420FCBlackLevel  : public CDevice::CBlackLevel
	{
		CITS1420FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCBlackLevel;


	class CITS1420FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CITS1420FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCSnapMode;

	class CITS1420FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CITS1420FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCTriggerPolarity;


	class CITS1420FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CITS1420FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCStrobePolarity;

	
	class CITS1420FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CITS1420FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCADCLevel;
	
	class CITS1420FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CITS1420FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCSnapSpeed;

	class CITS1420FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CITS1420FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCBlackLevelEnable;

	class CITS1420FCBlank : public CDevice::CBlank
	{
	public:
		CITS1420FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CITS1420FC;
	};
	friend class CITS1420FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_ITS1420FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















