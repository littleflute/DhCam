//-----------------------------------------------------------------------------
//  (c) 2008 by Daheng Imavision Technologies
//  Section:  Device Detect
//  Project:  
//  $Header: HVDevDetect.h, 5, 07/02/2008, Yans $
//-----------------------------------------------------------------------------
/**
  \file     HVDevDetect.h
 *
 * Utility functions and classes
 *
  \brief   HVDevDetect functions and classes
 */
//-----------------------------------------------------------------------------

#if !defined(__HV_DEVICE_DETECT__)
#define __HV_DEVICE_DETECT__

#define ID_PART_MASK_MCU 0x00000001
#define ID_PART_MASK_PLD 0x00000002

#define ID_PART_MASK_ALL (0 | ID_PART_MASK_MCU | ID_PART_MASK_PLD)

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif

HVSTATUS __stdcall HVInitDetectDeviceReset(HHV hhv);
HVSTATUS __stdcall HVDetectDeviceReset(HHV hhv, DWORD dwIdMask, DWORD *pResult);

/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif