// SV1311FC.h: interface for the CSV1311FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV1311FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV1311FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CSV1311FC : public CDevice  
{
public:
	CSV1311FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV1311FC();
	
	class CSV1311FCInfo : public CDevice::CInfo
	{
	public:
		CSV1311FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV1311FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV1311FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV1311FCTYPE;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCInfo;

	/// Inquire, get and set all scalar properties.
	class CSV1311FCGain  : public CDevice::CGain
	{
		CSV1311FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV1311FCShutter  : public CDevice::CShutter
	{
		CSV1311FCShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV1311FC;
	};
	friend class CSV1311FCShutter;

	/// Inquire, get and set all scalar properties.
	class CSV1311FCBlackLevel  : public CDevice::CBlackLevel
	{
		CSV1311FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCBlackLevel;


	class CSV1311FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV1311FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1310FCSnapMode;

	class CSV1311FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV1311FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCTriggerPolarity;


	class CSV1311FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV1311FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCStrobePolarity;

	
	class CSV1311FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV1311FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCADCLevel;
	
	class CSV1311FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV1311FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCSnapSpeed;

	class CSV1311FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV1311FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCBlackLevelEnable;

	class CSV1311FCBlank : public CDevice::CBlank
	{
	public:
		CSV1311FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1311FC;
	};
	friend class CSV1311FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV1311FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















