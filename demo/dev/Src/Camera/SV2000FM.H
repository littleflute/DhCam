// SV2000FM.h: interface for the CSV2000FM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV2000FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV2000FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CSV2000FM : public CDevice  
{
public:
	CSV2000FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV2000FM();
	
	class CSV2000FMInfo : public CDevice::CInfo
	{
	public:
		CSV2000FMInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV2000FM,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV2000FM,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV2000FMTYPE;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMInfo;

	/// Inquire, get and set all scalar properties.
	class CSV2000FMGain  : public CDevice::CGain
	{
		CSV2000FMGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV2000FMShutter  : public CDevice::CShutter
	{
		CSV2000FMShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV2000FM;
	};
	friend class CSV2000FMShutter;

	/// Inquire, get and set all scalar properties.
	class CSV2000FMBlackLevel  : public CDevice::CBlackLevel
	{
		CSV2000FMBlackLevel(unsigned long Value, CDevice *pDevice) :
			CBlackLevel(Value, pDevice)
		{}
	
		bool IsSupported() {return false;}
	
		virtual unsigned long Min(){ return -255;}
		virtual unsigned long Max(){ return 255;}
	
		private:
			friend class CSV2000FM;
	};
	friend class CSV2000FMBlackLevel;


	class CSV2000FMSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV2000FMSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV1310FCSnapMode;

	class CSV2000FMTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV2000FMTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMTriggerPolarity;


	class CSV2000FMStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV2000FMStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMStrobePolarity;

	
	class CSV2000FMADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV2000FMADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMADCLevel;
	
	class CSV2000FMSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV2000FMSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMSnapSpeed;

	class CSV2000FMBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV2000FMBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMBlackLevelEnable;

	class CSV2000FMBlank : public CDevice::CBlank
	{
	public:
		CSV2000FMBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CSV2000FM;
	};
	friend class CSV2000FMBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV2000FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















