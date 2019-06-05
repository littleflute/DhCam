// HV1300FM.h: interface for the CHV1300FM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HV1300FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_HV1300FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"



class CHV1300FM : public CDevice  
{
public:
	CHV1300FM(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CHV1300FM();
	
	class CHV1300FMInfo : public CDevice::CInfo
	{
	public:
		CHV1300FMInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_HV1300FM,lpTemp,MAX_PATH);		  
		  CString str;
		  str.Format("%s-%d", lpTemp,DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_HV1300FM,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp);
			return str;
		}
		virtual HVTYPE DeviceType() {return HV1300FMTYPE;}

		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMInfo;

	/// Inquire, get and set all scalar properties.
	class CHV1300FMGain  : public CDevice::CGain
	{
		CHV1300FMGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 127;}
	
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMGain;
	
	/// Inquire, get and set all scalar properties.
	class CHV1300FMShutter  : public CDevice::CShutter
	{
		CHV1300FMShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		
		virtual unsigned long Min(){
			ULONG ulRet;
			TUnit u = m_pDevice->m_pShutter->m_Unit.Value();
			switch(u){
			case UNIT_MICROSECOND:
				ulRet = 60;
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
		//	if (Value > Max() || Value < Min())
		//		throw HVBaseException(STATUS_PARAMETER_INVALID, "CShutter::Set()");
			Value = Value<Min()?Min():Value;
			Value = Value>Max()?Max():Value;
			
			//Ê±ÖÓÆµÂÊ
			long lClockFreq = m_pDevice->GetClockFreq();
			lClockFreq = lClockFreq/ m_Unit.Ratio();

			long lTb = m_pDevice->m_pBlank->Hor();
			lTb += 9;
			lTb -= 19;	
			if(lTb <= 0) lTb =0;
			
			unsigned long Register = 0;
			Register = ((Value * lClockFreq)+ 180.0) / (m_pDevice->Format.ImageSize().cx + 244.0 + lTb);
			if (Register <= 0)
				Register = 1;
			else if (Register > 16383) 
				Register = 16383;

			//HYL 2006.9.14
			
			HV_AEC_CONTROL_CONTEXT AECCtrl;
			AECCtrl.ctrl=AEC_EXPOSURE_TIME;

			CShutter::Set(Register,&AECCtrl);


	//		CShutter::Set(Register);
			m_AbsValue = Value;

		}
	
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMShutter;

	/// Inquire, get and set all scalar properties.
	class CHV1300FMBlackLevel  : public CDevice::CBlackLevel
	{
		CHV1300FMBlackLevel(unsigned long Value, CDevice *pDevice) :
	CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMBlackLevel;


	class CHV1300FMSnapMode : public CDevice::CSnapMode
	{
	public:
		CHV1300FMSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMSnapMode;

	class CHV1300FMTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CHV1300FMTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMTriggerPolarity;


	class CHV1300FMStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CHV1300FMStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMStrobePolarity;

	
	class CHV1300FMADCLevel : public CDevice::CADCLevel
	{
	public:
		CHV1300FMADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMADCLevel;
	
	class CHV1300FMSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CHV1300FMSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMSnapSpeed;
	
	class CHV1300FMBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CHV1300FMBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMBlackLevelEnable;

	class CHV1300FMBlank : public CDevice::CBlank
	{
	public:
		CHV1300FMBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV1300FM;
	};
	friend class CHV1300FMBlank;
	
	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_HV1300FM_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)





















