//-----------------------------------------------------------------------------
//  Company:  Basler Vision Technologies
//  Section:  Vision Components
//  Project:  1394 Driver
//  Subproject:  BcamViewer
//  <b> Visual SourceSafe Information:</b>
//  $Archive: /Software/src/BCAMViewer/Image.h $
//  $Author: Happe, A.$
//  $Revision: 3$
//  $Date: 9/25/02 11:15:55 AM$
//-----------------------------------------------------------------------------
/**
\file     Image.h
*
*  *
\brief   Interfaces for the CHVBitmap classe
*/
//-----------------------------------------------------------------------------


#if !defined(AFX_IMAGEBUFFER_H__F4349286_8B01_11D5_921E_0090278E5E96__INCLUDED_)
#define AFX_IMAGEBUFFER_H__F4349286_8B01_11D5_921E_0090278E5E96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "utility.h"

//------------------------------------------------------------------------------
// class CHVBitmap
// Author: 
// Date: 20.09.2002
//------------------------------------------------------------------------------
/**
* \brief   Wrapper class for DIBSections
*
* This wrapper for DIBSections is used by the BcamViewer as image buffers and to 
* display grabbed images. A CHVBitmap holds additional information, e.g. the size
* of the sensor, the AOI size and position
* 
* 
*/
//------------------------------------------------------------------------------

class CHVBitmap 
{
public:
	CHVBitmap(CSize& ImageSize, CSize& SensorSize, CPoint& Origin, HV_COLOR_CODE ColorCode) 
		:
    m_Size(ImageSize),
	m_SensorSize(SensorSize),
	m_Origin(Origin),
	m_ColorCode(ColorCode),
	m_bHasUserData(false),
	m_UserData(0)
	{ 
		m_BufferSize = ImageSize.cx * ImageSize.cy * HVBaseUtility::BitsPerPixel(ColorCode) / 8;
		
		BITMAPINFO* pBmi;
		if ( (ColorCode != COLOR_MONO8) && (ColorCode != COLOR_RAW8))//modified by xupx 2009.03.05
		{ 
			pBmi = (BITMAPINFO*) new char[sizeof(BITMAPINFO)];
			if ( pBmi == NULL )
			{
				throw HVBaseException(STATUS_NOT_ENOUGH_SYSTEM_MEMORY, "CHVBitmap()");
			}
			ZeroMemory(pBmi, sizeof(BITMAPINFO) );
			pBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			pBmi->bmiHeader.biWidth = m_Size.cx;
			pBmi->bmiHeader.biHeight =  m_Size.cy;
			pBmi->bmiHeader.biPlanes = 1;
			pBmi->bmiHeader.biCompression = BI_RGB;
			pBmi->bmiHeader.biSizeImage = 0;
			pBmi->bmiHeader.biXPelsPerMeter = 0;
			pBmi->bmiHeader.biYPelsPerMeter = 0;
			pBmi->bmiHeader.biClrImportant = 0;
			pBmi->bmiHeader.biBitCount = HVBaseUtility::BitsPerPixel(ColorCode);
			pBmi->bmiHeader.biClrUsed = 0;
			m_bIsTopDown = false;
		}
		else
		{
			pBmi = (BITMAPINFO*) new char[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
			if ( pBmi == NULL )
			{
				throw HVBaseException(E_OUTOFMEMORY, "CHVBitmap()");
			}
			ZeroMemory(pBmi, sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD));
			pBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			pBmi->bmiHeader.biWidth = m_Size.cx;
			pBmi->bmiHeader.biHeight =  m_Size.cy;
			pBmi->bmiHeader.biPlanes = 1;
			pBmi->bmiHeader.biCompression = BI_RGB;
			pBmi->bmiHeader.biSizeImage = 0;
			pBmi->bmiHeader.biXPelsPerMeter = 0;
			pBmi->bmiHeader.biYPelsPerMeter = 0;
			pBmi->bmiHeader.biClrImportant = 0;
			pBmi->bmiHeader.biBitCount = 8;
			pBmi->bmiHeader.biClrUsed = 0;
		
			for ( int i = 0; i < 256; i++ ){
				pBmi->bmiColors[i].rgbBlue = i;
				pBmi->bmiColors[i].rgbGreen = i;
				pBmi->bmiColors[i].rgbRed = i;
			}
			
			m_bIsTopDown = false;
		}  
		if ( m_Bitmap.CreateDIBSection(NULL, pBmi, DIB_RGB_COLORS, &m_pPixelData, NULL, NULL ) == NULL )
		{
			delete [] pBmi;
			throw HVBaseException(::GetLastError(), "CHVBitmap::CHVBitmap()");
		}
		delete [] pBmi;
	}
	
