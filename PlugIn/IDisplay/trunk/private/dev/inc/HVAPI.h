//---------------------------------------------------------
//   Company:  Daheng Imavision 
//   Project:  HVAPI
//   $Author:  zhou zhong ya$
//   $Date:  02/21/06 14:04$
//---------------------------------------------------------
/**
  *  \file     HVAPI.h
  *
  *  \brief  Interface for user
  *
  *  This file defines macros and function prototype for 
  *  Daheng Imavision Digital Camera driver.
  */
//---------------------------------------------------------


#if !defined(HVAPI_H)         //
#define HVAPI_H

/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif

typedef HANDLE HHV;

/// Different types of IsSupported inquiries
typedef enum tagHV_SUPPORTED {
  INQ_PRESENY,
  INQ_ABS_CONTROL,
  INQ_ONE_PUSH,
  INQ_READ_OUT,
  INQ_ON_OFF,
  INQ_AUTO,
  INQ_MANUAL,
  INQ_POLARITY,
  /////////////////////////////
  INQ_SNAP_SPEED_1        = 1,
  INQ_SNAP_SPEED_2        = 2,
  INQ_SNAP_SPEED_3        = 3,
  INQ_SNAP_SPEED_4        = 4,
  INQ_SNAP_SPEED_5        = 5,
  INQ_SNAP_SPEED_6        = 6,
  INQ_SNAP_SPEED_7        = 7,
  INQ_SNAP_SPEED_8        = 8,
  ////////////////////////////
  INQ_ADC_LEVEL_1         = 1,
  INQ_ADC_LEVEL_2         = 2,
  INQ_ADC_LEVEL_3         = 3,
  INQ_ADC_LEVEL_4         = 4,
  INQ_ADC_LEVEL_5         = 5,
  INQ_ADC_LEVEL_6         = 6,
  INQ_ADC_LEVEL_7         = 7,
  INQ_ADC_LEVEL_8         = 8
} HV_SUPPORTED;


/// Trigger modes defined by DCAM
typedef enum tagHV_TRIGGER_MODE {
  TRIGGER_MODE0 = 0,
  TRIGGER_MODE1,
  TRIGGER_MODE2,
  TRIGGER_MODE3
} HV_TRIGGER_MODE;


/// Trigger polarity
typedef enum tagHV_POLARITY {
  LOW  = 0,
  HIGH = 1 
} HV_POLARITY;

  
/// Video formats as defined by DCAM
typedef enum tagHV_VIDEO_FORMAT {
  VIDEO_FORMAT0 = 0,
  VIDEO_FORMAT1 = 1,
  VIDEO_FORMAT2 = 2,
  VIDEO_FORMAT6 = 6,
  VIDEO_FORMAT7 = 7
} HV_VIDEO_FORMAT;

  
/// Modes of a video format as defined by DCAM
typedef enum tagHV_VIDEO_MODE {
  IGNORE_VIDEO_MODE = -1,
  VIDEO_MODE0       = 0,
  VIDEO_MODE1       = 1,
  VIDEO_MODE2       = 2,
  VIDEO_MODE3       = 3,
  VIDEO_MODE4       = 4,
  VIDEO_MODE5       = 5,
  VIDEO_MODE6       = 6,
  VIDEO_MODE7       = 7
} HV_VIDEO_MODE;


/// Fixed frame rates as defined by DCAM
typedef enum tagHV_VIDEO_FRAME_RATE {
  IGNORE_FRAME_RATE = -1,
  RATE_1_875FPS     = 0,
  RATE_3_75FPS,
  RATE_7_5FPS,
  RATE_15FPS,
  RATE_30FPS,
  RATE_60FPS
} HV_VIDEO_FRAME_RATE;


/** These are the color modes, as defined by the Digital Camera Specification. */
typedef enum tagHV_COLOR_CODE {
  COLOR_MONO8 = 0,     ///< Y component has 8bit data
  COLOR_YUV8_4_1_1,    ///< 4:1:1 YUV 8 format, each component has 8bit data
  COLOR_YUV8_4_2_2,    ///< 4:2:2 YUV 8 format, each component has 8bit data
  COLOR_YUV8_4_4_4,    ///< 4:4:4 YUV 8 format, each component has 8bit data
  COLOR_RGB8,          ///< RGB 8 format, each component has 8bit data
  COLOR_MONO16,        ///< Y component has 16bit data
  COLOR_RGB16          ///< RGB 16 format, each component has 16bit data
} HV_COLOR_CODE;


/** Isochronous speed encoding */
typedef enum tagHV_ISO_SPEED {
  ISO_SPEED_UNKNOWN,
  ISO_SPEED_100,   ///< 100Mbit/s
  ISO_SPEED_200,   ///< 200Mbit/s
  ISO_SPEED_400,   ///< 400Mbit/s
  ISO_SPEED_800,   ///< 800Mbit/s
  ISO_SPEED_1600,  ///< 1600Mbit/s
  ISO_SPEED_3200   ///< 3200Mbit/s
} HV_ISO_SPEED;


/** Identifiers used to query the camera for string data */
typedef enum tagHV_STRING_ID {
  STRING_VENDOR_NAME,    ///< Specifies the camera vendor name
  STRING_MODEL_NAME,     ///< Specifies the camera model name
  STRING_CAMERA_NAME     ///< Specifies the camera name
} HV_STRING_ID;


typedef enum tagHV_INFO_ID {
  INFO_TYPE,
  INFO_MARK,
  INFO_SERIES_NUMBER,
  INFO_HARDWARE_VERSION
} HV_INFO_ID;

