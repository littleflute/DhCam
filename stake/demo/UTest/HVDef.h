
//-----------------------------------------------------------------------------
//  Company:   Daheng Imavision 
//  Section:   Software Department
//  Project:   HVDef.h    
//  
//  $Archive:  
//  $Author:   Software Department
//  $Revision: 2.0.1.7$
//  $Date:     2008.11.28 Fri 16:00pm$
//-----------------------------------------------------------------------------
/**
*  \file     HVDef.h
*  \brief    The head file of the HVAPI interface.
*   The file defines the macros and structs of HVAPI interface
*
*   The reference documents:
*   1. IIDC 1394-based Digital Camera Specification Ver.1.31.pdf,Revision:V1.31
*   2. DHCAM_Spec.doc,Revision:V1.3.1 
*  
*/
//-----------------------------------------------------------------------------

#if !defined(HVDEF_H)         
#define HVDEF_H


/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif


//////////////////////////////////////////////////////////////////////////
/// 标识设备句柄
typedef HANDLE HHV;

//////////////////////////////////////////////////////////////////////////
/// 标识数字摄像机的设备类型
typedef enum tagHVTYPE{
	UNKNOWN_TYPE = -1,
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

	SV1410GMTYPE = 47,
	SV2000GMTYPE = 48,
	ITS1410FCTYPE = 49,

	PD1420EMTYPE = 106,
	PD1420ECTYPE = 107,
	PD2000EMTYPE = 108,
	PD2000ECTYPE = 109,

}HVTYPE;

//////////////////////////////////////////////////////////////////////////
/// the return status for the interface function
typedef enum tagHVSTATUS{
	STATUS_OK                           = 0,
	STATUS_NO_DEVICE_FOUND              = -1,
	STATUS_DEVICE_HANDLE_INVALID        = -2,
	STATUS_HW_DEVICE_TYPE_ERROR         = -3,
	STATUS_HW_INIT_ERROR                = -4,
	STATUS_HW_RESET_ERROR               = -5,
	STATUS_NOT_ENOUGH_SYSTEM_MEMORY     = -6,
	STATUS_HW_IO_ERROR                  = -7,
	STATUS_HW_IO_TIMEOUT                = -8,
	STATUS_HW_ACCESS_ERROR              = -9,
	
	STATUS_OPEN_DRIVER_FAILED           = -10,
	STATUS_NOT_SUPPORT_INTERFACE        = -11,
	STATUS_PARAMETER_INVALID            = -12,
	STATUS_PARAMETER_OUT_OF_BOUND       = -13,
	STATUS_IN_WORK                      = -14,
	STATUS_NOT_OPEN_SNAP                = -15,
	STATUS_NOT_START_SNAP               = -16,
	STATUS_FILE_CREATE_ERROR            = -17,
	STATUS_FILE_INVALID                 = -18,
	STATUS_NOT_START_SNAP_INT           = -19,
	STATUS_INTERNAL_ERROR               = -20
}HVSTATUS;

//////////////////////////////////////////////////////////////////////////
/// The macros for the status
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


//////////////////////////////////////////////////////////////////////////
/// Function ordinal numbers, used to construct driver control codes 
typedef enum tagHV_CONTROL_CODE{
	//-----------BASIC FUNCTION ID-----------------------------------------------
	ORD_INIT_CAMERA                                         = 0x00000000,

	ORD_QUERY_STRING_PRESENCE                               = 0x00000004,
	ORD_QUERY_STRING_DESCRIPTOR                             = 0x00000005,
	ORD_GET_STRING                                          = 0x00000006,
	
	ORD_QUERY_RESET_CAMERA_PRESENCE                         = 0x00000008,
	ORD_RESET_CAMERA                                        = 0x0000000B,
	
	ORD_QUERY_LAST_STATUS_PRESENCE                          = 0x0000000C,
	ORD_GET_LAST_STATUS                                     = 0x0000000E,

	ORD_QUERY_CYCLE_PORT_PRESENCE        					= 0x00000010,
	ORD_CYCLE_PORT											= 0x00000013,
	

	//-----------INTERVAL FUNCTION ID--------------------------------------------
	// Used codes from 0x00000100 to 0x00000103
	// Used codes from 0x00000200 to 0x00000204

	//-----------DCAM ID---------------------------------------------------------
	ORD_QUERY_VIDEO_FORMAT_PRESENCE                         = 0x00001000,
	ORD_QUERY_VIDEO_MODE_PRESENCE                           = 0x00001004,
	
	ORD_QUERY_FRAME_RATE_PRESENCE                           = 0x00001008,
	ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE                    = 0x0000100A,
	ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE                    = 0x0000100B, 
	
	ORD_QUERY_FMT7_MODE_DESCRIPTOR                          = 0x0000100D,
	ORD_GET_FMT7_MODE                                       = 0x0000100E,
	ORD_SET_FMT7_MODE                                       = 0x0000100F,
	
	ORD_QUERY_FEATURE_PRESENCE                              = 0x00001010,  
	ORD_QUERY_FEATURE_DESCRIPTOR                            = 0x00001011,  
	ORD_GET_FEATURE                                         = 0x00001012,  
	ORD_SET_FEATURE                                         = 0x00001013,
	//ORD_GET_FEATURE_ABS_VALUE                               = 0x00001016,
	//ORD_GET_FEATURE_ABS_RANGE                               = 0x0000101A,
	ORD_QUERY_BASIC_FUNC_PRESENCE                           = 0x0000101C,
	ORD_QUERY_BASIC_FUNC_DESCRIPTOR                         = 0x0000101D,
	ORD_GET_BASIC_FUNC                                      = 0x0000101E,
	ORD_SET_BASIC_FUNC                                      = 0x0000101F,	

	ORD_QUERY_OPT_FUNC_PRESENCE                             = 0x00001020,
	
	ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE                      = 0x00001024,
	ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR                    = 0x00001025,
	ORD_GET_PIO_OUTPUT_PORT                                 = 0x00001026,
	ORD_SET_PIO_OUTPUT_PORT                                 = 0x00001027,
	
	//ORD_QUERY_SIO_PRESENCE                                  = 0x00001028,
	ORD_QUERY_SIO_DESCRIPTOR                                = 0x00001029,
	ORD_GET_SIO                                             = 0x0000102A,
	ORD_SET_SIO                                             = 0x0000102B,

	ORD_QUERY_STROBE_PRESENCE                               = 0x0000102C,
	ORD_QUERY_STROBE_DESCRIPTOR                             = 0x0000102D,
	ORD_GET_STROBE                                          = 0x0000102E,
	ORD_SET_STROBE                                          = 0x0000102F,
	//-----------DH DEFINE ID---------------------------------------------------
	//-----------DH SNAP IMAGE ID----------------------------------------
	ORD_OPEN_SNAP                                           = 0x00002000,
	ORD_CLOSE_SNAP                                          = 0x00002001,
	ORD_START_SNAP                                          = 0x00002002,
	ORD_STOP_SNAP                                           = 0x00002003,
	ORD_SNAP_SHOT                                           = 0x00002004,
	ORD_TRIGGER_SHOT                                        = 0x00002005,


	ORD_CMD_RESERVED0                                       = 0x00002100, 
	ORD_CMD_RESERVED1                                       = 0x00002101, 
	ORD_CMD_RESERVED2                                       = 0x00002102, 
    
	//-----------DHCAM ID------------------------------------------------
	ORD_QUERY_ADV_FEATURE_PRESENCE                          = 0x00003000,
	ORD_QUERY_ADV_FEATURE_DESCRIPTOR                        = 0x00003001,  
	ORD_GET_ADV_FEATURE                                     = 0x00003002, 
	ORD_SET_ADV_FEATURE                                     = 0x00003003,
	
	ORD_QUERY_ADV_BASIC_FUNC_PRESENCE                       = 0x00003004,
	//ORD_QUERY_ADV_BASIC_FUNC_DESCRIPTOR                     = 0x00003005,
	ORD_GET_ADV_BASIC_FUNC                                  = 0x00003006,
	ORD_SET_ADV_BASIC_FUNC                                  = 0x00003007,

	ORD_QUERY_ADV_OPT_FUNC_PRESENCE                         = 0x00003008,
	//ORD_QUERY_ADV_OPT_FUNC_DESCRIPTOR                       = 0x00003009,
	//ORD_GET_ADV_OPT_FUNC                                    = 0x0000300A,
	//ORD_SET_ADV_OPT_FUNC                                    = 0x0000300B,
	
	ORD_QUERY_ADV_LUT_DESCRIPTOR                            = 0x0000300D,
	ORD_GET_ADV_LUT                                         = 0x0000300E,
	ORD_SET_ADV_LUT                                         = 0x0000300F,
	
	ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR                    = 0x00003011,
	ORD_GET_ADV_USER_CUSTOM                                 = 0x00003012,
	ORD_SET_ADV_USER_CUSTOM                                 = 0x00003013,

	ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE                = 0x00003014,
	ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR              = 0x00003015,
	ORD_GET_ADV_USER_ENCRYPT_ZONE                           = 0x00003016,
	ORD_SET_ADV_USER_ENCRYPT_ZONE                           = 0x00003017,
	
	//-----------NET FUNCTION ID----------------------------------------
	ORD_QUERY_NET_FUNC_PRESENCE                             = 0x00004000,
	ORD_QUERY_NET_FUNC_DESCRIPTOR                           = 0x00004001,
	ORD_GET_NET_FUNC                                        = 0x00004002,
	ORD_SET_NET_FUNC                                        = 0x00004003,

	//-----------IMAGE ID-----------------------------------------------
	ORD_QUERY_IMAGE_INFO_PRESENCE                           = 0x00005000,
	ORD_QUERY_IMAGE_INFO_DESCRIPTOR                         = 0x00005001,
	ORD_GET_IMAGE_INFO                                      = 0x00005002,
	
	//-----------AUTO FUNCTION ID---------------------------------------
	ORD_QUERY_AUTO_FUNC_PRESENCE                            = 0x00006000,
	ORD_QUERY_AUTO_FUNC_DESCRIPTOR                          = 0x00006001,
	ORD_GET_AUTO_FUNC                                       = 0x00006002,
	ORD_SET_AUTO_FUNC                                       = 0x00006003,

}HV_CONTROL_CODE;



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//          The struct defined for the previously version               //
//                        of the HVDailt.dll                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/// 标识图像分辨率的类型
typedef enum tagHV_RESOLUTION{ 
	RES_MODE0       = 0,
	RES_MODE1       = 1,
	RES_MODE2       = 2,
	RES_MODE3       = 3	
}HV_RESOLUTION;

//////////////////////////////////////////////////////////////////////////
/// 标识图像采集模式
typedef enum tagHV_SNAP_MODE{
	CONTINUATION    = 0,
	TRIGGER         = 1,
	TRIGGER_EDGE    = 2,
	TRIGGER_LEVEL   = 3
}HV_SNAP_MODE;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机为外触发采集模式时，外触发信号的极性
typedef enum tagHV_POLARITY{ 
	LOW    = 0,
	HIGH   = 1
}HV_POLARITY;

//////////////////////////////////////////////////////////////////////////
/// 标识图像采集速度
typedef enum tagHV_SNAP_SPEED{ 
	NORMAL_SPEED    = 0,
	HIGH_SPEED      = 1		
}HV_SNAP_SPEED;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机的颜色通道 
typedef enum tagHV_CHANNEL{ 
	RED_CHANNEL         = 0x10,
	GREEN_CHANNEL1      = 0x11,
	GREEN_CHANNEL2      = 0x12,
	BLUE_CHANNEL        = 0x13	
}HV_CHANNEL;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机增益控制方式
typedef enum tagHV_AGC_CONTROL{ 
	AGC_GAIN_RED_CHANNEL        = 0x10,
	AGC_GAIN_GREEN_CHANNEL1     = 0x11,
	AGC_GAIN_GREEN_CHANNEL2     = 0x12,
	AGC_GAIN_BLUE_CHANNEL       = 0x13,
	AGC_GAIN                    = 0x2
} HV_AGC_CONTROL;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机的模拟/数字转换控制方式
typedef enum tagHV_ADC_CONTROL{ 
	ADC_BITS                        = 0,
	ADC_BLKLEVEL_CAL                = 1,
	ADC_BLKLEVEL_CAL_REDCHANNEL     = 0x10,
	ADC_BLKLEVEL_CAL_GREENCHANNEL1  = 0x11,
	ADC_BLKLEVEL_CAL_GREENCHANNEL2  = 0x12,
	ADC_BLKLEVEL_CAL_BLUECHANNEL    = 0x13,
	ADC_BRIGHTNESS                  = 0x2	
} HV_ADC_CONTROL;

//////////////////////////////////////////////////////////////////////////
/// 标识模拟/数字转换级别
typedef enum tagHV_ADC_LEVEL{ 
	ADC_LEVEL0      = 0,
	ADC_LEVEL1      = 1,
	ADC_LEVEL2      = 2,
	ADC_LEVEL3      = 3		
}HV_ADC_LEVEL;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机曝光控制方式
typedef enum tagHV_AEC_CONTROL { 
	AEC_EXPOSURE_TIME       = 1,
	AEC_SHUTTER_SPEED       = 2,
	AEC_SHUTTER_UNIT        = 3	
} HV_AEC_CONTROL;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机快门速度的单位
typedef enum tagSHUTTER_UNIT{ 
    SHUTTER_US      = 0,
    SHUTTER_MS      = 1,
    SHUTTER_S       = 2
}SHUTTER_UNIT_VALUE;

//////////////////////////////////////////////////////////////////////////
/// 
typedef struct tagHV_AEC_CONTROL_CONTEXT{
	HV_AEC_CONTROL ctrl;
}HV_AEC_CONTROL_CONTEXT,PHV_AEC_CONTROL_CONTEXT;

//////////////////////////////////////////////////////////////////////////
/// 标识图像采集的镜像功能
typedef enum tagHV_MIRROR_DIRECTION{ 
	VERT_DIR      = 1	
}HV_MIRROR_DIRECTION; 

//////////////////////////////////////////////////////////////////////////
/// The DeviceInfo ID
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
	DESC_DEVICE_BAYER_LAYOUT     = 11,
} HV_DEVICE_INFO;

//////////////////////////////////////////////////////////////////////////
/// 
typedef enum tagHV_PERIDEV_CONTROL{ 
	PERIDEV_BICOLOR_LAMP1 = 0,
	PERIDEV_BICOLOR_LAMP2 =	1,
	PERIDEV_IR_EMITTER	  =	2,
	PERIDEV_LED           =	3,
	PERIDEV_ARRIVE_CLEAR  =	4
}HV_PERIDEV_CONTROL;

//////////////////////////////////////////////////////////////////////////
/// 标识摄像机的控制功能
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
	CMD_HVAPI_CONTROL           = 0x100,
}HV_COMMAND_CODE;

//////////////////////////////////////////////////////////////////////////
/// 该结构包含设置包长大小的参数
typedef struct tagHV_CMD_PACKET_SIZE{
	DWORD dwSize;
}HV_CMD_PACKET_SIZE;

//////////////////////////////////////////////////////////////////////////
/// 该结构包含访问查找表数据的参数
typedef struct tagHV_CMD_LUT_DWENTRIES{
	DWORD dwOffset;
	DWORD *pBuffer;
	DWORD dwEntries;
}HV_CMD_LUT_DWENTRIES;

//////////////////////////////////////////////////////////////////////////
/// 该结构包含执行采集图像到内存的操作中，摄像机的工作状态
typedef struct tagHV_SNAP_INFO{
	HHV	hhv;
	int nDevice;
	int nIndex;
	void *pParam;
}HV_SNAP_INFO;

