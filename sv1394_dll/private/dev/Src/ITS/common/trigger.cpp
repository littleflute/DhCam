//File trigger.cpp
#include "CAMOBJ.h"

DWORD CTrigger::Get_Status()
{
	DWORD ulBits;
	HVSTATUS retval = m_pCamera->ReadQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, &ulBits);
	VerifyRetVal(retval);
    m_dwStatus = ulBits;
	return ulBits;
}

DWORD CTrigger::Inquire()
{
	DWORD ulBits;
	HVSTATUS retval = m_pCamera->ReadQuadlet(INQUIRY_INDEX + TRIGGER_MODE_OFFSET, &ulBits);
    VerifyRetVal(retval);
    m_inquire = ulBits;
	return ulBits;
}


HVSTATUS CTrigger::SetSource(DWORD source)
{
	DWORD ulBits=0;
	if(source >1)
		return STATUS_PARAMETER_INVALID;


	HVSTATUS retval;
	// 保存当前TriggerMode
	// Modify by lvxiaofeng
	//HVSTATUS retval = m_pCamera->ReadQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, &ulBits);	//TRIGGER_MODE: 0x830
	//VerifyRetVal(retval);
	ulBits = Get_Status();
	if(source == 1)		//internel trigger(for soft trigger),BIT[8,9,10]=[1,1,1]
	{
		//retval = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, ulBits | 0x00200000L);
		retval = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, ulBits | 0x00E00000L);		//2006.2.23
		VerifyRetVal(retval);
	}
	else	//if(source == 0)		//outside trigger,BIT[8,9,10]=[0,0,0]
	{
		//retval = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, ulBits & 0xFFDFFFFFL);
		retval = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, ulBits & 0xFF1FFFFFL);	//2006.2.23
		VerifyRetVal(retval);
	}
	return retval;
}

HVSTATUS CTrigger::Set_On_Off(BOOL b)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	Get_Status();
	m_OnOff = b;
	st = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, m_dwStatus);	
	return st;
}


HVSTATUS CTrigger::Set_Polarity(BOOL p)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	Get_Status();
	m_TriggerPolarity = p;
	st = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, m_dwStatus);	
	return st;
}

HVSTATUS CTrigger::Set_Source(UINT uiVal)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	Get_Status();
	m_TriggerSource = uiVal;
	st = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, m_dwStatus);	
	return st;
}
HVSTATUS CTrigger::Set_Mode(UINT uiVal)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	Get_Status();
	m_TriggerMode = uiVal;
	st = m_pCamera->WriteQuadlet(STATUS_INDEX + TRIGGER_MODE_OFFSET, m_dwStatus);	
	return st;
}
//////////////////////////////////////////////////////////////////////////////////////

DWORD CStrobe::Get_Inquire_Reg()
{
	DWORD ulBits;
	HVSTATUS retval = m_pCamStrobe->ReadQuadlet(STROBE_BASE_ADDR + 0x100, &ulBits);
    VerifyRetVal(retval);
    m_StrobeInq = ulBits;
	return ulBits;
}

DWORD CStrobe::Get_Status_Reg()
{
	DWORD ulBits;
	HVSTATUS retval = m_pCamStrobe->ReadQuadlet(STROBE_BASE_ADDR + 0x200, &ulBits);
	VerifyRetVal(retval);
    m_dwStatus = ulBits;
	return ulBits;
}



HVSTATUS CStrobe::Set_On_Off(BOOL b)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	Get_Status_Reg();
	OnOff = b;
	st = m_pCamStrobe->WriteQuadlet(STROBE_BASE_ADDR + 0x200, m_dwStatus);	
	return st;
}
HVSTATUS CStrobe::Set_Polarity(BOOL p)
{
	HVSTATUS st = STATUS_NOT_SUPPORT_INTERFACE;
	Get_Status_Reg();
//	SignalPolarity = !p; //因为FPGA里设置的极性是反的，所以这里取反
	SignalPolarity = p; //去掉取反,与冯立志调试SV1411FC
	st = m_pCamStrobe->WriteQuadlet(STROBE_BASE_ADDR + 0x200, m_dwStatus);	
	return st;
}