typedef union tagHV_CAMERA_INFO {
//  HVTYPE Type;                ///< Specifies the camera type
  WORD   Mark;                ///< Specifies the camera mark
  BYTE   Series[12];          ///< Specifies the camera serials number
  WORD   HardwareVersion;     ///< Specifies the camera hardware version
} HV_CAMERA_INFO;


/// A struct describing the image layout in format 7
typedef struct tagHV_VIDEO_FORMAT7_DESCRIPTOR {
  ULONG nTotalPixX          ///< Width in pixels
    , nTotalPixY;           ///< Height in pixels
  ULONG nSizeIncX,          ///< Increment for the image width
    nSizeIncY;              ///< Increment for the image height
  ULONG nPosIncX,           ///< Increment for the x-position
    nPosIncY;               ///< Increment for the y-position
  ULONG ColorCodingMask[8]; ///< Bit field describing the supported color modes
} HV_VIDEO_FORMAT7_DESCRIPTOR;


/// Bandwidth and buffer related data 
typedef struct tagHV_BW_DESC {
  ULONGLONG nBytesTotalFrameSize; ///< Image size in bytes
  ULONG nBytesPerPacketMax;       ///< Maximum packet size in bytes
  ULONG nBytesPerPacketRec;       ///< Recommended packet size in bytes
  ULONG nBytesPerPacketUnit;      ///< Increment for the packet size in bytes
  ULONG nBytesPerPacketCurrent;   ///< Current packet size in bytes
  ULONG nPacketsPerFrame;         ///< Number of packet per image
} HV_BW_DESC;



typedef enum tagHV_CHANNEL { 
  RED_CHANNEL      = 0x10,
  GREEN_CHANNEL1   = 0x11,   
  GREEN_CHANNEL2   = 0x12,
  BLUE_CHANNEL     = 0x13
} HV_CHANNEL;


/// Video mirror feature
typedef struct tagHV_VIDEO_MIRROR {
  ULONG                 : 15;   
  ULONG Mirror          :  1;   ///< Video mirror direction
  ULONG                 :  6;   
  ULONG MirrorInq2      :  1;   ///< Presence of video mirror 2
  ULONG MirrorInq1      :  1;   ///< Presence of video mirror 1
  ULONG                 :  7;
  ULONG Presence_Inq    :  1;   ///< Presence of this feature
} HV_VIDEO_MIRROR ;    


/// Blanking feature
typedef struct tagHV_BLANKING {
  ULONG                 : 15;   
  ULONG Mirror          :  1;   ///< Video mirror direction
  ULONG                 :  6;   
  ULONG MirrorInq2      :  1;   ///< Presence of video mirror 2
  ULONG MirrorInq1      :  1;   ///< Presence of video mirror 1
  ULONG                 :  7;
  ULONG Presence_Inq    :  1;   ///< Presence of this feature
} HV_BLANKING ;    


/** Feature ID codes */
typedef enum tagHV_FEATURE_ID {
  /* Standard features */
  FEATURE_ID_CAPTURE_QUALITY = 0x0E, ///< Specifies the image quality for format 6
  FEATURE_ID_CAPTURE_SIZE,           ///< Specifies the image size for format 6
  FEATURE_ID_OPTICAL_FILTER = 0x1C,  ///< Specifies the optical filter control
  FEATURE_ID_TILE,                   ///< Specifies the tilt control
  FEATURE_ID_PAN,                    ///< Specifies the pan control
  FEATURE_ID_ZOOM,                   ///< Specifies the zoom control
  FEATURE_ID_TRIGGER = 0x33,         ///< Specifies the trigger control
  FEATURE_ID_TEMPERATURE,            ///< Specifies the temperature control
  FEATURE_ID_FOCUS,                  ///< Specifies the focus control
  FEATURE_ID_IRIS,                   ///< Specifies the iris control
  FEATURE_ID_GAIN,                   ///< Specifies the gain control
  FEATURE_ID_SHUTTER,                ///< Specifies the shutter control
  FEATURE_ID_GAMMA,                  ///< Specifies the gamma control
  FEATURE_ID_SATURATION,             ///< Specifies the saturation control
  FEATURE_ID_HUE,                    ///< Specifies the hue control
  FEATURE_ID_WHITE_BALANCE,          ///< Specifies the white balance control
  FEATURE_ID_SHARPNESS,              ///< Specifies the sharpness control
  FEATURE_ID_AUTO_EXPOSURE,          ///< Specifies the auto exposure control
  FEATURE_ID_BRIGHTNESS,             ///< Specifies the brightness control
} HV_FEATURE_ID;


/// Values used by switches
typedef enum tagHV_ON_OFF{
  OFF = 0,
  ON
} HV_ON_OFF;


/** Rectangle --- used for AOI */
typedef struct tagHV_RECT{
  ULONG X0,		///< Left, x-coordinate of top-left corner
		Y0,     ///< Top, y-coordinate of top-left corner
		Xsize,  ///< Width in pixels
		Ysize;  ///< Height in pixels
} HV_RECT;


/** Flags denoting rectangle components */
#define HV_RECTCOMP_POSITION_X  0x1   ///< X position
#define HV_RECTCOMP_POSITION_Y  0x2   ///< Y position
#define HV_RECTCOMP_SIZE_X      0x4   ///< X size
#define HV_RECTCOMP_SIZE_Y      0x8   ///< Y size
#define HV_RECTCOMP_POSITION    (HV_RECTCOMP_POSITION_X | HV_RECTCOMP_POSITION_Y) ///< both x and y positions
#define HV_RECTCOMP_SIZE        (HV_RECTCOMP_SIZE_X | HV_RECTCOMP_SIZE_Y)         ///< both x and y sizes
#define HV_RECTCOMP_ALL         (HV_RECTCOMP_POSITION | HV_RECTCOMP_SIZE)  


