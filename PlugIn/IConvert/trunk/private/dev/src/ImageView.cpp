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

//extern int m_nNowDepth;
//extern BITMAPINFO *m_pBmpInfo;
/////////////////////////////////////////////////////////////////////////////
// CImageView

CImageView::CImageView(BYTE * pImage, int nWid, int nHei) : m_pImage(pImage), 
	m_nWid(nWid), m_nHei(nHei), m_bUpdate(TRUE)
{
	m_pSCImage = NULL;	
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
void CImageView::OnPrepareDC(CDC *pDC)
{
	CRect	ClientRect;
	this->GetClientRect(ClientRect);
	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_nWid, m_nHei);
	pDC->SetViewportExt(ClientRect.Width(), ClientRect.Height());
	
 	pDC->SetViewportOrg(0, 0);
}

void CImageView::OnDraw(CDC *pDC)
{
	if (m_pSCImage == NULL)
		return;
	
	HDC hdc = pDC->GetSafeHdc();
	HDC dcMem = NULL;
	int status = 0;
	
	CRect	ClientRect;
	this->GetClientRect(ClientRect);
	
	dcMem = ::CreateCompatibleDC(hdc);	
	//////////////////////////////////////////////////////////////////////////
 	OnPrepareDC(pDC);
	//////////////////////////////////////////////////////////////////////////
	
	CRect rect;
	GetClientRect(rect);
	
	CRect ImageRect = m_pSCImage->GetRect();  
	rect.IntersectRect(rect, ImageRect);	
	
	HBITMAP hBmpOld = (HBITMAP)::SelectObject(dcMem, (HBITMAP)(*m_pSCImage));
	
	pDC->SetStretchBltMode(COLORONCOLOR);
	
	::BitBlt(hdc, 0, 0, m_pSCImage->GetWidth(), m_pSCImage->GetHeight(), dcMem, 0, 0, SRCCOPY);	
	
	::SelectObject(dcMem, hBmpOld);
	
	DeleteDC(dcMem);	
	
	// Draw image rect
	for (int i = 0; i < m_arrayRect.GetSize(); ++i) {			
		m_arrayRect[i].Draw(pDC);		
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CImageView message handlers

void CImageView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	if(m_pSCImage)
		OnDraw(&dc);
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

