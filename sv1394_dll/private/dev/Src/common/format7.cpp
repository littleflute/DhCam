//File: format7.cpp

#include "CAMOBJ.h"


BOOL CFormat7::IsColorCodeAvailable(int nColorCode)
{
    DWORD mask = 0x80000000;
    BOOL retval;

    mask >>= nColorCode;
    retval = ((m_dwColorCodesMask & mask) != 0);

    return retval;
}

DWORD CFormat7::SetPosition(DWORD left, DWORD top)
{
    // For optimization
    if (left == m_Left && top == m_Top)
        return 0;

	DWORD retval;
	DWORD quadlet;

	if (m_offset == 0)
		return STATUS_INVALID_CAMERA;

	// Round to the nearest m_unitXpos

	left += m_Hposunit >> 1;
	left -= left % m_Hposunit;

	top += m_Vposunit >> 1;
	top -= top % m_Vposunit;

	// Insert a bit of idiotproofing here
	if ((left + m_Hunit) >= m_Hmax || (top + m_Vunit) >= m_Vmax) {
		return STATUS_INVALID_PARAMETER;
	}
	
	if ((left + m_Width) > m_Hmax || (top + m_Height) > m_Vmax) {
		return STATUS_INVALID_PARAMETER;
	}

	// Set members and write quadlet if things have changed
	if (left != m_Left || top != m_Top)	{
		m_Left = (USHORT)left;
		m_Top = (USHORT)top;
		
		quadlet = left;
		quadlet <<= 16;
		quadlet += top;
		retval = m_pCamera->WriteQuadlet(m_offset + 0x08, quadlet);
		VerifyRetVal(retval);
	} 

	return 0;
}

DWORD CFormat7::SetSize(DWORD width, DWORD height)
{
	DWORD retval;
	DWORD quadlet;
	DWORD newleft = m_Left, newtop = m_Top;

	// Round to the nearest m_unitX.
	width += m_Hunit >> 1;
	width -= width % m_Hunit;

	height += m_Vposunit >> 1;
	height -= height % m_Vunit;

    width  >>= 2;
    width  <<= 2;
    height >>= 1;
    height <<= 1;

    // Optimization.
    if (width == m_Width && height == m_Height)
        return 0;

	// insert a bit of idiotproofing here.
	if (width == 0 || width > m_Hmax || height == 0 || height > m_Vmax) {
		return STATUS_INVALID_PARAMETER;
	}

	if ((m_Left + width) > m_Hmax) {
		newleft = m_Hmax - width;
	}

	if ((m_Top + height) > m_Vmax) {
		newtop = m_Vmax - height;	
	}

	if (m_Left != newleft || m_Top != newtop) {
		retval = SetPosition(newleft,newtop);
        VerifyRetVal(retval);
	}

	// Set members and write quadlet if things have changed
	m_Width = (USHORT)width;
	m_Height = (USHORT)height;
	quadlet = width;
	quadlet <<= 16;
	quadlet += height;
	retval = m_pCamera->WriteQuadlet(m_offset + 0x0c, quadlet);
	VerifyRetVal(retval);

	return Update();
}


