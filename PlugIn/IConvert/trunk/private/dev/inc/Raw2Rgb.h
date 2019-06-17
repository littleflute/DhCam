/**
 @file  Raw2Rgb.h
 @brief  		
 
 Copyright (c) 2009, China Daheng Group,Inc. Beijing Image Vision Technology Branch
 Development Kit
	Microsoft Win32 SDK, Visual C++ 6.00 
 
 @Author Xu Yibo, HuJing, Liumin
 @Date   2009-10-14
 @version V1.7.7.4

*/

#ifndef __RAW_TO_RGB
#define __RAW_TO_RGB

/** By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#else

typedef char bool;

#endif

#include "HVDef.h"

typedef enum tagHV_CONVERT_CODE { 
	CONV_RAW2RGB = 0,
	CONV_GET_WHITEBALANCE_VALUE = 1
} HV_CONVERT_CODE;

typedef enum tagHV_BAYER_CONVERT_TYPE
{
	BAYER2RGB_ZHC,  
	BAYER2RGB_NEIGHBOUR,  
	BAYER2RGB_BILINER,    
	BAYER2RGB_PATTERN,       
	BAYER2RGB_EDGESENSING,  
	BAYER2RGB_RF,          
	BAYER2RGB_PIXELGROUPING, 
	BAYER2RGB_NEIGHBOUR1,
	BAYER2RGB_ADAPTIVE,	 
	BAYER2RGB_RESERVERED2,	 
	BAYER2RGB_RESERVERED3	
		
} HV_BAYER_CONVERT_TYPE;


typedef struct tagHV_CONVERT_RAW2RGB {	
	BYTE *pDestData;
	HV_COLOR_CODE ccDestColorCode;
	DWORD dwDestBitCount;
	DWORD dwDestDataDepth;
	BYTE *pSrceData;
	HV_COLOR_CODE ccSrceColorCode;
	DWORD dwSrceBitCount;
	DWORD dwSrceDataDepth;
	HV_COLOR_FILTER cfFilter;
	int nWidth;
	int nHeight;
	HV_BAYER_CONVERT_TYPE cvType;
	BYTE *pLutR;
	BYTE *pLutG;
	BYTE *pLutB;
	bool bIsFlip;
	
}HV_CONVERT_RAW2RGB, *PHV_CONVERT_RAW2RGB;


typedef struct tagHV_GET_WHITEBALANCE_VALUE {	
	BYTE *pData;
	HV_COLOR_CODE ccColorCode;
	DWORD dwBitCount;
	DWORD dwDataDepth;
	int nWidth;
	int nHeight;
	double *dValueR;
	double *dValueG;
	double *dValueB;
	
}HV_GET_WHITEBALANCE_VALUE, *PHV_GET_WHITEBALANCE_VALUE;



typedef enum tagHV_BAYER_LAYOUT
{
	BAYER_GB = 1,    ///< start from GB
	BAYER_GR = 2,    ///< start from GR
	BAYER_BG = 3,    ///< start from BG
	BAYER_RG = 4     ///< start from RG
} HV_BAYER_LAYOUT;


typedef enum tagHV_ILLUMINATE
{
	DAYLIGHT,
	FLUORESCENT,
	INCANDESCENT,
}HV_ILLUMINANT;

/**
@brief Convert Raw12Packed to Raw16
@param pInBuf  The input Buffer.      
@param pOutBuf The output Buffer.  
@param iWidth  The width of image.    
@param iHeight The height of image.    
*/
void __stdcall Raw12PackedToRaw16(BYTE* pInBuf, BYTE* pOutBuf,int iWidth, int iHeight);

/**
@brief Convert Raw10Packed to Raw16
@param pInBuf  The input Buffer.      
@param pOutBuf The output Buffer.  
@param iWidth  The width of image.    
@param iHeight The height of image.    
*/
void __stdcall Raw10PackedToRaw16(BYTE* pInBuf, BYTE* pOutBuf,int iWidth, int iHeight);