//////////////////////////////////////////////////////////////////////////
/// 标示回调函数的声明, 
typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO *pInfo);

//////////////////////////////////////////////////////////////////////////
/// 结构包含给定的摄像机查验参数
typedef struct tagHV_CHECK_PARAM{	
	BYTE byParam1;
	BYTE byParam2;
	BYTE byParam3;
	BYTE byParam4;
}HV_CHECK_PARAM;

//////////////////////////////////////////////////////////////////////////
/// 标识功能ID 
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
	LE_BE_MODE              = 22,
}HV_INTERFACE1_ID;

//////////////////////////////////////////////////////////////////////////
/// SV1311FC/M 的一些功能及SV1310FC/M、SV1300FC/M的闪光灯开关功能均可通过此结构设置。
typedef struct tagHV_INTERFACE1_CONTEXT{	
	HV_INTERFACE1_ID	ID;    
	DWORD				dwVal;	
}HV_INTERFACE1_CONTEXT;

//////////////////////////////////////////////////////////////////////////
/// 图像颜色格式
typedef enum tagCOLOR_CODE{ 
    MONO8    = 0,
	YUV8_411 = 1,
	YUV8_422 = 2,
	YUV8_444 = 3,
	RGB8     = 4,
	MONO16   = 5,
	RGB16    = 6
}COLOR_CODE;

//////////////////////////////////////////////////////////////////////////
/// The struct for the CMD_HVAPI_CONTROL function
typedef struct tagHVAPI_CONTROL_PARAMETER {
	HV_CONTROL_CODE code;
	void            *pInBuf;
	DWORD           dwInBufSize;
	void            *pOutBuf;	       
	DWORD           dwOutBufSize;   
	DWORD           *pBytesRet;   ///< 返回字节数，可以为NULL
}HVAPI_CONTROL_PARAMETER;




//////////////////////////////////////////////////////////////////////////
//                                                                      //
//            The basic functions of the camera                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/// Device interface for opening camera
typedef struct tagHV_DEVICE_INTERFACE{
	DWORD dwSize;
	char  szDevicePath[128];    ///< Device path
	GUID  ClassGUID;            ///< Device class guid
    DWORD Reserved;
}HV_DEVICE_INTERFACE;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_STRING_PRESENCE function
typedef struct tagHV_RES_QUERY_STRING_PRESENCE{
	DWORD                                 :21;  ///< Reserved
	DWORD FPGAVersion                     : 1;  ///< Presence FPGAVersion string, not supported is 0,supported is 1;
	DWORD FirwareVersion                  : 1;  ///< Presence FirwareVersion string, not supported is 0,supported is 1; 
	DWORD SysDriverVersion                : 1;  ///< Presence SysDriverVersion string, not supported is 0,supported is 1;
	DWORD DeviceDriverVersion             : 1;  ///< Presence DeviceDriverVersion string, not supported is 0,supported is 1;
	DWORD HVAPIVersion                    : 1;  ///< Presence HVAPIVersion string, not supported is 0,supported is 1; 
	DWORD HVDailtVersion                  : 1;  ///< Presence HVDailtVersion string, not supported is 0,supported is 1;
	DWORD DeviceName                      : 1;  ///< Presence DeviceName string, not supported is 0,supported is 1;
	DWORD DeviceChipId                    : 1;  ///< Presence Device Chip ID string, not supported is 0,supported is 1;
	DWORD DeviceSN                        : 1;  ///< Presence Device Serial number string, not supported is 0,supported is 1;
	DWORD VendorName                      : 1;  ///< Presence VendorName string, not supported is 0,supported is 1;
	DWORD DeviceType                      : 1;  ///< Presence DeviceType string, not supported is 0,supported is 1;	
}HV_RES_QUERY_STRING_PRESENCE;

/// HV String ID
typedef enum tagHV_STRING_ID{
	STRING_ID_DEVICE_TYPE              = 0,
	STRING_ID_VENDOR_NAME              = 1,
	STRING_ID_DEVICE_SERIESNUM         = 2,
	STRING_ID_DEVICE_CHIPID            = 3,
	STRING_ID_DEVICE_NAME              = 4,
	STRING_ID_HVDAILT_VERSION          = 5,
	STRING_ID_HVAPI_VERSION            = 6,
	STRING_ID_DEVICE_DRIVER_VERSION    = 7,
	STRING_ID_SYS_DRIVER_VERSION       = 8,
	STRING_ID_FIRMWARE_VERSION         = 9,
	STRING_ID_FPGA_VERSION             = 10,
}HV_STRING_ID;

/// Argument for the ORD_QUERY_STRING_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_STRING_DESCRIPTOR{
	HV_STRING_ID StringId;
}HV_ARG_QUERY_STRING_DESCRIPTOR;

/// Result of the ORD_QUERY_STRING_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_STRING_DESCRIPTOR{
    DWORD dwTotalByteLength;	            ///< 该字符串的总字节长度，不包含结尾符'\0'。	 
}HV_RES_QUERY_STRING_DESCRIPTOR;

/// Argument for the ORD_GET_STRING function
typedef struct tagHV_ARG_GET_STRING{
	HV_STRING_ID StringId;
	int nLength;                        ///< 要获得的字符串缓冲区长度，不包含结尾符'\0'
} HV_ARG_GET_STRING;

/// Result of the ORD_GET_STIRNG function
typedef struct tagHV_RES_GET_STRING {
    char *pBuffer;
}HV_RES_GET_STRING;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_RESET_CAMERA_PRESENCE function
typedef struct tagHV_RES_QUERY_RESET_CAMERA_PRESENCE{
	DWORD                 :31;  ///< Reserved
	DWORD ResetCamera     : 1;  ///< Presence ResetCamera function, not supported is 0,supported is 1; 
}HV_RES_QUERY_RESET_CAMERA_PRESENCE;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_LAST_STATUS_PRESENCE function
typedef struct tagHV_RES_QUERY_LAST_STATUS_PRESENCE{
	DWORD                 :31;  ///< Reserved
	DWORD LastStatus      : 1;  ///< Presence last status function, not supported is 0,supported is 1; 
}HV_RES_QUERY_LAST_STATUS_PRESENCE;

/// Return of the ORD_GET_LAST_STATUS
typedef struct tagHV_RES_GET_LAST_STATUS{
	DWORD dwParameter;
}HV_RES_GET_LAST_STATUS;

//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_CYCLE_PORT_PRESENCE function
typedef struct tagHV_RES_QUERY_CYCLE_PORT_PRESENCE{
	DWORD                 :31;  ///< Reserved
	DWORD CyclePort       : 1;  ///< Presence last status function, not supported is 0,supported is 1; 
}HV_RES_QUERY_CYCLE_PORT_PRESENCE;


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                   DCAM Standard Functions                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_VIDEO_FORMAT_PRESENCE function
typedef struct tagHV_RES_QUERY_VIDEO_FORMAT_PRESENCE{
	DWORD                   :24; ///< [8..31]Reserved;
	DWORD Format7           : 1; ///< [7]Format_7:Partial Image Size Format,supported is 1,not supported is 0;
	DWORD Format6           : 1; ///< [6]Format_6:Still Image Format,supported is 1,not supported is 0;
	DWORD                   : 3; ///< [3..5]Reserved; 
	DWORD Format2           : 1; ///< [2]Format_2:Super VGA non-compressed format (2),supported is 1,not supported is 0;
	DWORD Format1           : 1; ///< [1]Format_1:Super VGA non-compressed format (1),supported is 1,not supported is 0;
	DWORD Format0           : 1; ///< [0],Format_0:VGA non-compressed format(Maximum 640x480),supported is 1,not supported is 0;
}HV_RES_QUERY_VIDEO_FORMAT_PRESENCE;

/// Video formats ID 
typedef enum tagHV_VIDEO_FORMAT{
	VIDEO_FORMAT0          =  0,  ///< Format_1 id;
	VIDEO_FORMAT1          =  1,  ///< Format_2 id;
	VIDEO_FORMAT2          =  2,  ///< Format_3 id;
	VIDEO_FORMAT6          =  6,  ///< Format_6 id;
	VIDEO_FORMAT7          =  7,  ///< Format_7 id;
}HV_VIDEO_FORMAT;



//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_QUERY_VIDEO_MODE_PRESENCE function
typedef struct tagHV_ARG_QUERY_VIDEO_MODE_PRESENCE{
	HV_VIDEO_FORMAT VideoFormat;    ///< Specifies the video format;
}HV_ARG_QUERY_VIDEO_MODE_PRESENCE;

/// Result of the ORD_QUERY_VIDEO_MODE_PRESENCE function
typedef struct tagHV_RES_QUERY_VIDEO_MODE_PRESENCE{
	DWORD                   :25;   ///< [7..31]Reserved
	DWORD Mode6             : 1;   ///< [6]Format_x,Mode_6:supported is 1,not supported is 0;
	DWORD Mode5             : 1;   ///< [5]Format_x,Mode_5:supported is 1,not supported is 0;
	DWORD Mode4             : 1;   ///< [4]Format_x,Mode_4:supported is 1,not supported is 0;
	DWORD Mode3             : 1;   ///< [3]Format_x,Mode_3:supported is 1,not supported is 0;
	DWORD Mode2             : 1;   ///< [2]Format_x,Mode_2:supported is 1,not supported is 0;
	DWORD Mode1             : 1;   ///< [1]Format_x,Mode_1:supported is 1,not supported is 0;
	DWORD Mode0             : 1;   ///< [0]Format_x,Mode_0:supported is 1,not supported is 0;
}HV_RES_QUERY_VIDEO_MODE_PRESENCE;

/// Video modes ID 
typedef enum tagHV_VIDEO_MODE{
	VIDEO_MODE_IGNORE    = -1,  ///< Ignored the VideoMode
	VIDEO_MODE0          = 0,   ///< Mode_0 ID
	VIDEO_MODE1          = 1,
	VIDEO_MODE2          = 2,
	VIDEO_MODE3          = 3,
	VIDEO_MODE4          = 4,
	VIDEO_MODE5          = 5,
	VIDEO_MODE6          = 6,
}HV_VIDEO_MODE;



//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_QUERY_FRAME_RATE_PRESENCE function
typedef struct tagHV_ARG_QUERY_FRAME_RATE_PRESENCE{
	HV_VIDEO_FORMAT VideoFormat;      ///< Specifies the video format;
	HV_VIDEO_MODE   VideoMode;        ///< Specifies the video mode;
}HV_ARG_QUERY_FRAME_RATE_PRESENCE;

/// Result of the ORD_QUERY_FRAME_RATE_PRESENCE function
typedef struct tagHV_RES_QUERY_FRAME_RATE_PRESENCE{
	DWORD                        :24;  ///< [8..31]Reserved
	DWORD FrameRate7             : 1;  ///< [7]FrameRate_8 : 240fps, not supported is 0,supported is 1;
	DWORD FrameRate6             : 1;  ///< [6]FrameRate_7 : 120fps, not supported is 0,supported is 1;
	DWORD FrameRate5             : 1;  ///< [5]FrameRate_6 : 60fps, not supported is 0,supported is 1;
	DWORD FrameRate4             : 1;  ///< [4]FrameRate_5 : 30fps, not supported is 0,supported is 1;
	DWORD FrameRate3             : 1;  ///< [3]FrameRate_4 : 15fps, not supported is 0,supported is 1;
	DWORD FrameRate2             : 1;  ///< [2]FrameRate_3 : 7.5fps, not supported is 0,supported is 1;
	DWORD FrameRate1             : 1;  ///< [1]FrameRate_1 : 3.75fps, not supported is 0,supported is 1;
	DWORD FrameRate0             : 1;  ///< [0]FrameRate_0 : 1.875fps, not supported is 0,supported is 1;	
}HV_RES_QUERY_FRAME_RATE_PRESENCE;

/// Frame rates ID 
typedef enum tagHV_FRAME_RATE{
	FRAME_RATE_IGNORE          = -1,  ///< Ignored the FrameRate
	FRAME_RATE_1_875FPS        =  0,  ///< FrameRate_0 1.875fps 
	FRAME_RATE_3_75FPS         =  1,  ///< FrameRate_1 3.75fps
	FRAME_RATE_7_5FPS          =  2,  ///< FrameRate_2 7.5fps
	FRAME_RATE_15FPS           =  3,  ///< FrameRate_3 15fps
	FRAME_RATE_30FPS           =  4,  ///< FrameRate_4 30fps
	FRAME_RATE_60FPS           =  5,  ///< FrameRate_5 60fps
	FRAME_RATE_120FPS          =  6,  ///< FrameRate_6 120fps
	FRAME_RATE_240FPS          =  7,  ///< FrameRate_7 240fps 	
}HV_FRAME_RATE;

/// Result of the ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE function
typedef struct tagHV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE{
	HV_VIDEO_FORMAT VideoFormat;      ///< Specifies the video format;
	HV_VIDEO_MODE   VideoMode;        ///< Specifies the video mode;
	HV_FRAME_RATE   FrameRate;
}HV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE;

/// Argument for the ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE function
typedef struct tagHV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE{
	HV_VIDEO_FORMAT VideoFormat;      ///< Specifies the video format;
	HV_VIDEO_MODE   VideoMode;        ///< Specifies the video mode;
	HV_FRAME_RATE   FrameRate;        ///< Specifies the frame rate 
}HV_ARG_SET_VIDEO_FORMAT_MODE_FRAME_RATE;



//////////////////////////////////////////////////////////////////////////
/// The Format7 mode_x sub function id
typedef enum tagHV_FMT7_MODE_FUNC_ID{
	FMT7_MODE_FUNC_ID_AOI                 = 0,
	FMT7_MODE_FUNC_ID_COLOR_CODING        = 1,
	FMT7_MODE_FUNC_ID_BAND_WIDTH          = 2,
	FMT7_MODE_FUNC_ID_DATA_DEPTH          = 3,
	FMT7_MODE_FUNC_ID_COLOR_FILTER        = 4,
}HV_FMT7_MODE_FUNC_ID;

/// Color Filter id
typedef enum tagHV_COLOR_FILTER{
	COLOR_FILTER_RG_GB                  = 0,     ///< RGB primary color filter (RG/GB)
	COLOR_FILTER_GB_RG                  = 1,     ///< RGB primary color filter (GB/RG)
	COLOR_FILTER_GR_BG                  = 2,     ///< RGB primary color filter (GR/BG)
	COLOR_FILTER_BG_GR                  = 3,     ///< RGB primary color filter (BG/GR)
}HV_COLOR_FILTER;