/// Function codes of all functions which can be called asynchronously
typedef enum tagHV_FUNCTION_CODE {
   
} HV_FUNCTION_CODE;


typedef struct tagHV_OVERLAPPED {
  OVERLAPPED ol;            ///< Win32-Api defined structure, needed for all calls
  HV_FUNCTION_CODE function;  ///< Function code to identify the function called
  PVOID pContext;           ///< Pointer to user provided context 
} HV_OVERLAPPED;


/** Feature flag bit shift values */
typedef enum {
  FEAT_B_SHIFT_PRESENT = 0,                                ///< Specifies the feature presence bit
  FEAT_B_SHIFT_ABS_CONTROL,                                ///< Specifies the absolute value control
  FEAT_B_SHIFT_ONE_PUSH = 3,                            ///< Specifies the one-push automatic control
  FEAT_B_SHIFT_READ_OUT,                                ///< Specifies the readout capability
  FEAT_B_SHIFT_ON_OFF,                                    ///< Specifies the on/off control
  FEAT_B_SHIFT_AUTO,                                    ///< Specifies the automatic mode control
  FEAT_B_SHIFT_TRIGGER_POLARITY = FEAT_B_SHIFT_AUTO,    ///< Specifies the trigger input polarity control
  FEAT_B_SHIFT_Manual                                    ///< Specifies the manual mode control
};


/*
 * All features are equal, but some are more equal than others, so we have
 * to deal with a few special cases here. That's why there is that union.
 */
/// Descriptor used for inquiring the sub features of a DCAM feature
typedef union tagHV_FEATURE_INQ_DESCRIPTOR {
  struct {
    ULONG Max_Value       : 12;   ///< Maximum value for this feature
    ULONG Min_Value       : 12;   ///< Minimum value for this feature
    ULONG Manual_Inq      :  1;   ///< Availability of manual mode
    ULONG Auto_Inq        :  1;   ///< Availability of automatic mode
    ULONG On_Off_Inq      :  1;   ///< Availability of swithing this feature on and off
    ULONG Read_Out_Inq    :  1;   ///< Capability of reading the value of this feature
    ULONG One_Push_Inq    :  1;   ///< Availability of one push automatic mode
    ULONG                 :  1;
    ULONG Abs_Control_Inq :  1;   ///< Availability of absolute control
    ULONG Presence_Inq    :  1;   ///< Presence of this feature
  } Scalar;       ///< a scalar feature

  struct {
    ULONG                   : 12;
    ULONG Trigger_Mode3_Inq :  1; ///< Presence of Trigger mode 3
    ULONG Trigger_Mode2_Inq :  1; ///< Presence of Trigger mode 2
    ULONG Trigger_Mode1_Inq :  1; ///< Presence of Trigger mode 1
    ULONG Trigger_Mode0_Inq :  1; ///< Presence of Trigger mode 0
    ULONG                   :  9;
    ULONG Polarity_Inq      :  1; ///< Capability of changing the polarity of the trigger input
    ULONG On_Off_Inq        :  1; ///< Availability of switching this feature on and off
    ULONG Read_Out_Inq      :  1; ///< Capability of reading the value of this feature
    ULONG                   :  2; 
    ULONG Abs_Control_Inq   :  1; ///< Availability of absolute control
    ULONG Presence_Inq      :  1; ///< Presence of this feature
  } Trigger;      ///< trigger feature
} HV_FEATURE_INQ_DESCRIPTOR;


/** Write mask for setting feature */
#define HV_FEATURE_SET_VALUE               0x0001    ///< Specifies the value control
#define HV_FEATURE_SET_A_M_MODE            0x0002    ///< Specifies the automatic/manual mode control
#define HV_FEATURE_SET_TRIGGER_POLARITY    0x0004    ///< Specifies the trigger input polarity control
#define HV_FEATURE_SET_ON_OFF              0x0008    ///< Specifies the on/off control
#define HV_FEATURE_SET_ONE_PUSH            0x0010    ///< Specifies the one-push automatic mode control
#define HV_FEATURE_SET_ABS_CONTROL         0x0020    ///< Specifies the absolute value control
#define HV_FEATURE_SET_UB_VALUE            0x0040    ///< Specifies the white balance U/B value
#define HV_FEATURE_SET_VR_VALUE            0x0080    ///< Specifies the white balance V/R value
#define HV_FEATURE_SET_TARGET_TEMPERATURE  0x0100    ///< Specifies the target temperature
#define HV_FEATURE_SET_TRIGGER_MODE        0x0200    ///< Specifies the trigger mode
#define HV_FEATURE_SET_PARAMETER           0x0400    ///< Specifies the trigger parameter
#define HV_FEATURE_SET_VALID_FLAGS         0x07ff    ///< Specifies the set of valid flag bits


/*
 * All features are equal, but some are more equal than others, so we have
 * to deal with a few special cases here. That's why there is that union.
 */
