// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FA203538_A46F_4159_A6E6_4C552951EB8A__INCLUDED_)
#define AFX_STDAFX_H__FA203538_A46F_4159_A6E6_4C552951EB8A__INCLUDED_

// Change these values to use different versions
//#define WINVER		0x0400
#define _WIN32_WINNT	0x0400
//#define _WIN32_IE	0x0400
//#define _RICHEDIT_VER	0x0100

#define _WTL_USE_CSTRING

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#define _ATL_USE_CSTRING_FLOAT

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlmisc.h>
#include <atlctrlx.h>
#include "controls\atldock.h"
#include "controls\atldock2.h"
#include <atlddx.h>

/*
#pragma warning(disable:4786)
#pragma warning(disable:4251)
#pragma warning(disable:4273)
*/
#include <vector>
#include <map>
#include <list>
using namespace std;

#include "HVBase.h"
using namespace HVDevice;
//#include "1394add.h"

#include <assert.h>



#define IS_CCD_CAMERA(type)		((type) == (SV1300FCTYPE) || (type) == (SV1300FMTYPE) || \
								 (type) == (SV1310FCTYPE) || (type) == (SV1310FMTYPE) || \
								 (type) == (SV1311FCTYPE) || (type) == (SV1311FMTYPE) || \
								 (type) == (SV1400FCTYPE) || (type) == (SV1400FMTYPE) || \
								 (type) == (SV1410FCTYPE) || (type) == (SV1410FMTYPE) || \
								 (type) == (SV1420FCTYPE) || (type) == (SV1420FMTYPE) || \
								 (type) == (SV2000FCTYPE) || (type) == (SV2000FMTYPE) || \
								 (type) == (SV400FCTYPE ) || (type) == (SV400FMTYPE ))
								 //|| IS_ITS1394(type)
									 

#define IS_SV2000(type)			((type) == (SV2000FCTYPE) || (type) == (SV2000FMTYPE) )
#define IS_SV1420(type)			((type) == (SV1420FCTYPE) || (type) == (SV1420FMTYPE))

#define IS_SV1400(type)			((type) == (SV1400FCTYPE) || (type) == (SV1400FMTYPE))
#define IS_SV1410(type)			((type) == (SV1410FCTYPE) || (type) == (SV1410FMTYPE) || (type) == (ITS1410FCTYPE))
#define IS_SV1311(type)			((type) == (SV1311FCTYPE) || (type) == (SV1311FMTYPE))
#define IS_SV400(type)			((type) == (SV400FCTYPE)  || (type)==SV400FMTYPE)
#define IS_SV1310(type)			((type) == (SV1310FCTYPE) || (type) == (SV1310FMTYPE))
#define IS_SV1300(type)			((type) == (SV1300FCTYPE) || (type) == (SV1300FMTYPE))
#define IS_NOT_SV_COLOR(type)	((type) == (SV1300FMTYPE) || \
                                 (type) == (SV1310FMTYPE) || \
								 (type) == (SV1311FMTYPE) || \
								 (type) == (SV1410FMTYPE) || \
								 (type) == (SV1420FMTYPE) || \
								 (type) == (SV2000FMTYPE) || \
								 (type) == SV400FMTYPE)
#define IS_SV1300FC(type)		((type) == (SV1300FCTYPE))
#define IS_HV1300(type)			((type) == (HV1300FCTYPE) || (type) == (HV1300FMTYPE))
#define IS_HV2000(type)			((type) == (HV2000FCTYPE))
#define IS_HV3100(type)			((type) == (HV3100FCTYPE))
#define IS_CMOS_1394_CAM(type)	((type) == (HV1300FCTYPE) || \
								 (type) == (HV1300FMTYPE) || \
								 (type) == (HV2000FCTYPE) || \
								 (type) == (HV3100FCTYPE))

#define IS_OLD_GIGE(type)		 ((type) == (SV1410GCTYPE) ||\
								  (type) == (SV1410GMTYPE) || (type) == (SV1420GCTYPE) ||\
								  (type) == (SV1420GMTYPE) || (type) == (SV2000GCTYPE) ||\
								  (type) == (SV2000GMTYPE))

#define IS_OLD_ITS(type)		 ((type) == (ITS1410GCTYPE) || (type) == (ITS1420GCTYPE)||\
								  (type) == (ITS2000GCTYPE))

#define IS_OLD_GM(type)			 ((type) == (SV1410GMTYPE)  || (type) == (SV1420GMTYPE) ||\
								  (type) == (SV2000GMTYPE))

#define IS_OLD_GC(type)			 ((type) == (SV1410GCTYPE) || (type) == (SV1420GCTYPE) ||\
								  (type) == (SV2000GCTYPE))

#define IS_PD_CAM(type)          ((type) == (DRVIFENETTYPE))

#define IS_ITS1394(type)		 ((type) == (ITS1410FCTYPE) ||(type) == (ITS1420FCTYPE) ||\
								  (type) == (ITS2000FCTYPE))//(type) == (DRVIF1394TYPE))

#define IS_ITS_GC(type)			 ((type) == (ITS1410GCTYPE) || (type) == (ITS1420GCTYPE) ||\
								  (type) == (ITS2000GCTYPE))
								 
#define IS_GIGE(type)			 (IS_ITS_GC(type) || (type) == (DRVIFGIGETYPE))

#define IS_GM_CAM(type)			 ((type) == (SV1410GMTYPE) || (type) == (SV1420GMTYPE))

#define IS_READABLE_CAM(type)	 (IS_GIGE(type) || IS_GM_CAM(type)|| IS_PD_CAM(type) ||\
								  IS_ITS1394(type) || (type) == (DRVIF1394TYPE))

#define IS_HV_CAM(type)			 ((type) == (HV1310FCTYPE) || (type) == (HV1310FMTYPE) ||\
								  (type) == (HV3110FCTYPE) || (type) == (HV2010FCTYPE))

#define WM_DH_TEST_1						( WM_USER + 101 ) 
#define WM_DH_TEST_2						( WM_USER + 102 ) 
#define WM_DH_TEST_2_ID_SET_AOI				0x00000000
#define WM_DH_TEST_2_ID_GET_SHUTTER_SPEED	0x00000002


#define MINWIDTH				16
#define MINPOINT				16

#define MINWIDTH_HV				64
#define MINPOINT_HV				4


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FA203538_A46F_4159_A6E6_4C552951EB8A__INCLUDED_)
