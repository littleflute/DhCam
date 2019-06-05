//File: feature.h

#ifndef __FEATURE_STDAFX_H__
#define __FEATURE_STDAFX_H__

// The root register offsets
#define INQUIRY_INDEX            0x500
#define STATUS_INDEX             0x800

// The individual control offsets within the root offsets
#define BRIGHTNESS_OFFSET        0x00
#define AUTO_EXPOSURE_OFFSET     0x04
#define SHARPNESS_OFFSET         0x08
#define WHITE_BALANCE_OFFSET     0x0C
#define HUE_OFFSET               0x10
#define SATURATION_OFFSET        0x14
#define GAMMA_OFFSET             0x18
#define SHUTTER_OFFSET           0x1C
#define GAIN_OFFSET              0x20
#define IRIS_OFFSET              0x24
#define FOCUS_OFFSET             0x28
#define TEMPERATURE_OFFSET       0x2C
#define TRIGGER_MODE_OFFSET      0x30
#define TRIGGER_DLY_OFFSET       0x34
#define WHITE_SHD_OFFSET         0x38
#define FRAME_RATE_OFFSET        0x3C

// 0x40 - 0x7C is reserved for other FEATURE_HI

#define ZOOM_OFFSET              0x80
#define PAN_OFFSET               0x84
#define TILT_OFFSET              0x88
#define OPTICAL_FILTER_OFFSET    0x8C
// 0x90 - 0xBC is reserved for other FEATURE_LO

#define CAPTURE_SIZE_OFFSET      0xC0
#define CAPTURE_QUALITY_OFFSET   0xC4
// 0xC8 - 0xFC is reserved for other FEATURE_LO

//////////////////////////////////////////////////////////////////
class CCamera;
class CFeature
{
public:
    union {
        struct {
	        // Feature inquiry flags.
            UINT m_Max         : 12;  // Maximum value for this feature control.
            UINT m_Min         : 12;  // Minimum value for this feature control.
            UINT m_bManual     : 1;   // Manual mode (Controlled by user)
            UINT m_bAuto       : 1;   // Auto mode (Controlled automatically by camera)
            UINT m_bOnOff      : 1;   // Capability of switching this feature ON and OFF.
            UINT m_bReadOut    : 1;   // Capability of reading the value of this feature.
            UINT m_bOnePush    : 1;   // One push auto mode(controlled automatically by 
                                      // camera only once.
            UINT               : 1;   // Reserved.
            UINT m_bAbsControl : 1;   // Capability of control with absolute value.
            UINT m_bPresent    : 1;   // Presence of this feature.
        };

        DWORD m_inquire;
    };
	
    union {
        struct {
            // Feature status flags.
            UINT m_value       : 12;  // bits 20 to 31.
                                      // Write the value in Autol mode, this field is
                                      // ignored. If "ReadOut" Capability is not avail-
                                      // able, read value has not meaning.
            UINT m_ubvalue     : 12;  // bits 8 to 19, only used in WHITE_BALANCE.
                                      // U value / B Value.
                                      // Write the value in AUTO mode, this field is 
                                      // ignored. If "ReadOut" capability is not avail-
                                      // able, read value has no mean.
            UINT m_AutoManual  : 1;   // Write: set the mode.
                                      // Read:  read a current mode.
                                      // 0: Manual, 1: Auto.
            UINT m_OnOff       : 1;   // Write: ON or OFF this feature.
                                      // Read:  read a status.
                                      // 0: OFF, 1: ON If this bit=0, other fields will
                                      // be read only.
            UINT m_OnePush     : 1;   // Write '1': begin to work(Self cleared after operation)
                                      // Read: Value='1' in operation
                                      //       Value='0' not in operation
                                      // If m_AutoManaul =1, this bit is ignored.
            UINT               : 3;   // Reserved.
            UINT m_AbsControl  : 1;   // Absolute value control
                                      // 0: Control with value in the Value fidle.
                                      // 1: Control with value in the Absolute value in CSR.
                                      // If this bit=1, value in the m_value is ignored.
            UINT m_Present     : 1;   // Presence of this feature.
                                      // 0:N/A, 1:Available. 
        };

        DWORD m_status;
    };

    UINT m_FeatureOffset      : 8;
    CCamera* m_pCamera;
	
    void Init(CCamera* pCam, DWORD offset) {
        m_pCamera = pCam;
        m_FeatureOffset = offset;
    }
    
	HVSTATUS GetInquireRegister();
    HVSTATUS GetStatusRegister();
    HVSTATUS SetStatusRegister();
	
    HVSTATUS SetValue(DWORD value, DWORD ubvalue);
};
//////////////////////////////////////////////////////////////////
#endif // !__FEATURE_STDAFX_H__