/// Descriptor for controlling a DCAM feature
typedef union tagHV_FEATURE_DESCRIPTOR {
  struct {
    ULONG Value             : 12; ///< Value of this feature
    ULONG                   : 12; 
    ULONG A_M_Mode          :  1; ///< Automatic mode - manual mode switch
    ULONG ON_OFF            :  1; ///< Switch to turn this feature on or off, off is 0, on is 1
    ULONG One_Push          :  1; ///< One push automatic mode, off is 0, on is 1
    ULONG                   :  3;
    ULONG Abs_Control       :  1; ///< Absolute value control
    ULONG Presence_Inq      :  1; ///< Presence of this feature
  } Scalar; ///< a scalar feature
  
  struct {
    ULONG Parameter         : 12; ///< Parameter of the trigger function
    ULONG                   :  4;
    ULONG Trigger_Mode      :  4; ///< Trigger mode 0-15
    ULONG                   :  4;
    ULONG Trigger_Polarity  :  1; ///< Trigger polarity, high active is 1, low active is 0
    ULONG ON_OFF            :  1; ///< Switch to turn this feature on or off, off is 0, on is 1
    ULONG One_Push          :  1; ///< One push automatic mode, off is 0, on is 1
    ULONG                   :  3;
    ULONG Abs_Control       :  1; ///< Absolute value control
    ULONG Presence_Inq      :  1; ///< Presence of this feature
  } Trigger; ///< Trigger feature

  struct {
    ULONG V_R               : 12; ///< V value or R value
    ULONG U_B               : 12; ///< U value or B value
    ULONG A_M_Mode          :  1; ///< Automatic mode - manual mode switch
    ULONG ON_OFF            :  1; ///< Switch to turn this feature on or off, off is0, on is 1
    ULONG One_Push          :  1; ///< One push automatic mode, off is 0, on is 1
    ULONG                   :  3;
    ULONG Abs_Control       :  1; ///< Absolute value control
    ULONG Presence_Inq      :  1; ///< Presence of this feature
  } WhiteBalance; ///< White balance feature

  struct {
    ULONG Temperature       : 12; ///< Current value
    ULONG Target_Temperature: 12; ///< Aimed value
    ULONG A_M_Mode          :  1; ///< Automatic mode - manual mode switch
    ULONG ON_OFF            :  1; ///< Switch to turn this feature on or off, off is0, on is 1
    ULONG One_Push          :  1; ///< One push automatic mode, off is 0, on is 1
    ULONG                   :  3;
    ULONG Abs_Control       :  1; ///< Absolute value control
    ULONG Presence_Inq      :  1; ///< Presence of this feature
  } Temperature; ///< Temperature feature
} HV_FEATURE_DESCRIPTOR;

/** Macro to test whether a particular feature supports a given operation */
#define HV_FEATURE_TEST(desc, shft) ((desc)->Flags & (0x1UL << shft) != 0)


/** Commands for grabbing \see DCAM, ISO_EN, SINGLESHOT, MULTISHOT */
typedef enum tagHV_CAMERA_COMMAND {
  CAM_CMD_STOP,        ///< Turns ISO_EN off
  CAM_CMD_START,       ///< Turns ISO_EN on
  CAM_CMD_SINGLE_GRAB,  ///< Starts a single grab
  CAM_CMD_MULTI_GRAB    ///< Starts a sequence of grabs
} HV_CAMERA_COMMAND;


/// Device interface for opening camera
typedef struct tagHV_DEVICE_INTERFACE {
	DWORD size;
	char  DevicePath[128];     ///< Device path
	GUID  classguid;           ///< Device class guid
	DWORD reserved;
} HV_DEVICE_INTERFACE;


typedef struct tagHV_SANP_INFO {
	HHV	hhv;
	int nDevice;
	int nIndex;
	void *pParam;  
} HV_SNAP_INFO;
/// Callback function prototype
typedef int (CALLBACK* HV_SNAP_PROC)(HV_SNAP_INFO *pInfo);


typedef struct tagHV_CHECK_PARAM {    
  BYTE byParam1;
  BYTE byParam2;
  BYTE byParam3;
  BYTE byParam4;    
} HV_CHECK_PARAM;


typedef enum tagHVSTATUS { 
  STATUS_OK                            = 0, 
  ///////////////////////////////////////////
  STATUS_NO_DEVICE_FOUND               = -1,
  STATUS_DEVICE_HANDLE_INVALID         = -2,
  STATUS_HW_DEVICE_TYPE_ERROR          = -3,
  STATUS_HW_INIT_ERROR                 = -4,
  STATUS_HW_RESET_ERROR                = -5,
  STATUS_NOT_ENOUGH_SYSTEM_MEMORY      = -6,
  STATUS_HW_IO_ERROR                   = -7,
  STATUS_HW_IO_TIMEOUT                 = -8,
  STATUS_HW_ACCESS_ERROR               = -9,
  ///////////////////////////////////////////
  STATUS_OPEN_DRIVER_FAILED            = -10,
  STATUS_NOT_SUPPORT_INTERFACE         = -11,
  STATUS_PARAMETER_INVALID             = -12,
  STATUS_PARAMETER_OUT_OF_BOUND        = -13,
  STATUS_IN_WORK                       = -14,
  STATUS_NOT_OPEN_SNAP                 = -15,
  STATUS_NOT_START_SNAP                = -16,
  STATUS_FILE_CREATE_ERROR             = -17,
  STATUS_FILE_INVALID                  = -18,
  STATUS_NOT_START_SNAP_INT            = -19,
  ///////////////////////////////////////////
  STATUS_INTERNAL_ERROR                = -20
} HVSTATUS;