DWORD CFormat7::Status()
{
	DWORD status;
	DWORD retval;

	// Size control present?
	if (!Supported())
		return STATUS_CAMERA_IO_ERROR;

	// Mode supported?
	if (!ModeSupported(m_pCamera->m_videoMode))
		return STATUS_INVALID_MODE;

	if (m_offset == 0)
		return STATUS_INVALID_CAMERA;

	// Position at 0x08
    retval = m_pCamera->ReadQuadlet(m_offset + 0x08, &status);
    VerifyRetVal(retval);
	m_Top = (USHORT)(status & 0xffff);
	m_Left = (USHORT)(status >> 16);

	// Size at 0x0c
    retval = m_pCamera->ReadQuadlet(m_offset + 0x0c, &status);
	VerifyRetVal(retval);
	m_Height = (USHORT)(status & 0xffff);
	m_Width = (USHORT)(status >> 16);

	// Current color code at 0x10
    retval = m_pCamera->ReadQuadlet(m_offset + 0x10, &status);
	VerifyRetVal(retval);
	m_codingID = (status>>24) & 0x00ff;

	retval = m_pCamera->ReadQuadlet(m_offset + 0x34,&m_PixelPerFrame);
	VerifyRetVal(retval);

	retval = m_pCamera->ReadQuadlet(m_offset + 0x38,&m_BytePerFrameHi);
	VerifyRetVal(retval);

	retval = m_pCamera->ReadQuadlet(m_offset + 0x3c, &m_BytePerFrameLo);
	VerifyRetVal(retval);

	retval = m_pCamera->ReadQuadlet(m_offset + 0x40, &status); 
	VerifyRetVal(retval);
	m_MaxBytePerPacket = (USHORT)(status & 0xffff);
	m_MinBytePerPacket = (USHORT)(status >> 16);

	retval = m_pCamera->ReadQuadlet(m_offset + 0x44, &status); 
	VerifyRetVal(retval);
	m_BytePerPacket = (USHORT)(status >> 16);

	if(m_BytePerPacket < m_MinBytePerPacket)
		m_BytePerPacket = m_MinBytePerPacket;

	// Packets per frame as of 1.30
	retval = m_pCamera->ReadQuadlet(m_offset + 0x48, &status);
    if (retval != STATUS_OK)
		status = 0;

	if (status != 0)
		m_PacketPerFrame = status;
	else
		m_PacketPerFrame = m_BytePerFrameLo / m_BytePerPacket;
		
	// Value setting register at 0x7C as of 1.30
	retval = m_pCamera->ReadQuadlet(m_offset + 0x7C, &status);
    if (retval != STATUS_OK)
		status = 0;

	ErrorFlag_1 = (status & 0x00800000) != 0;
	ErrorFlag_2 = (status & 0x00400000) != 0;

	_RPT3(_CRT_WARN," CFormat7::Status(): m_PacketPerFrame = %d\n \
		\t\t m_BytePerPacket=%d \t\t size=%d\n",
		m_PacketPerFrame,m_BytePerPacket,
		m_PacketPerFrame*m_BytePerPacket);
	return STATUS_OK;
}

