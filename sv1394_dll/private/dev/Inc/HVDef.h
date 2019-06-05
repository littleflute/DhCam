
//-----------------------------------------------------------------------------
//  Company:   Daheng Imavision 
//  Section:   Software Department
//  Project:   HVDef.h    
//  
//  $Archive:  
//  $Author:   Software Department
//  $Revision: 2.0.3.16$
//  $Date:     2009.07.06 08:40$
//-----------------------------------------------------------------------------
/**
*  \file     HVDef.h
*  \brief   The head file of the HVAPI interface.
*
*   The file defines the macros and structs of HVAPI interface. \n
*   The reference documents: \n
*   1. IIDC 1394-based Digital Camera Specification Ver.1.31.pdf,Revision:V1.31. \n
*   2. DHCAM_Spec.doc,Revision:V1.3.1.
*
*/
//-----------------------------------------------------------------------------

#if !defined(HVDEF_H)
#define HVDEF_H


/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif


typedef HANDLE HHV; ///< Device Handle.


/*! Define the device types. */
typedef enum tagHVTYPE{
	UNKNOWN_TYPE = -1, ///< Invalid devcie type. 
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
	SV1410FCTYPE = 27,
	SV1410FMTYPE = 28,
	SV1420FCTYPE = 29,
	SV1420FMTYPE = 30,
	SV2000FCTYPE = 31,
	SV2000FMTYPE = 32,
	SV1400FCTYPE = 33,
	SV1400FMTYPE = 34,

	HV1350UCTYPE = 35,
	HV2050UCTYPE = 36,
	HV3150UCTYPE = 37,
	HV1350UMTYPE = 38,
	HV1360UCTYPE = 39,
	HV2060UCTYPE = 40,
	HV3160UCTYPE = 41,
	HV1360UMTYPE = 42,

	SV1410GCTYPE = 43,
	SV2000GCTYPE = 44,

	RESERVED_001 = 46,

	ITS1410FCTYPE = 49,
	ITS1420FCTYPE = 50,
	ITS2000FCTYPE = 51,
	PD1420EMTYPE = 106,
	PD1420ECTYPE = 107,

	PD2000EMTYPE = 108, ///< PD1420EC device type.
	PD2000ECTYPE = 109,  ///< PD2000EC device type.

	SV1410GMTYPE = 112,
	SV2000GMTYPE = 113,
	SV1420GMTYPE = 114,
	SV1420GCTYPE = 115,
	ITS1410GMTYPE= 120,
	ITS1410GCTYPE= 121,
	ITS1420GMTYPE= 122,
	ITS1420GCTYPE= 123,
	ITS2000GMTYPE= 124,
	ITS2000GCTYPE= 125,

	SV2001GMTYPE = 126,
	SV2001GCTYPE = 127,
	SV1411GMTYPE = 128,
	SV1411GCTYPE = 129,
	SV1421GMTYPE = 130,
	SV1421GCTYPE = 131,
	SV401GMTYPE  = 132,
	SV401GCTYPE  = 133,

	DRVIFENETTYPE= 8192,    //0x00002000

    DRVIFGIGETYPE= 12288   //0x00003000
	
}HVTYPE;

/*! Define the return status of the interface functions. */
typedef enum tagHVSTATUS{
	STATUS_OK                       = 0, ///< The operation is success.
	STATUS_NO_DEVICE_FOUND          = -1,///< The device handle is invalid.
	STATUS_DEVICE_HANDLE_INVALID    = -2,///< The device type is invalid.
	STATUS_HW_DEVICE_TYPE_ERROR     = -3,///< The Hardware init failed.
	STATUS_HW_INIT_ERROR            = -4,///< The hardware reset error.
	STATUS_HW_RESET_ERROR           = -5,///< 
	STATUS_NOT_ENOUGH_SYSTEM_MEMORY = -6,///<
	STATUS_HW_IO_ERROR              = -7,///<
	STATUS_HW_IO_TIMEOUT            = -8,///<
	STATUS_HW_ACCESS_ERROR          = -9,///<

	STATUS_OPEN_DRIVER_FAILED       = -10,///<
	STATUS_NOT_SUPPORT_INTERFACE    = -11,///<
	STATUS_PARAMETER_INVALID        = -12,///<
	STATUS_PARAMETER_OUT_OF_BOUND   = -13,///<
	STATUS_IN_WORK                  = -14,///<
	STATUS_NOT_OPEN_SNAP            = -15,///<
	STATUS_NOT_START_SNAP           = -16,///<
	STATUS_FILE_CREATE_ERROR        = -17,///<
	STATUS_FILE_INVALID             = -18,///<
	STATUS_NOT_START_SNAP_INT       = -19,///<
	STATUS_INTERNAL_ERROR           = -20 ///<
}HVSTATUS;


//////////////////////////////////////////////////////////////////////////
// The macros for the status.
#define HV_SUCCESS(status) ( ((status) == STATUS_OK))
#define HV_FAILED(status) ( (status) != STATUS_OK )

#ifdef _DEBUG
#define HV_VERIFY(status) \
		if((status) != STATUS_OK){ \
			::MessageBox(NULL, HVGetErrorString(status), \
						 "Error", MB_ICONWARNING | MB_OK); \
		}
#else
#define HV_VERIFY(status) (status)
#endif

#define HV_MESSAGE(status) \
		if((status) != STATUS_OK){ \
		::MessageBox(NULL, HVGetErrorString(status), \
		"Error message", MB_ICONWARNING | MB_OK);   \
		}


/*! 
Function Control Code for HVCommand.
*/
typedef enum tagHV_CONTROL_CODE{

	//-----------BASIC FUNCTION ID-----------------------------------------------
	ORD_INIT_CAMERA                            = 0x00000000,

	ORD_QUERY_STRING_PRESENCE                  = 0x00000004,
	ORD_QUERY_STRING_DESCRIPTOR                = 0x00000005,
	ORD_GET_STRING                             = 0x00000006,

	ORD_QUERY_RESET_CAMERA_PRESENCE            = 0x00000008,
	ORD_RESET_CAMERA                           = 0x0000000B,

	ORD_QUERY_LAST_STATUS_PRESENCE             = 0x0000000C,
	ORD_GET_LAST_STATUS                        = 0x0000000E,

	ORD_QUERY_CYCLE_PORT_PRESENCE              = 0x00000010,
	ORD_CYCLE_PORT                             = 0x00000013,

	ORD_QUERY_VIDEO_FORMAT_PRESENCE            = 0x00001000,
	ORD_QUERY_VIDEO_MODE_PRESENCE              = 0x00001004,

	ORD_QUERY_FRAME_RATE_PRESENCE              = 0x00001008,
	ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE       = 0x0000100A,
	ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE       = 0x0000100B,

	ORD_QUERY_FMT7_MODE_DESCRIPTOR             = 0x0000100D,
	ORD_GET_FMT7_MODE                          = 0x0000100E,
	ORD_SET_FMT7_MODE                          = 0x0000100F,

	ORD_QUERY_FEATURE_PRESENCE                 = 0x00001010,
	ORD_QUERY_FEATURE_DESCRIPTOR               = 0x00001011,
	ORD_GET_FEATURE                            = 0x00001012,
	ORD_SET_FEATURE                            = 0x00001013,
	//ORD_GET_FEATURE_ABS_VALUE                = 0x00001016,
	//ORD_GET_FEATURE_ABS_RANGE                = 0x0000101A,
	ORD_QUERY_BASIC_FUNC_PRESENCE              = 0x0000101C,
	ORD_QUERY_BASIC_FUNC_DESCRIPTOR            = 0x0000101D,
	ORD_GET_BASIC_FUNC                         = 0x0000101E,
	ORD_SET_BASIC_FUNC                         = 0x0000101F,

	ORD_QUERY_OPT_FUNC_PRESENCE                = 0x00001020,

	ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE         = 0x00001024,
	ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR       = 0x00001025,
	ORD_GET_PIO_OUTPUT_PORT                    = 0x00001026,
	ORD_SET_PIO_OUTPUT_PORT                    = 0x00001027,

	ORD_QUERY_SIO_DESCRIPTOR                   = 0x00001029,
	ORD_GET_SIO                                = 0x0000102A,
	ORD_SET_SIO                                = 0x0000102B,

	ORD_QUERY_STROBE_PRESENCE                  = 0x0000102C,
	ORD_QUERY_STROBE_DESCRIPTOR                = 0x0000102D,
	ORD_GET_STROBE                             = 0x0000102E,
	ORD_SET_STROBE                             = 0x0000102F,

	ORD_OPEN_SNAP                              = 0x00002000,
	ORD_CLOSE_SNAP                             = 0x00002001,
	ORD_START_SNAP                             = 0x00002002,
	ORD_STOP_SNAP                              = 0x00002003,
	ORD_SNAP_SHOT                              = 0x00002004,
	ORD_TRIGGER_SHOT                           = 0x00002005,

	ORD_SNAP_SHOT_EX                           = 0x00002006,
	ORD_REGISTER_CALLBACK                      = 0x00002007,
    ORD_DEREGISTER_CALLBACK                    = 0x00002008,


	ORD_CMD_RESERVED0                          = 0x00002100,
	ORD_CMD_RESERVED1                          = 0x00002101,
	ORD_CMD_RESERVED2                          = 0x00002102,
	
	ORD_QUERY_ADV_FEATURE_PRESENCE             = 0x00003000,
	ORD_QUERY_ADV_FEATURE_DESCRIPTOR           = 0x00003001,
	ORD_GET_ADV_FEATURE                        = 0x00003002,
	ORD_SET_ADV_FEATURE                        = 0x00003003,

	ORD_QUERY_ADV_BASIC_FUNC_PRESENCE          = 0x00003004,
	//ORD_QUERY_ADV_BASIC_FUNC_DESCRIPTOR      = 0x00003005,
	ORD_GET_ADV_BASIC_FUNC                     = 0x00003006,
	ORD_SET_ADV_BASIC_FUNC                     = 0x00003007,
 
	ORD_QUERY_ADV_OPT_FUNC_PRESENCE            = 0x00003008,
	//ORD_QUERY_ADV_OPT_FUNC_DESCRIPTOR        = 0x00003009,
	//ORD_GET_ADV_OPT_FUNC                     = 0x0000300A,
	//ORD_SET_ADV_OPT_FUNC                     = 0x0000300B,

	ORD_QUERY_ADV_LUT_DESCRIPTOR               = 0x0000300D,
	ORD_GET_ADV_LUT                            = 0x0000300E,
	ORD_SET_ADV_LUT                            = 0x0000300F,

	ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR       = 0x00003011,
	ORD_GET_ADV_USER_CUSTOM                    = 0x00003012,
	ORD_SET_ADV_USER_CUSTOM                    = 0x00003013,

	ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE   = 0x00003014,
	ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR = 0x00003015,
	ORD_GET_ADV_USER_ENCRYPT_ZONE              = 0x00003016,
	ORD_SET_ADV_USER_ENCRYPT_ZONE              = 0x00003017,

	
	ORD_QUERY_NET_FUNC_PRESENCE                = 0x00004000,
	ORD_QUERY_NET_FUNC_DESCRIPTOR              = 0x00004001,
	ORD_GET_NET_FUNC                           = 0x00004002,
	ORD_SET_NET_FUNC                           = 0x00004003,

	ORD_QUERY_IMAGE_INFO_PRESENCE              = 0x00005000,
	ORD_QUERY_IMAGE_INFO_DESCRIPTOR            = 0x00005001,
	ORD_GET_IMAGE_INFO                         = 0x00005002,
	
	ORD_QUERY_AUTO_FUNC_PRESENCE               = 0x00006000,
	ORD_QUERY_AUTO_FUNC_DESCRIPTOR             = 0x00006001,
	ORD_GET_AUTO_FUNC                          = 0x00006002,
	ORD_SET_AUTO_FUNC                          = 0x00006003,

	ORD_QUERY_OSD_FUNC_PRESENCE                = 0x00007000,
	ORD_QUERY_OSD_FUNC_DESCRIPTOR              = 0x00007001,
	ORD_GET_OSD_FUNC                           = 0x00007002,
	ORD_SET_OSD_FUNC                           = 0x00007003

}HV_CONTROL_CODE;


typedef enum tagHV_RESOLUTION{
	RES_MODE0 = 0,
	RES_MODE1 = 1,
	RES_MODE2 = 2,
	RES_MODE3 = 3
}HV_RESOLUTION;

typedef enum tagHV_SNAP_MODE{
	CONTINUATION = 0,
	TRIGGER      = 1,
	TRIGGER_EDGE = 2,
	TRIGGER_LEVEL= 3
}HV_SNAP_MODE;

typedef enum tagHV_POLARITY{
	LOW    = 0,
	HIGH   = 1
}HV_POLARITY;

typedef enum tagHV_SNAP_SPEED{
	NORMAL_SPEED = 0,
	HIGH_SPEED   = 1
}HV_SNAP_SPEED;

typedef enum tagHV_CHANNEL{
	RED_CHANNEL    = 0x10,
	GREEN_CHANNEL1 = 0x11,
	GREEN_CHANNEL2 = 0x12,
	BLUE_CHANNEL   = 0x13
}HV_CHANNEL;

typedef enum tagHV_AGC_CONTROL{
	AGC_GAIN_RED_CHANNEL    = 0x10,
	AGC_GAIN_GREEN_CHANNEL1 = 0x11,
	AGC_GAIN_GREEN_CHANNEL2 = 0x12,
	AGC_GAIN_BLUE_CHANNEL   = 0x13,
	AGC_GAIN                = 0x2
} HV_AGC_CONTROL;

typedef enum tagHV_ADC_CONTROL{
	ADC_BITS                       = 0,
	ADC_BLKLEVEL_CAL               = 1,
	ADC_BLKLEVEL_CAL_REDCHANNEL    = 0x10,
	ADC_BLKLEVEL_CAL_GREENCHANNEL1 = 0x11,
	ADC_BLKLEVEL_CAL_GREENCHANNEL2 = 0x12,
	ADC_BLKLEVEL_CAL_BLUECHANNEL   = 0x13,
	ADC_BRIGHTNESS                 = 0x2
} HV_ADC_CONTROL;

typedef enum tagHV_ADC_LEVEL{
	ADC_LEVEL0 = 0,
	ADC_LEVEL1 = 1,
	ADC_LEVEL2 = 2,
	ADC_LEVEL3 = 3
}HV_ADC_LEVEL;

typedef enum tagHV_AEC_CONTROL{
	AEC_EXPOSURE_TIME = 1,
	AEC_SHUTTER_SPEED = 2,
	AEC_SHUTTER_UNIT  = 3
} HV_AEC_CONTROL;

typedef enum tagSHUTTER_UNIT{
    SHUTTER_US = 0,
    SHUTTER_MS = 1,
    SHUTTER_S  = 2
}SHUTTER_UNIT_VALUE;

typedef struct tagHV_AEC_CONTROL_CONTEXT{
	HV_AEC_CONTROL ctrl;
}HV_AEC_CONTROL_CONTEXT,PHV_AEC_CONTROL_CONTEXT;

typedef enum tagHV_MIRROR_DIRECTION{
	VERT_DIR      = 1
}HV_MIRROR_DIRECTION;

typedef enum tagHV_DEVICE_INFO {
	DESC_DEVICE_TYPE             = 0,
	DESC_RESOLUTION              = 1,
	DESC_DEVICE_MARK             = 2,
	DESC_DEVICE_SERIESNUM        = 3,
	DESC_DEVICE_BLANKSIZE        = 4,
	DESC_DEVICE_CHIPID           = 5,
	DESC_DEVICE_HARDWARE_VERSION = 6,
	DESC_DEVICE_NAME             = 7,
	DESC_DEVICE_SOFTWARE_VERSION = 8,
	DESC_DEVICE_FPGA_VERSION     = 9,
	DESC_DEVICE_VENDOR_NAME      = 10,
	DESC_DEVICE_BAYER_LAYOUT     = 11
} HV_DEVICE_INFO;

typedef enum tagHV_PERIDEV_CONTROL{
	PERIDEV_BICOLOR_LAMP1 = 0,
	PERIDEV_BICOLOR_LAMP2 =	1,
	PERIDEV_IR_EMITTER	  =	2,
	PERIDEV_LED           =	3,
	PERIDEV_ARRIVE_CLEAR  =	4
}HV_PERIDEV_CONTROL;

typedef enum tagHV_COMMAND_CODE{
	CMD_RESERVED0               = 0x00,
	CMD_RESERVED1               = 0x01,
	CMD_GET_BYTE_PER_PACKET     = 0x10,
	CMD_SET_BYTE_PER_PACKET     = 0x11,
	CMD_FRAME_SHOT              = 0x12,
	CMD_FRAME_SHOT_FINISH       = 0x13,
	CMD_GET_LUT_DWENTRIES       = 0x14,
	CMD_SET_LUT_DWENTRIES       = 0x15,
	CMD_SET_STROBE_ON           = 0x16,
	CMD_SET_STROBE_OFF          = 0x17,
	CMD_SPEC_FUN_INTERFACE1     = 0x80,
	CMD_1394_READ_BLOCK         = 0x21,
	CMD_1394_WRITE_BLOCK        = 0x22,
	CMD_1394_READ_QUADLET       = 0x23,
	CMD_1394_WRITE_QUADLET      = 0x24,
	CMD_HVAPI_CONTROL           = 0x100
}HV_COMMAND_CODE;

typedef struct tagHV_CMD_PACKET_SIZE{
	DWORD dwSize;
}HV_CMD_PACKET_SIZE;

typedef struct tagHV_CMD_LUT_DWENTRIES{
	DWORD dwOffset;
	DWORD *pBuffer;
	DWORD dwEntries;
}HV_CMD_LUT_DWENTRIES;

/*! \brief callback type*/
typedef enum tagHV_CALLBACK_TYPE{
	HV_SNAP_CALLBACK_USE_HV_SNAP_INFO = 0,
	HV_SNAP_CALLBACK_USE_HV_SNAP_INFO_EX = 1
}HV_CALLBACK_TYPE;


/*! \brief Snap Parameter*/
typedef struct tagHV_SNAP_INFO{
	HHV	hhv;       ///< The device handle.
	int nDevice;   ///< The device number.
	int nIndex;    ///< The image buffer ID.
	void *pParam;  ///< User callback function's parameter.
}HV_SNAP_INFO;

/*!  \brief Snap callback function pointer.*/
typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO *pInfo);

/*! \brief Image descriptor*/
typedef struct tagHV_IMAGE
{
	int size; ///< sizeof(HV_IMAGE)
	int ID;   ///< Reserved.
	int channels;///< Reserved.
	int alphaChannel;///< Reserved.
	int depth;///< Reserved.
	char colorModel[4];///< Reserved.
	char channelSeq[4];///< Reserved.
	int  dataOrder;///< Reserved.
	int  origin;///< Reserved.
	int  align;///< Reserved.
	int  width;///< width of current image.
	int  height;///< height of current image..
	struct tagHV_IMAGE *roi;///< Reserved.
	struct tagHV_IMAGE *maskROI;///< Reserved.
	void  *imageId;///< Reserved.
	struct tagHV_IMAGE *tileInfo;///< Reserved.
	int  imageSize;///< Size of current image buffer.
	char *imageData;///< Pointer to current image buffer.
	int  widthStep;///< Reserved.
	int  borderMode[4];///< Reserved.
	int  borderConst[4];///< Reserved.
	char *imageDataOrigin;///< Reserved.
} HV_IMAGE;

