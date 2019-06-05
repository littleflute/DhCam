//File: format7.h

#ifndef __FORMAT7_H__
#define __FORMAT7_H__//
/////////////////////////////////////////////////////////////////////////////
// Stardand IIDC format7 offset.
//
#define IIDC_FORMAT7_OFFSET        0x2E0

//
// CCD camera new add features offset. Please use these offsets
// to read the final address in camera CSR.
//
#define ADVANCED_FEATURE_OFFSET    0x480
#define PIO_CONTROL_OFFSET         0x484
#define SIO_CONTROL_OFFSET         0x488
#define STROBE_CONTROL_OFFSET      0x48C
#define USER_CUSTOM_OFFSET         0x480
#define LUT_OFFSET                 0x484
#define ENCRYPT_OFFSET             0x488


class CCamera;
class CFormat7
{
public:
    CCamera* m_pCamera;

    USHORT m_Hmax;
    USHORT m_Vmax;
    USHORT m_Hunit;
    USHORT m_Vunit;
    USHORT m_Top;
    USHORT m_Left;
    USHORT m_Width;
    USHORT m_Height;
    UINT m_codingID;

    DWORD m_dwColorCodesMask;

    ULONG m_PixelPerFrame;
    ULONG m_BytePerFrameHi;
    ULONG m_BytePerFrameLo;

    USHORT m_MinBytePerPacket;
    USHORT m_MaxBytePerPacket;

    ULONG m_PacketPerFrame;

    USHORT m_BytePerPacket;

    UINT m_Hposunit;
    UINT m_Vposunit;

    // Value_Setting.
    UINT m_Presence       : 1;
    UINT m_Setting_1      : 1;
    UINT                  : 6;
    UINT ErrorFlag_1      : 1;
    UINT ErrorFlag_2      : 1;

    BOOL  Supported();

    DWORD SetSize(DWORD width, DWORD height);
    DWORD SetPosition(DWORD left, DWORD top);
    BOOL  IsColorCodeAvailable(int nColorCode);
    DWORD SetColorCode(int code);
    BOOL  ModeSupported(int mode);
    DWORD SetBytesPerPacket(int bytes);

    DWORD Inquire();
    DWORD Status();

private:
    DWORD Update();
    DWORD m_offset;

    //
    // CCD Camera add offsets.
    //
    DWORD m_offsetAdvanceFeature;
    DWORD m_offsetPIOControlFeature;
    DWORD m_offsetSIOControlFeature;
    DWORD m_offsetStrobeControl;
    DWORD m_offsetUserCustom;
    DWORD m_offsetLUT;
    DWORD m_offsetEncrypt;
};
/////////////////////////////////////////////////////////////////////////////
#endif // __FORMAT7_H__
