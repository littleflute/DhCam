// ImageView.cpp : implementation file
//

#include "stdafx.h"
#include "HVColor.h"
#include "ImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_nBitDepth;
extern BITMAPINFO *g_pBmpInfo;
/////////////////////////////////////////////////////////////////////////////
// CImageView

CImageView::CImageView(BYTE * pImage, int nWid, int nHei) : m_pImage(pImage), 
	m_nWid(nWid), m_nHei(nHei), m_bUpdate(TRUE)
{
}

CImageView::~CImageView()
{
}


BEGIN_MESSAGE_MAP(CImageView, CStatic)
	//{{AFX_MSG_MAP(CImageView)
	ON_WM_SETCURSOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//

void CImageView::Draw(CDC & dc, void * pImage, int nWid, int nHei)
{
	if ( !m_bUpdate )
		return;

	if ( !pImage ) 
		return;	

	g_pBmpInfo->bmiHeader.biWidth	= nWid;
	g_pBmpInfo->bmiHeader.biHeight	= nHei;
	g_pBmpInfo->bmiHeader.biBitCount = g_nBitDepth;
	
	if (g_nBitDepth <24 )
	{
		CRect rect;
		GetClientRect(&rect);
		
		dc.SetStretchBltMode(STRETCH_HALFTONE);
		SetBrushOrgEx(dc.GetSafeHdc(), 0, 0, NULL);

		StretchDIBits(
			dc.m_hDC,
			0,
			0,
			rect.Width(),
			rect.Height(),
			0,
			0,
			nWid,
			nHei,
			pImage,
			g_pBmpInfo,
			DIB_RGB_COLORS,
			SRCCOPY
			);
	}
	else
	{
		// Get display rect
		CRect rect;
		GetClientRect(&rect);

        //
		// Display image
        // When used with COLORONCOLOR mode, the gray image has many bad pixels,
        // i test STRETCH_HALTONE, it's good, so i changed this code.
        //

        //dc.SetStretchBltMode(COLORONCOLOR);
		dc.SetStretchBltMode(STRETCH_HALFTONE);
		SetBrushOrgEx(dc.GetSafeHdc(), 0, 0, NULL);

		StretchDIBits(
			dc.m_hDC,					// handle to device context
			0,							// x-coordinate of upper-left corner of dest. rectangle
			0,							// y-coordinate of upper-left corner of dest. rectangle
			rect.Width(),				// width of destination rectangle
			rect.Height(),				// height of destination rectangle
			0,							// x-coordinate of upper-left corner of source rectangle
			0,							// y-coordinate of upper-left corner of source rectangle
			nWid,						// width of source rectangle
			nHei,						// height of source rectangle
			pImage,						// address of bitmap bits
			g_pBmpInfo,					// address of bitmap data
			DIB_RGB_COLORS,             // usage flags
			SRCCOPY                     // raster operation code	       		
			);
	}

	// Draw image rect
	for (int i = 0; i < m_arrayRect.GetSize(); ++i) {			
		m_arrayRect[i].Draw(&dc);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageView message handlers

void CImageView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	Draw(dc, m_pImage, m_nWid, m_nHei);
}

BOOL CImageView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	for (int i = 0; i < m_arrayRect.GetSize(); ++i)
	{
		if( m_arrayRect[i].SetCursor(this, nHitTest) )
			return true;
	}	
	
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

