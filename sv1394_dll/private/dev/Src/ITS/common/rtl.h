//File rtl.h

#ifndef __1394_RTL_H__
#define __1394_RTL_H__

////////////////////////////////////////////////////////////////////////
// The defines/structs below are taken from 1394.h in the windows DDK //
// These *must* be defined before including 1394common.h              //
// They are redefined here so we don't need 1394.h to use the library //
////////////////////////////////////////////////////////////////////////

//
// Definition of flags for AllocateAddressRange Irb
//
#define BIG_ENDIAN_ADDRESS_RANGE                1

//
// Definition of fulAccessType for AllocateAddressRange
//
#define ACCESS_FLAGS_TYPE_READ                  1
#define ACCESS_FLAGS_TYPE_WRITE                 2
#define ACCESS_FLAGS_TYPE_LOCK                  4

//
// Definition of fulNotificationOptions for AllocateAddressRange
//
#define NOTIFY_FLAGS_NEVER                      0
#define NOTIFY_FLAGS_AFTER_READ                 1
#define NOTIFY_FLAGS_AFTER_WRITE                2
#define NOTIFY_FLAGS_AFTER_LOCK                 4

//
// Definitions of Lock transaction types
//
#define LOCK_TRANSACTION_MASK_SWAP              1
#define LOCK_TRANSACTION_COMPARE_SWAP           2
#define LOCK_TRANSACTION_FETCH_ADD              3
#define LOCK_TRANSACTION_LITTLE_ADD             4
#define LOCK_TRANSACTION_BOUNDED_ADD            5
#define LOCK_TRANSACTION_WRAP_ADD               6

//
// Definition of fulFlags in Async Read/Write/Lock requests
//

#define ASYNC_FLAGS_NONINCREMENTING             1

//
// flag instucts the port driver to NOT take an int for checking the status
// of this transaction. Always return success...
//

#define ASYNC_FLAGS_NO_STATUS               0x00000002

//
// Definitions of levels of Host controller information
//
#define GET_HOST_UNIQUE_ID                      1
#define GET_HOST_CAPABILITIES                   2
#define GET_POWER_SUPPLIED                      3
#define GET_PHYS_ADDR_ROUTINE                   4
#define GET_HOST_CONFIG_ROM                     5
#define GET_HOST_CSR_CONTENTS                   6

//
// Definitions of capabilities in Host info level 2
//
#define HOST_INFO_PACKET_BASED                  1
#define HOST_INFO_STREAM_BASED                  2
#define HOST_INFO_SUPPORTS_ISOCH_STRIPPING      4
#define HOST_INFO_SUPPORTS_START_ON_CYCLE       8
#define HOST_INFO_SUPPORTS_RETURNING_ISO_HDR    16
#define HOST_INFO_SUPPORTS_ISO_HDR_INSERTION    32

//
// Definitions of Bus Reset flags (used when Bus driver asks Port driver
// to perform a bus reset)
//
#define BUS_RESET_FLAGS_PERFORM_RESET           1
#define BUS_RESET_FLAGS_FORCE_ROOT              2

//
// Definitions of flags for GetMaxSpeedBetweenDevices and
// Get1394AddressFromDeviceObject
//
#define USE_LOCAL_NODE                          1

//
// Definition of flags for BusResetNotification Irb
//
#define REGISTER_NOTIFICATION_ROUTINE           1
#define DEREGISTER_NOTIFICATION_ROUTINE         2

//
// Definitions of Speed flags used throughout 1394 Bus APIs
//
#define SPEED_FLAGS_100                         0x01
#define SPEED_FLAGS_200                         0x02
#define SPEED_FLAGS_400                         0x04
#define SPEED_FLAGS_800                         0x08
#define SPEED_FLAGS_1600                        0x10
#define SPEED_FLAGS_3200                        0x20

#define SPEED_FLAGS_FASTEST                     0x80000000

//
// Definitions of Isoch Allocate Resources flags
//
#define RESOURCE_USED_IN_LISTENING              1
#define RESOURCE_USED_IN_TALKING                2
#define RESOURCE_BUFFERS_CIRCULAR               4
#define RESOURCE_STRIP_ADDITIONAL_QUADLETS      8
#define RESOURCE_TIME_STAMP_ON_COMPLETION       16
#define RESOURCE_SYNCH_ON_TIME                  32
#define RESOURCE_USE_PACKET_BASED               64