	CHVBitmap(CString FileName) 
		: m_Origin(0,0),
		m_bHasUserData(false),
		m_UserData(0)
	{
		HBITMAP hBitmap = (HBITMAP) ::LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if ( hBitmap == NULL )
			throw HVBaseException(::GetLastError(), "LoadImage()");
		m_Bitmap.Attach(hBitmap);
		// retrieve information about loaded image
		DIBSECTION DibSection;
		::GetObject(hBitmap, sizeof(DibSection), &DibSection);
		if ( DibSection.dsBmih.biBitCount == 8 )
			m_ColorCode = COLOR_MONO8;
		else if ( DibSection.dsBmih.biBitCount == 16 )
			m_ColorCode = COLOR_MONO16;
		else if ( DibSection.dsBmih.biBitCount == 24 )
			m_ColorCode = COLOR_RGB8;
		else{
			m_Bitmap.DeleteObject();
			throw HVBaseException(STATUS_FILE_INVALID, "LoadImage()");
		}
		m_Size.cx = m_SensorSize.cx = DibSection.dsBmih.biWidth;
		m_Size.cy = m_SensorSize.cy = DibSection.dsBmih.biHeight;
		// retrieve pointer to pixel
		m_pPixelData = DibSection.dsBm.bmBits;
		
		// images load from file are bottom-up images
		m_bIsTopDown = false;
		
	}
	
	virtual ~CHVBitmap()
	{
	}
	
	
	bool Save(CString FileName);
	bool CopyToClipboard(HWND hWnd);
	
	CSize GetSize() 
	{
		return m_Size;
	}
	
	CSize GetSensorSize()
	{
		return m_SensorSize;
	}
	
	CPoint GetOrigin()
	{
		return m_Origin;
	}
	
	void SetOrigin(CPoint Origin)
	{
		m_Origin = Origin;
	}
	
	CRect GetRect()
	{
		return CRect(m_Origin, m_Size);
	}
	
	HV_COLOR_CODE GetColorCode()
	{
		return m_ColorCode;
	}
	
	RGBTRIPLE operator()(unsigned int x, unsigned int y);
	
	operator CBitmap&() { return m_Bitmap; }
	
	operator const CBitmap&() const { return m_Bitmap; }
	
	operator HBITMAP() { return (HBITMAP) m_Bitmap; }
	
	operator void*()   { return m_pPixelData; }
	
	operator PBYTE()   { return (PBYTE) m_pPixelData; }
	
	unsigned long GetBufferSize()
	{
		return m_BufferSize;
	}
	
	void SetUserData(long data) { m_UserData = data; m_bHasUserData = true; }
	
	long GetUserData() { return m_UserData; }
	
	bool HasUserData() { return m_bHasUserData; }
	
	BOOL BitBlt(HDC hDest, HDC hSource, DWORD dwRop = SRCCOPY)
	{
		::BitBlt(hDest, m_Origin.x, m_Origin.y, m_Size.cx, m_Size.cy, hSource, 0, 0, dwRop);
		return 0;
	}
	
	bool IsTopDown() { return m_bIsTopDown; }
	
private:
	// hide copy constructor and assignment operator
	CHVBitmap( const CHVBitmap& );
	CHVBitmap& operator=( const CHVBitmap& );
	
	bool m_bIsTopDown;
	
public:
	void*           m_pPixelData;
	CBitmap         m_Bitmap;
	CSize           m_Size;
	CSize           m_SensorSize;
	CPoint          m_Origin;
	HV_COLOR_CODE     m_ColorCode;
	unsigned long   m_BufferSize;
	bool            m_bHasUserData;
	long            m_UserData;
	
};



#endif // !defined(AFX_IMAGEBUFFER_H__F4349286_8B01_11D5_921E_0090278E5E96__INCLUDED_)