/*! \brief HV_SNAP_PROC_EX callback function parameter*/
typedef struct tagHV_SNAP_INFO_EX
{
	int nImageIndex;     ///< The image buffer ID.
	HV_IMAGE imageDes;   ///< The image descriptor.
	DWORD statusID;      ///< Status ID.
	char chStatus[128];  ///< Status string.
	int nImageInfoIndex; ///< The image information buffer ID.
} HV_SNAP_INFO_EX;

/*! \brief General callback function Parameter*/
typedef struct tagHV_CALLBACK_INFO{		
	void *pParam;///< User callback function's parameter.
	HV_CALLBACK_TYPE type;///<callback ID
	union{
		struct  
		{
			HV_SNAP_INFO_EX *pInfo;///< Pointer to HV_SNAP_INFO_EX
		}HvSnapInfoEx;
	}Parameter;	
}HV_CALLBACK_INFO;

/*!  \brief general callback function pointer.*/
typedef int (CALLBACK *HV_CALLBACK_PROC)(HV_CALLBACK_INFO *pInfo);


typedef struct tagHV_CHECK_PARAM{
	BYTE byParam1; 
	BYTE byParam2;
	BYTE byParam3;
	BYTE byParam4;
}HV_CHECK_PARAM;

typedef enum tagHV_INTERFACE1_ID{
	STROBE_ON_OFF           = 1,
	TRIGGER_DELAY_ON_OFF    = 2,
	TRIGGER_DELAY_VALUE     = 3,
	COLOR_MODE              = 4,
	OUTPUT_IO_0             = 5,
	OUTPUT_IO_1             = 6,
	OUTPUT_IO_2             = 7,
	OUTPUT_IO_0_POL         = 8,
	OUTPUT_IO_1_POL         = 9,
	OUTPUT_IO_2_POL         = 10,
	TRIGGER_SOURCE_IO       = 11,
	TEST_IMAGE              = 12,
	TRANSFERS_DELAY_VALUE   = 13,
	GAIN_GREEN_R            = 14,
	GAIN_GREEN_B            = 15,

	FILTER_TIME_UNIT        = 20,
	FILTER_TIME             = 21,
	LE_BE_MODE              = 22
}HV_INTERFACE1_ID;


typedef struct tagHV_INTERFACE1_CONTEXT{
	HV_INTERFACE1_ID	ID;
	DWORD				dwVal;
}HV_INTERFACE1_CONTEXT;

typedef enum tagCOLOR_CODE{
    MONO8    = 0,
	YUV8_411 = 1,
	YUV8_422 = 2,
	YUV8_444 = 3,
	RGB8     = 4,
	MONO16   = 5,
	RGB16    = 6
}COLOR_CODE;

typedef struct tagHVAPI_CONTROL_PARAMETER {
	HV_CONTROL_CODE code;
	void *pInBuf;
	DWORD dwInBufSize;
	void *pOutBuf;
	DWORD dwOutBufSize;
	DWORD *pBytesRet;
}HVAPI_CONTROL_PARAMETER;

typedef enum tagHV_BEGIN_TYPE{ 
	FROM_INDEX     = 0,
	FROM_SERIESNUM = 1,
	FROM_MAC       = 2,
	FROM_IP        = 3
}HV_BEGIN_TYPE;
/// Device interface for opening camera
typedef struct tagHV_DEVICE_INTERFACE{
	DWORD dwSize;
	char  szDevicePath[128];
	GUID  ClassGUID;
    DWORD Reserved;
}HV_DEVICE_INTERFACE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_STRING_PRESENCE
//////////////////////////////////////////////////////////////////////////

/// Result of the ORD_QUERY_STRING_PRESENCE function
typedef struct tagHV_RES_QUERY_STRING_PRESENCE{
	DWORD                     :21;
	DWORD FPGAVersion         : 1; ///< Presence of FPGA Version,0:Not available 1:Available..  
	DWORD FirwareVersion      : 1; ///< Presence of Firmware Version,0:Not available 1:Available.
	DWORD SysDriverVersion    : 1; ///< Presence of Sys Version,0:Not available 1:Available.
	DWORD DeviceDriverVersion : 1; ///< Presence of Device Driver Version,0:Not available 1:Available.
	DWORD HVAPIVersion        : 1; ///< Presence of HVAPI Version,0:Not available 1:Available.
	DWORD HVDailtVersion      : 1; ///< Presence of HVDAILT Version,0:Not available 1:Available.
	DWORD DeviceName          : 1; ///< Presence of DeviceName,0:Not available 1:Available.
	DWORD DeviceChipId        : 1; ///< Presence of Device Chip ID,0:Not available 1:Available.
	DWORD DeviceSN            : 1; ///< Presence of Serial Number,0:Not available 1:Available.
	DWORD VendorName          : 1; ///< Presence of Vendor Name,0:Not available 1:Available.
	DWORD DeviceType          : 1; ///< Presence of Device Type,0:Not available 1:Available.

}HV_RES_QUERY_STRING_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_STRING_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////

typedef enum tagHV_STRING_ID{
	STRING_ID_DEVICE_TYPE           = 0,
	STRING_ID_VENDOR_NAME           = 1,
	STRING_ID_DEVICE_SERIESNUM      = 2,
	STRING_ID_DEVICE_CHIPID         = 3,
	STRING_ID_DEVICE_NAME           = 4,
	STRING_ID_HVDAILT_VERSION       = 5,
	STRING_ID_HVAPI_VERSION         = 6,
	STRING_ID_DEVICE_DRIVER_VERSION = 7,
	STRING_ID_SYS_DRIVER_VERSION    = 8,
	STRING_ID_FIRMWARE_VERSION      = 9,
	STRING_ID_FPGA_VERSION          = 10
}HV_STRING_ID;

/// Argument for the ORD_QUERY_STRING_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_STRING_DESCRIPTOR{
	HV_STRING_ID StringId;
}HV_ARG_QUERY_STRING_DESCRIPTOR;

/// Result of the ORD_QUERY_STRING_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_STRING_DESCRIPTOR{
    DWORD dwTotalByteLength; ///< Size of a string, not including the '\0'.
}HV_RES_QUERY_STRING_DESCRIPTOR;


//////////////////////////////////////////////////////////////////////////
// ORD_GET_STRING function
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_GET_STRING function
typedef struct tagHV_ARG_GET_STRING{
	HV_STRING_ID StringId;
	int nLength; ///< Size of a string buffer, excluding the terminating null character.
} HV_ARG_GET_STRING;

/// Result of the ORD_GET_STIRNG function
typedef struct tagHV_RES_GET_STRING {
    char *pBuffer;
}HV_RES_GET_STRING;

///////////////////////////////////////////////////////////////////////////
// ORD_QUERY_RESET_CAMERA_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_RESET_CAMERA_PRESENCE function
typedef struct tagHV_RES_QUERY_RESET_CAMERA_PRESENCE{
	DWORD             :31;
	DWORD ResetCamera : 1; ///< Presence of Reset Camera,0:Not available 1:Available.
}HV_RES_QUERY_RESET_CAMERA_PRESENCE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_LAST_STATUS_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_LAST_STATUS_PRESENCE function
typedef struct tagHV_RES_QUERY_LAST_STATUS_PRESENCE{
	DWORD          :29;
	DWORD Transfer : 1; ///< Presence of Last Transfer Status,0:Not available 1:Available.
	DWORD Snap     : 1; ///< Presence of Last Snap Status,0:Not available 1:Available.
	DWORD Control  : 1; ///< Presence of Last Control Status,0:Not available 1:Available.
}HV_RES_QUERY_LAST_STATUS_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_LAST_STATUS
//////////////////////////////////////////////////////////////////////////

typedef enum tagHV_LAST_STATUS {
	HV_LAST_STATUS_ERROR_CONTROL = 0, // extended error information about HVSTATUS
	HV_LAST_STATUS_SNAP          = 1, // snap status
	HV_LAST_STATUS_TRANSFER      = 2  // transfer status about callback function, see HV_SNAP_PROC
}HV_LAST_STATUS;

typedef struct tagHV_ARG_GET_LAST_STATUS{
	HV_LAST_STATUS type;
}HV_ARG_GET_LAST_STATUS;

/// Return of the ORD_GET_LAST_STATUS
typedef struct tagHV_RES_GET_LAST_STATUS{
	DWORD status;
}HV_RES_GET_LAST_STATUS;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_CYCLE_PORT_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_CYCLE_PORT_PRESENCE function
typedef struct tagHV_RES_QUERY_CYCLE_PORT_PRESENCE{
	DWORD           :31;
	DWORD CyclePort : 1; ///< Presence of Cycle Port,0:Not available 1:Available.
}HV_RES_QUERY_CYCLE_PORT_PRESENCE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_VIDEO_FORMAT_PRESENCE
//////////////////////////////////////////////////////////////////////////

/// Result of the ORD_QUERY_VIDEO_FORMAT_PRESENCE function
typedef struct tagHV_RES_QUERY_VIDEO_FORMAT_PRESENCE{
	DWORD         :24;
	DWORD Format7 : 1; ///< Presence of Format7,0:Not available 1:Available..
	DWORD Format6 : 1; ///< Reserved.
	DWORD         : 3;
	DWORD Format2 : 1; ///< Reserved.
	DWORD Format1 : 1; ///< Reserved.
	DWORD Format0 : 1; ///< Reserved.
}HV_RES_QUERY_VIDEO_FORMAT_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_VIDEO_MODE_PRESENCE
//////////////////////////////////////////////////////////////////////////

/// Video Formats ID
typedef enum tagHV_VIDEO_FORMAT{
	VIDEO_FORMAT0 = 0,
	VIDEO_FORMAT1 = 1,
	VIDEO_FORMAT2 = 2,
	VIDEO_FORMAT6 = 6,
	VIDEO_FORMAT7 = 7
}HV_VIDEO_FORMAT;

/// Argument for the ORD_QUERY_VIDEO_MODE_PRESENCE function
typedef struct tagHV_ARG_QUERY_VIDEO_MODE_PRESENCE{
	HV_VIDEO_FORMAT VideoFormat;

}HV_ARG_QUERY_VIDEO_MODE_PRESENCE;

/// Result of the ORD_QUERY_VIDEO_MODE_PRESENCE function
typedef struct tagHV_RES_QUERY_VIDEO_MODE_PRESENCE{
	DWORD       :25;
	DWORD Mode6 : 1; ///< Presence of Format_x,Mode_6,0:Not available 1:Available.
	DWORD Mode5 : 1; ///< Presence of Format_x,Mode_5,0:Not available 1:Available.
	DWORD Mode4 : 1; ///< Presence of Format_x,Mode_4,0:Not available 1:Available.
	DWORD Mode3 : 1; ///< Presence of Format_x,Mode_3,0:Not available 1:Available.
	DWORD Mode2 : 1; ///< Presence of Format_x,Mode_2,0:Not available 1:Available.
	DWORD Mode1 : 1; ///< Presence of Format_x,Mode_1,0:Not available 1:Available.
	DWORD Mode0 : 1; ///< Presence of Format_x,Mode_0,0:Not available 1:Available.
}HV_RES_QUERY_VIDEO_MODE_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_FRAME_RATE_PRESENCE
//////////////////////////////////////////////////////////////////////////

/// Video Modes ID
typedef enum tagHV_VIDEO_MODE{
	VIDEO_MODE_IGNORE = -1,
	VIDEO_MODE0       = 0,
	VIDEO_MODE1       = 1,
	VIDEO_MODE2       = 2,
	VIDEO_MODE3       = 3,
	VIDEO_MODE4       = 4,
	VIDEO_MODE5       = 5,
	VIDEO_MODE6       = 6
}HV_VIDEO_MODE;

/// Argument for the ORD_QUERY_FRAME_RATE_PRESENCE function
typedef struct tagHV_ARG_QUERY_FRAME_RATE_PRESENCE{
	HV_VIDEO_FORMAT VideoFormat;
	HV_VIDEO_MODE   VideoMode;

}HV_ARG_QUERY_FRAME_RATE_PRESENCE;

/// Result of the ORD_QUERY_FRAME_RATE_PRESENCE function
typedef struct tagHV_RES_QUERY_FRAME_RATE_PRESENCE{	
	DWORD            :24;
	DWORD FrameRate7 : 1;  ///< Presence of FrameRate7,0:Not available 1:Available.
	DWORD FrameRate6 : 1;  ///< Presence of FrameRate6,0:Not available 1:Available.
	DWORD FrameRate5 : 1;  ///< Presence of FrameRate5,0:Not available 1:Available.
	DWORD FrameRate4 : 1;  ///< Presence of FrameRate4,0:Not available 1:Available.
	DWORD FrameRate3 : 1;  ///< Presence of FrameRate3,0:Not available 1:Available.
	DWORD FrameRate2 : 1;  ///< Presence of FrameRate2,0:Not available 1:Available.
	DWORD FrameRate1 : 1;  ///< Presence of FrameRate1,0:Not available 1:Available.
	DWORD FrameRate0 : 1;  ///< Presence of FrameRate0,0:Not available 1:Available.
}HV_RES_QUERY_FRAME_RATE_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE
//////////////////////////////////////////////////////////////////////////
/// Frame Rates ID
typedef enum tagHV_FRAME_RATE{
	FRAME_RATE_IGNORE   = -1,
	FRAME_RATE_1_875FPS =  0,
	FRAME_RATE_3_75FPS  =  1,
	FRAME_RATE_7_5FPS   =  2,
	FRAME_RATE_15FPS    =  3,
	FRAME_RATE_30FPS    =  4,
	FRAME_RATE_60FPS    =  5,
	FRAME_RATE_120FPS   =  6,
	FRAME_RATE_240FPS   =  7
}HV_FRAME_RATE;

/// Result of the ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE function
typedef struct tagHV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE{
	HV_VIDEO_FORMAT VideoFormat;      ///< Specifies the video format;
	HV_VIDEO_MODE VideoMode;        ///< Specifies the video mode;
	HV_FRAME_RATE FrameRate;
}HV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE;
/// Argument for the ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE function
typedef struct tagHV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE{
	HV_VIDEO_FORMAT VideoFormat;
	HV_VIDEO_MODE VideoMode;
	HV_FRAME_RATE FrameRate;
}HV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE;

/// The Format7 mode_x function ID
typedef enum tagHV_FMT7_MODE_FUNC_ID{
	FMT7_MODE_FUNC_ID_AOI          = 0, ///< AOI
	FMT7_MODE_FUNC_ID_COLOR_CODING = 1, ///< Color Coding
	FMT7_MODE_FUNC_ID_BAND_WIDTH   = 2, ///< Band Width.
	FMT7_MODE_FUNC_ID_DATA_DEPTH   = 3, ///< Data Depth.
	FMT7_MODE_FUNC_ID_COLOR_FILTER = 4 ///< Color Filter Mode

}HV_FMT7_MODE_FUNC_ID;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_FMT7_MODE_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////

/// Color Filter ID
typedef enum tagHV_COLOR_FILTER{
	COLOR_FILTER_RG_GB  = 0, ///< RGB primary color filter (RG/GB)
	COLOR_FILTER_GB_RG  = 1, ///< RGB primary color filter (GB/RG)
	COLOR_FILTER_GR_BG  = 2, ///< RGB primary color filter (GR/BG)
	COLOR_FILTER_BG_GR  = 3 ///< RGB primary color filter (BG/GR)
}HV_COLOR_FILTER;

/// Function descriptor for format7 mode_x
typedef union tagHV_FMT7_MODE_DESCRIPTOR{	
	// AOI field for the FMT7_MODE_FUNC_ID_AOI function.
	struct {
		DWORD VerticalMaxInq    :16; ///<Maximum Vertical pixel number
		DWORD HorizontalMaxInq  :16; ///<Maximum Horizontal pixel number
		DWORD VerticalUnitInq   :16; ///<Vertical unit pixel number
		DWORD HorizontalUnitInq :16; ///<Horizontal unit pixel number
	} AOI;

	// Color coding field for the FMT7_MODE_FUNC_ID_COLOR_CODING function.
	struct {	
		DWORD                 :21;
		DWORD Raw16Inq        : 1; ///< Presence of Raw16,0:Not available 1:Available..
		DWORD Raw8Inq         : 1; ///< Presence of Raw8,0:Not available 1:Available..
		DWORD SingnedRGB16Inq : 1; ///< Presence of Singed RGB16,0:Not available 1:Available.
		DWORD SignedMono16Inq : 1; ///< Presence of Singed Mono16,0:Not available 1:Available..
		DWORD RGB16Inq        : 1; ///< Presence of RGB16,0:Not available 1:Available..
		DWORD Mono16Inq       : 1; ///< Presence of Mono16,0:Not available 1:Available..
		DWORD RGB8Inq         : 1; ///< Presence of RGB8,0:Not available 1:Available..
		DWORD YUV8_4_4_4Inq   : 1; ///< Presence of YUV 4:4:4,0:Not available 1:Available..
		DWORD YUV8_4_2_2Inq   : 1; ///< Presence of YUV 4:2:2,0:Not available 1:Available..
		DWORD YUV8_4_1_1Inq   : 1; ///< Presence of YUV 4:1:1,0:Not available 1:Available..
		DWORD Mono8Inq        : 1; ///< Presence of Mono8,0:Not available 1:Available..

		DWORD :32;
		DWORD :32;
		DWORD :32;
		
		DWORD                     :18;
		DWORD JpegInq             : 1;///< Presence of Jpeg, 0:Not available 1:Available.
		DWORD Dyuv422Inq          : 1;///< Presence of Dyuv422, 0:Not available 1:Available.
		DWORD Raw12PackedInq      : 1;///< Presence of Raw12_Packed, 0:Not available 1:Available.
		DWORD Raw12NormalizedInq  : 1;///< Presence of Raw12_Normalized, 0:Not available 1:Available.
		DWORD Raw12Inq            : 1;///< Presence of Raw12, 0:Not available 1:Available.
		DWORD Mono12PackedInq     : 1;///< Presence of Mono12_Packed, 0:Not available 1:Available.
		DWORD Mono12NormalizedInq : 1;///< Presence of Mono12_Normalized, 0:Not available 1:Available.
		DWORD Mono12Inq           : 1;///< Presence of Mono12, 0:Not available 1:Available.
		DWORD Raw10PackedInq      : 1;///< Presence of Raw10_Packed, 0:Not available 1:Available.
		DWORD Raw10NormalizedInq  : 1;///< Presence of Raw10_Normalized, 0:Not available 1:Available.
		DWORD Raw10Inq            : 1;///< Presence of Raw10, 0:Not available 1:Available.
		DWORD Mono10PackedInq     : 1;///< Presence of Mono10_Packed, 0:Not available 1:Available.
		DWORD Mono10NormalizedInq : 1;///< Presence of Mono10_Normalized, 0:Not available 1:Available.
		DWORD Mono10Inq           : 1;///< Presence of Mono10, 0:Not available 1:Available.

		DWORD :32;
		DWORD :32;
		DWORD :32;
	} ColorMode;

	// Band Width field for the FMT7_MODE_FUNC_ID_BAND_WIDTH function.
	struct {
		DWORD dwPixelPerFrameInq;      ///< Reserved.
		DWORD dwBytePerFrameHiInq;     ///< Reserved.
		DWORD dwBytePerFrameLoInq;     ///< Reserved.

		DWORD MaxBytePerPacketInq  :16;///< Reserved.
		DWORD UnitBytePerPacketInq :16;///< Reserved.

		DWORD RecBytePerpacketInq  :16;///< Reserved.
		DWORD BytePerPacketInq     :16;///< Reserved.

		DWORD PacketPerFrameInq;       ///< Reserved.

		DWORD VposunitInq          :16;///< Reserved.
		DWORD HposunitInq          :16;///< Reserved.
		DWORD dwFrameIntervalInq;      ///< Reserved.
	} BandWidth;

	// Data depth field for the FMT7_MODE_FUNC_ID_DATA_DEPTH function.
	struct {
		DWORD          :24;
		DWORD ValueInq :8 ;///< Image data depth
	} DataDepth;

	// Color filter mode field for the FMT7_MODE_FUNC_ID_COLOR_FILTER function.
	struct {
		HV_COLOR_FILTER Filter; ///< Color Filter
	} ColorFilterMode;

}HV_FMT7_MODE_DESCRIPTOR;


/// Argument for the ORD_QUERY_FMT7_MODE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_FMT7_MODE_DESCRIPTOR{
	HV_VIDEO_MODE VideoMode;
	HV_FMT7_MODE_FUNC_ID FuncId;    ///< Format7 mode_x function ID
}HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR;

/// Result of the ORD_QUERY_FMT7_MODE_DESCRIPTOR function
typedef HV_FMT7_MODE_DESCRIPTOR  HV_RES_QUERY_FMT7_MODE_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_FMT7_MODE
//////////////////////////////////////////////////////////////////////////

typedef enum tagHV_COLOR_CODE{
	COLOR_MONO8              = 0, ///< Y component has 8bit data
	COLOR_YUV8_4_1_1         = 1, ///< Y=U=V= 8bits, non compressed
	COLOR_YUV8_4_2_2         = 2, ///< Y=U=V=8bits, non compressed
	COLOR_YUV8_4_4_4         = 3, ///< Y=U=V=8bits, non compressed
	COLOR_RGB8               = 4, ///< RGB 8 format, each component has 8bit data
	COLOR_MONO16             = 5, ///< Y only, Y=16bits, non compressed(unsigned integer)
	COLOR_RGB16              = 6, ///< RGB 16 format, each component has 16bit data
	COLOR_SIGNED_MONO16      = 7, ///< Y only, Y=16bits, non compressed(signed integer)
	COLOR_SIGNED_RGB16       = 8, ///< R=G=B=16bits, non compressed(signed integer)
	COLOR_RAW8               = 9, 
	COLOR_RAW16              = 10,

	COLOR_MONO10             = 128,
	COLOR_MONO10_NORMALIZED  = 129,
	COLOR_MONO10_PACKED      = 130,
	COLOR_RAW10              = 131,
	COLOR_RAW10_NORMALIZED   = 132,
	COLOR_RAW10_PACKED       = 133,

	COLOR_MONO12             = 134,
	COLOR_MONO12_NORMALIZED  = 135,
	COLOR_MONO12_PACKED      = 136,
	COLOR_RAW12              = 137,
	COLOR_RAW12_NORMALIZED   = 138,
	COLOR_RAW12_PACKED       = 139,

	COLOR_DYUV422            = 160,
	COLOR_JPEG               = 170

}HV_COLOR_CODE;

/// Format7 mode_x Function
typedef union tagHV_FMT7_MODE{

	// AOI field for the FMT7_MODE_FUNC_ID_AOI function.
	struct {
		DWORD Top    :16; ///< Top of AOI Position, in pixel
		DWORD Left   :16; ///< Left of AOI Position, in pixel

		DWORD Height :16; ///< Height of AOI,in pixel
		DWORD Width  :16; ///< Width of AOI,in pixel
	} AOI;

	// Color coding field for the FMT7_MODE_FUNC_ID_COLOR_CODING function.
	struct {
		HV_COLOR_CODE Code; ///<Color Coding.
	} ColorMode;

	// Band Width field for the FMT7_MODE_FUNC_ID_BAND_WIDTH function.
	struct {
		DWORD            :16;
		DWORD PacketSize :16; ///< Transmission Packet Size
	} BandWidth;
}HV_FMT7_MODE;
/// The FMT7 mode item id for Set
typedef enum tagHV_FMT7_MODE_ITEM_ID{
	// AOI Function
	FMT7_MODE_ITEM_ID_AOI_LEFT     = 0x000000, ///< Top of AOI
	FMT7_MODE_ITEM_ID_AOI_TOP      = 0x000010, ///< Left of AOI
	FMT7_MODE_ITEM_ID_AOI_WIDTH    = 0x000100, ///< AOI Width
	FMT7_MODE_ITEM_ID_AOI_HEIGHT   = 0x000110, ///< AOI Height

	FMT7_MODE_ITEM_ID_AOI_POSITION = 0xFF000001, ///< AOI Position, including Top and Left.
	FMT7_MODE_ITEM_ID_AOI_SIZE     = 0xFF000002, ///< AOI Size, including Width and Height.
	FMT7_MODE_ITEM_ID_AOI_ALL      = 0xFF000003, ///< AOI, including Top,Left,Width and Height.
	
	FMT7_MODE_ITEM_ID_COLOR_MODE_COLOR_CODE  = 0x010000,///< Color Coding
	FMT7_MODE_ITEM_ID_BAND_WIDTH_PACKET_SIZE = 0x020010///< Band Width
}HV_FMT7_MODE_ITEM_ID;

/// Argument for the ORD_GET_FMT7_MODE function
typedef struct tagHV_ARG_GET_FMT7_MODE{
	HV_VIDEO_MODE VideoMode;
	HV_FMT7_MODE_FUNC_ID FuncId;
}HV_ARG_GET_FMT7_MODE;

/// Result of the ORD_GET_FMT7_MODE function
typedef HV_FMT7_MODE HV_RES_GET_FMT7_MODE;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_FMT7_MODE
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_FMT7_MODE function
typedef struct tagHV_ARG_SET_FMT7_MODE{
	HV_VIDEO_MODE VideoMode;
	HV_FMT7_MODE_FUNC_ID FuncId;
	HV_FMT7_MODE_ITEM_ID ItemId;
	HV_FMT7_MODE Fmt7Mode;

} HV_ARG_SET_FMT7_MODE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_FEATURE_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_FEATURE_PRESENCE function
typedef struct tagHV_RES_QUERY_FEATURE_PRESENCE{

	DWORD                :16;
	DWORD FrameRate      : 1;///< Presence of Frame Rate, 0:Not available 1:Available.
	DWORD WhiteShading   : 1;///< Presence of White Shading Compensation, 0:Not available 1:Available.
	DWORD TriggerDelay   : 1;///< Presence of Trigger Delay, 0:Not available 1:Available.
	DWORD Trigger        : 1;///< Presence of Trigger, 0:Not available 1:Available.
	DWORD                : 3;
	DWORD Gain           : 1;///< Presence of Gain, 0:Not available 1:Available.
	DWORD Shutter        : 1;///< Presence of Shutter Speed, 0:Not available 1:Available.
	DWORD Gamma          : 1;///< Presence of Brightness, 0:Not available 1:Available.
	DWORD                : 2;
	DWORD WhiteBalance   : 1;///< Presence of White Balance, 0:Not available 1:Available.
	DWORD Sharpness      : 1;///< Presence of Sharpness, 0:Not available 1:Available.
	DWORD AutoExposure   : 1;///< Presence of Auto Exposure, 0:Not available 1:Available.
	DWORD Brightness     : 1;///< Presence of Brightness, 0:Not available 1:Available.
	//
	//
	DWORD                :14;
	DWORD CaptureQuality :1;///< Reserved.
	DWORD CaptureSize    : 1;///< Reserved.
	DWORD                :12;
	DWORD OpticalFilter  : 1;///< Presence of Optical Filter, 0:Not available 1:Available.
	DWORD Tilt           : 1;///< Presence of TILT, 0:Not available 1:Available.
	DWORD Pan            : 1;///< Presence of PAN, 0:Not available 1:Available.
	DWORD Zoom           : 1;///< Presence of Zoom, 0:Not available 1:Available.
}HV_RES_QUERY_FEATURE_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_FEATURE_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// Standard Feature ID
typedef enum tagHV_FEATURE_ID{
	FEATURE_ID_BRIGHTNESS    = 0,///<The brightness control
	FEATURE_ID_AUTO_EXPOSURE = 1,///<The auto exposure control
	FEATURE_ID_SHARPNESS     = 2,///<The sharpness control
	FEATURE_ID_WHITE_BALANCE = 3,///<The white balance control
	FEATURE_ID_HUE           = 4,///<The hue control
	FEATURE_ID_SATURATION    = 5,///<The saturation control
	FEATURE_ID_GAMMA         = 6,///<The gamma control
	FEATURE_ID_SHUTTER       = 7,///<The shutter control
	FEATURE_ID_GAIN          = 8,///<The gain control
	FEATURE_ID_IRIS          = 9,///<The iris control
	FEATURE_ID_FOCUS         = 10,///<The focus control
	FEATURE_ID_TEMPERATURE   = 11,///<The temperature control
	FEATURE_ID_TRIGGER       = 12,///<The trigger control
	FEATURE_ID_TRIGGER_DELAY = 13,///<The Trigger Delay Control
	FEATURE_ID_WHITE_SHADING = 14,///<The White Shading Compensation Control
	FEATURE_ID_FRAME_RATE    = 15,///<The Frame rate prioritize control

	FEATURE_ID_ZOOM          = 32,///<The zoom control
	FEATURE_ID_PAN           = 33,///<The pan control
	FEATURE_ID_TILT          = 34,///<The tilt control
	FEATURE_ID_OPTICAL_FILTER= 35,///<The optical filter control

	FEATURE_ID_CAPTURE_SIZE  = 48,///<The image size for format 6
	FEATURE_ID_CAPTURE_QUALITY = 49///<The image quality for format 6
}HV_FEATURE_ID;

/// Feature Function Descriptor
typedef union tagHV_FEATURE_DESCRIPTOR {
	/*!
	// Scalar field for functions as:
	// FEATURE_ID_BRIGHTNESS,    FEATURE_ID_AUTO_EXPOSURE, FEATURE_ID_SHARPNESS,
	// FEATURE_ID_WHITE_BALANCE, FEATURE_ID_HUE,           FEATURE_ID_SATURATION,
	// FEATURE_ID_GAMMA,         FEATURE_ID_SHUTTER,       FEATURE_ID_GAIN,
	// FEATURE_ID_IRIS,          FEATURE_ID_FOCUS,         FEATURE_ID_TEMPERATURE,
	//                           FEATURE_ID_TRIGGER_DELAY, FEATURE_ID_WHITE_SHADING,
	// FEATURE_ID_FRAME_RATE,    FEATURE_ID_ZOOM,          FEATURE_ID_PAN,
	// FEATURE_ID_TILT,          FEATURE_ID_OPTICAL_FILTER,FEATURE_ID_CAPTURE_SIZE,
	// FEATURE_ID_CAPTURE_QUALITY
	*/
	struct {
		DWORD MaxValue      :12;///< Maximum value.
		DWORD MinValue      :12;///< Minimum value.
		DWORD ManualInq     : 1;///< Reserved.
		DWORD AutoInq       : 1;///< Reserved.
		DWORD OnOffInq      : 1;///< Availability of switching this feature on and off, 0:OFF 1:ON.
		DWORD ReadOutInq    : 1;///< Reserved.
		DWORD OnePushInq    : 1;///< Reserved.
		DWORD               : 1;
		DWORD AbsControlInq : 1;///< Reserved.
		DWORD PresenceInq   : 1;///< Presence of this Scalar feature
	} Scalar;

	/*! Trigger field for the FEATURE_ID_TRIGGER function.*/
	struct {
		DWORD               :12;///< Reserved
		DWORD Mode3Inq      : 1;///< Presence of Trigger Mode 3, 0:Not available 1:Available.
		DWORD Mode2Inq      : 1;///< Presence of Trigger Mode 2, 0:Not available 1:Available.
		DWORD Mode1Inq      : 1;///< Presence of Trigger Mode 1, 0:Not available 1:Available.
		DWORD Mode0Inq      : 1;///< Presence of Trigger Mode 0, 0:Not available 1:Available.
		DWORD SoftTriggerInq : 1; ///< Presence of Software Trigger, 0:Not available 1:Available.
		DWORD               : 3;
		DWORD Source3Inq    : 1;///< Presence of Software Trigger source 3, 0:Not available 1:Available.
		DWORD Source2Inq    : 1;///< Presence of Software Trigger source 2, 0:Not available 1:Available.
		DWORD Source1Inq    : 1;///< Presence of Software Trigger source 1, 0:Not available 1:Available.
		DWORD Source0Inq    : 1;///< Presence of Software Trigger source 0, 0:Not available 1:Available.
		DWORD ValueReadInq  : 1;///< Capability of reading raw trigger input, 0:Not available 1:Available.
		DWORD PolarityInq   : 1;///< Capability of changing polarity of the trigger input,0:Not available 1:Available.
		DWORD OnOffInq      : 1;///< Capability of switching this feature ON and OFF, 0:OFF 1:ON.
		DWORD ReadOutInq    : 1;///< Capability of reading the value of this feature.
		DWORD               : 2;
		DWORD AbsControlInq : 1;///< Reserved.
		DWORD PresenceInq   : 1;///<  Presence of this feature,0:Not available 1:Available.
	} Trigger;
}HV_FEATURE_DESCRIPTOR;

/// Argument for the ORD_QUERY_FEATURE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_FEATURE_DESCRIPTOR{
    HV_FEATURE_ID FeatureId;
}HV_ARG_QUERY_FEATURE_DESCRIPTOR;

/// Result of the ORD_QUERY_FEATURE_DESCRIPTOR function
typedef HV_FEATURE_DESCRIPTOR HV_RES_QUERY_FEATURE_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_FEATURE
//////////////////////////////////////////////////////////////////////////
/// Trigger Source ID
typedef enum tagHV_TRIGGER_SOURCE_ID {
	HV_TRIGGER_SOURCE_ID_INPUT0   = 0,
	HV_TRIGGER_SOURCE_ID_INPUT1   = 1,
	HV_TRIGGER_SOURCE_ID_INPUT2   = 2,
	HV_TRIGGER_SOURCE_ID_INPUT3   = 3,
	HV_TRIGGER_SOURCE_ID_SOFTWARE = 7,
} HV_TRIGGER_SOURCE_ID;
/// Feature Function
typedef union tagHV_FEATURE{
	/*!
	Scalar field for functions as:
	FEATURE_ID_BRIGHTNESS,    FEATURE_ID_AUTO_EXPOSURE, FEATURE_ID_SHARPNESS,
	                          FEATURE_ID_HUE,           FEATURE_ID_SATURATION,
	FEATURE_ID_GAMMA,         FEATURE_ID_SHUTTER,       FEATURE_ID_GAIN,
	FEATURE_ID_IRIS,          FEATURE_ID_FOCUS,         FEATURE_ID_TEMPERATURE,
	                          FEATURE_ID_TRIGGER_DELAY, FEATURE_ID_WHITE_SHADING,
	FEATURE_ID_FRAME_RATE,    FEATURE_ID_ZOOM,          FEATURE_ID_PAN,
	FEATURE_ID_TILT,          FEATURE_ID_OPTICAL_FILTER,FEATURE_ID_CAPTURE_SIZE,
	FEATURE_ID_CAPTURE_QUALITY
	*/
	struct {
		DWORD Value          :12;
		DWORD                :12;///< Reserved.
		DWORD AutoManualMode : 1;///< Reserved.
		DWORD OnOff          : 1;///< Switch to turn this feature ON or OFF, 0:OFF 1:ON
		DWORD OnePush        : 1;///< Reserved.
		DWORD                : 3;
		DWORD AbsControl     : 1;///< Reserved.
		DWORD PresenceInq    : 1;///< Presence of Scalar function,0:Not available 1:Available..
	} Scalar;

	/*! 
	Trigger field for the FEATURE_ID_TRIGGER function.
	*/
	struct {
		DWORD Parameter   :12;///< Parameter for trigger function, if required.(Optional)
		DWORD             : 4;
		DWORD Mode        : 4;///< Trigger mode
		DWORD             : 1;
		DWORD Source      : 3;///< Select trigger source
		DWORD Polarity    : 1;///< Select trigger polarity(Except for software trigger), 1:HIGH 0:LOW.
		DWORD OnOff       : 1;///< Switch to turn this function ON or OFF, 0:OFF 1:ON
		DWORD             : 4;
		DWORD AbsControl  : 1;///< Reserved.
		DWORD PresenceInq : 1;///< Presence of Trigger function,0:Not available 1:Available.
	} Trigger;


	/*! 
	WhiteBalance field for the FEATURE_ID_WHITE_BALANCE function.
	*/
	struct {
		DWORD VR             :12;///< V value or R value
		DWORD UB             :12;///< U value or B value
		DWORD AutoManualMode : 1;///< Reserved.
		DWORD OnOff          : 1;///< Switch to turn this feature ON or OFF, 0:OFF 1:ON
		DWORD OnePush        : 1;///< Reserved.
		DWORD                : 3;
		DWORD AbsControl     : 1;///< Reserved.
		DWORD PresenceInq    : 1;///< Presence of WhiteBalance function,0:Not available 1:Available.
	} WhiteBalance;
}HV_FEATURE;

/// The Feature ITEM id for set
typedef enum tagHV_FEATURE_ITEM_ID{
	FEATURE_ITEM_ID_SCALAR_PRESENCE_INQ     = 0x000000,
	FEATURE_ITEM_ID_SCALAR_ABS_CONTROL      = 0x000001,
	FEATURE_ITEM_ID_SCALAR_ONE_PUSH         = 0x000005,
	FEATURE_ITEM_ID_SCALAR_ON_OFF           = 0x000006,
	FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE = 0x000007,
	FEATURE_ITEM_ID_SCALAR_VALUE            = 0x000014,

	FEATURE_ITEM_ID_TRIGGER_PRESENCE_INQ    = 0x010000,
	FEATURE_ITEM_ID_TRIGGER_ABS_CONTROL     = 0x010001,
	FEATURE_ITEM_ID_TRIGGER_ON_OFF          = 0x010006,
	FEATURE_ITEM_ID_TRIGGER_POLARITY        = 0x010007,
	FEATURE_ITEM_ID_TRIGGER_SOURCE          = 0x010008,
	FEATURE_ITEM_ID_TRIGGER_MODE            = 0x01000C,
	FEATURE_ITEM_ID_TRIGGER_PARAMETER       = 0x010014,

	FEATURE_ITEM_ID_WHITE_BALANCE_PRESENCE_INQ     = 0x020000,
	FEATURE_ITEM_ID_WHITE_BALANCE_ABS_CONTROL      = 0x020001,
	FEATURE_ITEM_ID_WHITE_BALANCE_ONE_PUSH         = 0x020005,
	FEATURE_ITEM_ID_WHITE_BALANCE_ON_OFF           = 0x020006,
	FEATURE_ITEM_ID_WHITE_BALANCE_AUTO_MANUAL_MODE = 0x020007,
	FEATURE_ITEM_ID_WHITE_BALANCE_UB               = 0x020008,
	FEATURE_ITEM_ID_WHITE_BALANCE_VR               = 0x020014
}HV_FEATURE_ITEM_ID;

/// Argument for the ORD_GET_FEATURE function
typedef struct tagHV_ARG_GET_FEATURE{
    HV_FEATURE_ID FeatureId;

}HV_ARG_GET_FEATURE;

/// Result of the ORD_GET_FEATURE function
typedef HV_FEATURE HV_RES_GET_FEATURE;

