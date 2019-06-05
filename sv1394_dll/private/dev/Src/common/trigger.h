//File: trigger.h

#ifndef __TRIGGER_H__
#define __TRIGGER_H__
//////////////////////////////////////////////////////////////////////////
class CCamera;
class CTrigger 
{
public:
	HVSTATUS Set_On_Off(BOOL b);
	HVSTATUS Set_Polarity(BOOL p);
	HVSTATUS Set_Source(UINT uiVal);
	HVSTATUS Set_Mode(UINT uiVal);
    union {
        struct {
            // Feature inquiry values.
            UINT m_bTriggerMode15   : 1;  // Presence of Trigger Mode 15(Vendor unique trigger 1).
            UINT m_bTriggerMode14   : 1;  // Presence of Trigger Mode 14(Vendor unique trigger 0).
            UINT                    : 8;
            UINT m_bTriggerMode5    : 1;  // Presence of Trigger Mode 5.
            UINT m_bTriggerMode4    : 1;  // Presence of Trigger Mode 4.
            UINT m_bTriggerMode3    : 1;  // Presence of Trigger Mode 3.
            UINT m_bTriggerMode2    : 1;  // Presence of Trigger Mode 2.
            UINT m_bTriggerMode1    : 1;  // Presence of Trigger Mode 1.
            UINT m_bTriggerMode0    : 1;  // Presence of Trigger Mode 0.
            UINT m_bSoftwareTrigger : 1;  // Presence of Software Trigger. ID=7.
            UINT                    : 3;
            UINT m_bTriggerSource3  : 1;  // Presence of Trigger Source 3. ID=3.
            UINT m_bTriggerSource2  : 1;  // Presence of Trigger Source 2. ID=2.
            UINT m_bTriggerSource1  : 1;  // Presence of Trigger Source 1. ID=1.
            UINT m_bTriggerSource0  : 1;  // Presence of Trigger Source 0. ID=0.
            UINT m_bValueRead       : 1;  // Capability of reading raw trigger input.
            UINT m_bPolarity        : 1;  // Capability of changing polarity of the trigger input.
            UINT m_bOnOff           : 1;  // Capability of switching this feature ON and OFF.
            UINT m_bReadOut         : 1;  // Capability of reading the value of this feature.
            UINT                    : 2; 
            UINT m_bAbsControl      : 1;  // Capability of control with absolute value.
            UINT m_bPresent         : 1;  // Presence of this feature.
            };

        DWORD m_inquire;
    };
    
    union {
        struct {
            // Feature status.
            UINT m_Parameter        : 12; // Parameter for trigger function, if required. (Optional)
            UINT                    : 4;  
            UINT m_TriggerMode      : 4;  // Trigger mode. (Trigger)Mode_0..15).
            UINT m_TriggerValue     : 2;  // Trigger input raw signal value. Read only. 0:Low, 1:High.
            UINT m_TriggerSource    : 2;  // Select trigger source. Set trigger source ID from trigger source ID_Inq.
            UINT m_TriggerPolarity  : 1;  // Select trigger polarity.(Except for software trigger input)
                                          // If m_bPolarity=1, Write to change polarity of the trigger 
                                          // input. Read to get polarity of the trigger input.
                                          // If m_bPolarity=0, Read only. (0:Low active input, 1:High active input)
            UINT m_OnOff            : 1;  // Write: ON or OFF this feature,
                                          // Read: read a status.
                                          // 0:OFF, 1:ON.
                                          // If this bit=0, other fields will be read only.
            UINT                    : 4;
            UINT m_AbsControl       : 1;  // Absolute value control. 0:Control with value in value field.
                                          // 1: Control with value in the Absolute value CSR. If this bit
                                          // =1, value in the Value field is ignored.
            UINT m_Presence         : 1;  // Presence of this feature. 0:N/A 1:Available.
        };
        
        DWORD m_dwStatus;
    };
	
    CCamera* m_pCamera;
	
	HVSTATUS SetSource(DWORD source);
    ~CTrigger() {}
	
    DWORD Get_Status();
    DWORD Inquire();
};

// The root register offsets
#define STROBE_BASE_ADDR         0xf3f00300
 
class CStrobe 
{
public:
	HVSTATUS Set_On_Off(BOOL b);
	HVSTATUS Set_Polarity(BOOL p);
    union {
        struct {
			DWORD MaxValue       :12;  ///< [20..31]Maximum value for this feature
			DWORD MinValue       :12;  ///< [8..19]Minimum value for this feature
			DWORD                : 1;  ///< [7]Reserved
			DWORD PolarityInq    : 1;  ///< [6]Capability of changing polarity of the signal 
			DWORD OnOffInq       : 1;  ///< [5]Availability of switching this feature on and off
			DWORD ReadOutInq     : 1;  ///< [4]Capability of reading the value of this feature
			DWORD                : 3;  ///< [1..3]Reserved
			DWORD PresenceInq    : 1;  ///< [0]Presence of this feature
		};

        DWORD m_StrobeInq;
    };
    
    union {
        struct {
            // status.
            DWORD DurationValue    :12;  ///< [20..31]Duration of the strobe signal. 
			DWORD DelayValue       :12;  ///< [8..19]Delay after start of exposure until the strobe signal asserts. 
			DWORD SignalPolarity   : 1;  ///< [7]Select signal polarity,(0: Low active output, 1: High active output) 
			DWORD OnOff            : 1;  ///< [6]Switch to turn this feature on or off, off is 0, on is 1
			DWORD                  : 5;  ///< [1..5]Reserved
			DWORD PresenceInq      : 1;  ///< [0]Presence of this feature
		};
        
        DWORD m_dwStatus;
    };
	
    CCamera* m_pCamStrobe;
	 
    ~CStrobe() {}
	
    DWORD Get_Status_Reg();
    DWORD Get_Inquire_Reg();
};
//////////////////////////////////////////////////////////////////////////
#endif // __TRIGGER_H__