/// A union describing the image in format 7 
typedef union tagHV_FMT7_MODE_DESCRIPTOR{
	struct {
		DWORD VerticalMaxInq                :16;  ///< 000h[16..31],Maximum Vertical pixel number
		DWORD HorizontalMaxInq              :16;  ///< 000h[0..15],Maximum Horizontal pixel number
		DWORD VerticalUnitInq               :16;  ///< 004h[16..31],Vertical unit pixel number
		DWORD HorizontalUnitInq             :16;  ///< 004h[0..15],Horizontal unit pixel number 
	} AOI;    ///< The AOI descriptor struct

	struct {
		/* DCAM standard color coding */
		DWORD                          :21;  ///< [11..31]Reserved
		DWORD Raw16Inq                 : 1;  ///< [10]Inquiry for color coding (Raw16), not supported is 0,supported is 1;
		DWORD Raw8Inq                  : 1;  ///< [9]Inquiry for color coding (Raw8), not supported is 0,supported is 1;
		DWORD SingnedRGB16Inq          : 1;  ///< [8]Inquiry for color coding (Singed RGB16,R=G=B=16bits, non compressed(signed integer)), not supported is 0,supported is 1;
		DWORD SignedMono16Inq          : 1;  ///< [7]Inquiry for color coding (Singed Mono16,Y only, Y=16bits, non compressed(signed integer)), not supported is 0,supported is 1;
		DWORD RGB16Inq                 : 1;  ///< [6]Inquiry for color coding (RGB16), not supported is 0,supported is 1;
		DWORD Mono16Inq                : 1;  ///< [5]Inquiry for color coding (Mono16，Y only, Y=16bits, non compressed(unsigned integer)), not supported is 0,supported is 1;
		DWORD RGB8Inq                  : 1;  ///< [4]Inquiry for color coding (RGB8), not supported is 0,supported is 1;
		DWORD YUV8_4_4_4Inq            : 1;  ///< [3]Inquiry for color coding (4:4:4, Y=U=V= 8bits, non compressed), not supported is 0,supported is 1;
		DWORD YUV8_4_2_2Inq            : 1;  ///< [2]Inquiry for color coding (4:2:2, Y=U=V= 8bits, non compressed), not supported is 0,supported is 1; 
		DWORD YUV8_4_1_1Inq            : 1;  ///< [1]Inquiry for color coding (4:1:1, Y=U=V= 8bits, non compressed), not supported is 0,supported is 1;
		DWORD Mono8Inq                 : 1;  ///< [0]Inquiry for color coding (Mono8), not supported is 0,supported is 1;
		                                     ///< offset=014h
		DWORD                          :32;  ///< 018,Reserved for other Color_Coding.
		DWORD                          :32;  ///< 01C,Reserved for other Color_Coding.
		DWORD                          :32;  ///< 020,Reserved for other Color_Coding.
		/* Vendor Unique Color_Coding 0-127 */
		DWORD                          :20;  ///< [12..31]Reserved
		DWORD Raw12PackedInq           : 1;  ///< [11]Inquiry for color coding (Raw12_Packed), not supported is 0,supported is 1;
		DWORD Raw12NormalizedInq       : 1;  ///< [10]Inquiry for color coding (Raw12_Normalized), not supported is 0,supported is 1;
		DWORD Raw12Inq                 : 1;  ///< [9]Inquiry for color coding (Raw12), not supported is 0,supported is 1;
		DWORD Mono12PackedInq          : 1;  ///< [8]Inquiry for color coding (Mono12_Packed), not supported is 0,supported is 1;
		DWORD Mono12NormalizedInq      : 1;  ///< [7]Inquiry for color coding (Mono12_Normalized), not supported is 0,supported is 1; 
		DWORD Mono12Inq                : 1;  ///< [6]Inquiry for color coding (Mono12), not supported is 0,supported is 1; 
		DWORD Raw10PackedInq           : 1;  ///< [5]Inquiry for color coding (Raw10_Packed), not supported is 0,supported is 1; 
		DWORD Raw10NormalizedInq       : 1;  ///< [4]Inquiry for color coding (Raw10_Normalized), not supported is 0,supported is 1; 
		DWORD Raw10Inq                 : 1;  ///< [3]Inquiry for color coding (Raw10), not supported is 0,supported is 1; 
		DWORD Mono10PackedInq          : 1;  ///< [2]Inquiry for color coding (Mono10_Packed), not supported is 0,supported is 1; 
		DWORD Mono10NormalizedInq      : 1;  ///< [1]Inquiry for color coding (Mono10_Normalized), not supported is 0,supported is 1; 
		DWORD Mono10Inq                : 1;  ///< [0]Inquiry for color coding (Mono10), not supported is 0,supported is 1; 
                                             ///< offset=024h
		DWORD                          :32;  ///< 028h,Reserved for other Color_Coding.
		DWORD                          :32;  ///< 02Ch,Reserved for other Color_Coding.	
		DWORD                          :32;  ///< 030h,Reserved for other Color_Coding.
	} ColorMode;

	struct {
		DWORD dwPixelPerFrameInq;            ///< 034h,Inquiry for Pixel number per frame ,DCAM PIXEL_NUMBER_INQ
		DWORD dwBytePerFrameHiInq;           ///< 038h,Inquiry for Higher quadlet of total bytes of image data per frame,DCAM TOTAL_BYTES_HI_INQ
		DWORD dwBytePerFrameLoInq;           ///< 03Ch,Inquiry for Lower quadlet of total bytes of image data per frame,DCAM TOTAL_BYTES_LO_INQ
		
		DWORD MaxBytePerPacketInq      :16;  ///< 040h[16..31],Inquiry for Unit (Minimum) bytes per packet,Multiple by 4
		DWORD UnitBytePerPacketInq     :16;  ///< 040h[0..15],Inquiry for Maximum bytes per packet,Multiple by UnitBytePerPacket
		
		DWORD RecBytePerpacketInq      :16;  ///< 044h[16..31]Inquiry for Recommended bytes per packet. If this value is zero, shall ignore this field.
		DWORD BytePerPacketInq         :16;  ///< 044h,[0..15]Inquiry for Packet size
		
		DWORD PacketPerFrameInq;             ///< 048h,Inquiry for Number of Packets per frame.If this value is zero,the host shall calculate PacketPerFrame itself.

		DWORD VposunitInq              :16;  ///< 04Ch[16..31]，Inquiry for Vertical unit number for position ,If read value of Vposunit is 0, Vposunit = Vunit for compatibility.
		DWORD HposunitInq              :16;  ///< 04Ch[0..15],Inquiry for Horizontal unit pixel number for position 
		DWORD dwFrameIntervalInq;            ///< 050h,Inquiry for Current Frame interval (sec)(IEEE/REAL*4 Floating-Point Value)If read value of Frame_Interval is zero, shall ignore this field.
	} BandWidth;

	struct {
		DWORD                          :24;  ///< 054h[8..31],Reserved
		DWORD ValueInq                 :8 ;  ///< 054h[0..7],Effective image data depth of this mode,If read value of Data_Depth is zero, shall ignore this field. 
	} DataDepth;
	
	struct {
		HV_COLOR_FILTER  Filter;             ///< 058h,Specifies the ColorFilter
	} ColorFilterMode; 
}HV_FMT7_MODE_DESCRIPTOR;

/// Argument for the ORD_QUERY_FMT7_MODE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_FMT7_MODE_DESCRIPTOR{
	HV_VIDEO_MODE             VideoMode;           ///< Specifies the video mode
	HV_FMT7_MODE_FUNC_ID      FuncId;              ///< Specifies the format7 mode_x function id	
}HV_ARG_QUERY_FMT7_MODE_DESCRIPTOR;

/// Result of the ORD_QUERY_FMT7_MODE_DESCRIPTOR function
typedef HV_FMT7_MODE_DESCRIPTOR  HV_RES_QUERY_FMT7_MODE_DESCRIPTOR; 

/// These are the color codings, as defined by the Digital Camera Specification. 
typedef enum tagHV_COLOR_CODE{
	COLOR_MONO8                  = 0,      ///< Y component has 8bit data
	COLOR_YUV8_4_1_1             = 1,      ///< 4:1:1 YUV 8 format, each component has 8bit data
	COLOR_YUV8_4_2_2             = 2,      ///< 4:2:2 YUV 8 format, each component has 8bit data
	COLOR_YUV8_4_4_4             = 3,      ///< 4:4:4 YUV 8 format, each component has 8bit data
	COLOR_RGB8                   = 4,      ///< RGB 8 format, each component has 8bit data
	COLOR_MONO16                 = 5,      ///< Y component has 16bit data
	COLOR_RGB16                  = 6,      ///< RGB 16 format, each component has 16bit data
	COLOR_SIGNED_MONO16          = 7,
	COLOR_SIGNED_RGB16           = 8,
	COLOR_RAW8                   = 9,  
	COLOR_RAW16                  = 10,
	
	COLOR_MONO10                 = 128,
	COLOR_MONO10_NORMALIZED      = 129,
	COLOR_MONO10_PACKED          = 130,
	COLOR_RAW10                  = 131,
	COLOR_RAW10_NORMALIZED       = 132,
	COLOR_RAW10_PACKED           = 133,
	
	COLOR_MONO12                 = 134, 
	COLOR_MONO12_NORMALIZED      = 135, 
	COLOR_MONO12_PACKED          = 136,  
	COLOR_RAW12                  = 137,
	COLOR_RAW12_NORMALIZED       = 138,
	COLOR_RAW12_PACKED           = 139,
}HV_COLOR_CODE;

/// A union for the format7 mode_x
typedef union tagHV_FMT7_MODE{
  struct {	
	DWORD Top                  :16; ///< 008h[16..31],Top position of requested image region (pixels) 
	DWORD Left                 :16; ///< 008h[0..15],Left position of requested image region (pixels) 

	DWORD Height               :16; ///< 00Ch[16..31],Height of requested image region (pixels)  
	DWORD Width                :16; ///< 00Ch[0..15],Width of requested image region (pixels)  
  } AOI;

  struct {	
	HV_COLOR_CODE Code;             ///< 010h, Specifies the color coding   
  } ColorMode;

  struct {	
	DWORD                      :16; ///< 044h[16..31],Reserved
	DWORD PacketSize           :16; ///< 044h[0..15],Specifies the packet size
  } BandWidth;
}HV_FMT7_MODE;

/// The FMT7 mode item id for Set
typedef enum tagHV_FMT7_MODE_ITEM_ID{
	FMT7_MODE_ITEM_ID_AOI_LEFT                     = 0x000000,
	FMT7_MODE_ITEM_ID_AOI_TOP                      = 0x000010,

	FMT7_MODE_ITEM_ID_AOI_WIDTH                    = 0x000100,
	FMT7_MODE_ITEM_ID_AOI_HEIGHT                   = 0x000110,

	FMT7_MODE_ITEM_ID_AOI_POSITION                 = 0xFF000001,  ///< set the AOI position, left and top
	FMT7_MODE_ITEM_ID_AOI_SIZE                     = 0xFF000002,  ///< set the AOI size, width and height
	FMT7_MODE_ITEM_ID_AOI_ALL                      = 0xFF000003,  ///< Set the Size and position,left\ top\width\height

	FMT7_MODE_ITEM_ID_COLOR_MODE_COLOR_CODE        = 0x010000,

	FMT7_MODE_ITEM_ID_BAND_WIDTH_PACKET_SIZE       = 0x020010,
}HV_FMT7_MODE_ITEM_ID;

/// Argument for the ORD_GET_FMT7_MODE function
typedef struct tagHV_ARG_GET_FMT7_MODE{
	HV_VIDEO_MODE             VideoMode;           ///< Specifies the video mode
	HV_FMT7_MODE_FUNC_ID      FuncId;              ///< Specifies the format7 mode_x  function id	
}HV_ARG_GET_FMT7_MODE;

/// Result of the ORD_GET_FMT7_MODE function
typedef HV_FMT7_MODE HV_RES_GET_FMT7_MODE;

/// Argument for the ORD_SET_FMT7_MODE function
typedef struct tagHV_ARG_SET_FMT7_MODE{
	HV_VIDEO_MODE             VideoMode;           ///< Specifies the video mode
	HV_FMT7_MODE_FUNC_ID      FuncId;              ///< Specifies the format7 mode_x  function id
	HV_FMT7_MODE_ITEM_ID      ItemId;              ///< Specifies the format7 mode_x sub Item id 
	HV_FMT7_MODE              Fmt7Mode;            ///< Specifies the format7 mode_x
} HV_ARG_SET_FMT7_MODE;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_FEATURE_PRESENCE function
typedef struct tagHV_RES_QUERY_FEATURE_PRESENCE{
	/* DCAM Feature_Hi_Inq */
	DWORD                         :16;  ///< [16..31]Reserved    	
	DWORD FrameRate               : 1;  ///< [15]Inquiry for Frame rate prioritize control, not supported is 0,supported is 1;
	DWORD WhiteShading            : 1;  ///< [14]Inquiry for White Shading Compensation Control, not supported is 0,supported is 1; 
	DWORD TriggerDelay            : 1;  ///< [13]Inquiry for Trigger Delay Control, not supported is 0,supported is 1;     	
	DWORD Trigger                 : 1;  ///< [12]Inquiry for Trigger Control, not supported is 0,supported is 1;     
	DWORD                         : 3;  ///< [9..11]Reserved for Iris,Focus,Temperature
	DWORD Gain                    : 1;  ///< [8]Inquiry for Gain Control, not supported is 0,supported is 1;
	DWORD Shutter                 : 1;  ///< [7]Inquiry for Shutter Speed Control, not supported is 0,supported is 1;
	DWORD Gamma                   : 1;  ///< [6]Inquiry for Brightness Control, not supported is 0,supported is 1;   	
	DWORD                         : 2;  ///< [4..5]Reserved for hue,Saturation
	DWORD WhiteBalance            : 1;  ///< [3]Inquiry for White Balance Control, not supported is 0,supported is 1;     	
	DWORD Sharpness               : 1;  ///< [2]Inquiry for Sharpness Control, not supported is 0,supported is 1; 
	DWORD AutoExposure            : 1;  ///< [1]Inquiry for Auto Exposure Control, not supported is 0,supported is 1;     	
	DWORD Brightness              : 1;  ///< [0]Inquiry for Brightness Control, not supported is 0,supported is 1;  

	/* DCAM Feature_Lo_Inq */
	DWORD                         :14;  ///< [18..31]Reserved
	DWORD CaptureQuality          : 1;  ///< [17]Inquiry for Capture image quality for Format_6, not supported is 0,supported is 1; 
	DWORD CaptureSize             : 1;  ///< [16]Inquiry for Capture image size for Format_6, not supported is 0,supported is 1;
	DWORD                         :12;  ///< [4..15]Reserved 
	DWORD OpticalFilter           : 1;  ///< [3]Inquiry for Optical Filter Control, not supported is 0,supported is 1;     	
	DWORD Tilt                    : 1;  ///< [2]Inquiry for TILT Control, not supported is 0,supported is 1;     
	DWORD Pan                     : 1;  ///< [1]Inquiry for PAN Control, not supported is 0,supported is 1;     	
	DWORD Zoom                    : 1;  ///< [0]Inquiry for Zoom Control, not supported is 0,supported is 1;
}HV_RES_QUERY_FEATURE_PRESENCE;

/// DCAM Standard Feature ID
typedef enum tagHV_FEATURE_ID{
	FEATURE_ID_BRIGHTNESS        = 0,    ///< Specifies the brightness control
	FEATURE_ID_AUTO_EXPOSURE     = 1,    ///< Specifies the auto exposure control
	FEATURE_ID_SHARPNESS         = 2,    ///< Specifies the sharpness control
	FEATURE_ID_WHITE_BALANCE     = 3,    ///< Specifies the white balance control
	FEATURE_ID_HUE               = 4,    ///< Specifies the hue control
	FEATURE_ID_SATURATION        = 5,    ///< Specifies the saturation control
	FEATURE_ID_GAMMA             = 6,    ///< Specifies the gamma control
	FEATURE_ID_SHUTTER           = 7,    ///< Specifies the shutter control
	FEATURE_ID_GAIN              = 8,    ///< Specifies the gain control
	FEATURE_ID_IRIS              = 9,    ///< Specifies the iris control
	FEATURE_ID_FOCUS             = 10,   ///< Specifies the focus control
	FEATURE_ID_TEMPERATURE       = 11,   ///< Specifies the temperature control
	FEATURE_ID_TRIGGER           = 12,   ///< Specifies the trigger control
	FEATURE_ID_TRIGGER_DELAY     = 13,   ///< Specifies the Trigger Delay Control 
	FEATURE_ID_WHITE_SHADING     = 14,   ///< Specifies the White Shading Compensation Control  
	FEATURE_ID_FRAME_RATE        = 15,   ///< Specifies the Frame rate prioritize control
		
	FEATURE_ID_ZOOM              = 32,   ///< Specifies the zoom control
	FEATURE_ID_PAN               = 33,   ///< Specifies the pan control
	FEATURE_ID_TILT              = 34,   ///< Specifies the tilt control
	FEATURE_ID_OPTICAL_FILTER    = 35,   ///< Specifies the optical filter control
		
	FEATURE_ID_CAPTURE_SIZE      = 48,   ///< Specifies the image size for format 6
	FEATURE_ID_CAPTURE_QUALITY   = 49,   ///< Specifies the image quality for format 6
}HV_FEATURE_ID;

/// DCAM Standard Feature INQ Descriptor
typedef union tagHV_FEATURE_DESCRIPTOR {
	struct {
		DWORD MaxValue               :12;  ///< [20..31]Maximum value for this feature
		DWORD MinValue               :12;  ///< [8..19]Minimum value for this feature
		DWORD ManualInq              : 1;  ///< [7]Availability of manual mode
		DWORD AutoInq                : 1;  ///< [6]Availability of automatic mode
		DWORD OnOffInq               : 1;  ///< [5]Availability of swithing this feature on and off
		DWORD ReadOutInq             : 1;  ///< [4]Capability of reading the value of this feature
		DWORD OnePushInq             : 1;  ///< [3]Availability of one push automatic mode
		DWORD                        : 1;  ///< [2]Reserved
		DWORD AbsControlInq          : 1;  ///< [1]Availability of absolute control
		DWORD PresenceInq            : 1;  ///< [0]Presence of this feature
	} Scalar;       ///< a scalar feature,for brightness,auto exposure,sharpness,white balance,hue,saturation,gamma,shutter
	                ///< gain,iris,focus,temperature,Trigger Delay,White Shading,Frame rate prioritize,zoom,pan,tilt,optical filter,CAPTURE_SIZE,CAPTURE_QUALITY
	struct {
		DWORD                        :12; ///< [20..31],Reserved
		DWORD Mode3Inq               : 1; ///< [19]Presence of Trigger mode 3
		DWORD Mode2Inq               : 1; ///< [18]Presence of Trigger mode 2
		DWORD Mode1Inq               : 1; ///< [17]Presence of Trigger mode 1
		DWORD Mode0Inq               : 1; ///< [16]Presence of Trigger mode 0
		DWORD SoftTriggerInq         : 1; ///< [15]Presence of Software Trigger
		DWORD                        : 3; ///< [12..14]Reserved
		DWORD Source3Inq             : 1; ///< [11]Presence of Trigger source 3
		DWORD Source2Inq             : 1; ///< [10]Presence of Trigger source 2
		DWORD Source1Inq             : 1; ///< [9]Presence of Trigger source 1
		DWORD Source0Inq             : 1; ///< [8]Presence of Trigger source 0
		DWORD ValueReadInq           : 1; ///< [7]Capability of reading raw trigger input 
		DWORD PolarityInq            : 1; ///< [6]Capability of changing the polarity of the trigger input
		DWORD OnOffInq               : 1; ///< [5]Availability of switching this feature on and off
		DWORD ReadOutInq             : 1; ///< [4]Capability of reading the value of this feature
		DWORD                        : 2; ///< [2..3]Reserved
		DWORD AbsControlInq          : 1; ///< [1]Availability of absolute control
		DWORD PresenceInq            : 1; ///< [0]Presence of this feature
	} Trigger;      ///< trigger feature
}HV_FEATURE_DESCRIPTOR;

/// Argument for the ORD_QUERY_FEATURE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_FEATURE_DESCRIPTOR{
    HV_FEATURE_ID FeatureId;
}HV_ARG_QUERY_FEATURE_DESCRIPTOR;

/// Result of the ORD_QUERY_FEATURE_DESCRIPTOR function
typedef HV_FEATURE_DESCRIPTOR HV_RES_QUERY_FEATURE_DESCRIPTOR;

/// The DCAM Standard Feature CSR value, offset=800h
typedef union tagHV_FEATURE{
	struct {
		DWORD Value             :12; ///< [20..31]Value of this feature
		DWORD                   :12; ///< [8..19]Reserved
		DWORD AutoManualMode    : 1; ///< [7]Automatic mode - manual mode switch
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]Reserved
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} Scalar;       ///< a scalar feature,for brightness,auto exposure,sharpness,hue,saturation,gamma,shutter
	                ///< gain,iris,focus,temperature,Trigger Delay,White Shading,Frame rate prioritize,zoom,pan,tilt,optical filter,CAPTURE_SIZE,CAPTURE_QUALITY	
	struct {
		DWORD Parameter         :12; ///< [20..31]Parameter of the trigger function
		DWORD                   : 4; ///< [16..19]Reserved
		DWORD Mode              : 4; ///< [12..15]Trigger mode 0-15,如沿触发，电平触发类等
		DWORD                   : 1; ///< [11]Reserved
		DWORD Source            : 3; ///< [8..10],TriggerSource
		DWORD Polarity          : 1; ///< [7]Trigger polarity, high active is 1, low active is 0
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0,触发模式关，连续模式开;on is 1，触发模式开，连续模式关
		DWORD                   : 4; ///< [2..5]Reserved
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} Trigger;      ///< Trigger feature
	
	struct {
		DWORD VR                :12; ///< [20..31]V value or R value
		DWORD UB                :12; ///< [8..19]U value or B value
		DWORD AutoManualMode    : 1; ///< [7]Automatic mode - manual mode switch
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} WhiteBalance; ///< White balance feature 
}HV_FEATURE;

/// The Feature ITEM id for set
typedef enum tagHV_FEATURE_ITEM_ID{
	FEATURE_ITEM_ID_SCALAR_PRESENCE_INQ              = 0x000000,
	FEATURE_ITEM_ID_SCALAR_ABS_CONTROL               = 0x000001,
	FEATURE_ITEM_ID_SCALAR_ONE_PUSH                  = 0x000005,
	FEATURE_ITEM_ID_SCALAR_ON_OFF                    = 0x000006,
	FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE          = 0x000007,
	FEATURE_ITEM_ID_SCALAR_VALUE                     = 0x000014,

	FEATURE_ITEM_ID_TRIGGER_PRESENCE_INQ             = 0x010000,	
	FEATURE_ITEM_ID_TRIGGER_ABS_CONTROL              = 0x010001,
	FEATURE_ITEM_ID_TRIGGER_ON_OFF                   = 0x010006,
	FEATURE_ITEM_ID_TRIGGER_POLARITY                 = 0x010007,
	FEATURE_ITEM_ID_TRIGGER_SOURCE                   = 0x010008,
	FEATURE_ITEM_ID_TRIGGER_MODE                     = 0x01000C,	
	FEATURE_ITEM_ID_TRIGGER_PARAMETER                = 0x010014,

	FEATURE_ITEM_ID_WHITE_BALANCE_PRESENCE_INQ       = 0x020000,
	FEATURE_ITEM_ID_WHITE_BALANCE_ABS_CONTROL        = 0x020001,
	FEATURE_ITEM_ID_WHITE_BALANCE_ONE_PUSH           = 0x020005,
	FEATURE_ITEM_ID_WHITE_BALANCE_ON_OFF             = 0x020006,
	FEATURE_ITEM_ID_WHITE_BALANCE_AUTO_MANUAL_MODE   = 0x020007,
	FEATURE_ITEM_ID_WHITE_BALANCE_UB                 = 0x020008,
	FEATURE_ITEM_ID_WHITE_BALANCE_VR                 = 0x020014,
}HV_FEATURE_ITEM_ID;

/// Argument for the ORD_GET_FEATURE function
typedef struct tagHV_ARG_GET_FEATURE{
    HV_FEATURE_ID       FeatureId;
}HV_ARG_GET_FEATURE;

/// Result of the ORD_GET_FEATURE function
typedef HV_FEATURE HV_RES_GET_FEATURE;

/// Argument for the ORD_SET_FEATURE function
typedef struct tagHV_ARG_SET_FEATURE{
	HV_FEATURE_ID       FeatureId;
	HV_FEATURE_ITEM_ID  FeatureItemId;
	HV_FEATURE          Feature;
}HV_ARG_SET_FEATURE;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_BASIC_FUNC_PRESENCE function,DCAM Basic Function
typedef struct tagHV_RES_QUERY_BASIC_FUNC_PRESENCE{
	DWORD MemCh                 : 4; ///< [28..31]Maximum memory channel number (N) 
	                                 ///< Memory channel no
	                                 ///< 0 = Factory setting memory 
	                                 ///< 1 = Memory Ch 1 
		                             ///< 2 = Memory Ch 2 
	                                 ///<   :
	                                 ///< N = Memory Ch N 
		                             ///< If 0000, user memory is not available.
	DWORD                       : 7; ///< [21..27]Reserved;
	DWORD MultiShot             : 1; ///< [20]Multi shot transmission capability ,not supported is 0,supported is 1;
	DWORD OneShot               : 1; ///< [19]One shot transmission capability,not supported is 0,supported is 1;
	DWORD                       :15; ///< [4..18]Reserved 
	DWORD OptFunc               : 1; ///< [3]Inquiry for optional function,not supported is 0,supported is 1; 
	DWORD                       : 2; ///< [1..2]Reserved 
	DWORD AdvFeature            : 1; ///< [0]Inquiry for advanced feature.(Vendor Unique Features) ,not supported is 0,supported is 1;
}HV_RES_QUERY_BASIC_FUNC_PRESENCE;

/// The basic function id
typedef enum tagHV_BASIC_FUNC_ID{
	BASIC_FUNC_ID_MEM_CH_SAVE               = 28,
    BASIC_FUNC_ID_MEM_CH_CUR                = 29,
}HV_BASIC_FUNC_ID;

/// The descriptor struct of basic function
typedef union tagHV_BASIC_FUNC_DESCRIPTOR{
    struct {
		DWORD dwMaxNum;   ///< 显示支持的最大参数组数目, not supported is 0,supported is 1;
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
        DWORD                       :28; ///< [4..31]Reserved 
		DWORD Number                : 4; ///< [0..3]保存参数组号  
	} MemChSave;
	
	struct {
        DWORD                        :28; ///< [4..31]Reserved 
		DWORD Number                 : 4; ///< [0..3]当前参数组号  
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
    BASIC_FUNC_ITEM_ID_MEM_CH_SAVE_NUM          = 0x000000,
	BASIC_FUNC_ITEM_ID_MEM_CH_CUR_NUM           = 0x010000,		
}HV_BASIC_FUNC_ITEM_ID;

/// Argument for the ORD_SET_BASIC_FUNC function
typedef struct tagHV_ARG_SET_BASIC_FUNC{
    HV_BASIC_FUNC_ID      FuncId;
	HV_BASIC_FUNC_ITEM_ID ItemId;
	HV_BASIC_FUNC         Func;
}HV_ARG_SET_BASIC_FUNC;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_OPT_FUNC_PRESENCE function,DCAM Option Function
typedef struct tagHV_RES_QUERY_OPT_FUNC_PRESENCE{
	DWORD                       :28; ///< [4..31]Reserved;
	DWORD Strobe                : 1; ///< [3]Inquiry for Strobe signal output,not supported is 0,supported is 1; 
	DWORD SIO                   : 1; ///< [2]Inquiry for Serial Input/output control,not supported is 0,supported is 1; 
	DWORD PIO                   : 1; ///< [1]Inquiry for Parallel input/output control,not supported is 0,supported is 1;
	DWORD                       : 1; ///< [0]Reserved 
}HV_RES_QUERY_OPT_FUNC_PRESENCE;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_PIO_OUTPUT_PORT_PRESENCE function
typedef struct tagHV_RES_QUERY_PIO_OUTPUT_PORT_PRESENCE{
	DWORD                        :29; ///< Reserved
	DWORD Port2                  : 1; ///< Inquiry for OUTPUT_PORT_2,not supported is 0,supported is 1;
	DWORD Port1                  : 1; ///< Inquiry for OUTPUT_PORT_1,not supported is 0,supported is 1;
	DWORD Port0                  : 1; ///< Inquiry for OUTPUT_PORT_0,not supported is 0,supported is 1;
}HV_RES_QUERY_PIO_OUTPUT_PORT_PRESENCE;

/// PIO_OUTPUT_PORT_X ID
typedef enum tagHV_PIO_OUTPUT_PORT_ID{
	PIO_OUTPUT_PORT0      = 0,
	PIO_OUTPUT_PORT1      = 1,
	PIO_OUTPUT_PORT2      = 2,
}HV_PIO_OUTPUT_PORT_ID;

/// The descriptor struct of PIO_OUTPUT_PORT_X in the DHCAM 
typedef struct tagHV_PIO_OUTPUT_PORT_DESCRIPTOR{
	struct {
		DWORD                          : 5; ///< [27..31]Source_Select
		DWORD                          :23; ///< [4..26]Reserved;
		DWORD UserSetInq               : 1; ///< [3]显示是否有用户设定的功能,not supported is 0,supported is 1; 
		DWORD InvertInq                : 1; ///< [2]显示是否有IO反相的功能,not supported is 0,supported is 1;
		DWORD MonitorInq               : 1; ///< [1]显示是否有监控IO的功能,not supported is 0,supported is 1; 
		DWORD PresenceInq              : 1; ///< [0]显示是否有PORT_X的配置功能,not supported is 0,supported is 1;
	} Configration;                         ///< offset=00h
	struct { 
		DWORD                          :28; ///< [4..31]Reserved; 
		DWORD StrobeSignalInq          : 1; ///< [3]Inquiry for 闪光灯信号（默认）,not supported is 0,supported is 1; 
		DWORD UserSignalInq            : 1; ///< [2]Inquiry for 用户自定义信号,not supported is 0,supported is 1; 
		DWORD TriggerReadySignalInq    : 1; ///< [1]Inquiry for trigger_ready信号,not supported is 0,supported is 1; 
		DWORD IntegralSignalInq        : 1; ///< [0]Inquiry for 积分信号,not supported is 0,supported is 1;
	} SourceSelect;                         ///< software simulate offset=04h
}HV_PIO_OUTPUT_PORT_DESCRIPTOR;

/// Argument for the ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR{
	HV_PIO_OUTPUT_PORT_ID PortId;
}HV_ARG_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR;

/// Result of the ORD_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR function
typedef HV_PIO_OUTPUT_PORT_DESCRIPTOR HV_RES_QUERY_PIO_OUTPUT_PORT_DESCRIPTOR;

/// The PIO_PORT_X functions id
typedef enum tagHV_PIO_OUTPUT_PORT_FUNC_ID{
    PIO_OUTPUT_PORT_FUNC_ID_SOURCE_SELECT  = 0,
    PIO_OUTPUT_PORT_FUNC_ID_MONITOR        = 1,
    PIO_OUTPUT_PORT_FUNC_ID_INVERT         = 2,
    PIO_OUTPUT_PORT_FUNC_ID_USER_SET       = 3,
}HV_PIO_OUTPUT_PORT_FUNC_ID;

/// PIO_OUTPUT_PORT_CONTORL
typedef union tagHV_PIO_OUTPUT_PORT{
	struct {
		DWORD Value                    : 5; ///< [27..31],写入一个值来选择输出port0的信号来源：0: 积分信号,1：trigger_ready信号, 2: 用户自定义信号,3：闪光灯信号（默认）
		DWORD                          :27; ///< [0-26],used by PresenceInq,MonitorInq,InvertInq,UserSetInq
	} SourceSelect;                         ///< offset=00h
	
	struct {
		DWORD Value                    : 1; ///< [31],(read only)Show the current state of the output,0:low(non conducting),1:high(conducting);
		DWORD                          :31; ///< [0-30],Reserved
	} Monitor;                              ///< offset=08h
	
	struct {
		DWORD Value                    : 1; ///< [31],把输出信号反相,0:不反相,1:反相;
		DWORD                          :31; ///< [0-30],Reserved
	} Invert;                               ///< offset=0Ch
	
	struct {
		DWORD Value                    : 1; ///< [31],0:low,1:high
		DWORD                          :31; ///< [0-30],Reserved
	} UserSet;                              ///< offset=10h	
}HV_PIO_OUTPUT_PORT;

/// The PIO output port ITEM id for set
typedef enum tagHV_PIO_OUTPUT_PORT_ITEM_ID{
	PIO_OUTPUT_PORT_ITEM_ID_SOURCE_SELECT_VALUE    = 0x00001B,

	PIO_OUTPUT_PORT_ITEM_ID_MONITOR_VALUE          = 0x01001F,

	PIO_OUTPUT_PORT_ITEM_ID_INVERT_VALUE           = 0x02001F,

	PIO_OUTPUT_PORT_ITEM_ID_USER_SET_VALUE         = 0x03001F,
}HV_PIO_OUTPUT_PORT_ITEM_ID;

/// Argument for the ORD_GET_PIO_OUTPUT_PORT function
typedef struct tagHV_ARG_GET_PIO_OUTPUT_PORT{
	HV_PIO_OUTPUT_PORT_ID          PortId;
	HV_PIO_OUTPUT_PORT_FUNC_ID     PortFuncId;
}HV_ARG_GET_PIO_OUTPUT_PORT;

/// Result of the ORD_GET_PIO_OUTPUT_PORT function
typedef HV_PIO_OUTPUT_PORT HV_RES_GET_PIO_OUTPUT_PORT;

/// Argument for the ORD_SET_PIO_OUTPUT_PORT function
typedef struct tagHV_ARG_SET_PIO_OUTPUT_PORT{
	HV_PIO_OUTPUT_PORT_ID          PortId;
	HV_PIO_OUTPUT_PORT_FUNC_ID     PortFuncId;
	HV_PIO_OUTPUT_PORT_ITEM_ID     PortItemId;
	HV_PIO_OUTPUT_PORT             OutputPort;
}HV_ARG_SET_PIO_OUTPUT_PORT;



//////////////////////////////////////////////////////////////////////////
/// The descriptor struct for the SIO, software simulate
typedef struct tagHV_SIO_DESCRIPTOR{
	struct {
		DWORD ValueInq                : 8;  ///< [24..31],Buffer Size (Read Only),This field indicates the maximum size of  receive/transmit data buffer,
		                                    ///< If this value=1, Buffer_Status_Control,SIO_Data_Register Char1-3 should be ignored.
		DWORD                         :24;  ///< [0..23],Reserved for the band rate,char length,stop bits,parity,offset=000h. 
	} BufferSize;
	
	struct {
		DWORD                         :29;  ///< reserved
		DWORD bit2Inq                 : 1;  ///< Inquiry for Stop bits =2,not supported is 0,supported is 1;
		DWORD bit1_5Inq               : 1;  ///< Inquiry for Stop bits =1.5,not supported is 0,supported is 1;
		DWORD bit1Inq                 : 1;  ///< Inquiry for Stop bits =1,not supported is 0,supported is 1;
	} StopBitMode;
	
	struct {
		DWORD                         :29;  ///< reserved
		DWORD EvenInq                 : 1;  ///< Inquiry for Parity setting =even,not supported is 0,supported is 1;
		DWORD OddInq                  : 1;  ///< Inquiry for Parity setting =odd,not supported is 0,supported is 1;
		DWORD NoneInq                 : 1;  ///< Inquiry for Parity setting =none,not supported is 0,supported is 1;
	} ParityMode;
	
	struct {
		DWORD                         :23;  ///< reserved
		DWORD bits8Inq                : 1;  ///< Inquiry for Character length setting=8bits,not supported is 0,supported is 1;
		DWORD bits7Inq                : 1;  ///< Inquiry for Character length setting=7bits,not supported is 0,supported is 1;
		DWORD                         : 7;  ///< reserved
	} CharLengthMode;
	
	struct {
		DWORD                         :21;  ///< reserved
		DWORD bps230400Inq            : 1;  ///< Inquiry for band rate=230400bps,not supported is 0,supported is 1;
		DWORD bps115200Inq            : 1;  ///< Inquiry for band rate=115200bps,not supported is 0,supported is 1;
		DWORD bps57600Inq             : 1;  ///< Inquiry for band rate=57600bps,not supported is 0,supported is 1;
		DWORD bps38400Inq             : 1;  ///< Inquiry for band rate=38400bps,not supported is 0,supported is 1;
		DWORD bps19200Inq             : 1;  ///< Inquiry for band rate=19200bps,not supported is 0,supported is 1;
		DWORD bps9600Inq              : 1;  ///< Inquiry for band rate=9600bps,not supported is 0,supported is 1;
		DWORD bps4800Inq              : 1;  ///< Inquiry for band rate=4800bps,not supported is 0,supported is 1;
		DWORD bps2400Inq              : 1;  ///< Inquiry for band rate=2400bps,not supported is 0,supported is 1;
		DWORD bps1200Inq              : 1;  ///< Inquiry for band rate=1200bps,not supported is 0,supported is 1;
		DWORD bps600Inq               : 1;  ///< Inquiry for band rate=600bps,not supported is 0,supported is 1;
		DWORD bps300Inq               : 1;  ///< Inquiry for band rate=300bps,not supported is 0,supported is 1;
	} BandRateMode;
}HV_SIO_DESCRIPTOR;

/// Result of the ORD_QUERY_SIO_DESCRIPTOR function
typedef HV_SIO_DESCRIPTOR HV_RES_QUERY_SIO_DESCRIPTOR;

/// The sub functions of the SIO
typedef enum tagHV_SIO_FUNC_ID{
	SIO_FUNC_ID_MODE      = 0,             ///< 设置或者读取串口模式子功能ID
	SIO_FUNC_ID_CONTROL   = 1,             ///< 串口控制子功能ID，包括发送和接收
	SIO_FUNC_ID_DATA      = 2,             ///< 串口数据操作（读写）子功能ID
}HV_SIO_FUNC_ID;

/// The SIO union
typedef union tagHV_SIO{
	struct {
		DWORD                           :12;  ///< [20-31],Reserved
		DWORD StopBit                   : 2;  ///< [18..19],Stop bits,Write: Set stop bit,Read: Get current stop bit,0: 1,1: 1.5,2: 2.
		DWORD Parity                    : 2;  ///< [16..17],Parity setting, Write: Set parity ,Read: Get current, parity 0: None, 1: Odd, 2: Even.
		DWORD CharLength                : 8;  ///< [8..15], Character length setting Write: Set data length (must not be 0) Read: Get data length 
		                                      ///< 7: 7bits ,8: 8bits,Other values reserved. 
		DWORD RateValue                 : 8;  ///< [0..7],  0: 300bps,1: 600bps,2: 1200bps,3: 2400bps,4: 4800bps,5: 9600bps,6: 19200bps,
		                                      ///< 7: 38400bps,8: 57600bps,	9: 115200bps ,10: 230400bps ,Other values reserved.
	} Mode;      ///<  Serial_Mode_Reg,offset=000h,
	
	struct {
		DWORD                           :30;  ///< [2-31],Reserved
		DWORD TransmitEnable            : 1;  ///< [1],Transmit enable,Read: Current status ,Write: 0:Disable 1:Enable
		DWORD ReceiveEnable             : 1;  ///< [0],Receive enable,Read: Current status ,Write: 0:Disable 1:Enable 
	} Control;   ///< Serial_Control_Reg,offset=004h,

	struct {
		BYTE *pBuffer;                        ///< 数据缓冲区指针
		int  nLength;                         ///< 读写数据缓冲区长度，指明多少个byte 
	} Data;      ///< Serial data
}HV_SIO;

/// The SIO ITEM id for set
typedef enum tagHV_SIO_ITEM_ID{
	SIO_ITEM_ID_MODE_RATE_VALUE          = 0x000000,
	SIO_ITEM_ID_MODE_CHAR_LENGTH         = 0x000008,
	SIO_ITEM_ID_MODE_PARITY              = 0x000010,
	SIO_ITEM_ID_MODE_STOP_BIT            = 0x000012,

	SIO_ITEM_ID_CONTROL_RECEIVE_ENABLE   = 0x010000,
	SIO_ITEM_ID_CONTROL_TRANSMIT_ENABLE  = 0x010001,

	SIO_ITEM_ID_DATA                     = 0x020000,
}HV_SIO_ITEM_ID;

/// Argument for the ORD_GET_SIO function
typedef struct tagHV_ARG_GET_SIO{
   HV_SIO_FUNC_ID      SIOFuncId;  
}HV_ARG_GET_SIO;

/// Result of the ORD_GET_SIO function
typedef HV_SIO HV_RES_GET_SIO;

/// Argument for the ORD_SET_SIO function
typedef struct tagHV_ARG_SET_SIO{
	HV_SIO_FUNC_ID     SIOFuncId;
	HV_SIO_ITEM_ID     SIOItemId;
	HV_SIO             SIO;
}HV_ARG_SET_SIO;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_STROBE_PRESENCE function
typedef struct tagHV_RES_QUERY_STROBE_PRESENCE{
	DWORD                         :28; ///< [4..31]Reserved
	DWORD Strobe3Inq              : 1; ///< [3]Presence the Strobe_3, not supported is 0,supported is 1;
	DWORD Strobe2Inq              : 1; ///< [2]Presence the Strobe_2, not supported is 0,supported is 1;
	DWORD Strobe1Inq              : 1; ///< [1]Presence the Strobe_1, not supported is 0,supported is 1;
	DWORD Strobe0Inq              : 1; ///< [0]Presence the Strobe_0, not supported is 0,supported is 1;
}HV_RES_QUERY_STROBE_PRESENCE;

/// Strobe_Output_X ID
typedef enum tagHV_STROBE_ID{
	STROBE0        = 0,
	STROBE1        = 1,
	STROBE2        = 2,
	STROBE3        = 3,
}HV_STROBE_ID;

/// The descriptor struct of Strobe_Output_X in the DCAM 
typedef struct tagHV_STROBE_DESCRIPTOR{
    DWORD MaxValue       :12;  ///< [20..31]Maximum value for this feature
    DWORD MinValue       :12;  ///< [8..19]Minimum value for this feature
    DWORD                : 1;  ///< [7]Reserved
    DWORD PolarityInq    : 1;  ///< [6]Capability of changing polarity of the signal 
    DWORD OnOffInq       : 1;  ///< [5]Availability of switching this feature on and off
    DWORD ReadOutInq     : 1;  ///< [4]Capability of reading the value of this feature
    DWORD                : 3;  ///< [1..3]Reserved
    DWORD PresenceInq    : 1;  ///< [0]Presence of this feature
}HV_STROBE_DESCRIPTOR;

/// Argument for the ORD_QUERY_STROBE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_STROBE_DESCRIPTOR{
	HV_STROBE_ID StrobeId;
}HV_ARG_QUERY_STROBE_DESCRIPTOR;

/// Result of the ORD_QUERY_STROBE_DESCRIPTOR function
typedef HV_STROBE_DESCRIPTOR HV_RES_QUERY_STROBE_DESCRIPTOR;

// The Strobe_X_Cnt 
typedef struct tagHV_STROBE{
	DWORD DurationValue    :12;  ///< [20..31]Duration of the strobe signal.
	DWORD DelayValue       :12;  ///< [8..19]Delay after start of exposure until the strobe signal asserts. 
	DWORD SignalPolarity   : 1;  ///< [7]Select signal polarity,(0: Low active output, 1: High active output)
	DWORD OnOff            : 1;  ///< [6]Switch to turn this feature on or off, off is 0, on is 1
	DWORD                  : 5;  ///< [1..5]Reserved
	DWORD PresenceInq      : 1;  ///< [0]Presence of this feature
}HV_STROBE;

/// The strobe ITEM id for set
typedef enum tagHV_STROBE_ITEM_ID{
	STROBE_ITEM_ID_PRESENCE_INQ        = 0x000000,
	STROBE_ITEM_ID_ON_OFF              = 0x000006,
	STROBE_ITEM_ID_SIGNAL_POLARITY     = 0x000007,
	STROBE_ITEM_ID_DELAY_VALUE         = 0x000008,
	STROBE_ITEM_ID_DURATION_VALUE      = 0x000014,
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
//                                                                      //
//                 DHCAM SNAP Image Functions                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_OPEN_SNAP function
typedef struct tagHV_ARG_OPEN_SNAP{
	HV_SNAP_PROC pSnapFunc;   ///< Specifies the callback function
	void *pParam;             ///< Specifies user context
}HV_ARG_OPEN_SNAP;

/// Argument for the ORD_START_SNAP function
typedef struct tagHV_ARG_START_SNAP{
	BYTE **ppBuffer;          ///< Specifies buffers for snapping
	int  nSum;                ///< Specifies the number of buffer
}HV_ARG_START_SNAP;

/// Argument for the ORD_SNAP_SHOT function
typedef struct tagHV_ARG_SNAP_SHOT{
	BYTE **ppBuffer;          ///< Specifies buffers for snapping
	int  nSum;                ///< Specifies the number of buffer
}HV_ARG_SNAP_SHOT;



//////////////////////////////////////////////////////////////////////////
/// Argument for the ORD_CMD_RESERVED0 function
typedef struct tagHV_ARG_CMD_RESERVED0{
	int   nLength;
	void  *pContext;
}HV_ARG_CMD_RESERVED0;

/// Argument for the ORD_CMD_RESERVED1 function
typedef HV_ARG_CMD_RESERVED0 HV_ARG_CMD_RESERVED1;



//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                 DHCAM Advanced Functions                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
///Result of ORD_QUERY_ADV_FEATURE_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_FEATURE_PRESENCE{
	/*ADVC_FTR_FEATURE_HI_INQ*/
	DWORD                               :13; ///< [19..31]Reserved
	DWORD InterPacketDelay              : 1; ///< [18]Inquiry for 包间延迟功能查询, not supported is 0,supported is 1; 
	DWORD TriggerSourceStatus           : 1; ///< [17]Inquiry for 触发源状态查询, not supported is 0,supported is 1; 
	DWORD PixelEndianess                : 1; ///< [16]Inquiry for 像素大小端模式功能, not supported is 0,supported is 1;
	DWORD FilterTimeUnit                : 1; ///< [15]Inquiry for 滤波功能,时间 not supported is 0,supported is 1;
	DWORD FilterTime                    : 1; ///< [14]Inquiry for 滤波功能,时间 not supported is 0,supported is 1; 
	DWORD DelayTransmit                 : 1; ///< [13]Inquiry for 延迟传输, not supported is 0,supported is 1; 
	DWORD StartupMemCh                  : 1; ///< [12]Inquiry for 上电参数组设定功能, not supported is 0,supported is 1; 
	DWORD DigitalGain                   : 1; ///< [11]Inquiry for 数字移位功能, not supported is 0,supported is 1;
	DWORD MultiTriggerTime              : 1; ///< [10]Inquiry for 一次触发采集多帧功能,帧间隔, not supported is 0,supported is 1;
	DWORD MultiTriggerNum               : 1; ///< [9]Inquiry for 一次触发采集多帧功能,帧数, not supported is 0,supported is 1;  
	DWORD AdControl                     : 1; ///< [8]Inquiry for 噪声控制(位选择模式), not supported is 0,supported is 1; 
	DWORD TestImage                     : 1; ///< [7]Inquiry for 测试图像, not supported is 0,supported is 1; 
	DWORD ShutterUnit                   : 1; ///< [6]Inquiry for 曝光时间单位, not supported is 0,supported is 1; 
	DWORD MirrorMode                    : 1; ///< [5]Inquiry for 镜像模式, not supported is 0,supported is 1;
	DWORD BinMode                       : 1; ///< [4]Inquiry for 调合模式, not supported is 0,supported is 1; 
	DWORD SkipMode                      : 1; ///< [3]Inquiry for 抽行模式, not supported is 0,supported is 1; 
	DWORD GlobalBlackLevel              : 1; ///< [2]Inquiry for 全局黑电平, not supported is 0,supported is 1; 
	DWORD PixelsSeparateBlackLevel      : 1; ///< [1]Inquiry for 单一通道黑电平控制, not supported is 0,supported is 1; 
	DWORD PixelsSeparateGain            : 1; ///< [0]Inquiry for 单一通道增益控制, not supported is 0,supported is 1; 
	/*ADVC_FTR_FEATURE_LO_INQ*/
	DWORD                               :32; ///< Reserved
}HV_RES_QUERY_ADV_FEATURE_PRESENCE;