//////////////////////////////////////////////////////////////////////////
/// ORD_SET_FEATURE
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_FEATURE function
typedef struct tagHV_ARG_SET_FEATURE{
	HV_FEATURE_ID FeatureId;
	HV_FEATURE_ITEM_ID FeatureItemId;
	HV_FEATURE Feature;

}HV_ARG_SET_FEATURE;

//////////////////////////////////////////////////////////////////////////
/// ORD_QUERY_BASIC_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_BASIC_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_BASIC_FUNC_PRESENCE{
	DWORD MemCh      : 4; ///< Maximum Memory Channel Number (N)
	                      ///< Memory Channel No
	                      ///< 0 = Factory setting memory
	                      ///< 1 = Memory Ch 1
		                  ///< 2 = Memory Ch 2
	                      ///<   :
	                      ///< N = Memory Ch N
		                  ///< If 0000, user memory is not available.

	DWORD            : 7;
	DWORD MultiShot  : 1; ///< Presence of Multi Shot Transmission,0:Not available 1:Available.
	DWORD OneShot    : 1; ///< Presence of One Shot Transmission,0:Not available 1:Available.
	DWORD            :15;
	DWORD OptFunc    : 1; ///< Presence of Optional function,0:Not available 1:Available.
	DWORD            : 2;
	DWORD AdvFeature : 1; ///< Presence of Advanced Feature(Vendor Unique Features),0:Not available 1:Available.
}HV_RES_QUERY_BASIC_FUNC_PRESENCE;


//////////////////////////////////////////////////////////////////////////
/// ORD_GET_BASIC_FUNC
//////////////////////////////////////////////////////////////////////////
/// The basic function id
typedef enum tagHV_BASIC_FUNC_ID{
	BASIC_FUNC_ID_MEM_CH_SAVE = 28,
    BASIC_FUNC_ID_MEM_CH_CUR  = 29
}HV_BASIC_FUNC_ID;

/// The descriptor struct of basic function
typedef union tagHV_BASIC_FUNC_DESCRIPTOR{
    struct {
		DWORD dwMaxNum;   ///< Maximum value of memory channel.
	} MemCh;
}HV_BASIC_FUNC_DESCRIPTOR;

/// Argument for the ORD_QUERY_BASIC_FUNC_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_BASIC_FUNC_DESCRIPTOR{
	HV_BASIC_FUNC_ID FuncId;
}HV_ARG_QUERY_BASIC_FUNC_DESCRIPTOR;

/// Result of the ORD_QUERY_BASIC_FUNC_DESCRIPTOR function
typedef HV_BASIC_FUNC_DESCRIPTOR HV_RES_QUERY_BASIC_FUNC_DESCRIPTOR;

/// The union for the basic function
typedef union tagHV_BASIC_FUNC{

	struct {
		DWORD        :28;
		DWORD Number : 4; ///< Save Memory Channel Number
	} MemChSave;

	struct {
		DWORD        :28;
		DWORD Number : 4; ///< Current Memory Channel Number
	} MemChCur;
}HV_BASIC_FUNC;

/// Argument for the ORD_GET_BASIC_FUNC function
typedef struct tagHV_ARG_GET_BASIC_FUNC{
	HV_BASIC_FUNC_ID FuncId;
}HV_ARG_GET_BASIC_FUNC;

/// Result of the ORD_GET_BASIC_FUNC function
typedef HV_BASIC_FUNC HV_RES_GET_BASIC_FUNC;

/// The basic function item id for set
typedef enum tagHV_BASIC_FUNC_ITEM_ID{
	BASIC_FUNC_ITEM_ID_MEM_CH_SAVE_NUM = 0x000000,
	BASIC_FUNC_ITEM_ID_MEM_CH_CUR_NUM  = 0x010000
}HV_BASIC_FUNC_ITEM_ID;

//////////////////////////////////////////////////////////////////////////
/// ORD_SET_BASIC_FUNC
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_BASIC_FUNC function
typedef struct tagHV_ARG_SET_BASIC_FUNC{
    HV_BASIC_FUNC_ID FuncId;
	HV_BASIC_FUNC_ITEM_ID ItemId;
	HV_BASIC_FUNC Func;
}HV_ARG_SET_BASIC_FUNC;


//////////////////////////////////////////////////////////////////////////
/// ORD_QUERY_OPT_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_OPT_FUNC_PRESENCE function,DCAM Option Function
typedef struct tagHV_RES_QUERY_OPT_FUNC_PRESENCE{
	DWORD        :28;
	DWORD Strobe : 1; ///< Presence of Strobe Control,0:Not available 1:Available.
	DWORD SIO    : 1; ///< Presence of Serial Input/Output Control,0:Not available 1:Available.
	DWORD PIO    : 1; ///< Presence of Parallel Input/Output Control,0:Not available 1:Available.
	DWORD        : 1;
}HV_RES_QUERY_OPT_FUNC_PRESENCE;


//////////////////////////////////////////////////////////////////////////
/// ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE function
typedef struct tagHV_RES_QUERY_PIO_OUTPUT_PORT_PRESENCE{
	DWORD       :29;
	DWORD Port2 : 1; ///< Presence of OUTPUT_PORT_2,0:Not available 1:Available.
	DWORD Port1 : 1; ///< Presence of OUTPUT_PORT_1,0:Not available 1:Available.
	DWORD Port0 : 1; ///< Presence of OUTPUT_PORT_0,0:Not available 1:Available.
}HV_RES_QUERY_PIO_OUTPUT_PORT_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////

/// PIO_OUTPUT_PORT_X ID
typedef enum tagHV_PIO_OUTPUT_PORT_ID{
	PIO_OUTPUT_PORT0 = 0,
	PIO_OUTPUT_PORT1 = 1,
	PIO_OUTPUT_PORT2 = 2
}HV_PIO_OUTPUT_PORT_ID;

/// The descriptor struct of PIO_OUTPUT_PORT_X in the DHCAM
typedef struct tagHV_PIO_OUTPUT_PORT_DESCRIPTOR{
	struct {		
		DWORD             :28;
		DWORD UserSetInq  : 1; ///< Presence of User Setting Control,0:Not available 1:Available.
		DWORD InvertInq   : 1; ///< Presence of Invert Control,0:Not available 1:Available.
		DWORD MonitorInq  : 1; ///< Presence of Monito Control,0:Not available 1:Available.
		DWORD PresenceInq : 1; ///< Presence of Configration function,0:Not available 1:Available..
	} Configration;

	struct {
		DWORD                       :28; ///< Reserved.
		DWORD StrobeSignalInq       : 1; ///< Presence of Strobe Signal,0:Not available 1:Available.
		DWORD UserSignalInq         : 1; ///< Presence of User Signal,0:Not available 1:Available.
		DWORD TriggerReadySignalInq : 1; ///< Presence of Trigger Ready Signal,0:Not available 1:Available.
		DWORD IntegralSignalInq     : 1; ///< Presence of Integral Signal,0:Not available 1:Available.
	} SourceSelect;

}HV_PIO_OUTPUT_PORT_DESCRIPTOR;

/// Argument for the ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR{
	HV_PIO_OUTPUT_PORT_ID PortId;
}HV_ARG_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR;

/// Result of the ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR function
typedef HV_PIO_OUTPUT_PORT_DESCRIPTOR HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_PIO_OUTPUT_PORT
//////////////////////////////////////////////////////////////////////////
/// The PIO_PORT_X functions id
typedef enum tagHV_PIO_OUTPUT_PORT_FUNC_ID{
    PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT = 0,
    PIO_OUTPUT_PORT_FUNC_ID_MONITOR       = 1,
    PIO_OUTPUT_PORT_FUNC_ID_INVERT        = 2,
    PIO_OUTPUT_PORT_FUNC_ID_USER_SET      = 3
}HV_PIO_OUTPUT_PORT_FUNC_ID;

/*!
PIO_OUTPUT_PORT_CONTORL
*/
typedef union tagHV_PIO_OUTPUT_PORT{

	/*! 
	SourceSelect field for the PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT function.
	*/
	struct {
		DWORD Value : 5; ///< Select Signal Source. 0:Integral 1:trigger_ready 2:User Set 3:Strobe
		DWORD       :27;
	} SourceSelect;

    /*!
	Monitor field for the PIO_OUTPUT_PORT_FUNC_ID_MONITOR function.
	*/	
	struct {
		DWORD Value : 1; ///< Current Output State,0:LOW,1:HIGH.
		DWORD       :31;
	} Monitor;
	
	/*!
	Invert field for the PIO_OUTPUT_PORT_FUNC_ID_INVERT function.
	*/
	struct {
		DWORD Value : 1; ///< Signal Invert,0:Not invert,1:Invert;
		DWORD       :31;
	} Invert;
	
	/*!
	UserSet field for the PIO_OUTPUT_PORT_FUNC_ID_USER_SET function.
	*/
	struct {
		DWORD Value : 1; ///< Select User Set,0:LOW,1:HIGH
		DWORD       :31;
	} UserSet;
}HV_PIO_OUTPUT_PORT;

/// The PIO output port ITEM id for set
typedef enum tagHV_PIO_OUTPUT_PORT_ITEM_ID{
	PIO_OUTPUT_PORT_ITEM_ID_SOURCE_SELECT_VALUE = 0x00001B,
	PIO_OUTPUT_PORT_ITEM_ID_MONITOR_VALUE       = 0x01001F,
	PIO_OUTPUT_PORT_ITEM_ID_INVERT_VALUE        = 0x02001F,
	PIO_OUTPUT_PORT_ITEM_ID_USER_SET_VALUE      = 0x03001F
}HV_PIO_OUTPUT_PORT_ITEM_ID;

/// Argument for the ORD_GET_PIO_OUTPUT_PORT function
typedef struct tagHV_ARG_GET_PIO_OUTPUT_PORT{
	HV_PIO_OUTPUT_PORT_ID PortId;
	HV_PIO_OUTPUT_PORT_FUNC_ID PortFuncId;
}HV_ARG_GET_PIO_OUTPUT_PORT;

/// Result of the ORD_GET_PIO_OUTPUT_PORT function
typedef HV_PIO_OUTPUT_PORT HV_RES_GET_PIO_OUTPUT_PORT;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_PIO_OUTPUT_PORT
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_PIO_OUTPUT_PORT function
typedef struct tagHV_ARG_SET_PIO_OUTPUT_PORT{
	HV_PIO_OUTPUT_PORT_ID PortId;
	HV_PIO_OUTPUT_PORT_FUNC_ID PortFuncId;
	HV_PIO_OUTPUT_PORT_ITEM_ID PortItemId;
	HV_PIO_OUTPUT_PORT OutputPort;
}HV_ARG_SET_PIO_OUTPUT_PORT;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_SIO_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// The descriptor struct for the SIO
typedef struct tagHV_SIO_DESCRIPTOR{
	struct {
		DWORD ValueInq : 8; ///< Maximum Value of Buffer Size
		DWORD          :24;
	} BufferSize;

	struct {
		DWORD           :29;
		DWORD bit2Inq   : 1; ///< Presence of StopBits=2,0:Not available 1:Available.
		DWORD bit1_5Inq : 1; ///< Presence of StopBits=1.5,0:Not available 1:Available.
		DWORD bit1Inq   : 1; ///< Presence of StopBits=1,0:Not available 1:Available.
	} StopBitMode;

	struct {
		DWORD         :29;
		DWORD EvenInq : 1; ///< Presence of Parity=Even,0:Not available 1:Available.
		DWORD OddInq  : 1; ///< Presence of Parity=Odd,0:Not available 1:Available.
		DWORD NoneInq : 1; ///< Presence of Parity=None,0:Not available 1:Available.
	} ParityMode;

	struct {
		DWORD          :23;
		DWORD bits8Inq : 1; ///< Presence of CharLength=8bits,0:Not available 1:Available.
		DWORD bits7Inq : 1; ///< Presence of CharLength=7bits,0:Not available 1:Available.
		DWORD          : 7;
	} CharLengthMode;

	struct {
		DWORD              :21;
		DWORD bps230400Inq : 1; ///< Presence of BaudRate=230400bps,0:Not available 1:Available.
		DWORD bps115200Inq : 1; ///< Presence of BaudRate=115200bps,0:Not available 1:Available.
		DWORD bps57600Inq  : 1; ///< Presence of BaudRate=57600bps,0:Not available 1:Available.
		DWORD bps38400Inq  : 1; ///< Presence of BaudRate=38400bps,0:Not available 1:Available.
		DWORD bps19200Inq  : 1; ///< Presence of BaudRate=19200bps,0:Not available 1:Available.
		DWORD bps9600Inq   : 1; ///< Presence of BaudRate=9600bps,0:Not available 1:Available.
		DWORD bps4800Inq   : 1; ///< Presence of BaudRate=4800bps,0:Not available 1:Available.
		DWORD bps2400Inq   : 1; ///< Presence of BaudRate=2400bps,0:Not available 1:Available.
		DWORD bps1200Inq   : 1; ///< Presence of BaudRate=1200bps,0:Not available 1:Available.
		DWORD bps600Inq    : 1; ///< Presence of BaudRate=600bps,0:Not available 1:Available.
		DWORD bps300Inq    : 1; ///< Presence of BaudRate=300bps,0:Not available 1:Available.
	} BandRateMode;
}HV_SIO_DESCRIPTOR;

/// Result of the ORD_QUERY_SIO_DESCRIPTOR function
typedef HV_SIO_DESCRIPTOR HV_RES_QUERY_SIO_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_SIO
//////////////////////////////////////////////////////////////////////////
/// The sub functions of the SIO
typedef enum tagHV_SIO_FUNC_ID{
	SIO_FUNC_ID_MODE    = 0, ///< The SIO Mode Control
	SIO_FUNC_ID_CONTROL = 1, ///< The SIO Control Status Control
	SIO_FUNC_ID_DATA    = 2  ///< The SIO Data Control

}HV_SIO_FUNC_ID;

typedef enum tagHV_SIO_STOP_BIT{
	HV_SIO_STOP_BIT_1bit    = 0,
	HV_SIO_STOP_BIT_1p5bits = 1,
	HV_SIO_STOP_BIT_2bits   = 2
}HV_SIO_STOP_BIT;

typedef enum tagHV_SIO_PARITY{
	HV_SIO_PARITY_NONE = 0,
	HV_SIO_PARITY_ODD  = 1,
	HV_SIO_PARITY_EVEN = 2
}HV_SIO_PARITY;

typedef enum tagHV_SIO_CHAR_LENGTH{
	HV_SIO_CHAR_LENGTH_7bits = 7,
	HV_SIO_CHAR_LENGTH_8bits = 8
}HV_SIO_CHAR_LENGTH;

typedef enum tagHV_SIO_BAUD_RATE{
	HV_SIO_BAUD_RATE_300bps    = 0,
	HV_SIO_BAUD_RATE_600bps    = 1,
	HV_SIO_BAUD_RATE_1200bps   = 2,
	HV_SIO_BAUD_RATE_2400bps   = 3,
	HV_SIO_BAUD_RATE_4800bps   = 4,
	HV_SIO_BAUD_RATE_9600bps   = 5,
	HV_SIO_BAUD_RATE_19200bps  = 6,
	HV_SIO_BAUD_RATE_38400bps  = 7,
	HV_SIO_BAUD_RATE_57600bps  = 8,
	HV_SIO_BAUD_RATE_115200bps = 9,
	HV_SIO_BAUD_RATE_230400bps = 10
}HV_SIO_BAUD_RATE;

/// The SIO union
typedef union tagHV_SIO{
	struct {
		DWORD            :12;
		DWORD StopBit    : 2;
		DWORD Parity     : 2;
		DWORD CharLength : 8;
		DWORD RateValue  : 8;
	} Mode;

	struct {
		DWORD                :30;
		DWORD TransmitEnable : 1; ///< Transmit Enable,0:disable 1:enable.
		DWORD ReceiveEnable  : 1; ///< Receive Enable,0:disable 1:enable.
	} Control;

	struct {
		BYTE *pBuffer;///<Pointer to Data Buffer.
		int nLength;  ///< Size of Data Buffer,in bytes.
	} Data;

}HV_SIO;

/// The SIO ITEM id for set
typedef enum tagHV_SIO_ITEM_ID{
	SIO_ITEM_ID_MODE_RATE_VALUE         = 0x000000,
	SIO_ITEM_ID_MODE_CHAR_LENGTH        = 0x000008,
	SIO_ITEM_ID_MODE_PARITY             = 0x000010,
	SIO_ITEM_ID_MODE_STOP_BIT           = 0x000012,

	SIO_ITEM_ID_CONTROL_RECEIVE_ENABLE  = 0x010000,
	SIO_ITEM_ID_CONTROL_TRANSMIT_ENABLE = 0x010001,

	SIO_ITEM_ID_DATA                    = 0x020000

}HV_SIO_ITEM_ID;

/// Argument for the ORD_GET_SIO function
typedef struct tagHV_ARG_GET_SIO{
   HV_SIO_FUNC_ID SIOFuncId;
}HV_ARG_GET_SIO;

/// Result of the ORD_GET_SIO function
typedef HV_SIO HV_RES_GET_SIO;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_SIO
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_SIO function
typedef struct tagHV_ARG_SET_SIO{
	HV_SIO_FUNC_ID SIOFuncId;
	HV_SIO_ITEM_ID SIOItemId;
	HV_SIO SIO;
}HV_ARG_SET_SIO;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_STROBE_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_STROBE_PRESENCE function
typedef struct tagHV_RES_QUERY_STROBE_PRESENCE{
	DWORD            :28;
	DWORD Strobe3Inq : 1; ///< Presence of Strobe_3,0:Not available 1:Available.
	DWORD Strobe2Inq : 1; ///< Presence of Strobe_2,0:Not available 1:Available.
	DWORD Strobe1Inq : 1; ///< Presence of Strobe_1,0:Not available 1:Available.
	DWORD Strobe0Inq : 1; ///< Presence of Strobe_0,0:Not available 1:Available.
}HV_RES_QUERY_STROBE_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_STROBE_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// Strobe_Output_X ID
typedef enum tagHV_STROBE_ID{
	STROBE0 = 0,
	STROBE1 = 1,
	STROBE2 = 2,
	STROBE3 = 3
}HV_STROBE_ID;

/// The descriptor struct of Strobe_Output_X in the DCAM
typedef struct tagHV_STROBE_DESCRIPTOR{
    DWORD MaxValue    :12; ///< Maximum value
    DWORD MinValue    :12; ///< Minimum value
    DWORD             : 1;
    DWORD PolarityInq : 1; ///< Presence of Changing Polarity of the Signal,0:Not available 1:Available.
    DWORD OnOffInq    : 1; ///< Availability of switching this feature ON and OFF,0:OFF 1:ON
    DWORD ReadOutInq  : 1; ///< Reserved
    DWORD             : 3;
    DWORD PresenceInq : 1; ///< Presence of this feature
}HV_STROBE_DESCRIPTOR;

/// Argument for the ORD_QUERY_STROBE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_STROBE_DESCRIPTOR{
	HV_STROBE_ID StrobeId;
}HV_ARG_QUERY_STROBE_DESCRIPTOR;

/// Result of the ORD_QUERY_STROBE_DESCRIPTOR function
typedef HV_STROBE_DESCRIPTOR HV_RES_QUERY_STROBE_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_STROBE
//////////////////////////////////////////////////////////////////////////

