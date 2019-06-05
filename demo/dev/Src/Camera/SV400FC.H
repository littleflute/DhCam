// SV400FC.h: interface for the CSV400FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV400FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV400FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"



class CSV400FC : public CDevice  
{
public:
	CSV400FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV400FC();
	
	class CSV400FCInfo : public CDevice::CInfo
	{
	public:
		CSV400FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV400FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV400FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV400FCTYPE;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCInfo;

	/// Inquire, get and set all scalar properties.
	class CSV400FCGain  : public CDevice::CGain
	{
		CSV400FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV400FCShutter  : public CDevice::CShutter
	{
		CSV400FCShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV400FC;
	};
	friend class CSV400FCShutter;

	/// Inquire, get and set all scalar properties.
	class CSV400FCBlackLevel  : public CDevice::CBlackLevel
	{
		CSV400FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCBlackLevel;


	class CSV400FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV400FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCSnapMode;

	class CSV400FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV400FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCTriggerPolarity;


	class CSV400FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV400FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCStrobePolarity;

	
	class CSV400FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV400FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCADCLevel;
	
	class CSV400FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV400FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return false;} //modified by HYL 2007.02.05 
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCSnapSpeed;

	class CSV400FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV400FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCBlackLevelEnable;

	class CSV400FCBlank : public CDevice::CBlank
	{
	public:
		CSV400FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV400FC;
	};
	friend class CSV400FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV400FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