/* Function ordinal numbers, used to construct driver control codes */
typedef enum tagHV_CONTROL_CODE {
	//----------相机初始化,相关信息获取ID----------------------------------
	ORD_INIT_CAMERA											= 0x00000000,
	ORD_GET_STRING											= 0x00000001,
	ORD_GET_DEVICE_INFO										= 0x00000002,
	ORD_GET_ERROR_STRING									= 0x00000003,
	//......... 
		
	//-----------DCAM ID---------------------------------------------------
	ORD_GET_VIDEO_FORMAT_MASK								= 0x00001000,
	ORD_GET_VIDEO_MODE_MASK									= 0x00001001,
	ORD_GET_FRAME_RATE_MASK									= 0x00001002,
	ORD_SET_VIDEO_FORMAT_MODE_FRAME_RATE					= 0x00001003,
	ORD_GET_VIDEO_FORMAT_MODE_FRAME_RATE					= 0x00001004,
		
	ORD_FMT7_GET_MODE_DESCRIPTOR							= 0x00001005,
	ORD_FMT7_SET_AOI										= 0x00001006,
	ORD_FMT7_GET_AOI										= 0x00001007,
	ORD_FMT7_GET_COLOR_MODE									= 0x00001008,
	ORD_FMT7_SET_COLOR_MODE									= 0x00001009,
	ORD_FMT7_SET_CAMERA_BYTES_PER_PACKET					= 0x0000100A,	
	
	ORD_QUERY_FEATURE_PRESENCE								= 0x0000100B,
	ORD_QUERY_FEATURE_INQ									= 0x0000100C,
	ORD_SET_FEATURE											= 0x0000100D,
	ORD_GET_FEATURE											= 0x0000100E,
	ORD_GET_FEATURE_ABS_VALUE								= 0x0000100F,
	ORD_GET_FEATURE_ABS_RANGE								= 0x00001010,
		
	ORD_GET_BASIC_FUNC_INQ									= 0x00001011,
	//......... 
		
	//-----------DH 自定义 ID---------------------------------------------------
	//-----------DH-DCAM ID (采集图像)----------------------
	ORD_OPEN_SNAP											= 0x00002000,
	ORD_CLOSE_SNAP											= 0x00002001,
	ORD_START_SNAP											= 0x00002002,
	ORD_STOP_SNAP											= 0x00002003,
	ORD_SNAP_SHOT											= 0x00002004,
	ORD_TRIGGER_SHOT										= 0x00002005,
	//......... 
		
	//-----------DH-DCAM ID (功能设置)----------------------
	ORD_SET_SNAP_MODE										= 0x00003000,
	ORD_SET_TRIGGER_POLARITY								= 0x00003001,
	ORD_SET_STROBE_POLARITY									= 0x00003002,
	ORD_CHECK_PASSWORD										= 0x00003003,
	ORD_READ_BLOCK											= 0x00003004,
	ORD_WRITE_BLOCK											= 0x00003005,
	ORD_READ_QUADLET										= 0x00003006,
	ORD_WRITE_QUADLET										= 0x00003007,
	ORD_GET_USERDATA										= 0x00003008,
	ORD_SET_USERDATA										= 0x00003009,

	ORD_SET_EXPOSURE_TIME									= 0x0000300A,
	ORD_SET_RESOLUTION										= 0x0000300B,
	ORD_SET_BLACKLEVEL										= 0x0000300C,
	
	ORD_GET_USER_MEM										= 0x0000300D,
	ORD_SET_USER_MEM										= 0x0000300F,
	
	ORD_SET_TESTSNAPOUT                                     = 0x00003010,
	ORD_SET_STROBE_ON_OFF                                    = 0x00003011,//xin
	ORD_SET_DELAY_TIME                                      = 0x00003012,//xin
	ORD_SET_DELAY_TIME_VALUE                                = 0x00003013,//xin
	ORD_SET_8or12BitMode                                    = 0x00003014,//xin
	ORD_SET_OUTPUTIO_0                                      = 0x00003015,//1.4 2007
	ORD_SET_OUTPUTIO_1                                      = 0x00003016,//1.4 2007
    ORD_SET_OUTPUTIO_2                                      = 0x00003017,//1.4 2007
	ORD_SET_OUTPUTIO_0_CONTROL                              = 0x00003018,//1.4
	ORD_SET_OUTPUTIO_1_CONTROL                              = 0x00003019,//1.4
	ORD_SET_OUTPUTIO_2_CONTROL                              = 0x00003020,//1.4
	ORD_SET_INPUTIO_SET                                     = 0x00003021,//1.8

	
	ORD_CMD_RESERVED0										= 0x00003010,
	ORD_CMD_RESERVED1										= 0x00003011,
	ORD_DEVCTRL_RESERVED0									= 0x00003012,
	ORD_DEVCTRL_RESERVED1									= 0x00003013,
	ORD_DEVCTRL_RESERVED2									= 0x00003014,
	ORD_DEVCTRL_RESERVED3									= 0x00003015,
	ORD_DEVCTRL_RESERVED4									= 0x00003016,
	
	ORD_SPEC_FUN_INTERFACE1									= 0x00003100,
	
	//......... 
		
	//-----------DH-DCAM 1394 SPECIAL ID-------------------------------------------------
	ORD_GET_SHUTTER_UNIT									= 0x00004000,
	ORD_SET_SHUTTER_UNIT									= 0x00004001,
	ORD_FREEZE_FRAME										= 0x00004002,
	ORD_SET_LUT												= 0x00004003,
	ORD_GET_LUT												= 0x00004004,


	//......... 
		
	//-----------DH-DCAM USB SPECIAL ID-------------------------------------------------
	ORD_SET_SNAP_SPEED										= 0x00005000,
	ORD_GET_SNAP_SPEED										= 0x00005001,
	//......... 

	//-----------HYL_TEST_ID------------------------------------------------------------
	ORD_SET_DEVICE_PRP										= 0x00007000,
	ORD_GET_DEVICE_PRP										= 0x00007001,


	//.....
} HV_CONTROL_CODE;


/// Mask with video mode
typedef struct tagHV_RES_GET_VIDEO_FORMAT_MASK {
  ULONG VideoFormatMask; ///< Mask with the supported video formats
} HV_RES_GET_VIDEO_FORMAT_MASK;


/// Argument for the GetVideoModeMask function
typedef struct tagHV_HV_ARG_GET_VIDEO_MODE_MASK {
  ULONG VideoFormat;  ///< Specifies the video format
} HV_ARG_GET_VIDEO_MODE_MASK;
/// Result of the GetVideoModeMask function
typedef struct tagHV_RES_GET_VIDEO_MODE_MASK {
  ULONG VideoMode;    ///< Mask with the supported video modes
} HV_RES_GET_VIDEO_MODE_MASK;


