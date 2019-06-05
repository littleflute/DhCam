// ITS2000FC.h: interface for the CITS2000FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITS2000FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_ITS2000FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#include "..\Device.h"

    

class CITS2000FC : public CDevice  
{
public:
	CITS2000FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CITS2000FC();
	
	class CITS2000FCInfo : public CDevice::CInfo
	{
	public:
		CITS2000FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  

		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_ITS2000FC,lpTemp,MAX_PATH);	
		  CString str;
		  str.Format("%s-%d", lpTemp, DeviceNumber());
		  return str;
		}

		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_ITS2000FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp );
			return str;
		}
		virtual HVTYPE DeviceType() {return ITS2000FCTYPE;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCInfo;

	/// Inquire, get and set all scalar properties.
	class CITS2000FCGain  : public CDevice::CGain
	{
		CITS2000FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 1023;}
	
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CITS2000FCShutter  : public CDevice::CShutter
	{
		CITS2000FCShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		
		virtual unsigned long Min(){ 
			ULONG ulRet;
			ulRet = m_pDevice->m_ShutterRange.minV;
			return ulRet;
		}
		virtual unsigned long Max(){	// add by xupx 0530
			ULONG ulRet;
			ulRet = m_pDevice->m_ShutterRange.maxV;
			return ulRet;
		}
		
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
		friend class CITS2000FC;
	};
	friend class CITS2000FCShutter;

	/// Inquire, get and set all scalar properties.
	class CITS2000FCBlackLevel  : public CDevice::CBlackLevel
	{
		CITS2000FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	            CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCBlackLevel;


	class CITS2000FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CITS2000FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCSnapMode;

	class CITS2000FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CITS2000FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCTriggerPolarity;


	class CITS2000FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CITS2000FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCStrobePolarity;

	
	class CITS2000FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CITS2000FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCADCLevel;
	
	class CITS2000FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CITS2000FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCSnapSpeed;

	class CITS2000FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CITS2000FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCBlackLevelEnable;

	class CITS2000FCBlank : public CDevice::CBlank
	{
	public:
		CITS2000FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return false;}
		  
	private:
		friend class CITS2000FC;
	};
	friend class CITS2000FCBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_ITS2000FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