typedef struct tagHV_STROBE{
	DWORD DurationValue  :12; ///< Duration of the strobe signal.
	DWORD DelayValue     :12; ///< Delay after start of exposure until the strobe signal asserts.
	DWORD SignalPolarity : 1; ///< Select Signal Polarity,0:LOW, 1:HIGH.
	DWORD OnOff          : 1; ///< Switch to turn this feature on or off, off is 0, on is 1
	DWORD                : 5;
	DWORD PresenceInq    : 1; ///< Presence of this feature
}HV_STROBE;

typedef enum tagHV_STROBE_ITEM_ID{
	STROBE_ITEM_ID_PRESENCE_INQ    = 0x000000,
	STROBE_ITEM_ID_ON_OFF          = 0x000006,
	STROBE_ITEM_ID_SIGNAL_POLARITY = 0x000007,
	STROBE_ITEM_ID_DELAY_VALUE     = 0x000008,
	STROBE_ITEM_ID_DURATION_VALUE  = 0x000014
}HV_STROBE_ITEM_ID;

/// Argument for the ORD_GET_STROBE function
typedef struct tagHV_ARG_GET_STROBE{
	HV_STROBE_ID StrobeId;
}HV_ARG_GET_STROBE;

/// Result of the ORD_GET_STROBE function
typedef HV_STROBE HV_RES_GET_STROBE;

/// Argument for the ORD_SET_STROBE function
typedef struct tagHV_ARG_SET_STROBE{
	HV_STROBE_ID      StrobeId;
	HV_STROBE_ITEM_ID StrobeItemId;
	HV_STROBE         Strobe;
}HV_ARG_SET_STROBE;

//////////////////////////////////////////////////////////////////////////
// ORD_OPEN_SNAP
//////////////////////////////////////////////////////////////////////////


/// Argument for the ORD_OPEN_SNAP function
typedef struct tagHV_ARG_OPEN_SNAP{
	HV_SNAP_PROC pSnapFunc;///<Pointer to User CALLBACK Function
	void *pParam; ///< Pointer to a HV_SNAP_INFO variable.
}HV_ARG_OPEN_SNAP;

//////////////////////////////////////////////////////////////////////////
// ORD_START_SNAP
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_START_SNAP function
typedef struct tagHV_ARG_START_SNAP{
	BYTE **ppBuffer; ///< Pointer to image output buffer array.
	int nSum; ///< Number of image output buffer array.
}HV_ARG_START_SNAP;

//////////////////////////////////////////////////////////////////////////
// ORD_SNAP_SHOT
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SNAP_SHOT function
typedef struct tagHV_ARG_SNAP_SHOT{
	BYTE **ppBuffer;///< Pointer to image output buffer array.
	int nSum;///< Number of image output buffer array.
}HV_ARG_SNAP_SHOT;

//////////////////////////////////////////////////////////////////////////
// ORD_SNAP_SHOT
//////////////////////////////////////////////////////////////////////////

typedef struct tagHV_SNAP_SHOT_EX_INFO {
	BYTE *pImageBuff;///<Pointer to user image buffer.
	DWORD dwImageBuffSize;///<Size of user image buffer.
	HV_IMAGE *pImageDes;///<Pointer to user image descriptor.
	BYTE *pImageInfoBuff;///<Pointer to user image information buffer.
	DWORD dwImageInfoSize;///<Size of user image information buffer.
	void* pExtend;///<Reserved. Pointer to extend struct.
}HV_SNAP_SHOT_EX_INFO;

/// Argument for the ORD_SNAP_SHOT function
typedef struct tagHV_ARG_SNAP_SHOT_EX{
	DWORD dwTimeout;///<wait time
	HV_SNAP_SHOT_EX_INFO *pSnapShotExInfoArray; ///<Pointer to HV_SNAP_SHOT_EX_INFO array.
	int nSum;///<The number of HV_SNAP_SHOT_EX_INFO array.
}HV_ARG_SNAP_SHOT_EX;

//////////////////////////////////////////////////////////////////////////
// ORD_REGISTER_CALLBACK
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_REGISTER_CALLBACK function

/*! \brief The Parameter used to register snap callback extern function with HV_SNAP_INFO_EX*/
typedef struct tagHV_SNAP_INFO_EX_REGISTER{		
	DWORD dwImageBuffSize; ///< Size of image buffer.	
	BYTE **pImageInfoBuff;  ///< Pointer to image information buffer array
	int nImageInfoBuffSum;    ///< Element number of image information buffer array.
	DWORD dwImageInfoBuffSize;///< Size of image information buffer.
}HV_SNAP_INFO_EX_REGISTER;

typedef struct tagHV_ARG_REGISTER_CALLBACK {		
	HV_CALLBACK_TYPE type; 
	union{
		struct{
			HV_SNAP_PROC pFun;
			void* pParam;
		}SnapProc;

		struct{
			HV_CALLBACK_PROC pFun;
			void* pParam;
			HV_SNAP_INFO_EX_REGISTER *pRegister;
		}SnapProcEx;
	}Parameter;
}HV_ARG_REGISTER_CALLBACK;

//////////////////////////////////////////////////////////////////////////
// ORD_DEREGISTER_CALLBACK
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_DEREGISTER_CALLBACK function
typedef struct tagHV_ARG_DEREGISTER_CALLBACK {
	HV_CALLBACK_TYPE type;
	union{
		struct{
			HV_SNAP_PROC pFun;
			void* pParam;
		}SnapProc;
		
		struct{
			HV_CALLBACK_PROC pFun;
			void* pParam;		
		}CallbackProc;
	}Parameter;
}HV_ARG_DEREGISTER_CALLBACK;



//////////////////////////////////////////////////////////////////////////
// ORD_CMD_RESERVED0
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_CMD_RESERVED0 function
typedef struct tagHV_ARG_CMD_RESERVED0{
	int nLength; ///< Size of input buffer, in bytes.
	void *pContext; ///< Pointer to input buffer.
}HV_ARG_CMD_RESERVED0;

/// Argument for the ORD_CMD_RESERVED1 function
typedef HV_ARG_CMD_RESERVED0 HV_ARG_CMD_RESERVED1;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_FEATURE_PRESENCE
//////////////////////////////////////////////////////////////////////////
///Result of ORD_QUERY_ADV_FEATURE_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_FEATURE_PRESENCE{
	DWORD                     : 3;
	DWORD TriggerDelayUnit    : 1;///<Presence of Trigger Delay Unit,0:Not available 1:Available.
	DWORD ACPhaseAdjust       : 1;///<Presence of AC_PHASE_ADJUST,0:Not available 1:Available.	
	DWORD AntiFlicker         : 1;///<Presence of Anti-flicker,0:Not available 1:Available.
	DWORD McuTime             : 1;///<Presence of MCU Time,0:Not available 1:Available.
	DWORD McuDate             : 1;///<Presence of MCU Date,0:Not available 1:Available.
	DWORD JpegCompressQuality :1;///<Presence of JpegCompressQuality,0:Not available 1:Available.
	DWORD ImagePassiveTransmit : 1; ///<Presence of Image_Passive_Trans,0:Not available 1:Available.
	DWORD MeasureSpeedTimeout : 1; ///< Presence of measure speed timeout，0:Not available 1:Available.
	DWORD MeasureSpeedTriggerMode : 1; ///< Presence of measure speed trigger mode，0:Not available 1:Available.
	DWORD WatchDogResetMark   : 1; ///< Presence of watch dog reset，0:Not available 1:Available.
	DWORD InterPacketDelay    : 1; ///< Presence of Inter Packet Delay,0:Not available 1:Available.
	DWORD TriggerSourceStatus : 1; ///< Presence of Trigger Source Status,0:Not available 1:Available.
	DWORD PixelEndianess      : 1; ///< Presence of Pixel Endian,0:Not available 1:Available.
	DWORD FilterTimeUnit      : 1; ///< Presence of Filter Time Unit,0:Not available 1:Available.
	DWORD FilterTime          : 1; ///< Presence of Filter Time,0:Not available 1:Available.
	DWORD DelayTransmit       : 1; ///< Presence of Transmit Delay,0:Not available 1:Available.
	DWORD StartupMemCh        : 1; ///< Presence of Startup Memory Channel,0:Not available 1:Available.
	DWORD DigitalGain         : 1; ///< Presence of Digital Gain,0:Not available 1:Available.
	DWORD MultiTriggerTime    : 1; ///< Presence of Multi Trigger Time,0:Not available 1:Available.
	DWORD MultiTriggerNum     : 1; ///< Presence of Multi Trigger Number,0:Not available 1:Available.
	DWORD AdControl           : 1; ///< Presence of AD Control,0:Not available 1:Available.
	DWORD TestImage           : 1; ///< Presence of Test Image,0:Not available 1:Available.
	DWORD ShutterUnit         : 1; ///< Presence of Shutter Unit,0:Not available 1:Available.
	DWORD MirrorMode          : 1; ///< Presence of Mirror Mode,0:Not available 1:Available.
	DWORD BinMode             : 1; ///< Presence of Bin Mode,0:Not available 1:Available.
	DWORD SkipMode            : 1; ///< Presence of Skip Mode, ,0:Not available 1:Available.
	DWORD GlobalBlackLevel    : 1; ///< Presence of Global Black Level,0:Not available 1:Available.
	DWORD PixelsSeparateBlackLevel : 1; ///< Presence of Pixels Separate Black Level,0:Not available 1:Available.
	DWORD PixelsSeparateGain  : 1; ///< Presence of Pixels Separate Gain,0:Not available 1:Available.

	DWORD                     :32;
}HV_RES_QUERY_ADV_FEATURE_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_FEATURE_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////

/// Advanced Feature ID
typedef enum tagHV_ADV_FEATURE_ID{
	ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN         = 0, 
	ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL  = 1,
	ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL          = 2,
	ADV_FEATURE_ID_SKIP_MODE                   = 3,
	ADV_FEATURE_ID_BIN_MODE                    = 4,
	ADV_FEATURE_ID_MIRROR_MODE                 = 5,
	ADV_FEATURE_ID_SHUTTER_UNIT                = 6, 
	ADV_FEATURE_ID_TEST_IMAGE                  = 7, 
	ADV_FEATURE_ID_AD_CONTROL                  = 8, 
	ADV_FEATURE_ID_MULTI_TRIGGER_NUM           = 9, 
	ADV_FEATURE_ID_MULTI_TRIGGER_TIME          = 10,
	ADV_FEATURE_ID_DIGITAL_GAIN                = 11,
	ADV_FEATURE_ID_STARTUP_MEM_CH              = 12,
	ADV_FEATURE_ID_DELAY_TRANSMIT              = 13,
	ADV_FEATURE_ID_FILTER_TIME                 = 14,
	ADV_FEATURE_ID_FILTER_TIME_UNIT            = 15,
	ADV_FEATURE_ID_PIXEL_ENDIANESS             = 16,
	ADV_FEATURE_ID_TRIGGER_SOURCE_STATUS       = 17,
	ADV_FEATURE_ID_INTER_PACKET_DELAY          = 18,
	ADV_FEATURE_ID_WATCHDOG_RESET_MARK         = 19,
	ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE    = 20,
	ADV_FEATURE_ID_MEASURESPEED_TIMEOUT        = 21,
	ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT      = 22,	
	ADV_FEATURE_ID_JPEG_COMPRESS_QUALITY       = 23,
	ADV_FEATURE_ID_MCU_DATE                    = 24,
	ADV_FEATURE_ID_MCU_TIME                    = 25,
	ADV_FEATURE_ID_ANTI_FLICKER                = 26,
	ADV_FEATURE_ID_AC_PHASE_ADJUST             = 27,
	ADV_FEATURE_ID_TRIGGER_DELAY_UNIT          = 28

}HV_ADV_FEATURE_ID;

/// Advanced Feature INQ Descriptor
typedef union tagHV_ADV_FEATURE_DESCRIPTOR{
	/*!
	Scalar field for functions:
	ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN,
	ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL,
	ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL,
	ADV_FEATURE_ID_SKIP_MODE,
	ADV_FEATURE_ID_BIN_MODE,
	ADV_FEATURE_ID_MULTI_TRIGGER_NUM,
	ADV_FEATURE_ID_MULTI_TRIGGER_TIME,
	ADV_FEATURE_ID_DIGITAL_GAIN,
	ADV_FEATURE_ID_STARTUP_MEM_CH,
	ADV_FEATURE_ID_DELAY_TRANSMIT,	
	ADV_FEATURE_ID_FILTER_TIME_UNIT,	
	ADV_FEATURE_ID_INTER_PACKET_DELAY,	
	ADV_FEATURE_ID_JPEG_COMPRESS_QUALITY
	ADV_FEATURE_ID_MCU_DATE
	ADV_FEATURE_ID_MCU_TIME
	*/
	struct {
		DWORD MaxValue       :12; ///< Maximum value
		DWORD MinValue       :12; ///< Minimum value
		DWORD ManualInq      : 1; ///< Reserved
		DWORD AutoInq        : 1; ///< Reserved
		DWORD OnOffInq       : 1; ///< Availability of switching this feature ON or OFF,0:OFF 1:ON.
		DWORD ReadOutInq     : 1; ///< Reserved
		DWORD OnePushInq     : 1; ///< Reserved
		DWORD                : 1;
		DWORD AbsControlInq  : 1; ///< Reserved
		DWORD PresenceInq    : 1; ///< Presence of this Scalar function
	} Scalar;

	struct {
		DWORD MaxValue       :16;  ///< Maximum value
		DWORD MinValue       : 8;  ///< Minimum value
		DWORD ManualInq      : 1;  ///< Reserved
		DWORD AutoInq        : 1;  ///< Reserved
		DWORD OnOffInq       : 1;  ///< Availability of switching this feature ON or OFF, 0:OFF, 1:ON
		DWORD ReadOutInq     : 1;  ///< Reserved
		DWORD OnePushInq     : 1;  ///< Reserved
		DWORD                : 1;
		DWORD AbsControlInq  : 1;  ///< Reserved
		DWORD PresenceInq    : 1;  ///< Presence of this function
	}FilterTime, ///<FilterTime field for the ADV_FEATURE_ID_FILTER_TIME.
	 ScalarEx;   ///<ScalarEx field for the ADV_FEATURE_ID_MEASURESPEED_TIMEOUT.

	/*!
	Selector field for the functions:
	ADV_FEATURE_ID_MIRROR_MODE,
	ADV_FEATURE_ID_SHUTTER_UNIT,
	ADV_FEATURE_ID_TEST_IMAGE,
	ADV_FEATURE_ID_AD_CONTROL,
	ADV_FEATURE_ID_FILTER_TIME_UNIT,
	ADV_FEATURE_ID_PIXEL_ENDIANESS,
	ADV_FEATURE_ID_TRIGGER_DELAY_UNIT.	
	*/
	struct {
		DWORD               :10;///< Reserved
		DWORD Mode5Inq      : 1; ///< Presence of mode 5,0:Not available 1:Available.
		DWORD Mode4Inq      : 1; ///< Presence of mode 4,0:Not available 1:Available.
		DWORD Mode3Inq      : 1; ///< Presence of mode 3,0:Not available 1:Available.
		DWORD Mode2Inq      : 1; ///< Presence of mode 2,0:Not available 1:Available.
		DWORD Mode1Inq      : 1; ///< Presence of mode 1,0:Not available 1:Available.
		DWORD Mode0Inq      : 1; ///< Presence of mode 0,0:Not available 1:Available.
		DWORD               : 8;///< Reserved
		DWORD ManualInq     : 1; ///< Reserved
		DWORD AutoInq       : 1; ///< Reserved
		DWORD OnOffInq      : 1; ///< Availability of switching this feature ON or OFF, 0:OFF 1:ON.
		DWORD ReadOutInq    : 1; ///< Reserved
		DWORD OnePushInq    : 1; ///< Reserved
		DWORD               : 1;///< Reserved
		DWORD AbsControlInq : 1; ///< Reserved
		DWORD PresenceInq   : 1; ///< Presence of this function
	} Selector;

	/*!
	MeasureSpeedTriggerMode field for the ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE.
	AntiFlicker field for the ADV_FEATURE_ID_ANTI_FLICKER.
	*/
	struct {
		DWORD                   : 11;///< Reserved
		DWORD TriggerMode4Inq   : 1; ///< Presence of trigger mode 4,0:Not available 1:Available.
		DWORD TriggerMode3Inq   : 1; ///< Presence of trigger mode 3,0:Not available 1:Available.
		DWORD TriggerMode2Inq   : 1; ///< Presence of trigger mode 2,0:Not available 1:Available.
		DWORD TriggerMode1Inq   : 1; ///< Presence of trigger mode 1,0:Not available 1:Available.
		DWORD TriggerMode0Inq   : 1; ///< Presence of trigger mode 0,0:Not available 1:Available.
		DWORD                   : 4;///< Reserved
		DWORD TriggerSource3Inq : 1; ///< Presence of trigger source 3,0:Not available 1:Available.
		DWORD TriggerSource2Inq : 1; ///< Presence of trigger source 2,0:Not available 1:Available.
		DWORD TriggerSource1Inq : 1; ///< Presence of trigger source 1,0:Not available 1:Available.
		DWORD TriggerSource0Inq : 1; ///< Presence of trigger source 0,0:Not available 1:Available.
		DWORD ValueReadInq      : 1; ///< Capability of reading raw trigger input 
		DWORD PolarityInq       : 1; ///< Capability of changing polarity of the trigger input
		DWORD OnOffInq          : 1; ///< Availability of switching this feature on and off
		DWORD ReadOutInq        : 1; ///< Capability of reading the value of this feature
		DWORD OnePushInq        : 1; ///< One push auto mode(Controlled automatically by camera only once)
		DWORD                   : 1;///< Reserved
		DWORD AbsControlInq     : 1; ///< Availability of absolute control
		DWORD PresenceInq       : 1; ///< Presence of this feature
	} MeasureSpeedTriggerMode,///MeasureSpeedTriggerMode field for the ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE.
	  AntiFlicker;            ///AntiFlicker field for the ADV_FEATURE_ID_ANTI_FLICKER
 /*
  ImagePassiveTransmit field for the ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT
 */
 struct {
 	  DWORD MaxBufferNum         : 12; ///< Max frame number.
      DWORD                      : 13; ///< Reserved
 	  DWORD OnOffInq             : 1; ///< Availability of switching this feature on and off
 	  DWORD                      : 5; ///< Reserved
 	  DWORD PresenceInq          : 1; ///< Presence of this feature
  }ImagePassiveTransmitInq;

}HV_ADV_FEATURE_DESCRIPTOR;

/// Argument for the ORD_QUERY_ADV_FEATURE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR{
    HV_ADV_FEATURE_ID  AdvFeatureId;
}HV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR;

/// Result of the ORD_QUERY_ADV_FEATURE_DESCRIPTOR function
typedef HV_ADV_FEATURE_DESCRIPTOR HV_RES_QUERY_ADV_FEATURE_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_ADV_FEATURE
//////////////////////////////////////////////////////////////////////////

typedef union tagHV_ADV_FEATURE{

	/*!
	PixelSeparate field for the functions:
	ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN,
	ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL
	*/
	struct {
		DWORD BValue         : 6; ///< Blue Value
		DWORD GbValue        : 6; ///< Green B Value of RGB
		DWORD GrValue        : 6; ///< Green R Value of RGB
		DWORD RValue         : 6; ///< Read Value
		DWORD AutoManualMode : 1; ///< Reserved
		DWORD OnOff          : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD OnePush        : 1; ///< Reserved
		DWORD                : 3; ///< Reserved
		DWORD AbsControl     : 1; ///< Reserved
		DWORD PresenceInq    : 1; ///< Presence of this feature
	} PixelSeparate;

	/*!
	Scalar field for for the functions as follows:
	ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL,	
	ADV_FEATURE_ID_MULTI_TRIGGER_NUM,
	ADV_FEATURE_ID_MULTI_TRIGGER_TIME,
	ADV_FEATURE_ID_INTER_PACKET_DELAY,
	ADV_FEATURE_ID_JPEG_COMPRESS_QUALITY
	*/
	struct {
		DWORD Value          :12;
		DWORD                :12; ///< Reserved
		DWORD AutoManualMode : 1; ///< Reserved
		DWORD OnOff          : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD OnePush        : 1; ///< Reserved
		DWORD                : 3; 
		DWORD AbsControl     : 1; ///< Reserved
		DWORD PresenceInq    : 1; ///< Presence of this feature
	} Scalar;

	/*!
	SkipBinMode field for the ADV_FEATURE_ID_SKIP_MODE,ADV_FEATURE_ID_BIN_MODE.
	*/
	struct {
		DWORD ColumnSkip     :12; ///< Column Skip
		DWORD RowSkip        :12; ///< Row Skip
		DWORD AutoManualMode : 1; ///< Reserved
		DWORD OnOff          : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD OnePush        : 1; ///< Reserved
		DWORD                : 3; 
		DWORD AbsControl     : 1; ///< Reserved
		DWORD PresenceInq    : 1; ///< Presence of this feature
	} SkipBinMode;

	/*
	Selector field for the functions as follows:
	ADV_FEATURE_ID_MIRROR_MODE,
	ADV_FEATURE_ID_SHUTTER_UNIT,
	ADV_FEATURE_ID_TEST_IMAGE,
	ADV_FEATURE_ID_AD_CONTROL,
	ADV_FEATURE_ID_FILTER_TIME_UNIT,
	ADV_FEATURE_ID_PIXEL_ENDIANESS,
	ADV_FEATURE_ID_TRIGGER_DELAY_UNIT,
	*/
	struct {
		DWORD Parameter   :12; ///< Parameter,if required.(Optional)
		DWORD             : 4; ///< Reserved
		DWORD Mode        : 4; 
		DWORD             : 4;
		DWORD             : 1;
		DWORD OnOff       : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD OnePush     : 1; ///< Reserved
		DWORD             : 3;
		DWORD AbsControl  : 1; ///< Reserved
		DWORD PresenceInq : 1; ///< Presence of this feature
	} Selector;

	/*!
	DelayTransmit field for the ADV_FEATURE_ID_DELAY_TRANSMIT.
	*/
	struct {
		DWORD           :16;
		DWORD DelayTime :16;
	} DelayTransmit;

	/*!
	StartupMemCh field for the ADV_FEATURE_ID_STARTUP_MEM_CH.
	*/
	struct {
		DWORD Number      : 4; ///< Select Startup Memoru Channel. 0:factory set(default) 1、2、3-user set
		DWORD             :27; 
		DWORD PresenceInq : 1; ///< Presence of this feature
	} StartupMemCh;

	/*!
	DigitalGain field for the ADV_FEATURE_ID_DIGITAL_GAIN.
	*/
	struct {
		DWORD GainNum     : 8; ///< Gain Number, {1、2、4、8、16}
		DWORD             :17; 
		DWORD OnOff       : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD OnePush     : 1; ///< Reserved
		DWORD             : 3;
		DWORD AbsControl  : 1; ///< Reserved
		DWORD PresenceInq : 1; ///< Presence of this feature
	} DigitalGain;

	/*!
	FilterTime field for the ADV_FEATURE_ID_FILTER_TIME.
	*/
	struct {
		DWORD Value        :16; ///< [0,2000] (ns), default value is 330ns
		DWORD              : 9;
		DWORD OnOff        : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD OnePush      : 1; ///< Reserved
		DWORD              : 3;
		DWORD AbsControl   : 1; ///< Reserved
		DWORD PresenceInq  : 1; ///< Presence of this feature
	} FilterTime, ///<FilterTime field for the ADV_FEATURE_ID_FILTER_TIME.
	  ScalarEx;   ///<ScalarEx field for the ADV_FEATURE_ID_AC_PHASE_ADJUST.

	/*!
	MeasureSpeedTriggerMode field for the ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE.
	AntiFlicker field for the ADV_FEATURE_ID_ANTI_FLICKER.
	*/	
	struct {
		DWORD                 :21; ///<Reserved
		DWORD TriggerSource   : 3; ///<Select trigger source Set trigger source ID from trigger source ID
		DWORD TriggerPolarity : 1; ///<Select trigger polarity (Except for software trigger) ,0:LOW 1:HIGH
		DWORD OnOff           : 1; ///<Switch to turn this feature ON or OFF, 0:OFF 1:ON.
		DWORD                 : 6; ///<Reserved
	} MeasureSpeedTriggerMode,  ///MeasureSpeedTriggerMode field for the ADV_FEATURE_ID_MEASURESPEED_TRIGGERMODE.
	  AntiFlicker;              ///AntiFlicker  field for the ADV_FEATURE_ID_ANTI_FLICKER

	/*!
	FilterTime field for the ADV_FEATURE_ID_MEASURESPEED_TIMEOUT.
	*/	
	struct {
		DWORD TimeoutValue    :16; ///< ms.
		DWORD                 :16; ///< Reserved
	} MeasureSpeedTimeout;
	/*
  ImagePassiveTransmit field for the ADV_FEATURE_ID_IMAGE_PASSIVE_TRANSMIT
 */
 struct {
 	  DWORD NumOfImages          : 8; ///< Number of images currently in buffer. 
 	  DWORD BufferNum            : 8; ///< (Read Only)Maximum number of images in the current configuration.
 	  DWORD                      : 9; ///< Reserved 	
 	  DWORD HoldImg              : 1; ///< 0: disable hold image 1: store images to frame buffer rather than transmitting.
	  DWORD SendImg              : 1; ///< 0：no effect     1：Send the specified number of images from the buffer 
      DWORD                      : 4; ///< Reserved
 	  DWORD PresenceInq          : 1; ///< Presence of this feature
  }ImagePassiveTransmit;


 //ADV_FEATURE_ID_MCU_DATE
 struct {
	 DWORD Week  : 4;
	 DWORD Day   : 8;
	 DWORD Month : 4;
	 DWORD Year  : 16;
 }Date;

 //ADV_FEATURE_ID_MCU_TIME
 struct {
	 DWORD        : 8;
	 DWORD Second : 8;
	 DWORD Minute : 8;
	 DWORD Hour   : 8;
 }Time;

}HV_ADV_FEATURE;

/// The advanced feature ITEM id for set
typedef enum tagHV_ADV_FEATURE_ITEM_ID{
	ADV_FEATURE_ITEM_ID_PIXEL_PRESENCE_INQ            = 0x000000,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_ABS_CONTROL  = 0x000001,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_ONE_PUSH     = 0x000005,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_ON_OFF       = 0x000006,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_AUTO_MANUAL_MODE_VALUE = 0x000007,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_R_VALUE      = 0x000008,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GR_VALUE     = 0x00000E,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GB_VALUE     = 0x000014,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_B_VALUE      = 0x00001A,

	ADV_FEATURE_ITEM_ID_SCALAR_PRESENCE_INQ           = 0x010000,
	ADV_FEATURE_ITEM_ID_SCALAR_ABS_CONTROL            = 0x010001,
	ADV_FEATURE_ITEM_ID_SCALAR_ONE_PUSH               = 0x010005,
	ADV_FEATURE_ITEM_ID_SCALAR_ON_OFF                 = 0x010006,
	ADV_FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE_VALUE = 0x010007,
	ADV_FEATURE_ITEM_ID_SCALAR_VALUE                  = 0x010014,

	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_PRESENCE_INQ    = 0x020000,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ABS_CONTROL     = 0x020001,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ONE_PUSH        = 0x020005,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ON_OFF          = 0x020006,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_AUTO_MANUAL_MODE_VALUE = 0x020007,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ROW_SKIP        = 0x020008,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_COLUMN_SKIP     = 0x020014,

	ADV_FEATURE_ITEM_ID_SELECTOR_PRESENCE_INQ         = 0x030000,
	ADV_FEATURE_ITEM_ID_SELECTOR_ABS_CONTROL          = 0x030001,
	ADV_FEATURE_ITEM_ID_SELECTOR_ONE_PUSH             = 0x030005,
	ADV_FEATURE_ITEM_ID_SELECTOR_ON_OFF               = 0x030006,
	ADV_FEATURE_ITEM_ID_SELECTOR_MODE                 = 0x03000C,
	ADV_FEATURE_ITEM_ID_SELECTOR_PARAMETER            = 0x030014,

	ADV_FEATURE_ITEM_ID_DELAY_TRANSMIT_DELAY_TIME     = 0x040000,

	ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_PRESENCE_INQ   = 0x050000,
	ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_NUMBER         = 0x05001C,

	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_PRESENCE_INQ     = 0x060000,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_ABS_CONTROL      = 0x060001,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_ONE_PUSH         = 0x060005,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_ON_OFF           = 0x060006,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_GAIN_NUM         = 0x060018,

	ADV_FEATURE_ITEM_ID_FILTER_TIME_PRESENCE_INQ      = 0x070000,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_ABS_CONTROL       = 0x070001,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_ONE_PUSH          = 0x070005,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_ON_OFF            = 0x070006,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_VALUE             = 0x070010,

	ADV_FEATURE_ITEM_ID_MEASURESPEED_TRIGGERMODE_ONOFF = 0x080006,
	ADV_FEATURE_ITEM_ID_MEASURESPEED_TRIGGERPOLARITY   = 0x080007,
	ADV_FEATURE_ITEM_ID_MEASURESPEED_TRIGGERSOURCE     = 0x080008,
	
	ADV_FEATURE_ITEM_ID_MEASURESPEED_TIMEOUT_VALUE     = 0x090010, 
	ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_PRESENCE_INQ = 0x0A0000,
	ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_SEND_IMAGE = 0x0A0005,
	ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_HOLD_IMAGE = 0x0A0006, 
	ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_BUFFER_NUM = 0x0A0010,    //Read Only
	ADV_FEATURE_ITEM_ID_IMAGE_PASSIVE_TRANS_NUM_OF_IMAGES = 0x0A0018,

	ADV_FEATURE_ITEM_ID_ANTI_FLICKER_ONOFF             = 0x0B0006,
	ADV_FEATURE_ITEM_ID_ANTI_FLICKER_TRIGGERPOLARITY   = 0x0B0007,
	ADV_FEATURE_ITEM_ID_ANTI_FLICKER_TRIGGERSOURCE     = 0x0B0008,

	ADV_FEATURE_ITEM_ID_SCALAREX_PRESENCE_INQ          = 0x0C0000,
	ADV_FEATURE_ITEM_ID_SCALAREX_ABS_CONTROL           = 0x0C0001,
	ADV_FEATURE_ITEM_ID_SCALAREX_ONE_PUSH              = 0x0C0005,
	ADV_FEATURE_ITEM_ID_SCALAREX_ON_OFF                = 0x0C0006,
	ADV_FEATURE_ITEM_ID_SCALAREX_VALUE                 = 0x0C0014,

	ADV_FEATURE_ITEM_ID_MCU_DATE                       = 0x0D0000,
	ADV_FEATURE_ITEM_ID_MCU_TIME                       = 0x0E0000
	
}HV_ADV_FEATURE_ITEM_ID;

/// Argument for the ORD_GET_ADV_FEATURE function
typedef struct tagHV_ARG_GET_ADV_FEATURE{
    HV_ADV_FEATURE_ID AdvFeatureId;
}HV_ARG_GET_ADV_FEATURE;

/// Result of the ORD_GET_ADV_FEATURE function
typedef HV_ADV_FEATURE HV_RES_GET_ADV_FEATURE;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_ADV_FEATURE
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_ADV_FEATURE function
typedef struct tagHV_ARG_SET_ADV_FEATURE{
    HV_ADV_FEATURE_ID AdvFeatureId;
	HV_ADV_FEATURE_ITEM_ID AdvFeatureItemId;
	HV_ADV_FEATURE AdvFeature;
}HV_ARG_SET_ADV_FEATURE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_BASIC_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////

/// Result of ORD_QUERY_ADV_BASIC_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_BASIC_FUNC_PRESENCE{
	DWORD                   :22;
	DWORD DeferredTransInq  : 1; ///< Presence of deferred transmission,0:Not available 1:Available.
	DWORD AsynResetCapture  : 1; ///< Presence of Asynchronism Reset Capture,0:Not available 1:Available.
	DWORD MCUStatus         : 1; ///< Reserved
	DWORD PIOInputStatus    : 1; ///< Reserved
	DWORD FreezeFrame       : 1; ///< Reserved
	DWORD FpgaReset         : 1; ///< Reserved
	DWORD DefaultMemchReset : 1; ///< Reserved
	DWORD SIODebugEn        : 1; ///< Reserved
	DWORD FlashIap          : 1; ///< Reserved
	DWORD DiyAdvFeatureInq  : 1; ///< Reserved

}HV_RES_QUERY_ADV_BASIC_FUNC_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_ADV_BASIC_FUNC
//////////////////////////////////////////////////////////////////////////
/// The advanced basic functions id
typedef enum tagHV_ADV_BASIC_FUNC_ID{
	ADV_BASIC_FUNC_ID_FALSH_IAP           = 1,
	ADV_BASIC_FUNC_ID_SIO_DEBUG_ENABLE    = 2,
	ADV_BASIC_FUNC_ID_DEFAULT_MEMCH_RESET = 3,
	ADV_BASIC_FUNC_ID_FPGA_RESET          = 4,
	ADV_BASIC_FUNC_ID_FREEZE_FRAME        = 5,
	ADV_BASIC_FUNC_ID_PIO_INPUT_STATUS    = 6,
	ADV_BASIC_FUNC_ID_MCU_STATUS          = 7,
	ADV_BASIC_FUNC_ID_ASYNRESET_CAPTURE   = 8,
	ADV_BASIC_FUNC_ID_DEFERRED_TRANS      = 9
}HV_ADV_BASIC_FUNC_ID;

/*! A union for the advanced basic function
*/
typedef union tagHV_ADV_BASIC_FUNC{

	/*! MCUStatus field for the ADV_BASIC_FUNC_ID_MCU_STATUS function. */
	struct {
		DWORD dwLast; ///< The current status of MCU.
	} MCUStatus;

	/*! PIOInputStatus field for the ADV_BASIC_FUNC_ID_PIO_INPUT_STATUS function.*/
	struct {
		DWORD Input0      : 1; ///< 1:HIGH 0:LOW.
		DWORD Input1      : 1; ///< 1:HIGH 0:LOW.
		DWORD Input2      : 1; ///< 1:HIGH 0:LOW.
		DWORD             :28; ///< Reserved.
		DWORD PresenceInq : 1; ///< Presence of this function.
	} PIOInputStatus;

	/*! FreezeFrame field for the ADV_BASIC_FUNC_ID_FREEZE_FRAME function.*/
	struct {
		DWORD Count      :16; ///< Freeze Frame Number
		DWORD            :15; 
		DWORD StartFlag  : 1; ///< Switch to start freeze frame On or OFF, 0:OFF 1:ON.
	} FreezeFrame;

	/*! FPGAReset field for the ADV_BASIC_FUNC_ID_FPGA_RESET function. */
	struct {                    
		DWORD            :31;
		DWORD Enable     : 1; ///< Switch to FPGA reset On or OFF, 0:OFF 1:ON.
	} FPGAReset; 

    /*! DefaultMemChReset field for the ADV_BASIC_FUNC_ID_DEFAULT_MEMCH_RESET function. */
	struct { 
		DWORD            :31;
		DWORD Enable     : 1; ///< Switch to default memory channel reset On or OFF, 0:OFF 1:ON.					  
	} DefaultMemChReset;

    /*! SIODebug field for the ADV_BASIC_FUNC_ID_SIO_DEBUG_ENABLE function. */
	struct { 
		DWORD            :31;///< Reserved.
		DWORD Enable     : 1;///< Reserved.
	} SIODebug; 

    /*! FlashIAP field for the ADV_BASIC_FUNC_ID_FALSH_IAP function. */
	struct { 
		DWORD SaveDataSRAM :16; ///< 0xFFFF: Save Data to SRAM, Others: invalid.
		DWORD Burn         :16; ///< 0xFFFF: FLASH Burn, Others: invalid.
	} FlashIAP;   

    /*! AsynResetCapture field for the ADV_BASIC_FUNC_ID_ASYNRESET_CAPTURE function. */
	struct {
		DWORD              :31;
		DWORD Enable       : 1; ///< Switch to asynchronous reset capture On or OFF, 0:OFF 1:ON.
	} AsynResetCapture;

    /*! AsynResetCapture field for the ADV_BASIC_FUNC_ID_DEFERRED_TRANS function. */
    struct {
		DWORD              : 8;
		DWORD MemSize      : 8; ///< Size of flash memory or FiFO(Read Only)
		DWORD              : 6;
		DWORD HoldState    : 1; ///< 0:Camera is not in recording, 1:Camera is in Recording
		DWORD              : 1; ///<
		DWORD HoldImage    : 1; ///< 0:Stop Record Image, 1:Start Record Image
		DWORD SendImage    : 1; ///< 0:Stop Send image , 1:Start Send Image
		DWORD OnOff        : 1; ///< On:Use Deferrd_trans Mode, Off:No Use Deferrd_trans Mode
		DWORD              : 3;
		DWORD PresenceInq  : 1; ///< Presence of this feature
	} DeferredTrans ;
}HV_ADV_BASIC_FUNC;

/// The advanced basic function ITEM id for set
typedef enum tagHV_ADV_BASIC_FUNC_ITEM_ID{
	ADV_BASIC_FUNC_ITEM_ID_FREEZE_FRAME_START_FLAG  = 0x020000,
	ADV_BASIC_FUNC_ITEM_ID_FREEZE_FRAME_COUNT       = 0x020010,
	ADV_BASIC_FUNC_ITEM_ID_FPGA_RESET               = 0x030000,
	ADV_BASIC_FUNC_ITEM_ID_DEFAULT_MEM_CHANNEL_RESET_ENALBE = 0x040000,
	ADV_BASIC_FUNC_ITEM_ID_SIO_DEBUG_ENABLE         = 0x050000,
	ADV_BASIC_FUNC_ITEM_ID_FLASH_IAP_BURN           = 0x060000,
	ADV_BASIC_FUNC_ITEM_ID_FLASH_IAP_SAVE_DATA_SRAM = 0x060010,
	ADV_BASIC_FUNC_ITEM_ID_ASYNRESET_CAPTURE        = 0x070000,
	ADV_BASIC_FUNC_ITEM_ID_DEFERRED_TRANS_ONOFF     = 0x090000,
    ADV_BASIC_FUNC_ITEM_ID_DEFERRED_TRANS_SENDIMAGE = 0x090004,
	ADV_BASIC_FUNC_ITEM_ID_DEFERRED_TRANS_HOLDIMAGE = 0x090006,
	ADV_BASIC_FUNC_ITEM_ID_DEFERRED_TRANS_HOLDSTATE = 0x090008,
	ADV_BASIC_FUNC_ITEM_ID_DEFERRED_TRANS_MEMSIZE   = 0x090010

}HV_ADV_BASIC_FUNC_ITEM_ID;

/// Argument for the ORD_GET_ADV_BASIC_FUNC function
typedef struct tagHV_ARG_GET_ADV_BASIC_FUNC{
	HV_ADV_BASIC_FUNC_ID FuncId;
}HV_ARG_GET_ADV_BASIC_FUNC;

