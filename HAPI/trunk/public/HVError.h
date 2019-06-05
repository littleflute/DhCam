//-----------------------------------------------------------------------------
//  Company:   Daheng Imavision
//  Section:   Software Department
//  Project:   HVError.h
//  
//  $Archive:
//  $Author:   Software Department
//  $Revision: 1.0.0.0$
//  $Date:     2009.09.02 11:35$
//-----------------------------------------------------------------------------

#ifndef HV_ERROR_H
#define HV_ERROR_H

//
//  Values are 32 bit values laid out as follows:
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
//      C - is the Customer code flag = 1
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//


//////////////////////////////////////////////////////////////////////////
//
// Define the severity codes
//

//
// MessageId: HV_SEVERITY_SUCCESS
//
// MessageText_en:
//
//  Success
//
// MessageText_cn:
//
//  成功
//    
#define HV_SEVERITY_SUCCESS                 0

//
// MessageId: HV_SEVERITY_INFORMATION
//
// MessageText_en:
//
//  Informational
//
// MessageText_cn:
//
//  通知
//    
#define HV_SEVERITY_INFORMATION             1

//
// MessageId: HV_SEVERITY_WARNING
//
// MessageText_en:
//
//  Warning
//
// MessageText_cn:
//
//  警告
//    
#define HV_SEVERITY_WARNING                 2

//
// MessageId: HV_SEVERITY_ERROR
//
// MessageText_en:
//
//  Error
//
// MessageText_cn:
//
//  错误
//     
#define HV_SEVERITY_ERROR                   3


//////////////////////////////////////////////////////////////////////////
//
// Define the facility codes
//

//
// MessageId: HV_FACILITY_INVALID
//
// MessageText_en:
//
//  An invalid facility
//
// MessageText_cn:
//
//  无效设备代码
//      
#define HV_FACILITY_INVALID                0xFFF

//
// MessageId: HV_FACILITY_DLL
//
// MessageText_en:
//
//  The DLL facility
//
// MessageText_cn:
//
//  动态库
//      
#define HV_FACILITY_DLL                    0
//
// MessageId: HV_FACILITY_NET
//
// MessageText_en:
//
//  The net interface facility
//
// MessageText_cn:
//
//  网络接口
//
#define HV_FACILITY_NET                    1


//////////////////////////////////////////////////////////////////////////
//
// Define the status codes
//

//
// MessageId: HVSTATUS_SUCCESS
//
// MessageText_en:
//
//  The operation completed successfully
//
// MessageText_cn:
//
//  操作成功
//
#define HVSTATUS_SUCCESS                         0L


//
// MessageId: HVSTATUS_INFO_BUS_RESET_OCCUR
//
// MessageText_en:
//
//  The bus reset occured
//
// MessageText_cn:
//
//  总线发生复位
//
#define HVSTATUS_INFO_BUS_RESET_OCCUR            0x60040001L


//
// MessageId: HVSTATUS_INFO_DEVICE_RESET_OCCUR
//
// MessageText_en:
//
//  The device reset occured
//
// MessageText_cn:
//
//  设备发生复位
//
#define HVSTATUS_INFO_DEVICE_RESET_OCCUR         0x60040002L


//
// MessageId: HVSTATUS_ERROR_BUS_RESET_FAIL
//
// MessageText_en:
//
//  The bus reset operation fail
//
// MessageText_cn:
//
//  总线复位失败
//
#define HVSTATUS_ERROR_BUS_RESET_FAIL            0xE0040001L

//
// MessageId: HVSTATUS_ERROR_DEVICE_RESET_FAIL
//
// MessageText_en:
//
//  The device reset operation fail
//
// MessageText_cn:
//
//  设备复位失败
//
#define HVSTATUS_ERROR_DEVICE_RESET_FAIL         0xE0040002L

//
// MessageId: HVSTATUS_ERROR_CONTROL_IO_INVALID
//
// MessageText_en:
//
//  The Control IO invalid
//
// MessageText_cn:
//
//  控制通道无效
//
#define HVSTATUS_ERROR_CONTROL_IO_INVALID        0xE0040003L

//
// MessageId: HVSTATUS_ERROR_TRANS_IO_INVALID
//
// MessageText_en:
//
//  The trans IO invalid
//
// MessageText_cn:
//
//  控制通道无效
//
#define HVSTATUS_ERROR_TRANS_IO_INVALID          0xE0040004L


#endif // HV_ERROR_H

