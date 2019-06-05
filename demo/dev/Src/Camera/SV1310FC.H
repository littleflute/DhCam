// SV1310FC.h: interface for the CSV1310FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV1310FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV1310FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"



class CSV1310FC : public CDevice  
{
public:
	CSV1310FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV1310FC();
	
	class CSV1310FCInfo : public CDevice::CInfo
	{
	public:
		CSV1310FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV1310FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV1310FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV1310FCTYPE;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCInfo;

	/// Inquire, get and set all scalar properties.
	class CSV1310FCGain  : public CDevice::CGain
	{
		CSV1310FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV1310FCShutter  : public CDevice::CShutter
	{
		CSV1310FCShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV1310FC;
	};
	friend class CSV1310FCShutter;

	/// Inquire, get and set all scalar properties.
	class CSV1310FCBlackLevel  : public CDevice::CBlackLevel
	{
		CSV1310FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCBlackLevel;


	class CSV1310FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV1310FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCSnapMode;

	class CSV1310FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV1310FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCTriggerPolarity;


	class CSV1310FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV1310FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCStrobePolarity;

	
	class CSV1310FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV1310FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCADCLevel;
	
	class CSV1310FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV1310FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return false;} //modified by HYL 2007.02.05 
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCSnapSpeed;

	class CSV1310FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV1310FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCBlackLevelEnable;

	class CSV1310FCBlank : public CDevice::CBlank
	{
	public:
		CSV1310FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1310FC;
	};
	friend class CSV1310FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV1310FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