/// Result of the ORD_GET_ADV_BASIC_FUNC function
typedef HV_ADV_BASIC_FUNC HV_RES_GET_ADV_BASIC_FUNC ;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_ADV_BASIC_FUNC
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_ADV_BASIC_FUNC function
typedef struct tagHV_ARG_SET_ADV_BASIC_FUNC{
	HV_ADV_BASIC_FUNC_ID          FuncId;
	HV_ADV_BASIC_FUNC_ITEM_ID FuncItemId;
    HV_ADV_BASIC_FUNC               Func;
}HV_ARG_SET_ADV_BASIC_FUNC;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_OPT_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_ADV_OPT_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_OPT_FUNC_PRESENCE{
	DWORD                :20;
	DWORD PictureOSD     : 1; ///< Presence of Picture Text OSD Function,0:Not available 1:Available.
	DWORD TimeOSD        : 1; ///< Presence of Time OSD Function,0:Not available 1:Available.
	DWORD DynamicTextOSD : 1; ///< Presence of Dynamic Text OSD Function,0:Not available 1:Available.
	DWORD StaticTextOSD  : 1; ///< Presence of Static Text OSD Function,0:Not available 1:Available.
	DWORD AutoFunc       : 1; ///< Presence of Auto Function,0:Not available 1:Available.
	DWORD ImageInfo      : 1; ///< Presence of Image Information Function,0:Not available 1:Available.
	DWORD                : 1;
	DWORD                : 1;
	DWORD UserSWEncrypt  : 1; ///< Presence of User Software Encrypt function,0:Not available 1:Available.
	DWORD UserHWEncrypt  : 1; ///< Presence of User Hardware Encrypt function,0:Not available 1:Available.
	DWORD UserCustom     : 1; ///< Presence of User Customer function,0:Not available 1:Available.
	DWORD Lut            : 1; ///< Presence of LUT function,0:Not available 1:Available.

}HV_RES_QUERY_ADV_OPT_FUNC_PRESENCE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_LUT_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_ADV_LUT_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_ADV_LUT_DESCRIPTOR{
	DWORD TotalByteLengthInq :24;  ///< Maximum value for LUT Size, in BYTE.
    DWORD BitDepthInq        : 8;  ///< Bit Depth of LUT.
}HV_RES_QUERY_ADV_LUT_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_ADV_LUT
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_GET_ADV_LUT function
typedef struct tagHV_ARG_GET_ADV_LUT{
	DWORD dwOffset;///< Offset of LUT.
	int nLength;   ///< Size of LUT.
}HV_ARG_GET_ADV_LUT;

/// Result of the ORD_GET_ADV_LUT function
typedef struct tagHV_RES_GET_ADV_LUT{
	DWORD *pBuffer; ///< Pointer to LUT data Buffer.
}HV_RES_GET_ADV_LUT;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_ADV_LUT
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_ADV_LUT function
typedef struct tagHV_ARG_SET_ADV_LUT{
	DWORD dwOffset; ///< Offset of LUT.
	int nLength;    ///< Size of LUT.
	DWORD *pBuffer; ///< Pointer to LUT data buffer.
}HV_ARG_SET_ADV_LUT;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR{
	DWORD TotalByteLengthInq :24; ///< Maximum value for User Customer Size, in BYTE.
	DWORD BitDepthInq        : 8; ///< Bit Depth of User Customer.

}HV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_ADV_USER_CUSTOM
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_GET_ADV_USER_CUSTOM function
typedef struct tagHV_ARG_GET_ADV_USER_CUSTOM{
	DWORD dwOffset; ///< Offset of User Customer.
	int   nLength;  ///< Offset of User Customer.
}HV_ARG_GET_ADV_USER_CUSTOM;

/// Result of the ORD_GET_ADV_USER_CUSTOM function
typedef struct tagHV_RES_GET_ADV_USER_CUSTOM{
	DWORD *pBuffer; ///< Pointer to User Customer data Buffer.
}HV_RES_GET_ADV_USER_CUSTOM;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_ADV_USER_CUSTOM
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_ADV_USER_CUSTOM function
typedef struct tagHV_ARG_SET_ADV_USER_CUSTOM{
	DWORD dwOffset; ///< Offset of User Customer.
	int   nLength;  ///< Offset of User Customer.
	DWORD *pBuffer; ///< Pointer to User Customer data Buffer.
}HV_ARG_SET_ADV_USER_CUSTOM;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE{
	DWORD       :29;
	DWORD Zone2 : 1; ///< Presence of User Encrypt Zone 2 Control,0:Not available 1:Available.
	DWORD Zone1 : 1; ///< Presence of User Encrypt Zone 1 Control,0:Not available 1:Available.
	DWORD Zone0 : 1; ///< Presence of User Encrypt Zone 0 Control,0:Not available 1:Available.
}HV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
typedef enum tagHV_ADV_USER_ENCRYPT_ZONE_ID{
	ADV_USER_ENCRYPT_ZONE0 = 0,
	ADV_USER_ENCRYPT_ZONE1 = 1,
	ADV_USER_ENCRYPT_ZONE2 = 2

}HV_ADV_USER_ENCRYPT_ZONE_ID;

/// Argument for the ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR{
    HV_ADV_USER_ENCRYPT_ZONE_ID  ZoneId;
}HV_ARG_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR;

/// Result of the ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR{
    DWORD dwLengthInq ; ///< Size of User Encrypt Zone,in BYTE.
}HV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR;


//////////////////////////////////////////////////////////////////////////
// ORD_GET_ADV_USER_ENCRYPT_ZONE
//////////////////////////////////////////////////////////////////////////
/// The struct of the user encrypt zone  functions id
typedef enum tagHV_ADV_USER_ENCRYPT_ZONE_FUNC_ID{
	ADV_USER_ENCRYPT_ZONE_FUNC_ID_CHANGE_PASSWORD =  0, ///< The Changing Password Control
	ADV_USER_ENCRYPT_ZONE_FUNC_ID_DATA  =  1 ///< The Reading or Writing Control
}HV_ADV_USER_ENCRYPT_ZONE_FUNC_ID;


typedef struct tagHV_ADV_USER_ENCRYPT_ZONE_PASSWORD{
	DWORD        : 8;
	DWORD  Value :24; ///< Password
}HV_ADV_USER_ENCRYPT_ZONE_PASSWORD;

/// The Input parameter for read zone data
typedef union tagHV_GET_ZONE_PARAMETER{	
	// UserData field for the ADV_USER_ENCRYPT_ZONE_FUNC_ID_DATA function
	struct {
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD Password;
		DWORD dwOffset; ///< Offset of User Encrypt Zone.
		int   nLength;  ///< Size of User Encrypt Zone,in BYTE.
	}UserData;
}HV_GET_ZONE_PARAMETER;

/// Argument for the ORD_GET_ADV_USER_ENCRYPT_ZONE function
typedef struct tagHV_ARG_GET_ADV_USER_ENCRYPT_ZONE{
    HV_ADV_USER_ENCRYPT_ZONE_ID      ZoneId;
	HV_ADV_USER_ENCRYPT_ZONE_FUNC_ID ZoneFuncId;
    HV_GET_ZONE_PARAMETER            Parameter;
}HV_ARG_GET_ADV_USER_ENCRYPT_ZONE;

/// Result of the ORD_GET_ADV_USER_ENCRYPT_ZONE function
typedef struct tagHV_RES_GET_ADV_USER_ENCRYPT_ZONE{
	DWORD *pBuffer; ///< Pointer to output buffer.
}HV_RES_GET_ADV_USER_ENCRYPT_ZONE;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_ADV_USER_ENCRYPT_ZONE
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_ADV_USER_ENCRYPT_ZONE function
typedef union tagHV_SET_ZONE_PARAMETER{
	// Password field for the ADV_USER_ENCRYPT_ZONE_FUNC_ID_CHANGE_PASSWORD function
	struct {
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD Old;///< Old Password
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD New;///< New Password
	} Password;

	// Password field for the ADV_USER_ENCRYPT_ZONE_FUNC_ID_DATA function
	struct {
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD Password;
		DWORD dwOffset; ///< Offset of User Encrypt Zone.
		int   nLength;  ///< Size of User Encrypt Zone,in BYTE.
		DWORD *pBuffer; ///< Pointer to input buffer.
	} UserData;
}HV_SET_ZONE_PARAMETER;

/// Argument for the ORD_SET_ADV_USER_ENCRYPT_ZONE function
typedef struct tagHV_ARG_SET_ADV_USER_ENCRYPT_ZONE{
    HV_ADV_USER_ENCRYPT_ZONE_ID ZoneId;
	HV_ADV_USER_ENCRYPT_ZONE_FUNC_ID ZoneFuncId;
    HV_SET_ZONE_PARAMETER Parameter;
}HV_ARG_SET_ADV_USER_ENCRYPT_ZONE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_NET_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_NET_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_NET_FUNC_PRESENCE{
	DWORD                      :12;
	DWORD DeviceRequestTimeout : 1;///< Reserved.
	DWORD DeviceAnswerTimeout  : 1;///< Reserved.
	DWORD SnapPort             : 1;///< Reserved.
	DWORD ControlPort          : 1;///< Reserved.
	DWORD HostMAC              : 1;///< Presence of Host MAC,0:Not available 1:Available.
	DWORD HostIP               : 1;///< Presence of Host IP,0:Not available 1:Available.
	DWORD DeviceMAC            : 1;///< Presence of Device MAC,0:Not available 1:Available.
	DWORD DeviceIP             : 1;///< Presence of Device IP,0:Not available 1:Available.
	DWORD HeartBeatExpiration  : 1;///< Reserved.
	DWORD HeartBeatInterval    : 1;///< Reserved.
	DWORD CalculatedResult     : 1;///< Reserved.
	DWORD LinkSpeed            : 1;///< Reserved.
	DWORD ExpectDataRate       : 1;///< Reserved.
	DWORD InterPacketDelay     : 1;///< Reserved.
	DWORD FirstPacketTimeout   : 1;///< Reserved.
	DWORD PacketTimeout        : 1;///< Reserved.
	DWORD HostRequestTimeout   : 1;///< Reserved.
	DWORD HostAnswerTimeout    : 1;///< Reserved.
	DWORD CmdRetryCount        : 1;///< Reserved.
	DWORD PacketSize           : 1;///< Reserved.
}HV_RES_QUERY_NET_FUNC_PRESENCE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_NET_FUNC_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// The net function ID
typedef enum tagHV_NET_FUNC_ID{
	NET_FUNC_ID_PACKET_SIZE            = 0,
	NET_FUNC_ID_CMD_RETRY_COUNT        = 1,
	NET_FUNC_ID_HOST_ANSWER_TIMEOUT    = 2,

	NET_FUNC_ID_HOST_REQUEST_TIMEOUT   = 3,
	NET_FUNC_ID_PACKET_TIMEOUT         = 4,
	NET_FUNC_ID_FIRST_PACKET_TIMEOUT   = 5,

	NET_FUNC_ID_INTER_PACKET_DELAY     = 6,
	NET_FUNC_ID_EXPECT_DATA_RATE       = 7,
	NET_FUNC_ID_LINK_SPEED             = 8,

	NET_FUNC_ID_CALCULATED_RESULT      = 9,
	NET_FUNC_ID_HEARTBEAT_INTERVAL     = 10,
	NET_FUNC_ID_HEARTBEAT_EXPIRATION   = 11,

	NET_FUNC_ID_DEVICE_IP              = 12,
	NET_FUNC_ID_DEVICE_MAC             = 13,
	NET_FUNC_ID_HOST_IP                = 14,
	NET_FUNC_ID_HOST_MAC               = 15,

	NET_FUNC_ID_CONTROL_PORT           = 16,
	NET_FUNC_ID_SNAP_PORT              = 17,

	NET_FUNC_ID_DEVICE_ANSWER_TIMEOUT  = 18,
	NET_FUNC_ID_DEVICE_REQUEST_TIMEOUT = 19
}HV_NET_FUNC_ID;


/// Net Function INQ Descriptor
typedef union tagHV_NET_FUNC_DESCRIPTOR{
	//Scalar field for Some Reserved Function.
	struct {
		DWORD dwMaxValue;
		DWORD dwMinValue;
	} Scalar;

	//LinkSpeed field for the NET_FUNC_ID_LINK_SPEED Reserved Function.
	struct 	{
		DWORD              :29;
		DWORD MBits1000Inq : 1; ///< Presence of 1000 MBits/seconds,0:Not available 1:Available.
		DWORD MBits100Inq  : 1; ///< Presence of 100 MBits/seconds,0:Not available 1:Available.
		DWORD MBits10Inq   : 1; ///< Presence of 10 MBits/seconds,0:Not available 1:Available.
	} LinkSpeed;

	// Address field for the functions:
	//NET_FUNC_ID_DEVICE_IP,
	//NET_FUNC_ID_DEVICE_MAC,
	//NET_FUNC_ID_HOST_IP,
	//NET_FUNC_ID_HOST_MAC
	struct {
		DWORD dwTotalByteLength; ///< Size of string.
	} Address;

	struct {
	} Port;        ///< Reserved
}HV_NET_FUNC_DESCRIPTOR;

/// Argument for the ORD_QUERY_NET_FUNC_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_NET_FUNC_DESCRIPTOR{
    HV_NET_FUNC_ID   NetFuncId;
}HV_ARG_QUERY_NET_FUNC_DESCRIPTOR;

/// Result of ORD_QUERY_NET_FUNC_DESCRIPTOR
typedef HV_NET_FUNC_DESCRIPTOR HV_RES_QUERY_NET_FUNC_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_NET_FUNC
//////////////////////////////////////////////////////////////////////////
/// Net Function CSR value
typedef union tagHV_NET_FUNC{
	/*!
	Scalar field for Some reserved fucitons.
	*/
	struct {
		DWORD dwValue;
	} Scalar;

	/*/
	LinkSpeed field for the NET_FUNC_ID_LINK_SPEED function. Reserved.
	*/
	struct {
		DWORD dwValue;
	} LinkSpead;

    /*!
	Address field for the functions:
	NET_FUNC_ID_DEVICE_IP,
	NET_FUNC_ID_DEVICE_MAC,
	NET_FUNC_ID_HOST_IP,
	NET_FUNC_ID_HOST_MAC
	*/
	struct {
		char *pBuffer;///< Pointer to input/output buffer.
	} Address;

    /*!
	Port field for the functions:
	NET_FUNC_ID_CONTROL_PORT,
	NET_FUNC_ID_SNAP_PORT,
	*/
	struct {
        int nNumber; ///< Port value.
	} Port;
}HV_NET_FUNC;

/// Argument for the ORD_GET_NET_FUNC function
typedef struct tagHV_ARG_GET_NET_FUNC{
    HV_NET_FUNC_ID NetFuncId;
	int nLength; ///< Size of the output buffer, in bytes.If required the output buffer.
}HV_ARG_GET_NET_FUNC;

/// Result of ORD_GET_NET_FUNC function
typedef HV_NET_FUNC HV_RES_GET_NET_FUNC;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_NET_FUNC
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_NET_FUNC function
typedef struct tagHV_ARG_SET_NET_FUNC{
	HV_NET_FUNC_ID NetFuncId;
	HV_NET_FUNC NetFunc;
	int nLength;///< Size of the input buffer, in bytes.If required the input buffer.
}HV_ARG_SET_NET_FUNC;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_IMAGE_INFO_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_IMAGE_INFO_PRESENCE function
typedef struct tagHV_RES_QUERY_IMAGE_INFO_PRESENCE{
    DWORD                                          : 27;
    DWORD Measure_Speed_Info       : 1;///< Presence of measure speed information control,0:Not available 1:Available.
	DWORD TimeLabel_MultiTrigger_N : 1;///< Presence of image frame No of multi trigger control,0:Not available 1:Available.
	DWORD TimeLabel_Trigger_N      : 1;///< Presence of Trigger Number, 0:Not available 1:Available.
	DWORD TimeLabel_N              : 1;///< Presence of Image Frame Count,0:Not available 1:Available.
	DWORD TimeLabel_T              : 1;///< Presence of Image Frame Interval Time, 0:Not available 1:Available.

	DWORD Reserved[7];
}HV_RES_QUERY_IMAGE_INFO_PRESENCE;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_IMAGE_INFO_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
/// The Image information id
typedef enum tagHV_IMAGE_INFO_ID{
	IMAGE_INFO_ID_ALL           = 256,
	IMAGE_INFO_ID_TIME_LABEL_T  = 0,
	IMAGE_INFO_ID_TIME_LABEL_N  = 1,
	IMAGE_INFO_ID_TIME_LABEL_TRIGGER_N = 2,
	IMAGE_INFO_ID_TIME_LABEL_MUTI_TRIGGER_N = 3,
	IMAGE_INFO_ID_MEASURE_SPEED_INFO  = 4				
}HV_IMAGE_INFO_ID;

typedef union tagHV_IMAGE_INFO_DESCRIPTOR{
	struct {
		DWORD Offset; ///< Offset Of the Image Information.
		DWORD Length; ///< Size of the Image Information.
	} Scalar;
}HV_IMAGE_INFO_DESCRIPTOR;

typedef union tagHV_IMAGE_INFO{
	struct {
		void *pBuff;///< Pointer to image information buffer.
	} Scalar;
}HV_IMAGE_INFO;

/// Argument for the ORD_QUERY_IMAGE_INFO_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_IMAGE_INFO_DESCRIPTOR{
	HV_IMAGE_INFO_ID  Id;
}HV_ARG_QUERY_IMAGE_INFO_DESCRIPTOR;

/// Result of the ORD_QUERY_IMAGE_INFO_DESCRIPTOR function
typedef HV_IMAGE_INFO_DESCRIPTOR  HV_RES_QUERY_IMAGE_INFO_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_IMAGE_INFO
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_GET_IMAGE_INFO function
typedef HV_ARG_QUERY_IMAGE_INFO_DESCRIPTOR HV_ARG_GET_IMAGE_INFO;

/// Result of the ORD_GET_IMAGE_INFO function
typedef HV_IMAGE_INFO HV_RES_GET_IMAGE_INFO;

//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_AUTO_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_AUTO_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_AUTO_FUNC_PRESENCE{
	DWORD                      :25;///< Reserved
	DWORD AutoStrobeStatCtl    : 1;///< Presence of Auto strobe control,0:Not available 1:Available.
	DWORD AutoStrobeGainCtl    : 1;///< Presence of Auto strobe Shuuter control,0:Not available 1:Available.
    DWORD AutoStrobeShutterCtl : 1;///< Presence of Auto strobe Gain control,0:Not available 1:Available.
	DWORD AutoStrobeCtl        : 1;///< Presence of Auto strobe Stat control,0:Not available 1:Available.
	DWORD AutoFuncAOI          : 1;///< Presence of auto AOI control,0:Not available 1:Available.
	DWORD AutoShutterCtrl      : 1;///< Presence of auto shutter control,0:Not available 1:Available.
	DWORD AutoGainCtrl         : 1;///< Presence of auto gain control,0:Not available 1:Available.
}HV_RES_QUERY_AUTO_FUNC_PRESENCE;


