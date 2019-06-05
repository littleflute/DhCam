// SV1300FC.h: interface for the CSV1300FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV1300FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV1300FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"



class CSV1300FC : public CDevice  
{
public:
	CSV1300FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV1300FC();
	
	class CSV1300FCInfo : public CDevice::CInfo
	{
	public:
		CSV1300FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV1300FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV1300FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV1300FCTYPE;}

		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCInfo;

	/// Inquire, get and set all scalar properties.
	class CSV1300FCGain  : public CDevice::CGain
	{
		CSV1300FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV1300FCShutter  : public CDevice::CShutter
	{
		CSV1300FCShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV1300FC;
	};
	friend class CSV1300FCShutter;

	/// Inquire, get and set all scalar properties.
	class CSV1300FCBlackLevel  : public CDevice::CBlackLevel
	{
		CSV1300FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCBlackLevel;


	class CSV1300FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV1300FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCSnapMode;

	class CSV1300FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV1300FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCTriggerPolarity;


	class CSV1300FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV1300FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCStrobePolarity;

	
	class CSV1300FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV1300FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCADCLevel;
	
	class CSV1300FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV1300FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCSnapSpeed;

	class CSV1300FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV1300FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCBlackLevelEnable;

	class CSV1300FCBlank : public CDevice::CBlank
	{
	public:
		CSV1300FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV1300FC;
	};
	friend class CSV1300FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV1300FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















