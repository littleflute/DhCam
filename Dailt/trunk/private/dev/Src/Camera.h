// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_H__AB211B0C_C50D_4970_9ED5_55027A70B77D__INCLUDED_)
#define AFX_CAMERA_H__AB211B0C_C50D_4970_9ED5_55027A70B77D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#define SENSOR_START_REG						0xF0F00300L		//设置操作Sensor的起始寄存器序号
#define SENSOR_I2C_OPERATE						0xF0F003A0L		//对SENSOR寄存器操作的地址
#define NOISE_CONTROL							0xF0F008C8L		//相机10选8控制

//单色黑电平调节
#define REG_CAMERA_GREEN1_BRIGHTNESS_CTL		0xF0F008E4L     //绿一
#define REG_CAMERA_GREEN2_BRIGHTNESS_CTL		0xF0F008E8L     //绿二
#define REG_CAMERA_BLUE_BRIGHTNESS_CTL			0xF0F008ECL     //蓝色
#define REG_CAMERA_RED_BRIGHTNESS_CTL			0xF0F008F0L     //红色 

//DHCAM协议版本
#define DHCAM_VERSION_ADDRESS                   0xF2F00018
#define DHCAM_MIN_VERSION_SUPPORT_WHITE_BALANCE 0x00010501

class CCamera  
{
public:
	HVSTATUS Get_IP(HANDLE hDevice, char *pOut);
	HVSTATUS Get_MAC(HANDLE hDevice,char *pOut);
	CCamera();
	virtual ~CCamera();
	
	HVSTATUS DevInfo_Get_HVAPI_Ver(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Device_Drv_Ver(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Sys_Ver(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Hardware_Ver(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_FPGA_Ver(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_SN(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Bayer_Layout(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_SoftWare_Ver(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Resolution(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Device_Type(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Sensor_ID(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Device_Name(HANDLE hDevice, void *pContext,int *pSize);
	HVSTATUS DevInfo_Get_Vender_Name(HANDLE hDevice, void *pContext,int *pSize);

	HVSTATUS Set_AOI(HANDLE hDevice,HV_VIDEO_MODE mode,int nXStart, int nYStart, int nWidth, int nHeight);
	HVSTATUS Set_Snap_Mode(HANDLE hDevice,HV_SNAP_MODE mode);
	HVSTATUS Set_Set_Video_Mode(HANDLE hDevice,HV_VIDEO_MODE mode);
	HVSTATUS Set_Trigger_Polarity(HANDLE hDevice, HV_POLARITY polarity);
	HVSTATUS Set_Strobe_Polarity(HANDLE hDevice, HV_POLARITY polarity);
	HVSTATUS Set_Strobe_On_Off(HANDLE hDevice, bool b);
	HVSTATUS Fun_Interface1(HANDLE hDevice, void *pCtx);
	HVSTATUS Set_Freeze_Frame_On_Off(HANDLE hDevice, bool b);
	HVSTATUS Set_LUT(HANDLE hDevice, void *pCtx);
	HVSTATUS Set_Packet_Size(HANDLE hDevice, void *pCtx);

	HVSTATUS Set_Exposure_Time(HANDLE hDevice, long lVal);
	HVSTATUS Set_10_2_8_Mode(HANDLE hDevice, long lVal);
	HVSTATUS Set_Black_Level_On_Off(HANDLE hDevice, long lVal);
	HVSTATUS Set_Brightness(HANDLE hDevice,HV_ADC_CONTROL id, long lVal);

	HVSTATUS _Read_Quadlet(HANDLE hDevice,DWORD address, DWORD *pData);
private:	
	HVSTATUS _Write_Quadlet(HANDLE hDevice,DWORD address, DWORD dwData);
	HVSTATUS _Read_Block(HANDLE hDevice,DWORD address, DWORD *pData,int nLength);
	HVSTATUS _Write_Block(HANDLE hDevice,DWORD address, DWORD *pData,int nLength);
	HVSTATUS _Write_Sensor_Registers(HANDLE hDevice,DWORD* pData, DWORD dwStart,DWORD dwLen);
	HVSTATUS _Read_Sensor_Registers(HANDLE hDevice,DWORD* pData, DWORD dwStart,DWORD dwLen);

};

#endif // !defined(AFX_CAMERA_H__AB211B0C_C50D_4970_9ED5_55027A70B77D__INCLUDED_)
