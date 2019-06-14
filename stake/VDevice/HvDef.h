//File: HvDef.h

#if !defined(HV_HVDEF)         //
#define HV_HVDEF

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////
typedef HANDLE HHV;

typedef enum tagHVTYPE { 
	
	HV1300UCTYPE = 0,
	HV2000UCTYPE = 1,
	HV1301UCTYPE = 2,
	HV2001UCTYPE = 3,
	HV3000UCTYPE = 4,
	HV1300UMTYPE = 5,
	HV1302UCTYPE = 6,
	HV2002UCTYPE = 7,
	HV3102UCTYPE = 8,
	HV1302UMTYPE = 9,
	HV1300FCTYPE = 10,
	HV2000FCTYPE = 11,
	HV3100FCTYPE = 12,
	HV1300FMTYPE = 13,
	HV1303UCTYPE = 14,
	HV2003UCTYPE = 15,
	HV3103UCTYPE = 16,
	HV1303UMTYPE = 17,
	SV1300FMTYPE = 18,
	SV1300FCTYPE = 19,
	SV1310FCTYPE = 20,
	SV1310FMTYPE = 21,
	SV1311FCTYPE = 22,
    SV1311FMTYPE = 23,
	SV400FCTYPE  = 24,
	SV400FMTYPE  = 25,
	DH1394FXTYPE = 26,
	SV1410GCTYPE = 27,
	SV1410FMTYPE = 28,
	SV1420FCTYPE = 29,
	SV1420FMTYPE = 30,
	SV2000FCTYPE = 31,
	SV2000FMTYPE = 32,
	SV1400FCTYPE = 33,
	SV1400FMTYPE = 34,

} HVTYPE;

typedef enum tagHV_SNAP_SPEED_EX { 

	NORMAL_SPEED	= 0,	
	HIGH_SPEED	    = 1
		
} HV_SNAP_SPEED_EX;


typedef enum tagHV_RESOLUTION { 

	RES_MODE0		= 0,
	RES_MODE1		= 1,
	RES_MODE2		= 2,
	RES_MODE3		= 3
	
} HV_RESOLUTION;


#define MAX_CAMERA_NUM          16

typedef enum tagHV_INTERFACE1_ID{ 
	    STROBE_ON_OFF			= 1,
		TRIGGER_DELAY_ON_OFF	= 2,
		TRIGGER_DELAY_VALUE		= 3,
		COLOR_MODE				= 4,
		OUTPUT_IO_0				= 5,
		OUTPUT_IO_1				= 6,
		OUTPUT_IO_2				= 7,
		OUTPUT_IO_0_POL			= 8,
		OUTPUT_IO_1_POL			= 9,
		OUTPUT_IO_2_POL			= 10,
		TRIGGER_SOURCE_IO		= 11,
		TEST_IMAGE              = 12,
		TRANSFERS_DELAY_VALUE   = 13,
        
		SET_BLACK_CLAMP_LEVEL   = 30,     //2007.4.2  1311 ²å¼þ
		SET_H1DRV               = 31,
		SET_H2DRV               = 32,
		SET_H3DRV               = 33,
		SET_H4DRV               = 34,
		SET_RGDRV               = 35,
		SET_H1POL               = 36,
		SET_H1POSLOC            = 37,
		SET_RGPOL               = 38,
        SET_RGPOSLOC            = 39,
		SET_SHPPOSLOC           = 40,
		SET_SHDPOSLOC           = 41 
} HV_INTERFACE1_ID;

typedef struct tagHV_INTERFACE1_CONTEXT {
	
	HV_INTERFACE1_ID	ID;    
	DWORD				dwVal;
	
} HV_INTERFACE1_CONTEXT;

/////////////////////////////////////////////////////////////
/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif //HV_HVDEF