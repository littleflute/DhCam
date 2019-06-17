/**
	@file  CDib.cpp
	@brief the implimentation of SCImage class

	Copyright (c) 2009, Daheng Imavision 
  
	@author Pengxiang Xu
	@date 2009-05-25
	@version 1.0.1.0
*/

#include "stdafx.h"
#include "CDib.h"


SCImage::SCImage()
:
m_Size(0, 0),
m_Origin(0, 0),
m_bHasUserData(false),
m_UserData(0),
m_BufferSize(0),
m_bpp(0)
{
}

/**
	创建图像存储空间

    @param ImageSize      图像尺寸
	@param bitDepth       位深度
	@param Origin         图像起始点
	@return				  成功返回 true；失败返回 false
*/

bool SCImage::Create(CSize& ImageSize, int bitDepth, CPoint& Origin)
{	
	BITMAPINFO* pBmi;
	
	char buffer[4096];
	pBmi = (BITMAPINFO *)buffer;
	
	ZeroMemory(pBmi, sizeof(BITMAPINFO) );
	pBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBmi->bmiHeader.biWidth = ImageSize.cx;
	pBmi->bmiHeader.biHeight = ImageSize.cy;
	pBmi->bmiHeader.biPlanes = 1;
	pBmi->bmiHeader.biCompression = BI_RGB;
	pBmi->bmiHeader.biSizeImage = 0;
	pBmi->bmiHeader.biXPelsPerMeter = 0;
	pBmi->bmiHeader.biYPelsPerMeter = 0;
	pBmi->bmiHeader.biClrImportant = 0;
	pBmi->bmiHeader.biBitCount = bitDepth;
	
	pBmi->bmiHeader.biClrUsed = bitDepth == 8 ? 256 : 0;
	
	m_bIsTopDown = true;
	
	if(bitDepth == 8)
	{
		pBmi->bmiHeader.biHeight = -ImageSize.cy;
		for ( int i = 0; i < 256; i++ ){
			pBmi->bmiColors[i].rgbBlue = i;
			pBmi->bmiColors[i].rgbGreen = i;
			pBmi->bmiColors[i].rgbRed = i;
		}			
	}
	
	if ( m_Bitmap.CreateDIBSection(NULL, pBmi, DIB_RGB_COLORS, &m_pPixelData, NULL, NULL ) == NULL )
	{
		return false;
	}

	m_Size = ImageSize;
	m_Origin	 = Origin;
	m_bHasUserData = false;
	m_UserData	   = 0;
	m_bpp = bitDepth;
	m_BufferSize = ImageSize.cy * BYTESPERLINE(ImageSize.cx, bitDepth);

	return true;
}

