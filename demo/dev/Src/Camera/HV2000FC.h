// HV2000FC.h: interface for the CHV2000FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HV2000FC_H__93CC5358_4DA2_4D31_9EEB_D73A07D4F596__INCLUDED_)
#define AFX_HV2000FC_H__93CC5358_4DA2_4D31_9EEB_D73A07D4F596__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"


class CHV2000FC : public CDevice 
{
public:
	CHV2000FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CHV2000FC();

	class CHV2000FCInfo : public CDevice::CInfo
	{
	public:
		CHV2000FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_HV2000FC,lpTemp,MAX_PATH);		  
		  CString str;
		  str.Format("%s-%d", lpTemp,DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_HV2000FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp);
			return str;
		}
		virtual HVTYPE DeviceType() {return HV2000FCTYPE;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCInfo;

	/// Inquire, get and set all scalar properties.
	class CHV2000FCGain  : public CDevice::CGain
	{
		CHV2000FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 127;}
	
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CHV2000FCShutter  : public CDevice::CShutter
	{
		CHV2000FCShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		
		virtual unsigned long Min(){
			ULONG ulRet;
			TUnit u = m_pDevice->m_pShutter->m_Unit.Value();
			switch(u){
			case UNIT_MICROSECOND:
				ulRet = 73;
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
			
			//时钟频率
			long lClockFreq = m_pDevice->GetClockFreq();

			lClockFreq = lClockFreq/ m_Unit.Ratio();
			long lTb = m_pDevice->m_pBlank->Hor();
			lTb += 53;
		//----------2007.08.14 修改-----------
			if(lTb < 19) lTb = 19;
			unsigned long Register = 0;
			long lTrow = max(617, m_pDevice->Format.ImageSize().cx + 305.0 + lTb);
			float fReg = ( (Value * lClockFreq) + 180.0) / lTrow + 1;
			if((fReg - (int)fReg) > 0.5){				//  对数据 fReg 进行四舍五入
				fReg += 1.0;
			}
			Register = (unsigned long)fReg;
		//------------------------------------
			if (Register <= 0)
				Register = 1;
			else if (Register > 16383) 
				Register = 16383;

			
			//HYL 2006.9.13
			
			HV_AEC_CONTROL_CONTEXT AECCtrl;
			AECCtrl.ctrl=AEC_EXPOSURE_TIME;

			CShutter::Set(Register,&AECCtrl);


	//		CShutter::Set(Register);
			m_AbsValue = Value;
			
		}
	
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCShutter;
	
	/// Inquire, get and set all scalar properties.
	class CHV2000FCBlackLevel  : public CDevice::CBlackLevel
	{
		CHV2000FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCBlackLevel;


	class CHV2000FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CHV2000FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCSnapMode;

	class CHV2000FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CHV2000FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCTriggerPolarity;
	

	class CHV2000FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CHV2000FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCStrobePolarity;
	

	class CHV2000FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CHV2000FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCADCLevel;
	
	class CHV2000FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CHV2000FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.11
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCSnapSpeed;
	
	class CHV2000FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CHV2000FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCBlackLevelEnable;

	class CHV2000FCBlank : public CDevice::CBlank
	{
	public:
		CHV2000FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV2000FC;
	};
	friend class CHV2000FCBlank;

	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		virtual void CreateFeatures();
		virtual void DestoryFeatures();
};

#endif // !defined(AFX_HV2000FC_H__93CC5358_4DA2_4D31_9EEB_D73A07D4F596__INCLUDED_)