/// Argument for the GetFrameRateMask function
typedef struct tagHV_ARG_GET_FRAME_RATE_MASK {
  ULONG VideoFormat,    ///< Specifies the video format
    VideoMode;          ///< Specifies the video mode
} HV_ARG_GET_FRAME_RATE_MASK;
/// Result of the GetFrameRateMask function
typedef struct tagHV_RES_GET_FRAME_RATE_MASK {
  ULONG FrameRateMask;  ///< Mask with the supported frame rates
} HV_RES_GET_FRAME_RATE_MASK;



/// Argument for the GetCameraInfo function
typedef struct tagHV_ARG_GET_CAMERA_INFO {
  HV_INFO_ID Id;  ///< Specifies the info id to get.
} HV_ARG_GET_CAMERA_INFO;
/// Result of the GetCameraInfo function
typedef struct tagHV_RES_GET_CAMERA_INFO {
  HV_CAMERA_INFO Info;   ///< Specifies the camera information.
} HV_RES_GET_CAMERA_INFO;


/// Argument for the GetString function
typedef struct tagHV_ARG_GET_STRING {
  HV_STRING_ID Id;  ///< Specifies the string to get.
} HV_ARG_GET_STRING;

/// Argument for the Fmt7GetModeDescriptor function
typedef struct tagHV_ARG_FMT7_GET_MODE_DESCRIPTOR {
  ULONG VideoMode;  ///< Specifies the video mode
} HV_ARG_FMT7_GET_MODE_DESCRIPTOR;
/** \brief Result of the Fmt7GetModeDescriptor function */
typedef HV_VIDEO_FORMAT7_DESCRIPTOR HV_RES_FMT7_GET_MODE_DESCRIPTOR;


/// Argument for the Fmt7SetAOI function
typedef struct tagHV_ARG_FMT7_SET_AOI {
  ULONG VideoMode;  ///< Specifies the video mode
  HV_RECT AOI;     ///< Specifies the area of interest
  ULONG Mask;       ///< Bitmask for components to set (position, size or both)
} HV_ARG_FMT7_SET_AOI;


/// Argument for the Fmt7GetAOI function
typedef struct tagHV_ARG_FMT7_GET_AOI {
  ULONG VideoMode; ///< Specifies the video mode
} HV_ARG_FMT7_GET_AOI;
/// Parameter of the Fmt7GetAOI function
typedef HV_RECT HV_RES_FMT7_GET_AOI;


/// Argument for the Fmt7GetColorMode function
typedef struct tagHV_ARG_FMT7_GET_COLOR_MODE {
  ULONG VideoMode;        ///< Specifies the video mode
} HV_ARG_FMT7_GET_COLOR_MODE;
/// Result of the Fmt7GetColorMode function
typedef struct tagHV_RES_FMT7_GET_COLOR_MODE {
  HV_COLOR_CODE ColorMode; ///< Specifies the image type
} HV_RES_FMT7_GET_COLOR_MODE;


/// Argument of the Fmt7SetColorMode function
typedef struct tagHV_ARG_FMT7_SET_COLOR_MODE {
  ULONG VideoMode;        ///< Specifies the video mode
  HV_COLOR_CODE ColorMode; ///< Specifies the image type
} HV_ARG_FMT7_SET_COLOR_MODE;


/// Argument for the GetBandwidthInfo function
typedef struct tagHV_ARG_GET_BAND_WIDTH_INFO {
  ULONG VideoMode;    ///< Specifies the video mode
} HV_ARG_GET_BAND_WIDTH_INFO; 
/// Result of the GetBandwidthInfo function
typedef HV_BW_DESC HV_RES_GET_BAND_WIDTH_INFO; 


/// Argument for the CameraControl function.
typedef struct tagHV_ARG_CAMERA_CONTROL {
  HV_CAMERA_COMMAND Cmd;  ///< Specifies the camera command
  ULONG nFrames;          ///< Specifies the number of images to grab in case of CamCmd_MultiGrab
} HV_ARG_CAMERA_CONTROL;


/// Result of the QueryFeaturePresence
typedef struct tagHV_RES_QUERY_FEATURE_PRESENCE {
  ULONGLONG FeatureFlags; ///< Mask with the supported features.
} HV_RES_QUERY_FEATURE_PRESENCE; 


/// Argument to the QueryFeatureInq function
typedef struct tagHV_ARG_QUERY_FEATURE_INQ {
  HV_FEATURE_ID FeatId; ///< The feature identifier
} HV_ARG_QUERY_FEATURE_INQ; 
/// Result for the QueryFeatureInq function
typedef HV_FEATURE_INQ_DESCRIPTOR HV_RES_QUERY_FEATURE_INQ; 


/// Argument to the SetFeature function
typedef struct tagHV_ARG_SET_FEATURE {
  HV_FEATURE_ID FeatId;                     ///< Specifies the feature
  HV_FEATURE_DESCRIPTOR FeatDescriptor;        ///< The feature descriptor
  ULONG Mask;                               ///< Bit mask specifying changes to apply
} HV_ARG_SET_FEATURE;


/// Argument to the GetFeature function
typedef struct tagHV_ARG_GET_FEATURE {
  HV_FEATURE_ID FeatId;  ///< Specifies the feature
  BOOLEAN Reserved;      ///< This argument is currently unused. 
} HV_ARG_GET_FEATURE; 
/// Result of the GetFeature function
typedef HV_FEATURE_DESCRIPTOR HV_RES_GET_FEATURE; 


/// Argument to the AllocateResources function
typedef struct tagHV_ARG_ALLOCATE_RESOURCES {
  ULONG nBytesPerPacket;    ///< Packet size in bytes
  ULONG nNumberOfBuffers;   ///< Number of Buffers
  ULONG nMaxBufferSize;     ///< Maximum buffer size in bytes
} HV_ARG_ALLOCATE_RESOURCES; 
/// Result of the AllocateResource function
typedef struct tagHV_RES_ALLOCATE_RESOURCES { 
  ULONG Channel;                ///< Isochronous channel allocated
  ULONG nBytesPerPacketRemain;  ///< Unused bandwidth remaining after allocation
  HV_ISO_SPEED Speed;           ///< Isochronous transmission speed for device
} HV_RES_ALLOCATE_RESOURCES;


/// Argument for the SetCameraBytesPerPacket function
typedef struct tagHV_ARG_SET_CAMERA_BYTES_PER_PACKET {
  ULONG VideoMode,      ///< The video mode of format 7
    nBytesPerPacket;    ///< The packet size in bytes.
} HV_ARG_SET_CAMERA_BYTES_PER_PACKET;


/// Result of the GetVideoFormatModeFrameRate function.
typedef struct tagHV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE
{
    ULONG VideoFormat,  ///< The video format
    VideoMode,        ///< The video mode
    FrameRate;        ///< The frame rate
} HV_RES_GET_VIDEO_FORMAT_MODE_FRAME_RATE;


/// Result of the GetDcamVersion function
typedef ULONG HV_RES_GET_BASIC_FUNC_INQ;


/// Result of the QueryChannelFeaturePresence
typedef struct tagHV_RES_QUERY_CHANNEL_FEATURE_PRESENCE {
  ULONGLONG FeatureFlags; ///< Mask with the supported features.
} HV_RES_QUERY_CHANNEL_FEATURE_PRESENCE; 


/// Argument to the QueryChannelFeatureInq function
typedef struct tagHV_ARG_QUERY_CHANNEL_FEATURE_INQ {
  HV_FEATURE_ID FeatId; ///< The feature identifier
  HV_CHANNEL Channel;    ///< The channel of feature
} HV_ARG_QUERY_CHANNEL_FEATURE_INQ; 
/// Result for the QueryChannelFeatureInq function
typedef HV_FEATURE_INQ_DESCRIPTOR HV_RES_QUERY_CHANNEL_FEATURE_INQ; 


/// Argument to the SetChannelFeature function
typedef struct tagHV_ARG_SET_CHANNEL_FEATURE {
  HV_FEATURE_ID FeatId;                     ///< Specifies the feature
  HV_CHANNEL Channel;                        ///< Specifies the channel of feature
  HV_FEATURE_DESCRIPTOR FeatDescriptor;        ///< The feature descriptor
  ULONG Mask;                               ///< Bit mask specifying changes to apply
} HV_ARG_SET_CHANNEL_FEATURE;


/// Argument to the GetChannelFeature function
typedef struct tagHV_ARG_GET_CHANNEL_FEATURE {
  HV_FEATURE_ID FeatId;  ///< Specifies the feature
  HV_CHANNEL Channel;     ///< Specifies the channel of feature
  BOOLEAN Reserved;      ///< This argument is currently unused. 
} HV_ARG_GET_CHANNEL_FEATURE; 
/// Result of the GetChannelFeature function
typedef HV_FEATURE_DESCRIPTOR HV_RES_GET_CHANNEL_FEATURE; 


/// Argument for the SetVideoMirror function
typedef struct tagHV_ARG_SET_VIDEO_MIRROR {
  HV_VIDEO_MIRROR VideoMirror; ///< Specifies the video mirror mode
} HV_ARG_SET_VIDEO_MIRROR;


/// Result of the GetVideoMirror function
typedef struct tagHV_RES_GET_VIDEO_MIRROR {
  HV_VIDEO_MIRROR VideoMirror; ///< Specifies the video mirror advanced feature control register
} HV_RES_GET_VIDEO_MIRROR;


/// Argument for the OpenSnap function
typedef struct tagHV_ARG_OPEN_SNAP {
  HV_SNAP_PROC pSnapFunc; ///< Specifies the callback function
  void *pParam;   ///< Specifies user context
} HV_ARG_OPEN_SNAP;


/// Argument for the StartSnap function
typedef struct tagHV_ARG_START_SNAP {
  BYTE **ppBuffer; ///< Specifies buffers for snapping
  int nSum;   ///< Specifies the number of buffer
} HV_ARG_START_SNAP;


/// Argument for the SnapShot function
typedef struct tagHV_ARG_SNAP_SHOT {
  BYTE **ppBuffer; ///< Specifies buffers for snapping
  int nSum;   ///< Specifies the number of buffer
} HV_ARG_SNAP_SHOT;

//hujing add

typedef enum tagHV_ADC_CONTROL{ 
	ADC_BITS		                = 0,
	ADC_BLKLEVEL_CAL				= 1,
	ADC_BLKLEVEL_CAL_REDCHANNEL		= 0x10,
	ADC_BLKLEVEL_CAL_GREENCHANNEL1	= 0x11,
	ADC_BLKLEVEL_CAL_GREENCHANNEL2	= 0x12,
	ADC_BLKLEVEL_CAL_BLUECHANNEL	= 0x13,
	
	ADC_BRIGHTNESS					= 0x2
} HV_ADC_CONTROL;

typedef struct tagHV_HV_ARG_SET_BLACKLEVEL{
	HV_ADC_CONTROL ADCID;
	LONG nValue; 
}HV_ARG_SET_BLACKLEVEL;

typedef struct tagHV_HV_ARG_SET_EXPOSURE_TIME{
	ULONG nValue; 
}HV_ARG_SET_EXPOSURE_TIME;

typedef struct tagHV_HV_ARG_SET_RESOLUTION{
	ULONG nValue; 
}HV_ARG_SET_RESOLUTION;

typedef enum tagHV_SHUTTER_UNIT{
	SHUTTER_UNIT_US,
	SHUTTER_UNIT_MS,
	SHUTTER_UNIT_S
}HV_SHUTTER_UNIT;

typedef enum tagHV_SNAP_MODE { 
	CONTINUATION	= 0,
	TRIGGER			= 1,
	TRIGGER_EDGE	= 2,
	TRIGGER_LEVEL	= 3		
} HV_SNAP_MODE;

//read or write block
typedef struct tagHV_ARG_READ_BLOCK {
	DWORD address;
} HV_ARG_READ_BLOCK;
typedef struct tagHV_RES_READ_BLOCK {
	DWORD *pData;
	int nLength;
} HV_RES_READ_BLOCK;

typedef struct tagHV_ARG_WRITE_BLOCK {
	DWORD address;
	DWORD *pData;
	int nLength;
} HV_ARG_WRITE_BLOCK;

//read or write quadlet
typedef struct tagHV_ARG_READ_QUADLET {
	DWORD address;
} HV_ARG_READ_QUADLET;

typedef struct tagHV_RES_READ_QUADLET {
	DWORD *pData;
} HV_RES_READ_QUADLET;

typedef struct tagHV_ARG_WRITE_QUADLET {
	DWORD address;
	DWORD Data;
} HV_ARG_WRITE_QUADLET;

//shutter unit
typedef struct tagHV_ARG_SHUTTER_UNIT {
	HV_SHUTTER_UNIT UnitId; 
} HV_ARG_SHUTTER_UNIT; 

typedef struct tagHV_RES_SHUTTER_UNIT {
	HV_SHUTTER_UNIT UnitId; 
} HV_RES_SHUTTER_UNIT; 

//Load LUT
typedef struct tagHV_ARG_MEM {
	DWORD* ptr;
	int nLong; 
	int nOffset;
} HV_ARG_LUT,HV_ARG_USER_MEM; 

//freeze
typedef struct tagHV_ARG_FREEZE_FRAME {
	int nFreeze; 
} HV_ARG_FREEZE_FRAME; 


//strobe polarity
typedef struct tagHV_ARG_SET_STROBE_POLARITY {
	HV_POLARITY polarity; 
} HV_ARG_SET_STROBE_POLARITY; 

//trigger polarity
typedef struct tagHV_ARG_SET_TRIGGER_POLARITY {
	HV_POLARITY polarity; 
} HV_ARG_SET_TRIGGER_POLARITY; 

//snap mode
typedef struct tagHV_ARG_SET_SNAP_MODE {
	HV_SNAP_MODE SnapMode; 
} HV_ARG_SET_SNAP_MODE; 

//Argument for Device info
typedef enum tagHV_DEVICE_INFO_ID { 
		DESC_DEVICE_TYPE			 = 0,
		DESC_RESOLUTION				 = 1,
		DESC_DEVICE_MARK			 = 2, 
		DESC_DEVICE_SERIESNUM	     = 3,
		DESC_DEVICE_BLANKSIZE        = 4,
		DESC_DEVICE_CHIPID           = 5,
		DESC_DEVICE_HARDWARE_VERSION = 6,
		DESC_DEVICE_NAME             = 7,
		DESC_DEVICE_SOFTWARE_VERSION = 8,
		DESC_DEVICE_FPGA_VERSION     = 9,
		DESC_DEVICE_VENDOR_NAME      = 10,
		DESC_DEVICE_BAYER_LAYOUT     = 11,


} HV_DEVICE_INFO_ID;

typedef struct tagHV_ARG_GET_DEVICE_INFO {
	HV_DEVICE_INFO_ID			ID; 
	BYTE						*pContext;
	int							*pSize;
} HV_ARG_GET_DEVICE_INFO;

typedef struct tagHV_HV_ARG_ENCRYPT{
	DWORD *pContext;
	int  nLength;
}HV_ARG_ENCRYPT;

typedef struct tagHV_HV_ARG_USERDATA{
	BYTE *pData;
	int nLength;
}HV_ARG_USERDATA;

//end


/// function prototype for driver
BOOL __stdcall HVEnumerate(HV_DEVICE_INTERFACE *pDeviceInterface, DWORD cb, DWORD *cbNeeded);
HVSTATUS __stdcall HVOpen(char *DevicePath, GUID *pGuid, HANDLE *phCamera);
HVSTATUS __stdcall HVClose(HANDLE hCamera);
HVSTATUS __stdcall HVControl(HANDLE hCamera, HV_CONTROL_CODE code, 
        void *pInBuffer, DWORD nInBufferSize, void *pOutBuffer, DWORD nOutBufferSize, DWORD *pBytesReturned);
HVSTATUS __stdcall HVGetGUID(GUID *pguid, DWORD cb, DWORD *cbNeeded);


#define HV_SUCCESS(status) ( (status) == STATUS_OK )
#define HV_FAILED(status) ( (status) != STATUS_OK )

#ifdef _DEBUG
#define HV_VERIFY(status) \
  if((status) != STATUS_OK){ \
    ::MessageBox(NULL, HVGetErrorString(status), \
        "Error", MB_ICONWARNING | MB_OK);   \
  }
#else
#define HV_VERIFY(status) (status)
#endif

#define HV_MESSAGE(status) \
  if((status) != STATUS_OK){ \
    ::MessageBox(NULL, HVGetErrorString(status), \
         "Error message", MB_ICONWARNING | MB_OK);   \
  }  


/* extern "C" { */
#ifdef __cplusplus
}
#endif

#endif

