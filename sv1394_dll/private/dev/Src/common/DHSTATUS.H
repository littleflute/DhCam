//File: DHSTATUS.H

//MessageIdTypedef=NTSTATUS
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SOFTWARE                0x2
#define FACILITY_DHCAMARA                0x1


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: STATUS_OK
//
// MessageText:
//
//  No error
//
//#define STATUS_OK                        0x00000000L

//
// MessageId: STATUS_INVALID_CAMERA
//
// MessageText:
//
//  invalid camera handle or index
//
#define STATUS_INVALID_CAMERA            0xC0010001L

//
// MessageId: STATUS_INVALID_PARAMETER
//
// MessageText:
//
//  invalid parameter
//
#define STATUS_INVALID_PARAMETER         0xC0010002L

//
// MessageId: STATUS_INVALID_FEATUREID
//
// MessageText:
//
//  invalid feature index
//
#define STATUS_INVALID_FEATUREID         0xC0010003L

//
// MessageId: STATUS_INVALID_FORMAT
//
// MessageText:
//
//  invalid video format.
//
#define STATUS_INVALID_FORMAT            0xC0010004L

//
// MessageId: STATUS_INVALID_MODE
//
// MessageText:
//
//  invalid video mode
//
#define STATUS_INVALID_MODE              0xC0010005L

//
// MessageId: STATUS_INVALID_FRAMERATE
//
// MessageText:
//
//  invalid video framerate
//
#define STATUS_INVALID_FRAMERATE         0xC0010006L

//
// MessageId: STATUS_INVALID_COLORCODE
//
// MessageText:
//
//  invalid Format 7 colorcode
//
#define STATUS_INVALID_COLORCODE         0xC0010007L

//
// MessageId: STATUS_INVALIDBITSTYPE
//
// MessageText:
//
//  invalid bitstype value
//
#define STATUS_INVALIDBITSTYPE           0xC0010008L

//
// MessageId: STATUS_CAMERA_IO_ERROR
//
// MessageText:
//
//  Read or Write camera io error
//
#define STATUS_CAMERA_IO_ERROR           0xC0010009L

//
// MessageId: STATUS_FRAME_TIMEOUT
//
// MessageText:
//
//  Snap frame(s) timeout.
//
#define STATUS_FRAME_TIMEOUT             0xC001000AL

//
// MessageId: STATUS_ALREADYINITIALIZED
//
// MessageText:
//
//  camera is alread opened
//
#define STATUS_ALREADYINITIALIZED        0xC001000BL

//
// MessageId: STATUS_ALREADY_OPENED
//
// MessageText:
//
//  camera is alread opened
//
#define STATUS_ALREADY_OPENED            0xC001000CL

//
// MessageId: STATUS_ALREADY_LOCKING
//
// MessageText:
//
//  a frame is being locked
//
#define STATUS_ALREADY_LOCKING           0xC001000DL

//
// MessageId: STATUS_UNKNOWNCOLORCODING
//
// MessageText:
//
//  unknown transfer type for setdata
//
#define STATUS_UNKNOWNCOLORCODING        0xC001000EL

//
// MessageId: STATUS_UNKNOWNCAPABILITYID
//
// MessageText:
//
//  unknown transfer type for setdata
//
#define STATUS_UNKNOWNCAPABILITYID       0xC001000FL

//
// MessageId: STATUS_BUSY
//
// MessageText:
//
//  busy, cannot process
//
#define STATUS_BUSY                      0xC0010010L

//
// MessageId: STATUS_ABORT
//
// MessageText:
//
//  abort process
//
#define STATUS_ABORT                     0xC0010011L

//
// MessageId: STATUS_NOT_READY
//
// MessageText:
//
//  not ready state
//
#define STATUS_NOT_READY                 0xC0010012L

//
// MessageId: STATUS_NOT_STABLE
//
// MessageText:
//
//  not stable state
//
#define STATUS_NOT_STABLE                0xC0010013L

//
// MessageId: STATUS_NOT_UNSTABLE
//
// MessageText:
//
//  not unstable state
//
#define STATUS_NOT_UNSTABLE              0xC0010014L

//
// MessageId: STATUS_TIMEOUT
//
// MessageText:
//
//  timeout
//
#define STATUS_TIMEOUT                   0xC0010015L

//
// MessageId: STATUS_NOT_INIT
//
// MessageText:
//
//  not initial
//
#define STATUS_NOT_INIT                  0xC0010016L

//
// MessageId: STATUS_NO_RESOURCE
//
// MessageText:
//
//  not enough resource without memory and freespace of disk
//
#define STATUS_NO_RESOURCE               0xC0010017L

//
// MessageId: STATUS_NO_MEMORY
//
// MessageText:
//
//  not enough memory
//
#define STATUS_NO_MEMORY                 0xC0010018L

//
// MessageId: STATUS_NO_CAMERA
//
// MessageText:
//
//  no camera
//
#define STATUS_NO_CAMERA                 0xC0010019L

//
// MessageId: STATUS_NO_CAMERA_FOUND
//
// MessageText:
//
//  MSG_NOCAMERA_FOUND%n%r请插上相机后重试，如果仍不能找到相机，请更新相应驱动程序。
//
#define STATUS_NO_CAMERA_FOUND           0xC001001AL

//
// MessageId: STATUS_FAILURE_TO_CREATE_BUFFER
//
// MessageText:
//
//  MSG_FAILTO_CREATEBUFFER%n%r请不要频繁的更改采集宽度和高度。
//
#define STATUS_FAILURE_TO_CREATE_BUFFER  0xC001001BL

//
// MessageId: STATUS_TOO_MUCH_PICTURES
//
// MessageText:
//
//  MSG_TOOMUCH_PICTURES%n%r最多能采集50幅图像；请删除部分图像后再采集。
//
#define STATUS_TOO_MUCH_PICTURES         0x80010003L