/**
@brief Color Correction for AOI
@param pImg		: RGB Image Buffer
@param roi_left	: Left of ROI in Pixel
@param roi_top	: Top of ROI 
@param roi_wid	: Width of ROI
@param roi_hei	: Height of ROI
@param nImgWid	: Width of Image
@param nImgHei	: Height of Image
*/
void __stdcall ROIColorCorrection( void *pImg, int roi_left, int roi_top, int roi_wid, int roi_hei, int nImgWid, int nImgHei);

/*!
\brief 
8λͼ��˳ʱ����ת90��
\param 
pInputBuffer  
[in]    ����ͼ�񻺳���
\param 
nWidth
[in]    ͼ����
\param
nHeight
[in]    ͼ��߶�
\param 
pOutputBuffer  
[in]    ���ͼ�񻺳���
\return

*/
void __stdcall HVRotate90CW8B(
					const BYTE* pInputBuffer, 
					int nWidth, 
					int nHeight, 
					BYTE* pOutputBuffer);
					
/*!
\brief 
8λͼ����ʱ����ת90��
\param 
pInputBuffer  
[in]    ����ͼ�񻺳���
\param 
nWidth
[in]    ͼ����
\param
nHeight
[in]    ͼ��߶�
\param 
pOutputBuffer  
[in]    ���ͼ�񻺳���
\return

*/
void __stdcall HVRotate90CCW8B(
					const BYTE* pInputBuffer, 
					int nWidth, 
					int nHeight, 
					BYTE* pOutputBuffer);

/**
@brief ͼ�����ȵ���
@param pInputBuffer  ����ͼ�񻺳���      
@param nImagesize    ԭʼͼ��Ĵ�С�����ֽ�Ϊ��λ
@param factor        �������ӣ���Χ��-150~150
@param pOutputBuffer ���ͼ�񻺳���
*/
HVSTATUS __stdcall HVBrightness(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

/**
@brief ͼ��Աȶȵ���
@param pInputBuffer		����ͼ�񻺳���      
@param nImagesize		ԭʼͼ��Ĵ�С�����ֽ�Ϊ��λ
@param factor			�������ӣ���Χ��-50~100
@param pOutputBuffer	���ͼ�񻺳���
*/
HVSTATUS __stdcall HVContrast(BYTE* pInputBuffer,int nImagesize, int nFactor,BYTE* pOutputBuffer);

/**
@brief 24λRGBͼ����
@param pInputBuffer  ����ͼ�񻺳���      
@param nWidth  ͼ����
@param nHeight ͼ��߶�
@param factor �������ӣ���Χ��0.1~5.0
@param pOutputBuffer ���ͼ�񻺳���
*/
HVSTATUS __stdcall HVSharpen24B(BYTE* pInputBuffer,int nWidth,int nHeight,float factor,BYTE* pOutputBuffer);


void __stdcall HVConvert(HV_CONVERT_CODE ConvertCode, void *pContext, int *pLength);
void __stdcall ConvertBayer2Rgb(BYTE *pDest,BYTE *pSrce, int nWid,int nHei,HV_BAYER_CONVERT_TYPE cvtype,BYTE* pLutR,BYTE* pLutG, BYTE* pLutB,bool bFlip,HV_BAYER_LAYOUT Layout);
void __stdcall GetWhiteBalanceRatio(BYTE *pSrce,int nWid,int nHei,double* dRatioR,double* dRatioG,double* dRatioB);
void __stdcall SetGammaLut(BYTE pLutGamma[256],double dGammaRatio);
void __stdcall DetectDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nImgWid,int nImgHei);
void __stdcall EraseDeadPixel(BYTE * pRawImgBuf,BYTE * pBadPixelPosBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nBadPixelBufWid,int nBadPixelBufHei);
void __stdcall GetPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nPatternWid,int nPatternHei);
void __stdcall AvgPatternNoise(int * pPatternBuf,int nPatternWid,int nPatternHei,int nAvgTimes);
void __stdcall FixPatternNoise(BYTE * pRawImgBuf,int * pPatternBuf,int nXPos,int nYPos,int nImgWid,int nImgHei,int nPatternWid,int nPatternHei);
void __stdcall SetBadPixelThreshold(int BadPixelThreshold);


#ifdef __cplusplus
}
#endif

#endif