/// DHCAM Advanced Feature ID
typedef enum tagHV_ADV_FEATURE_ID{
	ADV_FEATURE_ID_PIXEL_SEPARATE_GAIN         = 0,  ///< Specifies 单一通道增益控制
	ADV_FEATURE_ID_PIXEL_SEPARATE_BLACK_LEVEL  = 1,  ///< Specifies 单一通道黑电平控制
	ADV_FEATURE_ID_GLOBAL_BLACK_LEVEL          = 2,  ///< Specifies 全局黑电平
	ADV_FEATURE_ID_SKIP_MODE                   = 3,  ///< Specifies 抽行模式	
	ADV_FEATURE_ID_BIN_MODE                    = 4,  ///< Specifies 调合模式		
	ADV_FEATURE_ID_MIRROR_MODE                 = 5,  ///< Specifies 镜像模式
	ADV_FEATURE_ID_SHUTTER_UNIT                = 6,  ///< Specifies 曝光时间单位
	ADV_FEATURE_ID_TEST_IMAGE                  = 7,  ///< Specifies 测试图像
	ADV_FEATURE_ID_AD_CONTROL                  = 8,  ///< Specifies 噪声控制(位选择模式)		
	ADV_FEATURE_ID_MULTI_TRIGGER_NUM           = 9,  ///< Specifies 一次触发采集多帧功能,采集帧数ID
	ADV_FEATURE_ID_MULTI_TRIGGER_TIME          = 10, ///< Specifies 一次触发采集多帧功能，帧间隔ID		
	ADV_FEATURE_ID_DIGITAL_GAIN                = 11, ///< Specifies 数字移位功能
	ADV_FEATURE_ID_STARTUP_MEM_CH              = 12, ///< Specifies 上电参数组设定功能
	ADV_FEATURE_ID_DELAY_TRANSMIT              = 13, ///< Specifies 延迟传输
	ADV_FEATURE_ID_FILTER_TIME                 = 14, ///< Specifies 滤波功能,时间		
	ADV_FEATURE_ID_FILTER_TIME_UNIT            = 15, ///< Specifies 滤波功能,时间单位		
	ADV_FEATURE_ID_PIXEL_ENDIANESS             = 16, ///< Specifies 像素大小端模式功能
	ADV_FEATURE_ID_TRIGGER_SOURCE_STATUS       = 17, ///< Specifies 触发源状态查询 		
	ADV_FEATURE_ID_INTER_PACKET_DELAY          = 18, ///< Specifies 包间延迟功能查询
}HV_ADV_FEATURE_ID;

/// Advanced Feature INQ Descriptor
typedef union tagHV_ADV_FEATURE_DESCRIPTOR{
	struct {
		DWORD MaxValue       :12;  ///< [20..31]Maximum value for this feature
		DWORD MinValue       :12;  ///< [8..19]Minimum value for this feature
		DWORD ManualInq      : 1;  ///< [7]Availability of manual mode
		DWORD AutoInq        : 1;  ///< [6]Availability of automatic mode
		DWORD OnOffInq       : 1;  ///< [5]Availability of switching this feature on and off
		DWORD ReadOutInq     : 1;  ///< [4]Capability of reading the value of this feature
		DWORD OnePushInq     : 1;  ///< [3]Availability of one push automatic mode
		DWORD                : 1;  ///< [2]Reserved
		DWORD AbsControlInq  : 1;  ///< [1]Availability of absolute control
		DWORD PresenceInq    : 1;  ///< [0]Presence of this feature
	} Scalar;       ///< A scalar feature,for PixelSeparateGain,PixleSeparateBlackLevel,GlobalBlackLevel
                  ///< SkipMode,BinMode,DelayTransmit,StartUpMemCh,MultiTriggerNumb,MultiTriggerTime,DigitalGain,InterPacketDelay
	struct {
		DWORD MaxValue       :16;  ///< [16..31]Maximum value for this feature
		DWORD MinValue       : 8;  ///< [8..15]Minimum value for this feature
		DWORD ManualInq      : 1;  ///< [7]Availability of manual mode
		DWORD AutoInq        : 1;  ///< [6]Availability of automatic mode
		DWORD OnOffInq       : 1;  ///< [5]Availability of switching this feature on and off
		DWORD ReadOutInq     : 1;  ///< [4]Capability of reading the value of this feature
		DWORD OnePushInq     : 1;  ///< [3]Availability of one push automatic mode
		DWORD                : 1;  ///< [2]Reserved
		DWORD AbsControlInq  : 1;  ///< [1]Availability of absolute control
		DWORD PresenceInq    : 1;  ///< [0]Presence of this feature
	} FilterTime;   ///< The Filter time feature INQ

	struct {
		DWORD                 :10; ///< [22..31]Presence of mode n
		DWORD Mode5Inq        : 1; ///< [21]Presence of mode 5
		DWORD Mode4Inq        : 1; ///< [20]Presence of mode 4
		DWORD Mode3Inq        : 1; ///< [19]Presence of mode 3
		DWORD Mode2Inq        : 1; ///< [18]Presence of mode 2 
		DWORD Mode1Inq        : 1; ///< [17]Presence of mode 1
		DWORD Mode0Inq        : 1; ///< [16]Presence of mode 0
		DWORD                 : 8; ///< [8..15]Reserved
		DWORD ManualInq       : 1; ///< [7]Availability of manual mode
		DWORD AutoInq         : 1; ///< [6]Availability of automatic mode
		DWORD OnOffInq        : 1; ///< [5]Availability of switching this feature on and off
		DWORD ReadOutInq      : 1; ///< [4]Capability of reading the value of this feature
		DWORD OnePushInq      : 1; ///< [3]One push auto mode(Controlled automatically by camera only once)
		DWORD                 : 1; ///< [2]Reserved
		DWORD AbsControlInq   : 1; ///< [1]Availability of absolute control
		DWORD PresenceInq     : 1; ///< [0]Presence of this feature
	} Selector;   ///< Enum mode class feature,such as MirrorMode,ShutterUnit,TestImage,AdControl,FilterTimeUnit,PixelEndianess;
}HV_ADV_FEATURE_DESCRIPTOR;

/// Argument for the ORD_QUERY_ADV_FEATURE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR{
    HV_ADV_FEATURE_ID  AdvFeatureId;
}HV_ARG_QUERY_ADV_FEATURE_DESCRIPTOR;

/// Result of the ORD_QUERY_ADV_FEATURE_DESCRIPTOR function
typedef HV_ADV_FEATURE_DESCRIPTOR HV_RES_QUERY_ADV_FEATURE_DESCRIPTOR;

/// The DHCAM Advanced Feature CSR value,DHCAM offset=800h
typedef union tagHV_ADV_FEATURE{
	struct {
		DWORD BValue            : 6; ///< [26..31]蓝通道分量控制
		DWORD GbValue           : 6; ///< [20..25]
		DWORD GrValue           : 6; ///< [14..19]绿通道分量控制
		DWORD RValue            : 6; ///< [8..13]红通道分量控制
		DWORD AutoManualMode    : 1; ///< [7]Automatic mode - manual mode switch
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]Reserved
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} PixelSeparate;  ///< Pixels separate gain and Pixels_Separate_Black_Level feature

	struct {
		DWORD Value             :12; ///< [20..31]Value of this feature
		DWORD                   :12; ///< [8..19]Reserved
		DWORD AutoManualMode    : 1; ///< [7]Automatic mode - manual mode switch
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} Scalar;        ///< a scalar feature,such as GlobalBlackLevel,InterPacketDelayTime,MultiTriggerNumb,MultiTriggerIntervalTime,
	
	struct {
		DWORD ColumnSkip        :12; ///< [20..31]抽行数
		DWORD RowSkip           :12; ///< [8..19]抽列数
		DWORD AutoManualMode    : 1; ///< [7]Automatic mode - manual mode switch
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]Reserved
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	  } SkipBinMode; ///< for the feature:SkipMode,BinMode,
	
	struct {
		DWORD Parameter         :12; ///< [20..31]Parameter for mirror function ,if required.(Optional)
		DWORD                   : 4; ///< [16..19]
		DWORD Mode              : 4; ///< [12..15]Specifies the mode
		DWORD                   : 4; ///< [8..11]Reserved
		DWORD                   : 1; ///< [7]Reserved
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]Reserved
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} Selector;      ///< a enum mode feature,such as MirrorMode,ShutterUnit,TestImage,AdControl,FilterTimeUnit,PixelEndianess
	
	struct {
		DWORD                   :16; ///< [16..31]Reserved;
		DWORD DelayTime         :16; ///< [0..15]传输延迟时间;
	} DelayTransmit; ///< The Delay transmit function

	struct {
		DWORD Number            : 4; ///< [28..31]指定上电参数组号: 0-factory set(default)1、2、3-user set
		DWORD                   :27; ///< [1..27]Reserved;
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} StartupMemCh;  ///< The Delay transmit function
  
	struct {
		DWORD GainNum           : 8; ///< [24..31]数据向右移的位数:1、2、4、8、16。
		DWORD                   :17; ///< [7..23]Reserved;
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]Reserved
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} DigitalGain;  ///< for the feature:Digital Gain,  

	struct {
		DWORD Value             :16; ///< [16..31]0－2000（默认330ns）
		DWORD                   : 9; ///< [7..15]Reserved;
		DWORD OnOff             : 1; ///< [6]Switch to turn this feature on or off, off is 0, on is 1
		DWORD OnePush           : 1; ///< [5]One push automatic mode, off is 0, on is 1
		DWORD                   : 3; ///< [2..4]
		DWORD AbsControl        : 1; ///< [1]Absolute value control
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} FilterTime;  ///< for the feature:Filter time 
}HV_ADV_FEATURE;

/// The advanced feature ITEM id for set
typedef enum tagHV_ADV_FEATURE_ITEM_ID{	
	ADV_FEATURE_ITEM_ID_PIXEL_PRESENCE_INQ                            = 0x000000,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_ABS_CONTROL                  = 0x000001,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_ONE_PUSH                     = 0x000005,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_ON_OFF                       = 0x000006,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_AUTO_MANUAL_MODE_VALUE       = 0x000007,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_R_VALUE                      = 0x000008,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GR_VALUE                     = 0x00000E,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_GB_VALUE                     = 0x000014,
	ADV_FEATURE_ITEM_ID_PIXEL_SEPERARATE_B_VALUE                      = 0x00001A,

	ADV_FEATURE_ITEM_ID_SCALAR_PRESENCE_INQ                           = 0x010000,
	ADV_FEATURE_ITEM_ID_SCALAR_ABS_CONTROL                            = 0x010001,
	ADV_FEATURE_ITEM_ID_SCALAR_ONE_PUSH                               = 0x010005,
	ADV_FEATURE_ITEM_ID_SCALAR_ON_OFF                                 = 0x010006,
	ADV_FEATURE_ITEM_ID_SCALAR_AUTO_MANUAL_MODE_VALUE                 = 0x010007,
	ADV_FEATURE_ITEM_ID_SCALAR_VALUE                                  = 0x010014,
	
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_PRESENCE_INQ                    = 0x020000,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ABS_CONTROL                     = 0x020001,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ONE_PUSH                        = 0x020005,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ON_OFF                          = 0x020006,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_AUTO_MANUAL_MODE_VALUE          = 0x020007,    
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_ROW_SKIP                        = 0x020008,
	ADV_FEATURE_ITEM_ID_SKIP_BIN_MODE_COLUMN_SKIP                     = 0x020014,

	ADV_FEATURE_ITEM_ID_SELECTOR_PRESENCE_INQ                         = 0x030000,
	ADV_FEATURE_ITEM_ID_SELECTOR_ABS_CONTROL                          = 0x030001,
	ADV_FEATURE_ITEM_ID_SELECTOR_ONE_PUSH                             = 0x030005,
	ADV_FEATURE_ITEM_ID_SELECTOR_ON_OFF                               = 0x030006,
	ADV_FEATURE_ITEM_ID_SELECTOR_MODE                                 = 0x03000C,
	ADV_FEATURE_ITEM_ID_SELECTOR_PARAMETER                            = 0x030014,

	ADV_FEATURE_ITEM_ID_DELAY_TRANSMIT_DELAY_TIME                     = 0x040000,

	ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_PRESENCE_INQ                   = 0x050000,
	ADV_FEATURE_ITEM_ID_STARTUP_MEM_CH_NUMBER                         = 0x05001C,

	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_PRESENCE_INQ                     = 0x060000,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_ABS_CONTROL                      = 0x060001,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_ONE_PUSH                         = 0x060005,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_ON_OFF                           = 0x060006,
	ADV_FEATURE_ITEM_ID_DIGITAL_GAIN_GAIN_NUM                         = 0x060018,

	ADV_FEATURE_ITEM_ID_FILTER_TIME_PRESENCE_INQ                      = 0x070000,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_ABS_CONTROL                       = 0x070001,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_ONE_PUSH                          = 0x070005,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_ON_OFF                            = 0x070006,
	ADV_FEATURE_ITEM_ID_FILTER_TIME_VALUE                             = 0x070010,
}HV_ADV_FEATURE_ITEM_ID;

/// Argument for the ORD_GET_ADV_FEATURE function
typedef struct tagHV_ARG_GET_ADV_FEATURE{
    HV_ADV_FEATURE_ID      AdvFeatureId;
}HV_ARG_GET_ADV_FEATURE;

/// Result of the ORD_GET_ADV_FEATURE function
typedef HV_ADV_FEATURE HV_RES_GET_ADV_FEATURE;

