// SV1311FM.h: interface for the CSV1311FM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV1311FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV1311FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CSV1311FM : public CDevice  
{
public:
	CSV1311FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV1311FM();
	
	class CSV1311FMInfo : public CDevice::CInfo
	{
	public:
		CSV1311FMInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV1311FM,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV1311FM,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV1311FMTYPE;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMInfo;

	/// Inquire, get and set all scalar properties.
	class CSV1311FMGain  : public CDevice::CGain
	{
		CSV1311FMGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV1311FMShutter  : public CDevice::CShutter
	{
		CSV1311FMShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV1311FM;
	};
	friend class CSV1311FMShutter;

	/// Inquire, get and set all scalar properties.
	class CSV1311FMBlackLevel  : public CDevice::CBlackLevel
	{
		CSV1311FMBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMBlackLevel;


	class CSV1311FMSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV1311FMSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1310FCSnapMode;

	class CSV1311FMTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV1311FMTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMTriggerPolarity;


	class CSV1311FMStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV1311FMStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMStrobePolarity;

	
	class CSV1311FMADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV1311FMADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMADCLevel;
	
	class CSV1311FMSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV1311FMSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMSnapSpeed;

	class CSV1311FMBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV1311FMBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMBlackLevelEnable;

	class CSV1311FMBlank : public CDevice::CBlank
	{
	public:
		CSV1311FMBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FM;
	};
	friend class CSV1311FMBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV1311FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















