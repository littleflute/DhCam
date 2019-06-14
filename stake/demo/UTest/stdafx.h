// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently

#define VC_EXTRALEAN

#include <afxwin.h>      // MFC core and standard components
#include <afxdlgs.h>        // CommDlg Wrappers



#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>			// MFC support for Windows Common Controls


#include "sizecbar.h"   ///English:    ///Chinese: Ìí¼Ó½øÀ´
#include "scbarg.h"


#include <Dbt.h>
#include "assert.h"
#include "HVDAILT.h"
#include "Raw2Rgb.h"
#include "HVDef.h"
#pragma comment(lib,"hvdailt.lib")
#pragma comment(lib,"raw2rgb.lib")
#pragma warning(disable:4244)

#include "resource.h"
#include "DlgPrp.h"
#include "MyCtrl.h"


#define IS_CCD_CAMERA(type)		((type) == (SV1300FCTYPE) || (type) == (SV1300FMTYPE) || \
								 (type) == (SV1310FCTYPE) || (type) == (SV1310FMTYPE) || \
								  (type) == (SV1311FCTYPE) || (type) == (SV1311FMTYPE) || \
								 (type) == (SV400FCTYPE )||(type) == (SV400FMTYPE ) ||\
								 (type) == (SV1400FCTYPE )||(type) == (SV1400FMTYPE )||\
								 (type) == (SV1410FCTYPE )||(type) == (SV1410FMTYPE )||\
								  (type) == (SV1420FCTYPE )||(type) == (SV1420FMTYPE )||\
								 (type) == (SV2000FCTYPE )||(type) == (SV2000FMTYPE )) 

#define IS_SV2000(type)			((type) == (SV2000FCTYPE)||(type) == (SV2000FMTYPE))
#define IS_SV1420(type)			((type) == (SV1420FCTYPE)||(type) == (SV1420FMTYPE))

#define IS_SV1410(type)			((type) == (SV1410FCTYPE)||(type) == (SV1410FMTYPE))
#define IS_SV1400(type)			((type) == (SV1400FCTYPE)||(type) == (SV1400FMTYPE))
								 
#define IS_SV1311(type)			((type) == (SV1311FCTYPE)||(type) == (SV1311FMTYPE))
#define IS_SV400(type)			((type) == (SV400FCTYPE)|| (type)==(SV400FMTYPE))
#define IS_SV1310(type)			((type) == (SV1310FCTYPE) || (type) == (SV1310FMTYPE))
#define IS_SV1300(type)			((type) == (SV1300FCTYPE) || (type) == (SV1300FMTYPE))


#define IS_NOT_SV_COLOR(type)	((type) == (SV1300FMTYPE) ||\
                                 (type) == (SV1310FMTYPE)||\
                                 (type) == (SV1311FMTYPE)||\
								 (type) == (SV1410FMTYPE)||\
								 (type) == (SV1420FMTYPE)||\
								 (type) == (SV2000FMTYPE)||\
                                 (type)==SV400FMTYPE)


#define IS_HV1300(type)			((type) == (HV1300FCTYPE) || (type) == (HV1300FMTYPE))
#define IS_HV2000(type)			((type) == (HV2000FCTYPE))
#define IS_HV3100(type)			((type) == (HV3100FCTYPE))
#define IS_CMOS_1394_CAM(type)	((type) == (HV1300FCTYPE) || (type) == (HV1300FMTYPE) || (type) == (HV2000FCTYPE) || (type) == (HV3100FCTYPE))