DWORD CFormat7::Inquire()
{
	DWORD status;
	DWORD retval;

	// Size control present?
	if (!Supported())
		return STATUS_CAMERA_IO_ERROR;

	// mode supported?
	if (!ModeSupported(m_pCamera->m_videoMode))
		return STATUS_INVALID_MODE;


    retval = m_pCamera->ReadQuadlet(IIDC_FORMAT7_OFFSET + 4*m_pCamera->m_videoMode, &status);
	VerifyRetVal(retval);

	m_offset = 4*status + 0xf0000000;
    

	// Maximum dimensions at 0x00
    retval = m_pCamera->ReadQuadlet(m_offset + 0x00, &status);
	VerifyRetVal(retval);

	m_Vmax = (USHORT)(status & 0xffff);
	m_Hmax = (USHORT)(status >> 16);

	// Size unit at 0x04
    retval = m_pCamera->ReadQuadlet(m_offset + 0x04, &status);
	VerifyRetVal(retval);
    
	m_Vunit = (USHORT)(status & 0xffff);
	m_Hunit = (USHORT)(status >> 16);

	// position unit was added in 1.30
	// try to read 0x4c, if zero or fail, use size unit
    retval = m_pCamera->ReadQuadlet(m_offset + 0x4c, &status);
	if (retval != STATUS_OK) {
		status = 0;
	}

	if (status == 0) {
		m_Hposunit = m_Hunit;
		m_Vposunit = m_Vunit;
	} else {
		m_Hposunit = (status >> 16) & 0xffff;
		m_Vposunit = (status) & 0xffff;
	}

	// Color codes at 0x14
    retval = m_pCamera->ReadQuadlet(m_offset + 0x14, &status);
    VerifyRetVal(retval);

    m_dwColorCodesMask = status;

	// value setting register at 0x7C as of 1.30
    retval = m_pCamera->ReadQuadlet(m_offset + 0x7C, &status);
	if (retval != STATUS_OK)
		status = 0;

	m_Setting_1 = (status & 0x80000000) != 0;

    //
    // Init other offsets used by CCD cameras.
    //
    if (m_pCamera->m_CameraType == CCD_CAMERA) {
        retval = m_pCamera->ReadQuadlet(ADVANCED_FEATURE_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetAdvanceFeature = 4*status + 0xf0000000;
        
        retval = m_pCamera->ReadQuadlet(PIO_CONTROL_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetPIOControlFeature = 4*status + 0xf0000000;
        
        retval = m_pCamera->ReadQuadlet(SIO_CONTROL_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetSIOControlFeature = 4*status + 0xf0000000;
        
        retval = m_pCamera->ReadQuadlet(STROBE_CONTROL_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetStrobeControl = 4*status + 0xf0000000;
        
        retval = m_pCamera->ReadQuadlet(0xf2F00000 + USER_CUSTOM_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetUserCustom = 4*status + 0xf0000000;
        
        retval = m_pCamera->ReadQuadlet(0xf2F00000 + LUT_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetLUT = 4*status + 0xf0000000;
        
        retval = m_pCamera->ReadQuadlet(0xf2F00000 + ENCRYPT_OFFSET, &status);
        VerifyRetVal(retval);  
        m_offsetEncrypt = 4*status + 0xf0000000;
    }

	return STATUS_OK;
}

DWORD CFormat7::Update()
{
	DWORD retval;
	DWORD status;

	//kludge... must fix later
	if (m_Setting_1) {
		retval = m_pCamera->WriteQuadlet(m_offset + 0x7C,0x40000000);
		VerifyRetVal(retval);

		while(retval = m_pCamera->ReadQuadlet(m_offset + 0x7C,&status))
        {
            if(!(status & 0x40000000))
                break;
        }
			
	}
	
	retval = Status();
    VerifyRetVal(retval);
	
	m_pCamera->m_Width = m_Width;
	m_pCamera->m_Height = m_Height;
	
	return STATUS_OK;
}

BOOL CFormat7::Supported()
{
    DWORD status;
    DWORD retval;
    
    if(retval = m_pCamera->ReadQuadlet(0x100,&status))
        return FALSE;
    
    return (status & 0x01000000) != 0;
}

BOOL CFormat7::ModeSupported(int mode)
{
    DWORD status;
    DWORD temp = 0x80000000;
    DWORD retval;
    
    if(retval = m_pCamera->ReadQuadlet(0x19c,&status))
        return FALSE;
    
    temp >>= mode;
    return ((status & temp) != 0);
}



DWORD CFormat7::SetColorCode(int code)
{
	DWORD retval;
	DWORD quadlet;

    if (code < 0 || code >= 8 || !IsColorCodeAvailable(code)) {
		return STATUS_INVALID_COLORCODE;
	}

	m_codingID = code;
	quadlet = code;
	quadlet <<= 24;

	retval = m_pCamera->WriteQuadlet(m_offset + 0x10, quadlet);
	VerifyRetVal(retval);

	return Update();
}

	
DWORD CFormat7::SetBytesPerPacket(int bytes)
{
	// need more idiotproofing here
	DWORD retval;
	DWORD quadlet;
    
	m_BytePerPacket = (USHORT)bytes;
	quadlet = bytes;
	quadlet <<= 16;

	retval = m_pCamera->WriteQuadlet(m_offset + 0x44, quadlet);
	VerifyRetVal(retval);

	//Qiao,2005.12.30
	Update();
	//end //Qiao,2005.12.30

	return STATUS_OK;
}