/// Argument for the ORD_SET_ADV_FEATURE function
typedef struct tagHV_ARG_SET_ADV_FEATURE{
    HV_ADV_FEATURE_ID      AdvFeatureId;
	HV_ADV_FEATURE_ITEM_ID AdvFeatureItemId;
	HV_ADV_FEATURE         AdvFeature;
}HV_ARG_SET_ADV_FEATURE;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_ADV_BASIC_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_BASIC_FUNC_PRESENCE{
	DWORD                        :23; ///< [9..31]Reserved
	DWORD AsynResetCapture       : 1; ///< [8]Inquiry for 异步复位采集，not supported is 0,supported is 1
	DWORD MCUStatus              : 1; ///< [7]Inquiry for 状态查询功能, not supported is 0,supported is 1
	DWORD PIOInputStatus         : 1; ///< [6]Inquiry for 输入IO状态查询功能, not supported is 0,supported is 1
	DWORD FreezeFrame            : 1; ///< [5]Inquiry for 帧冻结功能, not supported is 0,supported is 1
	DWORD FpgaReset              : 1; ///< [4]Inquiry for FPGA软复位功能, not supported is 0,supported is 1
	DWORD DefaultMemchReset      : 1; ///< [3]Inquiry for 出厂默认值可修改功能, not supported is 0,supported is 1; 
	DWORD SIODebugEn             : 1; ///< [2]Inquiry for 输出信息开关功能,not supported is 0,supported is 1; 
	DWORD FlashIap               : 1; ///< [1]Inquiry for 在线烧写flash功能, not supported is 0,supported is 1; 
	DWORD DiyAdvFeatureInq       : 1; ///< [0]Inquiry for 自定义的高级功能开启INQ,not supported is 0,supported is 1
}HV_RES_QUERY_ADV_BASIC_FUNC_PRESENCE;

/// The advanced basic functions id
typedef enum tagHV_ADV_BASIC_FUNC_ID{
	ADV_BASIC_FUNC_ID_FALSH_IAP              = 1,
	ADV_BASIC_FUNC_ID_SIO_DEBUG_ENABLE       = 2,
	ADV_BASIC_FUNC_ID_DEFAULT_MEMCH_RESET    = 3,
	ADV_BASIC_FUNC_ID_FPGA_RESET             = 4,
	ADV_BASIC_FUNC_ID_FREEZE_FRAME           = 5,
	ADV_BASIC_FUNC_ID_PIO_INPUT_STATUS       = 6,
	ADV_BASIC_FUNC_ID_MCU_STATUS             = 7,
	ADV_BASIC_FUNC_ID_ASYNRESET_CAPTURE      = 8,
}HV_ADV_BASIC_FUNC_ID;

/// A union for the advanced basic function
typedef union tagHV_ADV_BASIC_FUNC{
	struct {
		DWORD dwLast;                     ///< 返回当前的工作状态码
	} MCUStatus;       ///< 只能Get，不能Set  

	struct { 
		DWORD Input0                 : 1; ///< [31]1-高电平	0-低电平
		DWORD Input1                 : 1; ///< [30]1-高电平	0-低电平
		DWORD Input2                 : 1; ///< [29]1-高电平	0-低电平 
		DWORD                        :28; ///< [1..28]Reserved
		DWORD PresenceInq            : 1; ///< [0]显示是否有此功能0：没有1：有						  
	} PIOInputStatus;  ///< 只能Get，不能Set 

	struct { 
		DWORD Count                  :16; ///< [16..31]冻结的帧数(0 ＝ 1)
		DWORD                        :15; ///< [1..15]Reserved
		DWORD StartFlag              : 1; ///< [0]启动帧冻结,1:冻结帧,0:取消冻结							  
	} FreezeFrame;

	struct { 
		DWORD                        :31; ///< [1..31]Reserved
		DWORD Enable                 : 1; ///< [0]1,fpga软复位，自清零							  
	} FPGAReset; 

	struct { 
		DWORD                        :31; ///< [1..31]Reserved
		DWORD Enable                 : 1; ///< [0]1,重新烧写出厂0参数组;0 默认							  
	} DefaultMemChReset;

	struct { 
		DWORD                        :31; ///< [1..31]Reserved
		DWORD Enable                 : 1; ///< [0]1,开启调试输出信息
	} SIODebug; 

	struct { 
		DWORD SaveDataSRAM           :16; ///< [16..31](0X0000FFFF)定义为存储数据到SRAM中；全0不操作
		DWORD Burn                   :16; ///< [0..15](0XFFFF0000)定义为烧写FLASH
	} FlashIAP;   
	struct {
		DWORD                        :31; ///< [1..31]Reserved
		DWORD Enable                 : 1; ///< [0]1,采集 异步复位，自清零	
	} AsynResetCapture;
}HV_ADV_BASIC_FUNC;

/// The advanced basic function ITEM id for set
typedef enum tagHV_ADV_BASIC_FUNC_ITEM_ID{
	ADV_BASIC_FUNC_ITEM_ID_FREEZE_FRAME_START_FLAG               = 0x020000,
	ADV_BASIC_FUNC_ITEM_ID_FREEZE_FRAME_COUNT                    = 0x020010,

	ADV_BASIC_FUNC_ITEM_ID_FPGA_RESET                            = 0x030000,

	ADV_BASIC_FUNC_ITEM_ID_DEFAULT_MEM_CHANNEL_RESET_ENALBE      = 0x040000,

	ADV_BASIC_FUNC_ITEM_ID_SIO_DEBUG_ENABLE                      = 0x050000,

	ADV_BASIC_FUNC_ITEM_ID_FLASH_IAP_BURN                        = 0x060000,
	ADV_BASIC_FUNC_ITEM_ID_FLASH_IAP_SAVE_DATA_SRAM              = 0x060010,
	
	ADV_BASIC_FUNC_ITEM_ID_ASYNRESET_CAPTURE                     = 0x070000,
}HV_ADV_BASIC_FUNC_ITEM_ID;

/// Argument for the ORD_GET_ADV_BASIC_FUNC function
typedef struct tagHV_ARG_GET_ADV_BASIC_FUNC{
	HV_ADV_BASIC_FUNC_ID       FuncId;
}HV_ARG_GET_ADV_BASIC_FUNC;

/// Result of the ORD_GET_ADV_BASIC_FUNC function
typedef HV_ADV_BASIC_FUNC HV_RES_GET_ADV_BASIC_FUNC ;

/// Argument for the ORD_SET_ADV_BASIC_FUNC function
typedef struct tagHV_ARG_SET_ADV_BASIC_FUNC{
	HV_ADV_BASIC_FUNC_ID       FuncId;
	HV_ADV_BASIC_FUNC_ITEM_ID  FuncItemId;
    HV_ADV_BASIC_FUNC          Func;
}HV_ARG_SET_ADV_BASIC_FUNC;



//////////////////////////////////////////////////////////////////////////
/// Result of ORD_QUERY_ADV_OPT_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_OPT_FUNC_PRESENCE{
	DWORD                      :24;       ///< [8..31]Reserved
	DWORD AutoFunc              : 1;       ///< [7]Inquiry for 自动处理控制功能，not supported is 0,supported is 1;
	DWORD ImageInfo            : 1;       ///< [6]Inquiry for 图像信息，not supported is 0,supported is 1;
	DWORD                      : 1;       ///< [5]Reserved
	DWORD                      : 1;       ///< [4]Reserved
	DWORD UserSWEncrypt        : 1;       ///< [3]Inquiry for 用户软件加密区, not supported is 0,supported is 1;
	DWORD UserHWEncrypt        : 1;       ///< [2]Inquiry for 用户硬件加密区, not supported is 0,supported is 1;
	DWORD UserCustom           : 1;       ///< [1]Inquiry for 用户自定义区, not supported is 0,supported is 1; 
	DWORD Lut                  : 1;       ///< [0]Inquiry for 查找表功能,not supported is 0,supported is 1; 
}HV_RES_QUERY_ADV_OPT_FUNC_PRESENCE;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_ADV_LUT_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_ADV_LUT_DESCRIPTOR{
	DWORD TotalByteLengthInq        :24;  ///< [8..31]LUT长度,单位是BYTE
    DWORD BitDepthInq               : 8;  ///< [0..7]查找表位数
}HV_RES_QUERY_ADV_LUT_DESCRIPTOR;

/// Argument for the ORD_GET_ADV_LUT function
typedef struct tagHV_ARG_GET_ADV_LUT{
	DWORD dwOffset;                       ///< 查找表偏移地址
	int   nLength;                        ///< 查找表数据长度 
}HV_ARG_GET_ADV_LUT;

/// Result of the ORD_GET_ADV_LUT function
typedef struct tagHV_RES_GET_ADV_LUT{
	DWORD *pBuffer;                       ///< LUT数据缓冲区指针
}HV_RES_GET_ADV_LUT;

/// Argument for the ORD_SET_ADV_LUT function
typedef struct tagHV_ARG_SET_ADV_LUT{
	DWORD dwOffset;                       ///< 查找表偏移地址
	int   nLength;                        ///< 查找表数据长度 
	DWORD *pBuffer;                       ///< LUT数据缓冲区指针    
}HV_ARG_SET_ADV_LUT;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_ADV_USER_CUSTOM_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR{
	DWORD TotalByteLengthInq     :24;     ///< [8..31]用户自定义区总长度,单位是BYTE
	DWORD BitDepthInq            : 8;     ///< [0..7]用户自定义区位数
}HV_RES_QUERY_ADV_USER_CUSTOM_DESCRIPTOR;

/// Argument for the ORD_GET_ADV_USER_CUSTOM function
typedef struct tagHV_ARG_GET_ADV_USER_CUSTOM{
	DWORD dwOffset;                       ///< 用户自定义区偏移地址
	int   nLength;                        ///< 用户自定义区数据长度 
}HV_ARG_GET_ADV_USER_CUSTOM;

/// Result of the ORD_GET_ADV_USER_CUSTOM function
typedef struct tagHV_RES_GET_ADV_USER_CUSTOM{
	DWORD *pBuffer;                       ///< USER_CUSTOM数据缓冲区指针
}HV_RES_GET_ADV_USER_CUSTOM;

/// Argument for the ORD_SET_ADV_USER_CUSTOM function
typedef struct tagHV_ARG_SET_ADV_USER_CUSTOM{
	DWORD dwOffset;                       ///< 用户自定义区偏移地址
	int   nLength;                        ///< 用户自定义区数据长度 
	DWORD *pBuffer;                       ///< 用户自定义区数据缓冲区指针    
}HV_ARG_SET_ADV_USER_CUSTOM;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE function
typedef struct tagHV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE{ 
	DWORD                  :29;       ///< [3..31]Reserved
	DWORD Zone2            : 1;       ///< [2]说明ZONE的2区是否可用,0不可用,1可用;
	DWORD Zone1            : 1;       ///< [1]说明ZONE的1区是否可用,0不可用,1可用;
	DWORD Zone0            : 1;       ///< [0]说明ZONE的0区是否可用,0不可用,1可用;
}HV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_PRESENCE;

/// The index of the encrypt zone 加密区编号ID
typedef enum tagHV_ADV_USER_ENCRYPT_ZONE_ID{
	ADV_USER_ENCRYPT_ZONE0      = 0,
	ADV_USER_ENCRYPT_ZONE1      = 1,
	ADV_USER_ENCRYPT_ZONE2      = 2,
}HV_ADV_USER_ENCRYPT_ZONE_ID;

/// Argument for the ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR{
    HV_ADV_USER_ENCRYPT_ZONE_ID  ZoneId;   ///< 用户加密区Zone的ID号
}HV_ARG_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR;

/// Result of the ORD_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR function
typedef struct tagHV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR{
    DWORD dwLengthInq ;                   ///< 用户加密区每个Zone的长度，字节数
}HV_RES_QUERY_ADV_USER_ENCRYPT_ZONE_DESCRIPTOR;

/// The struct of the user encrypt zone  functions id
typedef enum tagHV_ADV_USER_ENCRYPT_ZONE_FUNC_ID{
	ADV_USER_ENCRYPT_ZONE_FUNC_ID_CHANGE_PASSWORD      =  0,   ///< 修改用户加密区密码
	ADV_USER_ENCRYPT_ZONE_FUNC_ID_DATA                 =  1,   ///< 读写用户加密区数据		
}HV_ADV_USER_ENCRYPT_ZONE_FUNC_ID;

typedef struct tagHV_ADV_USER_ENCRYPT_ZONE_PASSWORD{
	DWORD                 : 8;  ///< [24..31]Reserved
	DWORD  Value          :24;  ///< [0..23]操作需要输入的密码
}HV_ADV_USER_ENCRYPT_ZONE_PASSWORD;

/// The Input parameter for read zone data
typedef union tagHV_GET_ZONE_PARAMETER{
	struct {
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD Password;    ///< 读加密区需要的读密码
		DWORD dwOffset;                                ///< 用户加密数据区偏移地址i
		int   nLength;                                 ///< 用户加密数据的长度 
	}UserData;  ///< 用户加密区数据
}HV_GET_ZONE_PARAMETER;                

/// Argument for the ORD_GET_ADV_USER_ENCRYPT_ZONE function
typedef struct tagHV_ARG_GET_ADV_USER_ENCRYPT_ZONE{
    HV_ADV_USER_ENCRYPT_ZONE_ID           ZoneId;      ///< 用户加密区Zone的ID号
	HV_ADV_USER_ENCRYPT_ZONE_FUNC_ID      ZoneFuncId;  ///< 用户加密区功能ID，只能读数据
    HV_GET_ZONE_PARAMETER                 Parameter;   ///< 读用户加密区参数             
}HV_ARG_GET_ADV_USER_ENCRYPT_ZONE;

/// Result of the ORD_GET_ADV_USER_ENCRYPT_ZONE function
typedef struct tagHV_RES_GET_ADV_USER_ENCRYPT_ZONE{
	DWORD *pBuffer;                                    ///< 用户加密数据区数据缓冲区指针 
}HV_RES_GET_ADV_USER_ENCRYPT_ZONE;

/// The union for the SET user encrypt zone,该联合结构作为输入结构参数，对应于相应的子功能ID
typedef union tagHV_SET_ZONE_PARAMETER{
	struct { 
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD  Old;        ///< 旧密码
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD  New;        ///< 新密码	
	} Password;     ///< 密码结构，修改密码时使用,针对设置密码子功能
  
	struct { 
		HV_ADV_USER_ENCRYPT_ZONE_PASSWORD   Password;  ///< 写加密区需要的写密码
		DWORD dwOffset;                                ///< 用户加密数据区偏移地址 
		int   nLength;                                 ///< 用户加密数据的长度 
		DWORD *pBuffer;                                ///< 用户加密数据区数据缓冲区指针 	
	} UserData;     ///< 用户加密数据结构定义
}HV_SET_ZONE_PARAMETER;  

