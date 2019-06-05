
//-----------------------------------------------------------------------------
//  Company:   Daheng Imavision
//  Section:   Software Department
//  Project:   HVInter.h
//
//  $Archive:
//  $Author:   Software Department
//  $Revision: 1.0.0.3$
//  $Date:     2008.09.02 15:00$
//-----------------------------------------------------------------------------
/**
*  \file     HVInter.h
*  \brief    The interval file of HVAPI interface.
*   The file defines the interval function code numbers and structs of HVAPI interface
*
*   The reference documents:
*   1. IIDC 1394-based Digital Camera Specification Ver.1.31.pdf,Revision:V1.31
*   2. DHCAM_Spec.doc,Revision:V1.3.1
*
*/
//-----------------------------------------------------------------------------

#if !defined(HVINTER_H)
#define HVINTER_H


/* By C++ language compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#include "HVDef.h"

//////////////////////////////////////////////////////////////////////////
//
/// Product ID
    typedef enum tagHV_PRODUCT_ID{
        HV_PRODUCT_ID_HV1300FM  = 0,
        HV_PRODUCT_ID_HV1300FC  = 1,
        HV_PRODUCT_ID_HV2000FC  = 2,
        HV_PRODUCT_ID_HV3100FC  = 3,
        HV_PRODUCT_ID_SV1300FM  = 4,
        HV_PRODUCT_ID_SV1300FC  = 5,
        HV_PRODUCT_ID_SV1310FM  = 6,
        HV_PRODUCT_ID_SV1310FC  = 7,
        HV_PRODUCT_ID_SV1311FM  = 8,
        HV_PRODUCT_ID_SV1311FC  = 9,
        HV_PRODUCT_ID_SV400FM   = 10,
        HV_PRODUCT_ID_SV400FC   = 11,
        HV_PRODUCT_ID_SV1410FM  = 12,
        HV_PRODUCT_ID_SV1410FC  = 13,
        HV_PRODUCT_ID_SV1420FM  = 14,
        HV_PRODUCT_ID_SV1420FC  = 15,
        HV_PRODUCT_ID_SV2000FM  = 16,
        HV_PRODUCT_ID_SV2000FC  = 17,
        HV_PRODUCT_ID_SV1400FM  = 18,
        HV_PRODUCT_ID_SV1400FC  = 19,
        HV_PRODUCT_ID_ITS1410FC = 20,
        HV_PRODUCT_ID_ITS1420FC = 21,
        HV_PRODUCT_ID_ITS2000FC = 22,

        HV_PRODUCT_ID_HV1310FM  = 23,
        HV_PRODUCT_ID_HV1310FC  = 24,
        HV_PRODUCT_ID_HV2010FC  = 25,
        HV_PRODUCT_ID_HV3110FC  = 26,

		HV_PRODUCT_ID_HV1351UC  = 5654,
		HV_PRODUCT_ID_HV2051UC  = 5655,
		HV_PRODUCT_ID_HV3151UC  = 5656,
		HV_PRODUCT_ID_HV1351UM  = 5657,

    } HV_PRODUCT_ID;

#define HV_PEODUCT_ID_TO_HVTYPE(ID)  \
	((ID)==HV_PRODUCT_ID_HV1300FM) ? HV1300FMTYPE: \
	((ID)==HV_PRODUCT_ID_HV1300FC) ? HV1300FCTYPE: \
	((ID)==HV_PRODUCT_ID_HV2000FC) ? HV2000FCTYPE: \
	((ID)==HV_PRODUCT_ID_HV3100FC) ? HV3100FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV1300FM) ? SV1300FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV1300FC) ? SV1300FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV1310FM) ? SV1310FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV1310FC) ? SV1310FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV1311FM) ? SV1311FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV1311FC) ? SV1311FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV400FM)  ? SV400FMTYPE:  \
	((ID)==HV_PRODUCT_ID_SV400FC)  ? SV400FCTYPE:  \
	((ID)==HV_PRODUCT_ID_SV1410FM) ? SV1410FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV1410FC) ? SV1410FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV1420FM) ? SV1420FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV1420FC) ? SV1420FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV2000FM) ? SV2000FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV2000FC) ? SV2000FCTYPE: \
	((ID)==HV_PRODUCT_ID_SV1400FM) ? SV1400FMTYPE: \
	((ID)==HV_PRODUCT_ID_SV1400FC) ? SV1400FCTYPE: \
	((ID)==HV_PRODUCT_ID_ITS1410FC)? ITS1410FCTYPE:\
	((ID)==HV_PRODUCT_ID_ITS1420FC)? ITS1420FCTYPE:\
	((ID)==HV_PRODUCT_ID_ITS2000FC)? ITS2000FCTYPE:\
	((ID)==HV_PRODUCT_ID_HV1310FM) ? HV1310FMTYPE: \
	((ID)==HV_PRODUCT_ID_HV1310FC) ? HV1310FCTYPE: \
	((ID)==HV_PRODUCT_ID_HV2010FC) ? HV2010FCTYPE: \
	((ID)==HV_PRODUCT_ID_HV3110FC) ? HV3110FCTYPE: \
	((ID)==HV_PRODUCT_ID_HV1351UC) ? HV1351UCTYPE: \
	((ID)==HV_PRODUCT_ID_HV2051UC) ? HV2051UCTYPE: \
	((ID)==HV_PRODUCT_ID_HV3151UC) ? HV3151UCTYPE: \
	((ID)==HV_PRODUCT_ID_HV1351UM) ? HV1351UMTYPE: \
	UNKNOWN_TYPE

//////////////////////////////////////////////////////////////////////////
/// Extension function ordinal numbers, used to construct driver control codes

#define  ORD_READ_QUADLET        0x00000100
#define  ORD_WRITE_QUADLET       0x00000101
#define  ORD_READ_BLOCK          0x00000102
#define  ORD_WRITE_BLOCK         0x00000103


#define  ORD_DEVCTRL_RESERVED0	 0x00000200
#define  ORD_DEVCTRL_RESERVED1	 0x00000201
#define  ORD_DEVCTRL_RESERVED2	 0x00000202
#define  ORD_DEVCTRL_RESERVED3	 0x00000203
#define  ORD_DEVCTRL_RESERVED4	 0x00000204


//////////////////////////////////////////////////////////////////////////
///Argument for the ORD_READ_QUADLET function
    typedef struct tagHV_ARG_READ_QUADLET
    {
        DWORD dwAddress;
    } HV_ARG_READ_QUADLET;

/// Result of the ORD_READ_QUADLET function
    typedef struct tagHV_RES_READ_QUADLET
    {
        DWORD dwData;
    } HV_RES_READ_QUADLET;

/// Argument for the ORD_WRITE_QUADLET function
    typedef struct tagHV_ARG_WRITE_QUADLET
    {
        DWORD dwAddress;
        DWORD dwData;
    } HV_ARG_WRITE_QUADLET;



//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_READ_BLOCK function
    typedef struct tagHV_ARG_READ_BLOCK
    {
        DWORD dwAddress;
        int   nLength;
    } HV_ARG_READ_BLOCK;

/// Result of the ORD_READ_BLOCK function
    typedef struct tagHV_RES_READ_BLOCK
    {
        BYTE *pData;
    } HV_RES_READ_BLOCK;

/// Argument for the ORD_WRITE_BLOCK function
    typedef struct tagHV_ARG_WRITE_BLOCK
    {
        DWORD dwAddress;
        BYTE *pData;
        int   nLength;
    } HV_ARG_WRITE_BLOCK;


//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_DEVCTRL_RESERVED0 function
    typedef struct tagHV_RES_DEVCTRL_RESERVED0
    {
        int   nLength;
        void *pContext;
    } HV_RES_DEVCTRL_RESERVED0;

/// Result of the ORD_DEVCTRL_RESERVED1 function
    typedef HV_RES_DEVCTRL_RESERVED0 HV_RES_DEVCTRL_RESERVED1;

/// Result of the ORD_DEVCTRL_RESERVED2 function
    typedef HV_RES_DEVCTRL_RESERVED0 HV_RES_DEVCTRL_RESERVED2;

/// Result of the ORD_DEVCTRL_RESERVED3 function
    typedef HV_RES_DEVCTRL_RESERVED0 HV_RES_DEVCTRL_RESERVED3;


//////////////////////////////////////////////////////////////////////////


    /* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif
