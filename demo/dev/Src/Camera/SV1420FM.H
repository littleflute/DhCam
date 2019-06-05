// SV1420FM.h: interface for the CSV1420FM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV1420FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV1420FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CSV1420FM : public CDevice  
{
public:
	CSV1420FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV1420FM();
	
	class CSV1420FMInfo : public CDevice::CInfo
	{
	public:
		CSV1420FMInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV1420FM,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV1420FM,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV1420FMTYPE;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMInfo;

	/// Inquire, get and set all scalar properties.
	class CSV1420FMGain  : public CDevice::CGain
	{
		CSV1420FMGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV1420FMShutter  : public CDevice::CShutter
	{
		CSV1420FMShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		
		virtual unsigned long Min(){
			ULONG ulRet;
			TUnit u = m_pDevice->m_pShutter->m_Unit.Value();
			switch(u){
			case UNIT_MICROSECOND:
				ulRet = 20; //各型号不一样
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
		friend class CSV1420FM;
	};
	friend class CSV1420FMShutter;

	/// Inquire, get and set all scalar properties.
	class CSV1420FMBlackLevel  : public CDevice::CBlackLevel
	{
		CSV1420FMBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMBlackLevel;


	class CSV1420FMSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV1420FMSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMSnapMode;

	class CSV1420FMTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV1420FMTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMTriggerPolarity;


	class CSV1420FMStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV1420FMStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMStrobePolarity;

	
	class CSV1420FMADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV1420FMADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMADCLevel;
	
	class CSV1420FMSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV1420FMSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMSnapSpeed;

	class CSV1420FMBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV1420FMBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMBlackLevelEnable;

	class CSV1420FMBlank : public CDevice::CBlank
	{
	public:
		CSV1420FMBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CSV1420FM;
	};
	friend class CSV1420FMBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV1420FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