//
// Definitions of Isoch Descriptor flags
//
#define DESCRIPTOR_SYNCH_ON_SY                  0x00000001
#define DESCRIPTOR_SYNCH_ON_TAG                 0x00000002
#define DESCRIPTOR_SYNCH_ON_TIME                0x00000004
#define DESCRIPTOR_USE_SY_TAG_IN_FIRST          0x00000008
#define DESCRIPTOR_TIME_STAMP_ON_COMPLETION     0x00000010
#define DESCRIPTOR_PRIORITY_TIME_DELIVERY       0x00000020
#define DESCRIPTOR_HEADER_SCATTER_GATHER        0x00000040

//
// Definitions of Isoch synchronization flags
//
#define SYNCH_ON_SY                             DESCRIPTOR_SYNCH_ON_SY
#define SYNCH_ON_TAG                            DESCRIPTOR_SYNCH_ON_TAG
#define SYNCH_ON_TIME                           DESCRIPTOR_SYNCH_ON_TIME

//
// flags for the SetPortProperties request
//
#define SET_LOCAL_HOST_PROPERTIES_NO_CYCLE_STARTS       0x00000001
#define SET_LOCAL_HOST_PROPERTIES_GAP_COUNT             0x00000002
#define SET_LOCAL_HOST_PROPERTIES_MODIFY_CROM           0x00000003

//
// definition of Flags for SET_LOCAL_HOST_PROPERTIES_MODIFY_CROM
//
#define SLHP_FLAG_ADD_CROM_DATA         0x01
#define SLHP_FLAG_REMOVE_CROM_DATA      0x02

//
// Various Interesting 1394 IEEE 1212 locations
//
#define INITIAL_REGISTER_SPACE_HI               0xffff
//#define INITIAL_REGISTER_SPACE_LO               0xf0000000
#define TOPOLOGY_MAP_LOCATION                   0xf0001000
#define SPEED_MAP_LOCATION                      0xf0002000

//
// 1394 Cycle Time format
//
typedef struct _CYCLE_TIME
{
    DWORD               CL_CycleOffset:12;      // Bits 0-11
    DWORD               CL_CycleCount:13;       // Bits 12-24
    DWORD               CL_SecondCount:7;       // Bits 25-31
} CYCLE_TIME, *PCYCLE_TIME;

//
// 1394 Node Address format
//
typedef struct _NODE_ADDRESS
{
    USHORT              NA_Node_Number:6;       // Bits 10-15
    USHORT              NA_Bus_Number:10;       // Bits 0-9
} NODE_ADDRESS, *PNODE_ADDRESS;

//
// 1394 Address Offset format (48 bit addressing)
//
typedef struct _ADDRESS_OFFSET
{
    USHORT              Off_High;
    DWORD               Off_Low;
} ADDRESS_OFFSET, *PADDRESS_OFFSET;

//
// 1394 I/O Address format
//
typedef struct _IO_ADDRESS
{
    NODE_ADDRESS        IA_Destination_ID;
    ADDRESS_OFFSET      IA_Destination_Offset;
} IO_ADDRESS, *PIO_ADDRESS;

//
// 1394 Self ID packet format
//
typedef struct _SELF_ID
{
    DWORD               SID_Phys_ID:6;          // Byte 0 - Bits 0-5
    DWORD               SID_Packet_ID:2;        // Byte 0 - Bits 6-7
    DWORD               SID_Gap_Count:6;        // Byte 1 - Bits 0-5
    DWORD               SID_Link_Active:1;      // Byte 1 - Bit 6
    DWORD               SID_Zero:1;             // Byte 1 - Bit 7
    DWORD               SID_Power_Class:3;      // Byte 2 - Bits 0-2
    DWORD               SID_Contender:1;        // Byte 2 - Bit 3
    DWORD               SID_Delay:2;            // Byte 2 - Bits 4-5
    DWORD               SID_Speed:2;            // Byte 2 - Bits 6-7
    DWORD               SID_More_Packets:1;     // Byte 3 - Bit 0
    DWORD               SID_Initiated_Rst:1;    // Byte 3 - Bit 1
    DWORD               SID_Port3:2;            // Byte 3 - Bits 2-3
    DWORD               SID_Port2:2;            // Byte 3 - Bits 4-5
    DWORD               SID_Port1:2;            // Byte 3 - Bits 6-7
} SELF_ID, *PSELF_ID;

//
// Additional 1394 Self ID packet format (only used when More bit is on)
//
typedef struct _SELF_ID_MORE
{
    DWORD               SID_Phys_ID:6;          // Byte 0 - Bits 0-5
    DWORD               SID_Packet_ID:2;        // Byte 0 - Bits 6-7
    DWORD               SID_PortA:2;            // Byte 1 - Bits 0-1
    DWORD               SID_Reserved2:2;        // Byte 1 - Bits 2-3
    DWORD               SID_Sequence:3;         // Byte 1 - Bits 4-6
    DWORD               SID_One:1;              // Byte 1 - Bit 7
    DWORD               SID_PortE:2;            // Byte 2 - Bits 0-1
    DWORD               SID_PortD:2;            // Byte 2 - Bits 2-3
    DWORD               SID_PortC:2;            // Byte 2 - Bits 4-5
    DWORD               SID_PortB:2;            // Byte 2 - Bits 6-7
    DWORD               SID_More_Packets:1;     // Byte 3 - Bit 0
    DWORD               SID_Reserved3:1;        // Byte 3 - Bit 1
    DWORD               SID_PortH:2;            // Byte 3 - Bits 2-3
    DWORD               SID_PortG:2;            // Byte 3 - Bits 4-5
    DWORD               SID_PortF:2;            // Byte 3 - Bits 6-7
} SELF_ID_MORE, *PSELF_ID_MORE;

//
// 1394 Topology Map format
//
typedef struct _TOPOLOGY_MAP
{
    USHORT              TOP_Length;             // number of quadlets in map
    USHORT              TOP_CRC;                // 16 bit CRC defined by 1212
    DWORD               TOP_Generation;         // Generation number
    USHORT              TOP_Node_Count;         // Node count
    USHORT              TOP_Self_ID_Count;      // Number of Self IDs
    SELF_ID             TOP_Self_ID_Array[1];    // Array of Self IDs
} TOPOLOGY_MAP, *PTOPOLOGY_MAP;

//
// 1394 Speed Map format
//
typedef struct _SPEED_MAP
{
    USHORT              SPD_Length;             // number of quadlets in map
    USHORT              SPD_CRC;                // 16 bit CRC defined by 1212
    DWORD               SPD_Generation;         // Generation number
    UCHAR               SPD_Speed_Code[4032];
} SPEED_MAP, *PSPEED_MAP;

//
// Definitions of the structures that correspond to the Host info levels
//
typedef struct _GET_LOCAL_HOST_INFO1
{
    LARGE_INTEGER       UniqueId;
} GET_LOCAL_HOST_INFO1, *PGET_LOCAL_HOST_INFO1;

typedef struct _GET_LOCAL_HOST_INFO2
{
    DWORD               HostCapabilities;
    DWORD               MaxAsyncReadRequest;
    DWORD               MaxAsyncWriteRequest;
} GET_LOCAL_HOST_INFO2, *PGET_LOCAL_HOST_INFO2;

typedef struct _GET_LOCAL_HOST_INFO3
{
    DWORD               deciWattsSupplied;
    DWORD               Voltage;                    // x10 -> +3.3 == 33
    // +5.0 == 50,+12.0 == 120
    // etc.
} GET_LOCAL_HOST_INFO3, *PGET_LOCAL_HOST_INFO3;

typedef struct _GET_LOCAL_HOST_INFO4
{
    PVOID               PhysAddrMappingRoutine;
    PVOID               Context;
} GET_LOCAL_HOST_INFO4, *PGET_LOCAL_HOST_INFO4;

//
// the caller can set ConfigRomLength to zero, issue the request, which will
// be failed with STATUS_INVALID_BUFFER_SIZE and the ConfigRomLength will be set
// by the port driver to the proper length. The caller can then re-issue the request
// after it has allocated a buffer for the configrom with the correct length
//
typedef struct _GET_LOCAL_HOST_INFO5
{
    PVOID                   ConfigRom;
    DWORD                   ConfigRomLength;
} GET_LOCAL_HOST_INFO5, *PGET_LOCAL_HOST_INFO5;

typedef struct _GET_LOCAL_HOST_INFO6
{
    ADDRESS_OFFSET          CsrBaseAddress;
    DWORD                   CsrDataLength;
    UCHAR                   CsrDataBuffer[1];
} GET_LOCAL_HOST_INFO6, *PGET_LOCAL_HOST_INFO6;

