// SV1310FM.h: interface for the CSV1310FM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV1310FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV1310FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"



class CSV1310FM : public CDevice  
{
public:
	CSV1310FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV1310FM();
	
	class CSV1310FMInfo : public CDevice::CInfo
	{
	public:
		CSV1310FMInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV1310FM,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV1310FM,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV1310FMTYPE;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMInfo;

	/// Inquire, get and set all scalar properties.
	class CSV1310FMGain  : public CDevice::CGain
	{
		CSV1310FMGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV1310FMShutter  : public CDevice::CShutter
	{
		CSV1310FMShutter(unsigned long Value, CDevice *pDevice) :
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
		virtual unsigned long Max(){ return 1000;}
		
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
		friend class CSV1310FM;
	};
	friend class CSV1310FMShutter;

	/// Inquire, get and set all scalar properties.
	class CSV1310FMBlackLevel  : public CDevice::CBlackLevel
	{
		CSV1310FMBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMBlackLevel;


	class CSV1310FMSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV1310FMSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMSnapMode;

	class CSV1310FMTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV1310FMTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMTriggerPolarity;


	class CSV1310FMStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV1310FMStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMStrobePolarity;

	
	class CSV1310FMADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV1310FMADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMADCLevel;
	
	class CSV1310FMSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV1310FMSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMSnapSpeed;

	class CSV1310FMBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV1310FMBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMBlackLevelEnable;

	class CSV1310FMBlank : public CDevice::CBlank
	{
	public:
		CSV1310FMBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FM;
	};
	friend class CSV1310FMBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV1310FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















