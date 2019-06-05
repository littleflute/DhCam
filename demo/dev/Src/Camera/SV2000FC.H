// SV2000FC.h: interface for the CSV2000FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SV2000FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_SV2000FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CSV2000FC : public CDevice  
{
public:
	CSV2000FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CSV2000FC();
	
	class CSV2000FCInfo : public CDevice::CInfo
	{
	public:
		CSV2000FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_SV2000FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_SV2000FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return SV2000FCTYPE;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCInfo;

	/// Inquire, get and set all scalar properties.
	class CSV2000FCGain  : public CDevice::CGain
	{
		CSV2000FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CSV2000FCShutter  : public CDevice::CShutter
	{
		CSV2000FCShutter(unsigned long Value, CDevice *pDevice) :
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
		friend class CSV2000FC;
	};
	friend class CSV2000FCShutter;

	/// Inquire, get and set all scalar properties.
	class CSV2000FCBlackLevel  : public CDevice::CBlackLevel
	{
		CSV2000FCBlackLevel(unsigned long Value, CDevice *pDevice) :
			CBlackLevel(Value, pDevice)
		{}
	
		bool IsSupported() {return false;}
	
		virtual unsigned long Min(){ return -255;}
		virtual unsigned long Max(){ return 255;}
	
		private:
			friend class CSV2000FC;
	};
	friend class CSV2000FCBlackLevel;


	class CSV2000FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CSV2000FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV1310FCSnapMode;

	class CSV2000FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CSV2000FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCTriggerPolarity;


	class CSV2000FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CSV2000FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCStrobePolarity;

	
	class CSV2000FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CSV2000FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCADCLevel;
	
	class CSV2000FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CSV2000FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCSnapSpeed;

	class CSV2000FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CSV2000FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCBlackLevelEnable;

	class CSV2000FCBlank : public CDevice::CBlank
	{
	public:
		CSV2000FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CSV2000FC;
	};
	friend class CSV2000FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_SV2000FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















