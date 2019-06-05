//File: feature.cpp
#include "CAMOBJ.h"

HVSTATUS CFeature::GetInquireRegister() 
{
	HVSTATUS retval = m_pCamera->ReadQuadlet(m_FeatureOffset + INQUIRY_INDEX, &m_inquire);
    VerifyRetVal(retval);
	return retval;
}

HVSTATUS CFeature::GetStatusRegister()
{
	HVSTATUS retval = m_pCamera->ReadQuadlet(m_FeatureOffset + STATUS_INDEX, &m_status);
	return retval;
}
HVSTATUS CFeature::SetStatusRegister()
{
	HVSTATUS retval = m_pCamera->WriteQuadlet(m_FeatureOffset + STATUS_INDEX, m_status);
	return retval;
}


//value is write to bit[..31]
HVSTATUS CFeature::SetValue( DWORD value,DWORD ubvalue)
{
	HVSTATUS retval = STATUS_OK;
	
    if (m_bPresent) {
        if ((m_bOnOff && m_OnOff) || (m_bOnOff == FALSE)) {
            m_value = (value & 0xFFFUL);
            m_ubvalue = (ubvalue & 0xFFFUL);
            retval = m_pCamera->WriteQuadlet(m_FeatureOffset + STATUS_INDEX, m_status);
            VerifyRetVal(retval);
        }
    }  

	return retval;
}



