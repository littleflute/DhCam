
//-----------------------------------------------------------------------------
//  Company:   Daheng Imavision 
//  Section:   Software Department
//  Project:   HVInter.h    
//  
//  $Archive:  
//  $Author:   Software Department
//  $Revision: 1.0.0.1$
//  $Date:     2008.07.24 15:35pm$
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
extern "C" {
#endif


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
typedef struct tagHV_ARG_READ_QUADLET{
	DWORD dwAddress;
}HV_ARG_READ_QUADLET;

/// Result of the ORD_READ_QUADLET function
typedef struct tagHV_RES_READ_QUADLET{
	DWORD dwData;
}HV_RES_READ_QUADLET;

/// Argument for the ORD_WRITE_QUADLET function
typedef struct tagHV_ARG_WRITE_QUADLET{
	DWORD dwAddress;
	DWORD dwData;
}HV_ARG_WRITE_QUADLET;



//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_READ_BLOCK function
typedef struct tagHV_ARG_READ_BLOCK{
	DWORD dwAddress;
	int   nLength;
}HV_ARG_READ_BLOCK;

/// Result of the ORD_READ_BLOCK function
typedef struct tagHV_RES_READ_BLOCK{
	BYTE *pData;
}HV_RES_READ_BLOCK;

/// Argument for the ORD_WRITE_BLOCK function
typedef struct tagHV_ARG_WRITE_BLOCK{
	DWORD dwAddress;
	BYTE *pData;
	int   nLength;
}HV_ARG_WRITE_BLOCK;


//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_DEVCTRL_RESERVED0 function
typedef struct tagHV_RES_DEVCTRL_RESERVED0{
	int   nLength;
	void *pContext;
}HV_RES_DEVCTRL_RESERVED0;

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
