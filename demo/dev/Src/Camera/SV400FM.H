// SV400FM.h: interface for the CSV400FM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV400FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV400FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"



class CSV400FM : public CDevice  
{
public:
	CSV400FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV400FM();
	
	class CSV400FMInfo : public CDevice::CInfo
	{
	public:
		CSV400FMInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV400FM,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV400FM,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV400FMTYPE;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMInfo;

	/// Inquire, get and set all scalar properties.
	class CSV400FMGain  : public CDevice::CGain
	{
		CSV400FMGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV400FMShutter  : public CDevice::CShutter
	{
		CSV400FMShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		
		virtual unsigned long Min(){
			ULONG ulRet;
			TUnit u = m_pDevice->m_pShutter->m_Unit.Value();
			switch(u){
			case UNIT_MICROSECOND:
				ulRet = 20;
				break;
			case UNIT_MILLISECOND:				
			case UNIT_SECOND:
				ulRet = 1;
				break;
			}
			return ulRet;
		}
		virtual unsigned long Max(){ return 3000;}
		
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
		friend class CSV400FM;
	};
	friend class CSV400FMShutter;

	/// Inquire, get and set all scalar properties.
	class CSV400FMBlackLevel  : public CDevice::CBlackLevel
	{
		CSV400FMBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMBlackLevel;


	class CSV400FMSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV400FMSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMSnapMode;

	class CSV400FMTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV400FMTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMTriggerPolarity;


	class CSV400FMStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV400FMStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMStrobePolarity;

	
	class CSV400FMADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV400FMADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMADCLevel;
	
	class CSV400FMSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV400FMSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return false;} //modified by HYL 2007.02.05 
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMSnapSpeed;

	class CSV400FMBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV400FMBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMBlackLevelEnable;

	class CSV400FMBlank : public CDevice::CBlank
	{
	public:
		CSV400FMBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FM;
	};
	friend class CSV400FMBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV400FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