//
// Definitions of the structures that correspond to the Host info levels
//
typedef struct _SET_LOCAL_HOST_PROPS2
{
    DWORD       GapCountLowerBound;
} SET_LOCAL_HOST_PROPS2, *PSET_LOCAL_HOST_PROPS2;

typedef struct _SET_LOCAL_HOST_PROPS3
{
    DWORD       fulFlags;
    HANDLE      hCromData;
    DWORD       nLength;
    UCHAR       Buffer[1];
//    PMDL        Mdl;
} SET_LOCAL_HOST_PROPS3, *PSET_LOCAL_HOST_PROPS3;

//
// 1394 Phy Configuration packet format
//
typedef struct _PHY_CONFIGURATION_PACKET
{
    DWORD               PCP_Phys_ID:6;          // Byte 0 - Bits 0-5
    DWORD               PCP_Packet_ID:2;        // Byte 0 - Bits 6-7
    DWORD               PCP_Gap_Count:6;        // Byte 1 - Bits 0-5
    DWORD               PCP_Set_Gap_Count:1;    // Byte 1 - Bit 6
    DWORD               PCP_Force_Root:1;       // Byte 1 - Bit 7
    DWORD               PCP_Reserved1:8;        // Byte 2 - Bits 0-7
    DWORD               PCP_Reserved2:8;        // Byte 3 - Bits 0-7
    DWORD               PCP_Inverse;            // Inverse quadlet
} PHY_CONFIGURATION_PACKET, *PPHY_CONFIGURATION_PACKET;

////////////////////////////////////////////////
//                                            //
// End structures and defines from DDK 1394.h //
//                                            //
////////////////////////////////////////////////
//#define DEFINEGUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
//        EXTERN_C const GUID name \
//                = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

//DEFINEGUID(GUID_1394CMDR, 0x3214b6bf, 0x789b, 0x40aa, 0x8c, 0x80, 0xe8, 0x5e, 0x21, 0x2f, 0x13, 0xca);
extern const GUID GUID_1394CMDR;



//Add by WXH 2004.11.12
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#define FILE_DEVICE_UNKNOWN             0x00000022
#define FILE_ANY_ACCESS                 0
#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1

//
// these guys are meant to be called from a ring 3 app
// call through the port device object
//
#define IOCTL_1394_TOGGLE_ENUM_TEST_ON          CTL_CODE( \
                                                FILE_DEVICE_UNKNOWN, \
                                                0x88, \
                                                METHOD_BUFFERED, \
                                                FILE_ANY_ACCESS \
                                                )

#define IOCTL_1394_TOGGLE_ENUM_TEST_OFF         CTL_CODE( \
                                                FILE_DEVICE_UNKNOWN, \
                                                0x89, \
                                                METHOD_BUFFERED, \
                                                FILE_ANY_ACCESS \
                                                )

//
// IOCTL info, needs to be visible for application
//
#define DIAG1394_IOCTL_INDEX                            0x0800
#define CMDR1394_IOCTL_INDEX							DIAG1394_IOCTL_INDEX

#define IOCTL_ALLOCATE_ADDRESS_RANGE                    CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 0,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_FREE_ADDRESS_RANGE                        CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 1,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ASYNC_READ                                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 2,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ASYNC_WRITE                               CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 3,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ASYNC_LOCK                                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 4,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_ALLOCATE_BANDWIDTH                  CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 5,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_ALLOCATE_CHANNEL                    CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 6,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_ALLOCATE_RESOURCES                  CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 7,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_ATTACH_BUFFERS                      CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 8,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_DETACH_BUFFERS                      CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 9,       \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_FREE_BANDWIDTH                      CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 10,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_FREE_CHANNEL                        CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 11,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_FREE_RESOURCES                      CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 12,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_LISTEN                              CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 13,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_QUERY_CURRENT_CYCLE_TIME            CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 14,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_QUERY_RESOURCES                     CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 15,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_SET_CHANNEL_BANDWIDTH               CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 16,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_STOP                                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 17,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ISOCH_TALK                                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 18,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_LOCAL_HOST_INFORMATION                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 19,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_1394_ADDRESS_FROM_DEVICE_OBJECT       CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 20,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_CONTROL                                   CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 21,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_MAX_SPEED_BETWEEN_DEVICES             CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 22,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_SET_DEVICE_XMIT_PROPERTIES                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 23,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_CONFIGURATION_INFORMATION             CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 24,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_BUS_RESET                                 CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 25,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_GENERATION_COUNT                      CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 26,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_SEND_PHY_CONFIGURATION_PACKET             CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 27,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_BUS_RESET_NOTIFICATION                    CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 28,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ASYNC_STREAM                              CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 29,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_SET_LOCAL_HOST_INFORMATION                CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 30,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_SET_ADDRESS_DATA                          CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 40,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_ADDRESS_DATA                          CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 41,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_BUS_RESET_NOTIFY                          CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 50,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_DIAG_VERSION                          CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        DIAG1394_IOCTL_INDEX + 51,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_CMDR_STATE                            CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 52,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_RESET_CMDR_STATE                          CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 53,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_SET_CMDR_TRACELEVEL                       CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 54,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_READ_REGISTER								CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 55,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_WRITE_REGISTER							CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 56,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_MODEL_NAME							CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 57,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_VENDOR_NAME							CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 58,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_CAMERA_SPECIFICATION					CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 59,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_CAMERA_UNIQUE_ID						CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 60,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_ATTACH_BUFFER								CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 61,      \
                                                        METHOD_IN_DIRECT,                \
                                                        FILE_ANY_ACCESS)