/// Argument for the ORD_SET_ADV_USER_ENCRYPT_ZONE function
typedef struct tagHV_ARG_SET_ADV_USER_ENCRYPT_ZONE{
    HV_ADV_USER_ENCRYPT_ZONE_ID           ZoneId;      ///< 用户加密区Zone的ID号
	HV_ADV_USER_ENCRYPT_ZONE_FUNC_ID      ZoneFuncId;  ///< 用户加密区自功能ID，说明要进行什么操作
    HV_SET_ZONE_PARAMETER                 Parameter;   ///< 对应子功能相应的参数
}HV_ARG_SET_ADV_USER_ENCRYPT_ZONE;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_NET_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_NET_FUNC_PRESENCE{
	DWORD                              :12;  ///< Reserved
	DWORD DeviceRequestTimeout         : 1;  ///< Inquiry for 请求超时设置功能,not supported is 0,supported is 1;
	DWORD DeviceAnswerTimeout          : 1;  ///< Inquiry for 应答超时设置功能,not supported is 0,supported is 1; 	
	DWORD SnapPort                     : 1;  ///< Inquiry for 采集端口号读写功能,not supported is 0,supported is 1;
	DWORD ControlPort                  : 1;  ///< Inquiry for 控制端口号读写功能,not supported is 0,supported is 1;
	DWORD HostMAC                      : 1;  ///< Inquiry for 主机MAC地址读取功能,not supported is 0,supported is 1;
	DWORD HostIP                       : 1;  ///< Inquiry for 主机IP地址读取功能,not supported is 0,supported is 1; 
	DWORD DeviceMAC                    : 1;  ///< Inquiry for 设备MAC地址读取功能,not supported is 0,supported is 1;
	DWORD DeviceIP                     : 1;  ///< Inquiry for 设备IP地址读取功能,not supported is 0,supported is 1; 
	DWORD HeartBeatExpiration          : 1;  ///< Inquiry for 心跳期满设置功能,not supported is 0,supported is 1;
	DWORD HeartBeatInterval            : 1;  ///< Inquiry for 心跳间隔设置功能,not supported is 0,supported is 1; 
	DWORD CalculatedResult             : 1;  ///< Inquiry for 包间延迟结果读取功能,not supported is 0,supported is 1;
	DWORD LinkSpeed                    : 1;  ///< Inquiry for 链接速度设置功能,not supported is 0,supported is 1; 	
	DWORD ExpectDataRate               : 1;  ///< Inquiry for 预期数据率设置功能,not supported is 0,supported is 1;
	DWORD InterPacketDelay             : 1;  ///< Inquiry for 包间延迟设置功能,not supported is 0,supported is 1; 
	DWORD FirstPacketTimeout           : 1;  ///< Inquiry for 第一个包超时设置功能,not supported is 0,supported is 1;
	DWORD PacketTimeout                : 1;  ///< Inquiry for 包超时设置功能,not supported is 0,supported is 1; 	
	DWORD HostRequestTimeout           : 1;  ///< Inquiry for 请求超时设置功能,not supported is 0,supported is 1;
	DWORD HostAnswerTimeout            : 1;  ///< Inquiry for 应答超时设置功能,not supported is 0,supported is 1; 	
	DWORD CmdRetryCount                : 1;  ///< Inquiry for 命令重传次数设置功能,not supported is 0,supported is 1;
	DWORD PacketSize                   : 1;  ///< Inquiry for 网络包长读写功能,not supported is 0,supported is 1; 	
}HV_RES_QUERY_NET_FUNC_PRESENCE;

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
	NET_FUNC_ID_DEVICE_REQUEST_TIMEOUT = 19,

}HV_NET_FUNC_ID;


/// Net Function INQ Descriptor
typedef union tagHV_NET_FUNC_DESCRIPTOR{
	struct {
		DWORD dwMaxValue;           ///< Maximum value for this feature
		DWORD dwMinValue;           ///< Minimum value for this feature  
	} Scalar;      ///< a scalar function,for PacketSize,CmdRetryCount,AnswerTimeout,RequestTimeout,ControlPort,SnapPort
	              
	struct {	
		DWORD                  :29; ///< Reserved
		DWORD MBits1000Inq     : 1; ///< Presence of 1000 MBits/seconds 
		DWORD MBits100Inq      : 1; ///< Presence of 100 MBits/seconds
		DWORD MBits10Inq       : 1; ///< Presence of 10 MBits/seconds  
	} LinkSpeed;   ///< Enum mode LinkSpeed;

	struct {
		DWORD dwTotalByteLength;    ///< IP地址，mac地址总字节长度,不包含结尾符'\0'		 
	} Address;     ///< for the IP and MAC adress string

	struct {
	} Port;        ///< Reserved
}HV_NET_FUNC_DESCRIPTOR;

/// Argument for the ORD_QUERY_NET_FUNC_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_NET_FUNC_DESCRIPTOR{
    HV_NET_FUNC_ID   NetFuncId;
}HV_ARG_QUERY_NET_FUNC_DESCRIPTOR;

/// Result of ORD_QUERY_NET_FUNC_DESCRIPTOR 
typedef HV_NET_FUNC_DESCRIPTOR HV_RES_QUERY_NET_FUNC_DESCRIPTOR;

/// Net Function CSR value 
typedef union tagHV_NET_FUNC{
	struct {
		DWORD dwValue;    ///< The value to be set or get 
	} Scalar;

	struct {
		DWORD dwValue;    ///< Link speed for set or get,0:10Mbps,1:100Mbps,2:1000Mbps
	} LinkSpead;

	struct {
		char  *pBuffer;   ///< IP地址和MAC地址字符串缓冲区指针
	} Address;
	
	struct {
        int   nNumber;    ///< The port number
	} Port;               ///< for the snap port and control port
}HV_NET_FUNC;

/// Argument for the ORD_GET_NET_FUNC function
typedef struct tagHV_ARG_GET_NET_FUNC{
    HV_NET_FUNC_ID NetFuncId;
	int            nLength;       ///< 对应于Get IP地址和MAC地址子功能需要输入的字符串长度参数，其他子功能该参数无效
}HV_ARG_GET_NET_FUNC;

/// Result of ORD_GET_NET_FUNC function
typedef HV_NET_FUNC HV_RES_GET_NET_FUNC;

/// Argument for the ORD_SET_NET_FUNC function
typedef struct tagHV_ARG_SET_NET_FUNC{
	HV_NET_FUNC_ID NetFuncId;
	HV_NET_FUNC    NetFunc;
	int            nLength;      ///< 对应于Set IP地址和MAC地址子功能需要输入的字符串长度参数，其他子功能该参数无效
}HV_ARG_SET_NET_FUNC;
 
//////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_IMAGE_INFO_PRESENCE function
typedef struct tagHV_RES_QUERY_IMAGE_INFO_PRESENCE{
    DWORD                             : 28; // 保留
	DWORD TimeLabel_MultiTrigger_N    : 1;  // 触发采多帧帧标号
	DWORD TimeLabel_Trigger_N         : 1;  // 触发个数标号
	DWORD TimeLabel_N                 : 1;  // 帧号
	DWORD TimeLabel_T                 : 1;  // 帧间隔
	DWORD Reserved[7];                      // 保留
}HV_RES_QUERY_IMAGE_INFO_PRESENCE;

/// The Image information id
typedef enum tagHV_IMAGE_INFO_ID{
	IMAGE_INFO_ID_ALL             = 256,
		IMAGE_INFO_ID_TIME_LABEL_T    = 0,    
		IMAGE_INFO_ID_TIME_LABEL_N    = 1, 
		IMAGE_INFO_ID_TIME_LABEL_TRIGGER_N  = 2,
		IMAGE_INFO_ID_TIME_LABEL_MUTI_TRIGGER_N  =  3,			
}HV_IMAGE_INFO_ID;

/// Argument for the ORD_QUERY_IMAGE_INFO_DESCRIPTOR function
typedef union tagHV_IMAGE_INFO_DESCRIPTOR{
	struct {
		DWORD  Offset; //<图像信息起始偏移
		DWORD  Length; //<图像信息大小，单位：字节
	} Scalar;
}HV_IMAGE_INFO_DESCRIPTOR;

/// Argument for the ORD_GET_IMAGE_INFO function
typedef union tagHV_IMAGE_INFO{
	struct {
		void *pBuff; // 接收缓冲区
	} Scalar;
}HV_IMAGE_INFO;

/// Argument for the ORD_QUERY_IMAGE_INFO_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_IMAGE_INFO_DESCRIPTOR{
	HV_IMAGE_INFO_ID  Id; 
}HV_ARG_QUERY_IMAGE_INFO_DESCRIPTOR;

/// Result of the ORD_QUERY_IMAGE_INFO_DESCRIPTOR function
typedef HV_IMAGE_INFO_DESCRIPTOR  HV_RES_QUERY_IMAGE_INFO_DESCRIPTOR;

/// Argument for the ORD_GET_IMAGE_INFO function
typedef HV_ARG_QUERY_IMAGE_INFO_DESCRIPTOR HV_ARG_GET_IMAGE_INFO;

/// Result of the ORD_GET_IMAGE_INFO function
typedef HV_IMAGE_INFO HV_RES_GET_IMAGE_INFO;



//////////////////////////////////////////////////////////////////////////
/// Result of the ORD_QUERY_AUTO_FUNC_PRESENCE function
/// Result of the ORD_QUERY_AUTO_FUNC_PRESENCE function
typedef struct tagHV_RES_QUERY_AUTO_FUNC_PRESENCE{
	DWORD                                :29;  ///< [3..31]Reserved
	DWORD AutoFuncAOI                    : 1;  ///< [2]Inquiry for 自动功能AOI功能,not supported is 0,supported is 1; 	
	DWORD AutoShutterCtrl                : 1;  ///< [1]Inquiry for 自动曝光控制功能,not supported is 0,supported is 1;
	DWORD AutoGainCtrl                   : 1;  ///< [0]Inquiry for 自动增益控制功能,not supported is 0,supported is 1; 	
}HV_RES_QUERY_AUTO_FUNC_PRESENCE;

///The Auto Func ID
typedef enum tagHV_AUTO_FUNC_ID{
	AUTO_FUNC_ID_AUTO_GAIN_CTRL              = 0, ///< Specifies 自动增益范围控制
	AUTO_FUNC_ID_AUTO_SHUTTER_CTRL           = 1, ///< Specifies 自动曝光范围控制
	AUTO_FUNC_ID_AUTO_FUNC_AOI               = 2, ///< Specifies 自动功能感兴趣区域控制

}HV_AUTO_FUNC_ID;

/// Argument for the ORD_QUERY_AUTO_FUNC_DESCRIPTOR function
typedef union tagHV_AUTO_FUNC_DESCRIPTOR{
	struct {
		DWORD MaxValue       :12;  ///< [20..31]Maximum value for this feature
		DWORD MinValue       :12;  ///< [8..19]Minimum value for this feature
		DWORD ManualInq      : 1;  ///< [7]Availability of manual mode
		DWORD AutoInq        : 1;  ///< [6]Availability of automatic mode
		DWORD OnOffInq       : 1;  ///< [5]Availability of switching this feature on and off
		DWORD ReadOutInq     : 1;  ///< [4]Capability of reading the value of this feature
		DWORD OnePushInq     : 1;  ///< [3]Availability of one push automatic mode
		DWORD                : 1;  ///< [2]Reserved
		DWORD AbsControlInq  : 1;  ///< [1]Availability of absolute control
		DWORD PresenceInq    : 1;  ///< [0]Presence of this feature
	} Scalar;       ///<AutoGainCtrl,AutoShutterCtrl

///////////////////////////////////////////////////////////////////
    // 其他不规则的寄存器块，使用下面的结构
	struct 
	{
		DWORD length;
		DWORD *pBuff; //指向
	}Block;
	
}HV_AUTO_FUNC_DESCRIPTOR;

typedef	struct {
		DWORD VmaxInq         :16;  ///< [16..31],Maximum Vertical pixel number
		DWORD HmaxInq         :16;  ///< [0..15],Maximum Horizontal pixel number
		DWORD VunitInq        :16;  ///< [16..31],Vertical unit pixel number
		DWORD HunitInq        :16;  ///< [0..15],Horizontal unit pixel number 
} HV_AUTO_FUNC_DESCRIPTOR_AOI;    ///< The Auto_Func_AOI descriptor struct



/// Argument for the ORD_QUERY_AUTO_FUNC_DESCRIPTOR function
typedef struct tagHV_ARG_QUERY_AUTO_FUNC_DESCRIPTOR{
    HV_AUTO_FUNC_ID  AutoFuncId;
}HV_ARG_QUERY_AUTO_FUNC_DESCRIPTOR;

/// Result of the ORD_QUERY_AUTO_FUNC_DESCRIPTOR function
typedef HV_AUTO_FUNC_DESCRIPTOR HV_RES_QUERY_AUTO_FUNC_DESCRIPTOR;

/// The DHCAM AUTO FUNC CSR value
typedef union tagHV_AUTO_FUNC{
	struct {
		DWORD MinValue          :12; ///< [12..31] Minimum auto gain value
		DWORD                   : 4; ///< [16..19] Reserved
		DWORD MaxValue          :12; ///< [4..15]  Maximum auto gain value
		DWORD                   : 3; ///< [1..3]   Reserved
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature
	} AutoGainCtrl;
 
 /////////////////////////////////////////////////////////////////
    // 其他不规则的寄存器块，使用下面的结构
 struct 
 {
  DWORD length;
  DWORD *pBuff; //指向HV_AUTO_FUNC_SHUTTER_CTRL，HV_AUTO_FUNC_AOI
 }Block;

}HV_AUTO_FUNC;

typedef struct {
  DWORD MaxValue          :32; ///< [0..31] Maximum auto shutter value
  DWORD MinValue          :32; ///< [0..31] Minimum auto shutter value
  DWORD                   :31; ///< [1..31] Reserved
  DWORD PresenceInq       : 1; ///< [0]Presence of this feature
}HV_AUTO_FUNC_SHUTTER_CTRL;


typedef	struct {
		DWORD Height            :16; ///< [16..31] 
		DWORD Width             :16; ///< [0..15]
			
		DWORD Top               :16; ///< [16..31] 
		DWORD Left              :16; ///< [0..15]
			
		DWORD                   :24; ///< [7..31] Reserved
		DWORD OnOff             : 1; ///< [6] Enable/Disable AOI
		DWORD                   : 1; ///< [5] Reserved
		DWORD ShowWorkArea      : 1; ///< [4] Show work area
		DWORD                   : 3; ///< [1..3] Reserved 
		DWORD PresenceInq       : 1; ///< [0]Presence of this feature	
	} HV_AUTO_FUNC_AOI;



/// The auto func ITEM id for set
typedef enum tagHV_AUTO_FUNC_ITEM_ID{	
  AUTO_FUNC_ITEM_ID_AUTO_GAIN_PRESENCE_INQ                        = 0x000000,
	AUTO_FUNC_ITEM_ID_AUTO_GAIN_MAX_VALUE                           = 0x000004,
	AUTO_FUNC_ITEM_ID_AUTO_GAIN_MIN_VALUE                           = 0x000014,
	
	AUTO_FUNC_ITEM_ID_AUTO_SHUTTER_PRESENCE_INQ                     = 0x010000,
	AUTO_FUNC_ITEM_ID_AUTO_SHUTTER_MIN_VALUE                        = 0x020000,
	AUTO_FUNC_ITEM_ID_AUTO_SHUTTER_MAX_VALUE                        = 0x030000,
	
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_PRESENCE_INQ                    = 0x040000,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_SHOWWORKAREA                    = 0x040004,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_ON_OFF                          = 0x040006,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_LEFT                            = 0x050000,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_TOP                             = 0x050010,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_WIDTH                           = 0x060000,
	AUTO_FUNC_ITEM_ID_AUTO_FUNC_AOI_HEIGHT                          = 0x060010,
	
}HV_AUTO_FUNC_ITEM_ID;

/// Argument for the ORD_GET_AUTO_FUNC function
typedef struct tagHV_ARG_GET_AUTO_FUNC{
    HV_AUTO_FUNC_ID      AutoFuncId;
}HV_ARG_GET_AUTO_FUNC;

/// Result of the ORD_GET_AUTO_FUNC function
typedef HV_AUTO_FUNC HV_RES_GET_AUTO_FUNC;

/// Argument for the ORD_SET_AUTO_FUNC function
typedef struct tagHV_ARG_SET_AUTO_FUNC{
    HV_AUTO_FUNC_ID       AutoFuncId;
    HV_AUTO_FUNC_ITEM_ID  AutoFuncItemId;
    HV_AUTO_FUNC          AutoFunc;
}HV_ARG_SET_AUTO_FUNC;

/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif
