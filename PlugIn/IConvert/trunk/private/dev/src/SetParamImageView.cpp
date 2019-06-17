// SetParamImageView.cpp: implementation of the CSetParamImageView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvcolor.h"
#include "SetParamImageView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern int m_nNowDepth;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CSetParamImageView::CSetParamImageView()
{
	
}

CSetParamImageView::~CSetParamImageView()
{

}

BEGIN_MESSAGE_MAP(CSetParamImageView, CImageView)
	//{{AFX_MSG_MAP(CSetParamImageView)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// Implement

void CSetParamImageView::GetValidRect(CRect * rect)
{
	GetClientRect(rect);
	rect->left   += 4;
	rect->right  -= 4;
	rect->top    += 4;
 	rect->bottom -= 4;
}

BOOL CSetParamImageView::IsValidClick(CPoint point)
{
	CRect          rect;
	GetValidRect(&rect);
	return rect.PtInRect(point);
}

BOOL CSetParamImageView::HitRectBorder(CPoint point)
{
	CRect          trueRect;	
	m_arrayRect[0].GetTrueRect(trueRect);		
	return trueRect.PtInRect(point) && !m_arrayRect[0].m_rect.PtInRect(point);
}

//////////////////////////////////////////////////////////////////////////
// Operation

void CSetParamImageView::InitialRect(BITMAPINFO *pBmpInfo, int nNowDepth)
{
	CRect         rect;
	GetValidRect(&rect);
	m_arrayRect.RemoveAll();
	m_arrayRect.Add(CImageRectTracker(rect, CRectTracker::hatchedBorder|CRectTracker::resizeOutside));
	m_pBmpInfo = pBmpInfo;
	m_nNowDepth = nNowDepth;
}

void CSetParamImageView::UpdateImage(void * pImage, int nWid, int nHei)
{
	if(m_pSCImage)
		delete m_pSCImage;
	
	m_pSCImage = new SCImage;
	CSize ImageSize;
	ImageSize.cx = nWid;
	ImageSize.cy = nHei;
	
	if ( m_pSCImage->Create(ImageSize, m_nNowDepth, CPoint(0,0)))
		CopyMemory(*(m_pSCImage), pImage, nHei * BYTESPERLINE(nWid, m_nNowDepth) );
	
	m_pImage = (BYTE*)pImage;
	m_nWid   = nWid;
	m_nHei   = nHei;
}

void CSetParamImageView::GetSelectRect(CRect * rect, int nWid, int nHei)
{
	*rect = m_arrayRect[0].m_rect;
	LogicRcToImageRc(rect, nWid, nHei);
	VerticalInvertRect(rect, nHei); 
}

void CSetParamImageView::GetCursorPoint(CPoint * point, int nWid, int nHei)
{
	GetCursorPos(point);
	ScreenToClient(point);
	LogicPtToImagePt(point, nWid, nHei);
	point->y = nHei - point->y;
}

void CSetParamImageView::GetRectInfo(CString * info, void *pSrc, int nWid, int nHei)
{
	CRect                      rect;
	GetSelectRect(&rect, nWid, nHei);
	
	int grayValue = 0;
	int rValue = 0;
	int gValue = 0;
	int bValue = 0;
	if(m_nNowDepth < 24)
	{
		CColorRender::GetRectGrayValue((BYTE*)pSrc + (rect.top * nWid  + rect.left), 
			rect.Width(), rect.Height(), nWid, &grayValue);
		
		info->Format(" AOI:(%d, %d): %d x %d Gray:%d", rect.left, nHei - rect.bottom,
			rect.Width(), rect.Height(), grayValue);		
	}
	else
	{
		CColorRender::GetRectRGBValue((BYTE*)pSrc + (rect.top * nWid  + rect.left)* 3, 
			rect.Width(), rect.Height(), nWid * 3, &rValue, &gValue, &bValue);
		
		info->Format(" AOI:(%d, %d): %d x %d R:%d G:%d B:%d", rect.left, nHei - rect.bottom,
			rect.Width(), rect.Height(), rValue, gValue, bValue);		
	}

}

void CSetParamImageView::GetPointInfo(CString * info,  void *pSrc, int nWid, int nHei)
{
	CPoint                      point;
	GetCursorPoint(&point, nWid, nHei);

	CRect rect(0, 0, nWid, nHei);

	if ( rect.PtInRect(point) )
	{
		if(m_nNowDepth < 24)
		{
			BYTE * pCurrent = (BYTE*)pSrc + point.y * nWid + point.x;
			
			info->Format("(%d, %d) Gray:%d ", point.x, nHei - point.y, *pCurrent);			
		}
		else
		{
			BYTE * pCurrent = (BYTE*)pSrc + point.y * nWid * 3 + point.x * 3;
			
			info->Format("(%d, %d) R:%d G:%d B:%d", point.x, nHei - point.y, 
				*(pCurrent+2), *(pCurrent+1), *pCurrent);			
		}
	}
	
	else
	{
		info->Format("");
	}
}

//////////////////////////////////////////////////////////////////////////
//

void CSetParamImageView::LogicPtToImagePt(CPoint * point, int nWid, int nHei)
{
	CRect          rect;
	GetClientRect(&rect);
	point->x = (int)(point->x * (double)nWid / rect.Width());
	point->y = (int)(point->y * (double)nHei / rect.Height());
}

void CSetParamImageView::LogicRcToImageRc(CRect * rect, int nWid, int nHei)
{
	LogicPtToImagePt(&(rect->TopLeft()), nWid, nHei );
	LogicPtToImagePt(&(rect->BottomRight()), nWid, nHei);
}

void CSetParamImageView::VerticalInvertRect(CRect * rect, int nHei)
{
	CRect temp   = *rect;
	rect->top    = nHei - temp.bottom;
	rect->bottom = nHei - temp.top;
}

//////////////////////////////////////////////////////////////////////////
// CSetParamImageView message header

void CSetParamImageView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect new_rect;
	CRect old_rect;

	GetValidRect(&new_rect);
	GetClipCursor(&old_rect);

	CImageRectTracker tmp_tracker(m_arrayRect[0].m_rect, CRectTracker::hatchedBorder);

	m_bUpdate = FALSE;

	if ( !HitRectBorder(point) && IsValidClick(point) )
	{
		ClientToScreen(&new_rect);
		ClipCursor(&new_rect);

		CRectTracker tracker;

		if ( tracker.TrackRubberBand(this, point, FALSE) )
		{
			tracker.m_rect.NormalizeRect();
			m_arrayRect[0].m_rect = tracker.m_rect;

			tmp_tracker.Draw(GetDC());
			m_arrayRect[0].Draw(GetDC());
		}
	}
	
	if ( HitRectBorder(point) )
	{
		if ( m_arrayRect[0].HitTest(point) == CRectTracker::hitMiddle )
		{
			new_rect.left   += point.x - m_arrayRect[0].m_rect.left;
			new_rect.right  += point.x - m_arrayRect[0].m_rect.right;
			new_rect.top    += point.y - m_arrayRect[0].m_rect.top;
			new_rect.bottom += point.y - m_arrayRect[0].m_rect.bottom;
		}

		ClientToScreen(&new_rect);
		ClipCursor(&new_rect);
		
		if (m_arrayRect[0].Track(this, point))
		{
			tmp_tracker.Draw(GetDC());
			m_arrayRect[0].Draw(GetDC());
		}
	}

	m_bUpdate = TRUE;
	
	ClipCursor(&old_rect);
}