#define IOCTL_GET_DRIVE_VERSION							CTL_CODE( FILE_DEVICE_UNKNOWN,  \
                                                        CMDR1394_IOCTL_INDEX + 62,      \
                                                        METHOD_BUFFERED,                \
                                                        FILE_ANY_ACCESS)
// now for the structures that go with the control codes, to make this a truly "common" header file
//
// struct used to pass in with IOCTL_ALLOCATE_ADDRESS_RANGE
//
typedef struct _ALLOCATE_ADDRESS_RANGE
{
    DWORD           fulAllocateFlags;
    DWORD           fulFlags;
    DWORD           nLength;
    DWORD           MaxSegmentSize;
    DWORD           fulAccessType;
    DWORD           fulNotificationOptions;
    ADDRESS_OFFSET  Required1394Offset;
    HANDLE          hAddressRange;
    UCHAR           Data[1];
} ALLOCATE_ADDRESS_RANGE, *PALLOCATE_ADDRESS_RANGE;

//
// struct used to pass in with IOCTL_ASYNC_READ
//
typedef struct _ASYNC_READ
{
    DWORD           bRawMode;
    DWORD           bGetGeneration;
    IO_ADDRESS      DestinationAddress;
    DWORD           nNumberOfBytesToRead;
    DWORD           nBlockSize;
    DWORD           fulFlags;
    DWORD           ulGeneration;
    UCHAR           Data[4];
} ASYNC_READ, *PASYNC_READ;

//
// struct used to pass in with IOCTL_SET_ADDRESS_DATA
//
typedef struct _SET_ADDRESS_DATA
{
    HANDLE          hAddressRange;
    DWORD           nLength;
    DWORD           ulOffset;
    UCHAR           Data[1];
} SET_ADDRESS_DATA, *PSET_ADDRESS_DATA,GET_ADDRESS_DATA, *PGET_ADDRESS_DATA;

//
// struct used to pass in with IOCTL_ASYNC_WRITE
//
typedef struct _ASYNC_WRITE
{
    DWORD           bRawMode;
    DWORD           bGetGeneration;
    IO_ADDRESS      DestinationAddress;
    DWORD           nNumberOfBytesToWrite;
    DWORD           nBlockSize;
    DWORD           fulFlags;
    DWORD           ulGeneration;
    UCHAR           Data[4];
} ASYNC_WRITE, *PASYNC_WRITE;

//
// struct used to pass in with IOCTL_ASYNC_LOCK
//
typedef struct _ASYNC_LOCK
{
    DWORD           bRawMode;
    DWORD           bGetGeneration;
    IO_ADDRESS      DestinationAddress;
    DWORD           nNumberOfArgBytes;
    DWORD           nNumberOfDataBytes;
    DWORD           fulTransactionType;
    DWORD           fulFlags;
    DWORD           Arguments[2];
    DWORD           DataValues[2];
    DWORD           ulGeneration;
    DWORD           Buffer[2];
} ASYNC_LOCK, *PASYNC_LOCK;

//
// struct used to pass in with IOCTL_ASYNC_STREAM
//
typedef struct _ASYNC_STREAM
{
    DWORD           nNumberOfBytesToStream;
    DWORD           fulFlags;
    DWORD           ulTag;
    DWORD           nChannel;
    DWORD           ulSynch;
    DWORD           nSpeed;
    UCHAR           Data[1];
} ASYNC_STREAM, *PASYNC_STREAM;

