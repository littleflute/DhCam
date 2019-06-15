// Camera.h: interface for the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_H__1F5B549A_1DE2_4317_B2E3_88608B1F8C35__INCLUDED_)
#define AFX_CAMERA_H__1F5B549A_1DE2_4317_B2E3_88608B1F8C35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagHVFile {
	char szfile[296];
} HV_FILE;

#define nMaxWidth       4272   //载入图片
#define nMaxHeight      2848 
#define nMaxPicCount    60

class CCamera  
{
public:
	CCamera();
	virtual ~CCamera();
public:
	char SNum[20];
	void _Set_Device_Path(char *pszPath);
	HVSTATUS ORD_Fun_Interface_1(void *pInBuffer);
	BOOL OpenRawFile(LPCSTR lpFileName, BYTE *pRawBuffer);
	HVSTATUS local_SnapShot(void *pInBuffer);
	HANDLE Get_Device_HANDLE(TCHAR* szDeviceName);

	HVSTATUS Control(HV_CONTROL_CODE code, 
			 void *pInBuffer, DWORD nInBufferSize, 
			 void *pOutBuffer, DWORD nOutBufferSize);

	DWORD DisplayThreadProc( );
	HV_SNAP_PROC m_lpSnapProc;
    void*  m_pSnapParam;
	DWORD  m_BufferNum;
	BYTE** m_ppSnapBuffer;
	HANDLE m_hDisplayThread;
	UINT  m_bStopDisplay;
	UCHAR* m_pData;

	USHORT  m_Width;
    USHORT  m_Height;
	USHORT  m_Left;
	USHORT  m_Top;

	int				m_nRGain;
	int				m_nBGain;
	int				m_nGain;
	int				m_nShutter;
	int				m_nBrightness;
    DWORD           m_dwTriggerOnOff;
	HANDLE			m_hSoftTrigger; 

private:
	DWORD m_T1,m_T2;
	BITMAPINFO *m_pBmpInfo;      
	char m_chBmpBuf[2048];       
	BYTE *m_pImageBuffer;  //载入的图片数据缓冲区
    HANDLE m_hDisplayOver;
	HV_FILE m_hvFile[nMaxPicCount];
	int  m_nPicCount;      //载入的图片数
	BOOL	m_bDeviceRemove;  
	
	char	m_szDevicePath[128];
	char	m_szSN[15];

private:
	void local_FindMyFile(char* pfileType,HV_FILE* pHvfile,int* pCount);
	void local_UnBayerTest(int w,int h,BYTE*pRGBBuf);
	HVSTATUS F_SET_FEATURE(void *pInBuffer);
	HVSTATUS F_ORD_QUERY_STRING_DESCRIPTOR(void*pInPut,void*pOut);
	HVSTATUS F_ORD_GET_STRING(void*pInPut,void*pOut);
	static DWORD WINAPI local_Stub_Display_Fun(LPVOID lpParam);
	HVSTATUS local_Get_SN(void *pContext,int *pSize);
	HVSTATUS local_Get_FPGA_Ver(void *pContext,int *pSize);
	HVSTATUS local_Get_Bayer_Layout(void *pContext,int *pSize);

	HVSTATUS local_OpenSnap(void *pInBuffer);
	HVSTATUS local_StartSnap(void *pInBuffer);
	HVSTATUS local_StopSnap();
	HVSTATUS local_CloseSnap();
};

#endif // !defined(AFX_CAMERA_H__1F5B549A_1DE2_4317_B2E3_88608B1F8C35__INCLUDED_)
