// HV3100FC.h: interface for the CHV3100FC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HV3100FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
#define AFX_HV3100FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\Device.h"


class CHV3100FC : public CDevice  
{
public:
	CHV3100FC(int nNumber, int nIndex, HWND hWndNotify, CMainFrame& MainFrame);
	virtual ~CHV3100FC();
	
	class CHV3100FCInfo : public CDevice::CInfo
	{
	public:
		CHV3100FCInfo(CDevice *pDevice):
		  CInfo(pDevice)	
		  {}
		  
		virtual CString DeviceName() 
		{
		  char lpTemp[256];		  
		  LoadString(NULL,IDS_STRING_HV3100FC,lpTemp,MAX_PATH);		  
		  CString str;
		  str.Format("%s-%d", lpTemp,DeviceNumber());
		  return str;
		}
		virtual CString ModelName()
		{
			char lpTemp[256];		  
			LoadString(NULL,IDS_STRING_HV3100FC,lpTemp,MAX_PATH);	
			CString str;
			str.Format("%s", lpTemp);
			return str;
			//return "HV3100FC";
		}
		virtual HVTYPE DeviceType() {return HV3100FCTYPE;}

		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCInfo;

	/// Inquire, get and set all scalar properties.
	class CHV3100FCGain  : public CDevice::CGain
	{
		CHV3100FCGain(unsigned long Value, CDevice *pDevice) :
			CGain(Value, pDevice)
		{}
	
		bool IsSupported() {return true;}
	
		virtual unsigned long Min(){ return 0;}
		virtual unsigned long Max(){ return 127;}
	
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCGain;
	
	/// Inquire, get and set all scalar properties.
	class CHV3100FCShutter  : public CDevice::CShutter
	{
		CHV3100FCShutter(unsigned long Value, CDevice *pDevice) :
		CShutter(Value, pDevice)
		{}
		
		bool IsSupported() {return true;}
		
		virtual unsigned long Min(){
			ULONG ulRet;
			TUnit u = m_pDevice->m_pShutter->m_Unit.Value();
			switch(u){
			case UNIT_MICROSECOND:
				ulRet = 106;
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
		//----------2007.08.14 --------------
			lTb += 142;				//  确保数据为正值
			if (lTb < 21)			//使得lTb（即P3）为大于21的数值
				lTb = 21;
			long lP1 = 331, lP2 = 38; 
			long lP4 = 316;	
			if(m_pDevice->Format.Resolution() == RES_MODE1){
				lP1 = 673;
				lP2 = 22;
				lP4 = 316 * 2;
			}			
			long lAQ = m_pDevice->Format.ImageSize().cx + lP1 + lP2 + lTb;

			long lTrow = max(lAQ, (lP1 + lP4));
			
			float fReg = ((Value * lClockFreq) + lP1 - 132.0) / lTrow;
			if((fReg - (int)fReg) > 0.5){				//  对数据 fReg 进行四舍五入
				fReg += 1.0;
			}
			unsigned long Register;
			Register = (unsigned long)fReg;
		//------------------------------------
			if (Register <= 0)
				Register = 1;
			else if (Register > 1048575) 
				Register = 1048575;

			//HYL 2006.9.13
			
			HV_AEC_CONTROL_CONTEXT AECCtrl;
			AECCtrl.ctrl=AEC_EXPOSURE_TIME;

			CShutter::Set(Register,&AECCtrl);


	//		CShutter::Set(Register);

			m_AbsValue = Value;

		}
	
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCShutter;

	/// Inquire, get and set all scalar properties.
	class CHV3100FCBlackLevel  : public CDevice::CBlackLevel
	{
		CHV3100FCBlackLevel(unsigned long Value, CDevice *pDevice) :
	CBlackLevel(Value, pDevice)
	{}
	
	bool IsSupported() {return true;}
	
	virtual unsigned long Min(){ return -255;}
	virtual unsigned long Max(){ return 255;}
	
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCBlackLevel;

	
	class CHV3100FCSnapMode : public CDevice::CSnapMode
	{
	public:
		CHV3100FCSnapMode(int Value, CDevice *pDevice):
		  CSnapMode(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCSnapMode;

	class CHV3100FCTriggerPolarity : public CDevice::CTriggerPolarity
	{
	public:
		CHV3100FCTriggerPolarity(int Value, CDevice *pDevice):
		  CTriggerPolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCTriggerPolarity;
	

	class CHV3100FCStrobePolarity : public CDevice::CStrobePolarity
	{
	public:
		CHV3100FCStrobePolarity(int Value, CDevice *pDevice):
		  CStrobePolarity(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCStrobePolarity;

	class CHV3100FCADCLevel : public CDevice::CADCLevel
	{
	public:
		CHV3100FCADCLevel(int Value, CDevice *pDevice):
		  CADCLevel(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCADCLevel;
	
	class CHV3100FCSnapSpeed : public CDevice::CSnapSpeed
	{
	public:
		CHV3100FCSnapSpeed(int Value, CDevice *pDevice):
		  CSnapSpeed(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return FALSE;}//2007.4.12
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCSnapSpeed;

	class CHV3100FCBlackLevelEnable : public CDevice::CBlackLevelEnable
	{
	public:
		CHV3100FCBlackLevelEnable(int Value, CDevice *pDevice):
		  CBlackLevelEnable(Value, pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCBlackLevelEnable;
	
	class CHV3100FCBlank : public CDevice::CBlank
	{
	public:
		CHV3100FCBlank(int hor, int vert, CDevice *pDevice):
		  CBlank(hor, vert,  pDevice)	
		  {}
		  
		  bool IsSupported() {return true;}
		  
	private:
		friend class CHV3100FC;
	};
	friend class CHV3100FCBlank;

	protected:
//		virtual	CSize SensorSize(HVResolution mode) ;
		
	private:
		void CreateFeatures();
		void DestoryFeatures();
};

#endif // !defined(AFX_HV3100FC_H__64A5CAC2_83A7_4C1E_9747_4D92B4F47CDE__INCLUDED_)