//
// struct used to pass in with IOCTL_ISOCH_ALLOCATE_BANDWIDTH
//
typedef struct _ISOCH_ALLOCATE_BANDWIDTH
{
    DWORD           nMaxBytesPerFrameRequested;
    DWORD           fulSpeed;
    HANDLE          hBandwidth;
    DWORD           BytesPerFrameAvailable;
    DWORD           SpeedSelected;
} ISOCH_ALLOCATE_BANDWIDTH, *PISOCH_ALLOCATE_BANDWIDTH;

//
// struct used to pass in with IOCTL_ISOCH_ALLOCATE_CHANNEL
//
typedef struct _ISOCH_ALLOCATE_CHANNEL
{
    DWORD           nRequestedChannel;
    DWORD           Channel;
    LARGE_INTEGER   ChannelsAvailable;
} ISOCH_ALLOCATE_CHANNEL, *PISOCH_ALLOCATE_CHANNEL;

//
// struct used to pass in with IOCTL_ISOCH_ALLOCATE_RESOURCES
//
typedef struct _ISOCH_ALLOCATE_RESOURCES
{
    DWORD           fulSpeed;
    DWORD           fulFlags;
    DWORD           nChannel;
    DWORD           nMaxBytesPerFrame;
    DWORD           nNumberOfBuffers;
    DWORD           nMaxBufferSize;
    DWORD           nQuadletsToStrip;
    HANDLE          hResource;
} ISOCH_ALLOCATE_RESOURCES, *PISOCH_ALLOCATE_RESOURCES;

//
// struct used to pass in isoch descriptors
//
typedef struct _RING3_ISOCH_DESCRIPTOR
{
    DWORD           fulFlags;
    DWORD           ulLength;
    DWORD           nMaxBytesPerFrame;
    DWORD           ulSynch;
    DWORD           ulTag;
    CYCLE_TIME      CycleTime;
    DWORD           bUseCallback;
    DWORD           bAutoDetach;
    UCHAR           Data[1];
} RING3_ISOCH_DESCRIPTOR, *PRING3_ISOCH_DESCRIPTOR;

//
// struct used to pass in with IOCTL_ISOCH_ATTACH_BUFFERS
// also used by IOCTL_ATTACH_BUFFER (the new one)
//
typedef struct _ISOCH_ATTACH_BUFFERS
{
    HANDLE                  hResource;
    DWORD                   nNumberOfDescriptors;
    DWORD                   ulBufferSize;
    DWORD                   pIsochDescriptor;
    RING3_ISOCH_DESCRIPTOR  R3_IsochDescriptor[1];
} ISOCH_ATTACH_BUFFERS, *PISOCH_ATTACH_BUFFERS;

//
// struct used to pass in with IOCTL_ISOCH_DETACH_BUFFERS
//
typedef struct _ISOCH_DETACH_BUFFERS
{
    HANDLE          hResource;
    DWORD           nNumberOfDescriptors;
    DWORD           pIsochDescriptor;
} ISOCH_DETACH_BUFFERS, *PISOCH_DETACH_BUFFERS;

//
// struct used to pass in with IOCTL_ISOCH_LISTEN
//
typedef struct _ISOCH_LISTEN
{
    HANDLE          hResource;
    DWORD           fulFlags;
    CYCLE_TIME      StartTime;
} ISOCH_LISTEN, *PISOCH_LISTEN;

//
// struct used to pass in with IOCTL_ISOCH_QUERY_RESOURCES
//
typedef struct _ISOCH_QUERY_RESOURCES
{
    DWORD           fulSpeed;
    DWORD           BytesPerFrameAvailable;
    LARGE_INTEGER   ChannelsAvailable;
} ISOCH_QUERY_RESOURCES, *PISOCH_QUERY_RESOURCES;

//
// struct used to pass in with IOCTL_ISOCH_SET_CHANNEL_BANDWIDTH
//
typedef struct _ISOCH_SET_CHANNEL_BANDWIDTH
{
    HANDLE          hBandwidth;
    DWORD           nMaxBytesPerFrame;
} ISOCH_SET_CHANNEL_BANDWIDTH, *PISOCH_SET_CHANNEL_BANDWIDTH;

//
// struct used to pass in with IOCTL_ISOCH_STOP
//
typedef struct _ISOCH_STOP
{
    HANDLE          hResource;
    DWORD           fulFlags;
} ISOCH_STOP, *PISOCH_STOP;

//
// struct used to pass in with IOCTL_ISOCH_TALK
//
typedef struct _ISOCH_TALK
{
    HANDLE          hResource;
    DWORD           fulFlags;
    CYCLE_TIME      StartTime;
} ISOCH_TALK, *PISOCH_TALK;

//
// struct used to pass in with IOCTL_GET_LOCAL_HOST_INFORMATION
//
typedef struct _GET_LOCAL_HOST_INFORMATION
{
    DWORD           Status;
    DWORD           nLevel;
    DWORD           ulBufferSize;
    UCHAR           Information[1];
} GET_LOCAL_HOST_INFORMATION, *PGET_LOCAL_HOST_INFORMATION;

//
// struct used to pass in with IOCTL_GET_1394_ADDRESS_FROM_DEVICE_OBJECT
//
typedef struct _GET_1394_ADDRESS
{
    DWORD           fulFlags;
    NODE_ADDRESS    NodeAddress;
} GET_1394_ADDRESS, *PGET_1394_ADDRESS;

//
// struct used to pass in with IOCTL_GET_MAX_SPEED_BETWEEN_DEVICES
//
typedef struct _GET_MAX_SPEED_BETWEEN_DEVICES
{
    DWORD           fulFlags;
    DWORD           ulNumberOfDestinations;
    DWORD           hDestinationDeviceObjects[64];
    DWORD           fulSpeed;
} GET_MAX_SPEED_BETWEEN_DEVICES, *PGET_MAX_SPEED_BETWEEN_DEVICES;

//
// struct used to pass in with IOCTL_SET_DEVICE_XMIT_PROPERTIES
//
typedef struct _DEVICE_XMIT_PROPERTIES
{
    DWORD           fulSpeed;
    DWORD           fulPriority;
} DEVICE_XMIT_PROPERTIES, *PDEVICE_XMIT_PROPERTIES;

//
// struct used to pass in with IOCTL_SET_LOCAL_HOST_INFORMATION
//
typedef struct _SET_LOCAL_HOST_INFORMATION
{
    DWORD           nLevel;
    DWORD           ulBufferSize;
    UCHAR           Information[1];
} SET_LOCAL_HOST_INFORMATION, *PSET_LOCAL_HOST_INFORMATION;

//
// define's used to make sure the dll/sys driver are in synch
//
#define DIAGNOSTIC_VERSION                      1
#define DIAGNOSTIC_SUB_VERSION                  1

//
// struct used to pass in with IOCTL_GET_CMDR_VERSION
//
typedef struct _VERSION_DATA
{
    DWORD           ulVersion;
    DWORD           ulSubVersion;
} VERSION_DATA, *PVERSION_DATA;

//
// struct for use with reading/writing registers
//

typedef struct _REGISTER_IOBUF
{
    DWORD		ulOffset;
    UCHAR		data[4];
} REGISTER_IOBUF, *PREGISTER_IOBUF;

//
// struct used to get camera specification information
//

typedef struct _CAMERA_SPECIFICATION
{
    DWORD		ulSpecification;
    DWORD		ulVersion;
} CAMERA_SPECIFICATION, *PCAMERA_SPECIFICATION;

// structures and defines used by the exported C functions:


#define bswap(value)    (((DWORD) (value)) << 24 |\
                        (((DWORD) (value)) & 0x0000FF00) << 8 |\
                        (((DWORD) (value)) & 0x00FF0000) >> 8 |\
                        ((DWORD) (value)) >> 24)

#define bswapw(value)   (((USHORT) (value)) << 8 |\
                        (((USHORT) (value)) & 0xff00) >> 8)

//
// function prototypes
//

// isochapi.c

DWORD
IsochAllocateBandwidth(
    TCHAR*                        szDeviceName,
    PISOCH_ALLOCATE_BANDWIDTH   isochAllocateBandwidth
);

DWORD
IsochAllocateChannel(
    TCHAR*                        szDeviceName,
    PISOCH_ALLOCATE_CHANNEL     isochAllocateChannel
);

DWORD
IsochAllocateResources(
    TCHAR*                        szDeviceName,
    PISOCH_ALLOCATE_RESOURCES   isochAllocateResources
);

DWORD
IsochFreeBandwidth(
    TCHAR*    szDeviceName,
    HANDLE  hBandwidth
);

DWORD
IsochFreeChannel(
    TCHAR*    szDeviceName,
    DWORD   nChannel
);

DWORD
IsochFreeResources(
    TCHAR*    szDeviceName,
    HANDLE  hResource
);

DWORD
IsochListen(
    TCHAR*            szDeviceName,
    PISOCH_LISTEN   isochListen
);

DWORD
IsochQueryCurrentCycleTime(
    TCHAR*            szDeviceName,
    PCYCLE_TIME     CycleTime
);

DWORD
IsochQueryResources(
    TCHAR*                        szDeviceName,
    PISOCH_QUERY_RESOURCES      isochQueryResources
);

DWORD
IsochSetChannelBandwidth(
    TCHAR*                            szDeviceName,
    PISOCH_SET_CHANNEL_BANDWIDTH    isochSetChannelBandwidth
);

DWORD
IsochStop(
    TCHAR*            szDeviceName,
    PISOCH_STOP     isochStop
);

DWORD
IsochTalk(
    TCHAR*            szDeviceName,
    PISOCH_TALK     isochTalk
);

// 1394main.c

void
ResetCameraState(
    TCHAR* szDeviceName
);

DWORD
ReadRegister(
    TCHAR* szDeviceName,
    DWORD ulOffset,
    PUCHAR bytes
);

DWORD
ReadRegisterUL(
    TCHAR* szDeviceName,
    DWORD ulOffset,
    DWORD* pData
);

DWORD
WriteRegister(
    TCHAR* szDeviceName,
    DWORD ulOffset,
    PUCHAR bytes
);

DWORD
WriteRegisterUL(
    TCHAR* szDeviceName,
    DWORD ulOffset,
    DWORD data
);

DWORD
ReadBlock(
    TCHAR* szDeviceName,
    DWORD ulOffset,//读取数据的偏移地址
    DWORD ulLength,//读取数据的长度
    PUCHAR pBuf//数据存放地址
);

DWORD
WriteBlock(
    TCHAR* szDeviceName,
    DWORD ulOffset,
    DWORD ulLength,
    PUCHAR pBuf
);

DWORD
GetModelName(
    TCHAR* szDeviceName,
    TCHAR* buffer,
    DWORD buflen
);

DWORD
GetVendorName(
    TCHAR* szDeviceName,
    TCHAR* buffer,
    DWORD buflen
);

DWORD
GetCameraSpecification(
    TCHAR* szDeviceName,
    PCAMERA_SPECIFICATION pSpec
);

WORD
CSR_CRC16
(unsigned long *data, int length);

DWORD Reverse(DWORD src);

DWORD
GetCameraInfo
(TCHAR* szDeviceName, PCAMERA_INFO pCell);

DWORD ModelName2Type
(TCHAR* lpModelName);

DWORD Type2ModelName
(DWORD dwDHType, TCHAR* lpModelName);

DWORD Multiply2Resolution
(DWORD multiply, DWORD* pResolution);

DWORD
GetCmdrState(
    TCHAR* szDeviceName
);

DWORD
ResetCmdrState(
    TCHAR* szDeviceName
);
/*
DWORD
SetCmdrTraceLevel(
	TCHAR* szDeviceName,
	DWORD nlevel
	);
*/


DWORD
GetCmdrVersion(
    TCHAR*    szDeviceName,
    PVERSION_DATA   Version,
    BOOL   bMatch
);

DWORD
GetMaxSpeedBetweenDevices(
    TCHAR* szDeviceName,
    PGET_MAX_SPEED_BETWEEN_DEVICES GetMaxSpeedBetweenDevices
);

HANDLE
OpenDevice(
    TCHAR* szDeviceName,
    BOOL   bOverLapped
);

HVSTATUS All_Open(char *DevicePath,HANDLE *phCamera, GUID *pGuid, GUID Guid);
HVSTATUS All_Close(HANDLE hCamera);
bool All_Enumerate(int MemberIndex, HV_DEVICE_INTERFACE *pDeviceInterface,GUID Guid_1394);

typedef struct tag_SVOBJ
{
    HANDLE phCamera;
    HHV pCam;

} SVOBJ,* PSVOBJ;

#define CAMOBJ	SVOBJ
#define PCAMOBJ	PSVOBJ

#endif // !__1394_RTL_H__