//////////////////////////////////////////////////////////////////////////
// ORD_QUERY_AUTO_FUNC_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////
///The Auto Func ID
typedef enum tagHV_AUTO_FUNC_ID{
	AUTO_FUNC_ID_AUTO_GAIN_CTRL           = 0,
	AUTO_FUNC_ID_AUTO_SHUTTER_CTRL        = 1,
	AUTO_FUNC_ID_AUTO_FUNC_AOI            = 2,
	AUTO_FUNC_ID_AUTO_STROBE_CTRL         = 3,
	AUTO_FUNC_ID_AUTO_STROBE_SHUTTER_CTRL = 4,
	AUTO_FUNC_ID_AUTO_STROBE_GAIN_CTRL    = 5,
	AUTO_FUNC_ID_AUTO_STROBE_STAT_CTRL    = 6	

}HV_AUTO_FUNC_ID;

/// Argument for the ORD_QUERY_AUTO_FUNC_DESCRIPTOR function
typedef union tagHV_AUTO_FUNC_DESCRIPTOR{
	/*!
	Scalar field for the functions:
	AUTO_FUNC_ID_AUTO_GAIN_CTRL,
	AUTO_FUNC_ID_AUTO_SHUTTER_CTRL,
	AUTO_FUNC_ID_AUTO_STROBE_CTRL,
	AUTO_FUNC_ID_AUTO_STROBE_SHUTTER_CTRL,
	AUTO_FUNC_ID_AUTO_STROBE_GAIN_CTRL,
	AUTO_FUNC_ID_AUTO_STROBE_STAT_CTRL
	*/
	struct {
		DWORD MaxValue      :12; ///< Maximum value
		DWORD MinValue      :12; ///< Minimum value
		DWORD ManualInq     : 1; ///< Reserved.
		DWORD AutoInq       : 1; ///< Reserved.
		DWORD OnOffInq      : 1; ///< Availability of switching this feature on and off
		DWORD ReadOutInq    : 1; ///< Capability of reading the value of this feature
		DWORD OnePushInq    : 1; ///< Reserved.
		DWORD               : 1; ///< Reserved.
		DWORD AbsControlInq : 1; ///< Reserved.
		DWORD PresenceInq   : 1; ///< Presence of this feature
	} Scalar;

	/*!
	Block field for the AUTO_FUNC_ID_AUTO_FUNC_AOI.
	*/
	struct{
		DWORD length; ///< Size of the input buffer.
		DWORD *pBuff; ///< Pointer to the input buffer.
	}Block;

}HV_AUTO_FUNC_DESCRIPTOR;

typedef	struct tagHV_AUTO_FUNC_DESCRIPTOR_AOI{
		DWORD VmaxInq  :16; ///< Maximum Vertical pixel number.
		DWORD HmaxInq  :16; ///< Maximum Horizontal pixel number.
		DWORD VunitInq :16; ///< Vertical unit pixel number.
		DWORD HunitInq :16; ///< Horizontal unit pixel number.
} HV_AUTO_FUNC_DESCRIPTOR_AOI;

/// Argument for the ORD_QUERY_AUTO_FUNC_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_AUTO_FUNC_DESCRIPTOR{
    HV_AUTO_FUNC_ID  AutoFuncId;
}HV_ARG_QUERY_AUTO_FUNC_DESCRIPTOR;

/// Result of the ORD_QUERY_AUTO_FUNC_DESCRIPTOR function
typedef HV_AUTO_FUNC_DESCRIPTOR HV_RES_QUERY_AUTO_FUNC_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
// ORD_GET_AUTO_FUNC
//////////////////////////////////////////////////////////////////////////
/// The DHCAM AUTO FUNC CSR value
typedef union tagHV_AUTO_FUNC{
	/*!
	Scalar field for the functions:
	AUTO_FUNC_ID_AUTO_STROBE_CTRL,
	AUTO_FUNC_ID_AUTO_STROBE_SHUTTER_CTRL,
	AUTO_FUNC_ID_AUTO_STROBE_GAIN_CTRL,	
	*/
	struct {
		DWORD OnValue             :12; ///< value
		DWORD OffValue            :12; ///< value
		DWORD AutoManualMode      : 1; ///< Reserved.	
		DWORD OnOff               : 1; ///< Availability of switching this feature on and off
		DWORD OnePush             : 1; ///< Reserved.
		DWORD                     : 3; ///< Reserved.
		DWORD AbsControl          : 1; ///< Reserved.
		DWORD PresenceInq         : 1; ///< Presence of this feature
	} AutoStrobeScalar;
	/*!
	StrobeStat field for AUTO_FUNC_ID_AUTO_STROBE_STAT_CTRL
	*/
	struct {
		DWORD Value         :12; ///< value
		DWORD               :12; ///< Reserved.	
		DWORD               : 1; ///< Reserved.	
		DWORD OnOff         : 1; ///< Availability of switching this feature on and off
		DWORD OnePush       : 1; ///< Reserved.
        DWORD               : 3; ///< Reserved.
		DWORD AbsControl    : 1; ///< Reserved.
		DWORD PresenceInq   : 1; ///< Presence of this feature
	} AutoStrobeStat;

	/*! AutoGainCtrl field for the AUTO_FUNC_ID_AUTO_GAIN_CTRL.*/
	struct {
		DWORD MinValue    :12;///< Minimum value of auto gain.
		DWORD             : 4;
		DWORD MaxValue    :12;///< Maximum value of auto gain.
		DWORD             : 3;
		DWORD PresenceInq : 1;///< Presence of this feature.
	} AutoGainCtrl;

    /*!
	Block field for the functions:
	AUTO_FUNC_ID_AUTO_SHUTTER_CTRL,
	AUTO_FUNC_ID_AUTO_FUNC_AOI
	*/
	struct{
		DWORD length;///< Size of a parameter buffer.
		DWORD *pBuff;///< Pointer to a parameter buffer.
	}Block;

}HV_AUTO_FUNC;

typedef	struct tagHV_AUTO_FUNC_SHUTTER_CTRL{
	DWORD MaxValue;         ///<Maximum value of auto shutter.
	DWORD MinValue;         ///<Minimum value of auto shutter.
	DWORD             :31;
	DWORD PresenceInq : 1; ///< Presence of this feature.
}HV_AUTO_FUNC_SHUTTER_CTRL;

typedef	struct tagHV_AUTO_FUNC_AOI{
	DWORD Height :16;
	DWORD Width  :16;

	DWORD Top    :16;
	DWORD Left   :16;

	DWORD              :24;
	DWORD OnOff        : 1; ///< Switch to turn this feature ON or OFF, 0:OFF 1:ON
	DWORD              : 1; 
	DWORD ShowWorkArea : 1; ///< Switch to Show work area ON or OFF, 0:OFF 1:ON
	DWORD              : 3; 
	DWORD PresenceInq  : 1; ///< Presence of this feature
} HV_AUTO_FUNC_AOI;


/// The auto func ITEM id for set
typedef enum tagHV_AUTO_FUNC_ITEM_ID{
    AUTO_FUNC_ITEM_ID_AUTO_GAIN_PRESENCE_INQ     = 0x000000,
	AUTO_FUNC_ITEM_ID_AUTO_GAIN_MAX_VALUE        = 0x000004,
	AUTO_FUNC_ITEM_ID_AUTO_GAIN_MIN_VALUE        = 0x000014,

	AUTO_FUNC_ITEM_ID_AUTO_SHUTTER_PRESENCE_INQ  = 0x010000,
	AUTO_FUNC_ITEM_ID_AUTO_SHUTTER_MIN_VALUE     = 0x020000,
	AUTO_FUNC_ITEM_ID_AUTO_SHUTTER_MAX_VALUE     = 0x030000,

	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_PRESENCE_INQ = 0x040000,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_SHOWWORKAREA = 0x040004,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_ON_OFF       = 0x040006,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_LEFT         = 0x050000,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_TOP          = 0x050010,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_WIDTH        = 0x060000,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_HEIGHT       = 0x060010,

	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_PRESENCE_INQ     = 0x070000,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_ABS_CONTROL      = 0x070001,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_ONE_PUSH         = 0x070005,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_ON_OFF           = 0x070006,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_AUTO_MANUAL_MODE = 0x070007,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_OFF_VALUE        = 0x070008,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_SCALAR_ON_VALUE         = 0x070014,

	AUTO_FUNC_ITEM_ID_AUTO_STROBE_STAT_PRESENCE_INQ = 0x080000,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_STAT_ABS_CONTROL  = 0x080001,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_STAT_ONE_PUSH     = 0x080005,
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_STAT_ON_OFF       = 0x080006,	
	AUTO_FUNC_ITEM_ID_AUTO_STROBE_STAT_VALUE        = 0x080014,

}HV_AUTO_FUNC_ITEM_ID;

/// Argument for the ORD_GET_AUTO_FUNC function
typedef struct tagHV_ARG_GET_AUTO_FUNC{
    HV_AUTO_FUNC_ID AutoFuncId;
}HV_ARG_GET_AUTO_FUNC;

/// Result of the ORD_GET_AUTO_FUNC function
typedef HV_AUTO_FUNC HV_RES_GET_AUTO_FUNC;

//////////////////////////////////////////////////////////////////////////
// ORD_SET_AUTO_FUNC
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_SET_AUTO_FUNC function
typedef struct tagHV_ARG_SET_AUTO_FUNC{
    HV_AUTO_FUNC_ID AutoFuncId;
    HV_AUTO_FUNC_ITEM_ID AutoFuncItemId;
    HV_AUTO_FUNC AutoFunc;
}HV_ARG_SET_AUTO_FUNC;

//////////////////////////////////////////////////////////////////////////
//ORD_QUERY_OSD_FUNC_PRESENCE
//////////////////////////////////////////////////////////////////////////
typedef struct tagHV_RES_QUERY_OSD_FUNC_PRESENCE {
    DWORD                              :25;
	DWORD UserUnitNumInq               : 1;
	DWORD UserSetPictureInq            : 1;
	DWORD FontTransparencyInq          : 1;
	DWORD FontBackgroundColorInq       : 1;
	DWORD FontColorInq                 : 1;	
	DWORD UintPositionInq              : 1;
	DWORD FontSizeInq                  : 1;
    DWORD :32;
	DWORD :32; 
    DWORD :32;
}HV_RES_QUERY_OSD_FUNC_PRESENCE;

//////////////////////////////////////////////////////////////////////////
//ORD_QUERY_OSD_FUNC_DESCRIPTOR
//////////////////////////////////////////////////////////////////////////

typedef enum tagHV_OSD_TYPE {
	HV_OSD_TYPE_GLOBAL      = 0,
	HV_OSD_TYPE_STATIC_TEXT = 1,
	HV_OSD_TYPE_DATE        = 2,
	HV_OSD_TYPE_TIME        = 3
}HV_OSD_TYPE;

typedef enum tagHV_OSD_FUNC_ID_GLOBAL {
	// Function ID for HV_OSD_TYPE_GLOBAL
	HV_OSD_FUNC_ID_GLOBAL_ENABLE                       = 0,///< Enable all OSD function
	HV_OSD_FUNC_ID_GLOBAL_SAVE                         = 1,///< Save all OSD function
	HV_OSD_FUNC_ID_GLOBAL_FONT_MODE                    = 2,///< Font size mode of all static text.
	HV_OSD_FUNC_ID_GLOBAL_FONT_COLOR                   = 3,///< Font Color of all static text.
	HV_OSD_FUNC_ID_GLOBAL_BACKGROUND_COLOR             = 4,///< Font background color of all static text.
	HV_OSD_FUNC_ID_GLOBAL_BACKGROUND_TRANSPARENCY      = 5,///< Font background transparency of all static text.
}HV_OSD_FUNC_ID_GLOBAL;

typedef enum tagHV_OSD_FUNC_ID_STATIC_TEXT{
	// Function ID for HV_OSD_TYPE_STATIC_TEXT
	HV_OSD_FUNC_ID_STATIC_TEXT_CLEAN                   = 0, ///< Clean all static text OSD information.
	HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_NUMBER             = 1, ///< Unit number of all static text.	
	HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_ENABLE             = 2, ///< Enable some specific static text unit.
	HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_POSITION           = 3, ///< Position of some specific static text unit.
	HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_WORD_NUMBER        = 4, ///< Word number of some specific static text unit.
	HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_DATA               = 5, ///< Data buffer of some specific static text unit.
	HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_DATA_SIZE          = 6  ///< Data Size of some specific static text unit.
}HV_OSD_FUNC_ID_STATIC_TEXT;

typedef enum tagHV_OSD_FUNC_ID_DATE {	
	// Function ID for HV_OSD_TYPE_GLOBAL_DATE
	HV_OSD_FUNC_ID_DATE_ENABLE                         = 0,///< Enable DATE OSD information.
	HV_OSD_FUNC_ID_DATE_POSITION                       = 1,///< Position of DATE OSD.
	HV_OSD_FUNC_ID_DATE_MODE                           = 2 ///< Mode of DATE OSD.
}HV_OSD_FUNC_ID_DATE;

typedef enum tagHV_OSD_FUNC_ID_TIME {
	// Function ID for HV_OSD_TYPE_GLOBAL_TIME
	HV_OSD_FUNC_ID_TIME_ENABLE                         = 0,///< Enable TIME OSD information.	
	HV_OSD_FUNC_ID_TIME_POSITION                       = 1,///< Position of TIME OSD.
	HV_OSD_FUNC_ID_TIME_MODE                           = 2 ///< Mode of TIME OSD.
}HV_OSD_FUNC_ID_TIME;


typedef struct tagHV_OSD_FUNC_ID {
	HV_OSD_TYPE OsdType;

	union {
		///<HV_OSD_TYPE_GLOBAL
		struct {
			HV_OSD_FUNC_ID_GLOBAL      FunID;
		}Global;
		
		///<HV_OSD_TYPE_STATIC_TEXT
		struct {
			int UnitID;
			HV_OSD_FUNC_ID_STATIC_TEXT FunID;
		}StaticText;
		
		///<HV_OSD_TYPE_DATE
		struct {
			HV_OSD_FUNC_ID_DATE        FunID;
		}Date;
		
		///<HV_OSD_TYPE_TIME
		struct {
			HV_OSD_FUNC_ID_TIME        FunID;		
		}Time;
	};	
}HV_OSD_FUNC_ID;

/// Argument For the ORD_QUERY_OSD_FUNC_DESCRIPTOR function
typedef union tagHV_ARG_QUERY_OSD_FUNC_DESCRIPTOR {	
	HV_OSD_FUNC_ID OsdFuncID;
}HV_ARG_QUERY_OSD_FUNC_DESCRIPTOR;

/// Result of the ORD_QUERY_OSD_FUNC_DESCRIPTOR function
typedef union tagHV_RES_QUERY_OSD_FUNC_DESCRIPTOR {
	union{
		// HV_OSD_FUNC_ID_GLOBAL_FONT_MODE
		struct {
          DWORD             :30;///< Reserved
		  DWORD Mode1Inq    : 1;///< 24*24
		  DWORD Mode0Inq    : 1;///< 16*16
		}FontMode;
		
		// HV_OSD_FUNC_ID_GLOBAL_BACKGROUND_TRANSPARENCY
		struct {
			DWORD MaxValue : 16;
			DWORD MinValue : 16;
		}BackgroundTransparency;

	}Global;
	union{	
		// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_NUMBER
		struct {
			DWORD MaxValue;
		}UnitNumber;

		// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_WORD_NUMBER
		struct {
			DWORD MinValue : 16;
			DWORD MaxValue : 16;			
		}UnitWordNumber;

	}StaticText;
	
	union{
		// HV_OSD_FUNC_ID_DATE	
		struct {
		DWORD          :30; ///< Reserved
		DWORD Mode1Inq : 1; ///< YYYY年MM月DD日
		DWORD Mode0Inq : 1; ///< YYYY-MM-DD
		}Mode;
	}Date;
	
	union {
		// HV_OSD_FUNC_ID_TIME
		struct {
		DWORD          :30; ///< Reserved
		DWORD Mode1Inq : 1; ///< HH时MM分SS秒
		DWORD Mode0Inq : 1; ///< HH:MM:SS
		}Mode;
	}Time;
}HV_RES_QUERY_OSD_FUNC_DESCRIPTOR;

//////////////////////////////////////////////////////////////////////////
//ORD_GET_OSD_FUNC
//////////////////////////////////////////////////////////////////////////

typedef enum tagHV_OSD_TIME_MODE_ID {
	HV_OSD_TIME_MODE_0 = 0x80000000,
	HV_OSD_TIME_MODE_1 = 0x40000000
}HV_OSD_TIME_MODE_ID;

typedef enum tagHV_OSD_DATE_MODE_ID {
	HV_OSD_DATE_MODE_0 = 0x80000000,
	HV_OSD_DATE_MODE_1 = 0x40000000
}HV_OSD_DATE_MODE_ID;

typedef enum tagHV_OSD_FONT_SIZE_MODE_ID {
	HV_OSD_FONT_SIZE_MODE0_16_16 = 0x80000000,
	HV_OSD_FONT_SIZE_MODE1_24_24 = 0x40000000
}HV_OSD_FONT_SIZE_MODE_ID;


/// Argument For the ORD_GET_OSD_FUNC function
typedef union tagHV_ARG_GET_OSD_FUNC {	
	HV_OSD_FUNC_ID OsdFuncID;	
}HV_ARG_GET_OSD_FUNC;

typedef union tagHV_OSD_INFO {	
	// HV_OSD_FUNC_ID_GLOBAL_ENABLE	
	// HV_OSD_FUNC_ID_GLOBAL_SAVE 
	// HV_OSD_FUNC_ID_STATIC_TEXT_CLEAN 
	// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_ENABLE
	// HV_OSD_FUNC_ID_DATE_ENABLE
	// HV_OSD_FUNC_ID_TIME_ENABLE
	struct {
		DWORD        : 31;///< Reserved
		DWORD OnOff  : 1; ///< 0:disable, 1: enable.
	}Enable;	
	
	// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_POSITION
	// HV_OSD_FUNC_ID_DATE_POSITION
	// HV_OSD_FUNC_ID_TIME_POSITION
	struct {	
		DWORD Top  : 16;
		DWORD Left : 16;
	}Position;

	// HV_OSD_FUNC_ID_GLOBAL_BACKGROUND_TRANSPARENCY
	// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_WORD_NUMBER	
	// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_DATA_SIZE
	struct{
		DWORD Value;
	}Scalar;

	// HV_OSD_FUNC_ID_GLOBAL_FONT_MODE
	// HV_OSD_FUNC_ID_DATE_MODE
	// HV_OSD_FUNC_ID_TIME_MODE
	struct {
		DWORD Mode;
	}Selector;

	// HV_OSD_FUNC_ID_GLOBAL_FONT_COLOR,
	// HV_OSD_FUNC_ID_GLOBAL_BACKGROUND_COLOR
	struct {		
		DWORD     : 8;
		DWORD Cr  : 8;
		DWORD Cb  : 8;
		DWORD Y   : 8;
	}Color;	

	// HV_OSD_FUNC_ID_STATIC_TEXT_UNIT_DATA
	struct {
		BYTE *pBuffer;///<Pointer to Data Buffer.
		int nLength;  ///< Size of Data Buffer,in bytes.
	} Data;

}HV_OSD_INFO;

/// Result of the ORD_GET_OSD_FUNC function
typedef HV_OSD_INFO  HV_RES_GET_OSD_FUNC;

//////////////////////////////////////////////////////////////////////////
//ORD_SET_OSD_FUNC
//////////////////////////////////////////////////////////////////////////
/// Argument For the ORD_SET_OSD_FUNC function
typedef struct tagHV_ARG_SET_OSD_FUNC {	
	HV_OSD_FUNC_ID OsdFuncID;
	HV_OSD_INFO    OsdInfo;
}HV_ARG_SET_OSD_FUNC;
/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif
