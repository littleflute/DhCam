//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ImageView.cpp, 5, 10/24/02 3:26:24 PM, Happe, A.$
//-----------------------------------------------------------------------------
/**
\file     ImageView.cpp
*
* \brief Implementation of CImageRectTracker and CImageView
*

*/
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "RectTracker.h"
#include "imageview.h"



/////////////////////////////////////////////////////////////////////////////
// CImageRectTracker

CImageRectTracker::CImageRectTracker(CImageView& view)
: CRectTracker(CRect(0,0,0,0), solidLine | resizeOutside | hatchedBorder),
m_View(view)
{
    m_nHandleSize = 8;
}

void CImageRectTracker::AdjustRect(int nHandle, LPRECT lpRect)
{
	
	// CRectTracker constraints on rectangle dimensions. 
	CRectTracker::AdjustRect(nHandle, lpRect); 
	
	// Enforce rect inside image canvas and adjusted to valid AOI positions and sizes
	m_View.AdjustRect(nHandle, lpRect);  
	
}

void CImageRectTracker::AdjustPoint(LPPOINT lpPoint)
{
	m_View.AdjustPoint(lpPoint);
}

void CImageRectTracker::OnChangedRect(const CRect& rcOld )
{
	m_View.MoveTrackerTo(&m_rect, &rcOld, TRUE);
	CRectTracker::OnChangedRect(rcOld);  
}

void CImageRectTracker::OnTrackingFinished(BOOL bCancelled )
{
	
	m_View.StopTimer();
	if ( ! bCancelled )
	{
		m_View.SetNewAOI( );
	}
	else
	{
		m_View.MoveTrackerTo(m_rect, m_rectSave, TRUE);
//		m_View.SetNewAOI(); //去掉双击效果 HYL 2006.8.17 
	}
	m_View.SetAOIRect(m_rect);

	CRectTracker::OnTrackingFinished(bCancelled);
}


void CImageRectTracker::LPtoDP(LPPOINT lpPoints, int nCount) const 
{ 
	m_View.LPtoDP( lpPoints, nCount);
};

void CImageRectTracker::DPtoLP(LPPOINT lpPoints, int nCount) const
{ 
	m_View.DPtoLP( lpPoints, nCount);
};



/////////////////////////////////////////////////////////////////////////////
// CImageView

const CSize CImageView::s_sizBorder = CSize(10,10);
DWORD CImageView::s_dwDragTimerInterval = 100;


void CImageView::SetSensorSize(CSize SensorSize)
{
	// if ( m_VideoFormat == DCS_Format7)
	{
		// we only allow AOI widths which are multiples of 4, so the sensor rect has to be adjusted
		//   m_pDevice->AlignAOI(SensorSize, m_VideoMode);
	}
	m_SensorSize = SensorSize;
	m_SensorRect = CRect(CPoint(0,0), SensorSize);
	
	double ZoomLevel = m_dZoomScale;  // SetScrollSize resets the zoom level to 1 -> save current zoom state
	SetScrollSize(SensorSize.cx + ( 2 * s_sizBorder.cx ),  SensorSize.cy + ( s_sizBorder.cy * 2 ) );
	if ( ZoomLevel != 1 )
	{  // restore zoom state
		SetZoomLevel(ZoomLevel); 
		ZoomIn(NULL, 0);		// add by xupx 2008.09.22
		ZoomLevelChanged();
	}	
}

void CImageView::ConfigurationChanged(HV_VIDEO_MODE mode, CSize SensorSize, CSize ImageSize, CPoint Origin)
{
	//  if ( format == DCS_Format7 )
	{
		//    m_pDevice->AlignAOI(SensorSize, mode);
	}
	if (/* m_VideoFormat != format  ||*/ m_VideoMode != mode || m_SensorSize != SensorSize  )
	{
//		m_bFit = false;
		//  m_VideoFormat = format;
		m_VideoMode = mode;
		
		if ( SensorSize != m_SensorSize )
		{
			SetSensorSize(SensorSize);
		}
		
		m_bScalable = mode != VIDEO_MODE_IGNORE;
		if ( m_bScalable )
		{
		/*  m_SizeInc.cx = lcm(4, m_pDevice->FormatSeven[mode].Size.Inc().cx);
			m_SizeInc.cy = m_pDevice->FormatSeven[mode].Size.Inc().cy;
			m_PosInc = m_pDevice->FormatSeven[mode].Position.Inc();  */
		
			if (IS_HV_CAM(m_pDevice->m_HVTYPE))
			{
				m_SizeInc.cx =MINWIDTH_HV;//16;//lcm(4, m_pDevice->Format.ImageSize().cx);
				m_SizeInc.cy =MINWIDTH_HV;// 16;//m_pDevice->Format.ImageSize().cy;
				m_PosInc = CPoint(MINPOINT_HV, MINPOINT_HV);//CPoint(16, 16);//m_pDevice->Format.ImagePosition();  
			}
			else{
				m_SizeInc.cx =MINWIDTH;//16;//lcm(4, m_pDevice->Format.ImageSize().cx);
				m_SizeInc.cy =MINWIDTH;// 16;//m_pDevice->Format.ImageSize().cy;
				m_PosInc = CPoint(MINPOINT, MINPOINT);//CPoint(16, 16);//m_pDevice->Format.ImagePosition();  
			}
			m_RectTracker.m_sizeMin = CSize(m_SizeInc.cx, m_SizeInc.cy );
	    		
		/*	m_SizeInc.cx =4;//lcm(4, m_pDevice->Format.ImageSize().cx);
			m_SizeInc.cy = 2;//m_pDevice->Format.ImageSize().cy;
			m_PosInc = CPoint(4, 4);//m_pDevice->Format.ImagePosition();  			
			m_RectTracker.m_sizeMin = CSize(32, 32); */
		}
		
	}
	
	CRect imageRect = CRect(Origin, ImageSize);
	m_RectTracker.m_rect = imageRect;
	
	// if area of interest is not visible -> center viewport on AOI
	if ( m_bScalable )
	{
		CRect client;
		GetClientRect(&client);
		ClientToLogical(client);
		client.IntersectRect(imageRect, client);
		if ( client.IsRectEmpty() )
			UpdateViewPort(NULL);
	}
	
	Invalidate();
	UpdateWindow();
	
	ShowAOISize();
}


void CImageView::DoPaint(CDCHandle dc) 
{
	assert(m_pDevice != NULL );
	CHVBitmap* pBitmap = m_pDevice->GetBitmap();
	if ( pBitmap != NULL )
	{
		
		// Bitblt bitmap
		BitBlt(dc, pBitmap);
		
		
		// if size of image is less than size of sensor fill the parts of the sensor not
		// covered by the image
		CRect ImageRect = pBitmap->GetRect();  // image dimension
		
		HBRUSH hBrush = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
		
		if ( ImageRect.left > m_SensorRect.left )
		{
			RECT rectLeft = m_SensorRect;
			rectLeft.right = ImageRect.left;
			dc.FillRect(&rectLeft, hBrush);
		}
		
		if ( ImageRect.top > m_SensorRect.top )
		{
			RECT rectTop = m_SensorRect;
			rectTop.bottom = ImageRect.top;
			dc.FillRect(&rectTop, hBrush);
		}
		
		if ( ImageRect.right < m_SensorRect.right )
		{
			RECT rectRight = m_SensorRect;
			rectRight.left = ImageRect.right;
			dc.FillRect(&rectRight, hBrush);
		}
		
		if ( ImageRect.bottom < m_SensorRect.bottom  )
		{
			RECT rectBottom  = m_SensorRect;
			rectBottom.top = ImageRect.bottom;
			dc.FillRect(&rectBottom, hBrush);
		}  
	}
	else
	{
		// no bitmap available -> fill the whole sensor area
		dc.FillRect(&m_SensorRect, (HBRUSH) GetStockObject(LTGRAY_BRUSH));
	} 
	
	// draw bounding rectangle around sensor area
	CRect BorderRect = m_SensorRect;
	BorderRect.InflateRect(1,1);
	HBRUSH oldBrush = dc.SelectStockBrush(NULL_BRUSH);
	dc.Rectangle(&BorderRect);
	dc.SelectBrush(oldBrush); 
	
	// draw rubber band
	if ( m_bScalable )
	{
		m_RectTracker.Draw(dc);
	}
	
}

void CImageView::ZoomToFit()
{
	
	if ( ! m_bFit )
	{
		
		// ensure that actually no scroll bars are visible
		m_sizeAll = CSize(0,0);
		RecalcBars();
		
		double scale;
		bool modifyWidth;
		double scaley =  (double) ( m_sizeClient.cy - 2 * s_sizBorder.cy ) / (double) m_SensorSize.cy;
		double scalex = (double) ( m_sizeClient.cx  - 2 * s_sizBorder.cx ) / (double) m_SensorSize.cx;
		if ( scalex < scaley )
		{
			scale = scalex;
			modifyWidth = false;
		}
		else
		{
			scale = scaley;
			modifyWidth = true;
		}
		
		SetZoomLevel(scale); 
		ZoomIn(NULL, 0);
		
		
		if ( ! ::IsZoomed(GetParent()) )
		{
			// Adjust window dimension
			
			CRect newRect(CPoint(0,0), m_sizeClient);
			
			// first calculate new client size 
			if ( modifyWidth )
				newRect.right = 2 * s_sizBorder.cx + m_SensorSize.cx * scale;
			else
				newRect.bottom = 2 * s_sizBorder.cy + m_SensorSize.cy * scale;
			// transform client rect to window rect of inner window
			AdjustWindowRectEx(&newRect, GetWindowLong(GWL_STYLE), FALSE,GetWindowLong(GWL_EXSTYLE));
			// The window rect of the inner window is the desired client rect of the outer window (i.e. the MDI Child window). 
			// Calculate the window rect of the MDI window from the desired client rect
			AdjustWindowRectEx(&newRect, ::GetWindowLong(GetParent(), GWL_STYLE), FALSE, ::GetWindowLong(GetParent(), GWL_EXSTYLE));
			
			// Set the new window rect of the MDI window
			CRect oldRect;
			::GetWindowRect(GetParent(), &oldRect);
			if ( modifyWidth )
				::SetWindowPos(GetParent(), NULL, 0, 0, newRect.Width(), oldRect.Height(), SWP_NOMOVE | SWP_NOZORDER );
			else
				::SetWindowPos(GetParent(), NULL, 0, 0, oldRect.Width(), newRect.Height(), SWP_NOMOVE | SWP_NOZORDER );
		}
		
		m_bFit = true;
	}
	
}




void CImageView::BitBlt(CDCHandle dc, CHVBitmap* pBitmap) {
	
	m_CriticalSection.Lock();
	
	PrepareDC(dc);
	
	// determine region of bitmap which has to be blitted
	
	// first get client rect and transform it into the sensor coordinate system
	CRect rect;
	GetClientRect(rect);
	dc.DPtoLP(rect);
	rect.InflateRect(1,1);
	
	CRect ImageRect = pBitmap->GetRect();  // image dimensions
	
	rect.IntersectRect(rect, ImageRect);
	
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(dc);
	HBITMAP hBmpOld = dcMem.SelectBitmap(*pBitmap);


	if ( m_dZoomScale < 1 )
	{
		dc.SetStretchBltMode(COLORONCOLOR);
	}
	
	if ( m_dZoomScale > 1 )
	{
		// we observed that the performance of StretchBlt may dramatically decrease if the mouse cursor 
		// is placed outside the bitmap window. To avoid this strange behaviour we do a kind of magic. Selecting a 
		// clipping region according to the bitmapsize and reducing it by one single pixel will increase the performance
		// of StretchBlt
		
		CRgn rgn;
		CRect ClipRect = rect;
		dc.LPtoDP(ClipRect);
		
		dc.SaveDC();
		rgn.CreateRectRgnIndirect(ClipRect);
		dc.SelectClipRgn(rgn);
		CRect rect2(0, 0, 1, 1);
		rect2.OffsetRect(max(ClipRect.left, 0), max(ClipRect.top,0));
		CRgn rgn2;
		rgn2.CreateRectRgnIndirect(rect2);
		dc.SelectClipRgn(rgn2, RGN_DIFF);
	}
	
	dc.BitBlt(
		rect.left,
		rect.top, 
		rect.Width(), 
		rect.Height(), 
		dcMem, 
		rect.left - ImageRect.left, 
		rect.top - ImageRect.top, 
		SRCCOPY
		);   
	
	if ( m_dZoomScale > 1 )
		dc.RestoreDC(-1);
	
	dcMem.SelectBitmap(hBmpOld);
	
	if ( m_bScalable && ! ImageRect.EqualRect(m_RectTracker.m_rect) )
	{
		m_RectTracker.Draw(dc);
	}
	
	m_CriticalSection.Unlock();
	
}


LRESULT CImageView::OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	
	assert ( m_pDevice );
	
	CDCHandle dc = (HDC)wParam;
	CRect ClientRect;
	GetClientRect(&ClientRect);
	
	CPoint pt;
	pt.x = m_SensorSize.cx;
	pt.y = m_SensorSize.cy;
	
	PrepareDC(dc);  
	
	dc.DPtoLP(&ClientRect);
	ClientRect.InflateRect(1,1);
	
	if ( ClientRect.left < 0 )
	{
		RECT ClientRectLeft = ClientRect;
		ClientRectLeft.right = 0;
		ClientRectLeft.bottom = pt.y;
		dc.FillRect(&ClientRectLeft, (HBRUSH) GetStockObject(GRAY_BRUSH));
	}
	
	if ( ClientRect.top < 0 )
	{
		RECT ClientRectTop = ClientRect;
		ClientRectTop.bottom = 0;
		dc.FillRect(&ClientRectTop, (HBRUSH) GetStockObject(GRAY_BRUSH));
	}
	
	if( ClientRect.right > m_SensorSize.cx )
	{
		RECT ClientRectRight = ClientRect;
		ClientRectRight.left = pt.x;
		ClientRectRight.bottom = pt.y;
		dc.FillRect(&ClientRectRight, (HBRUSH) GetStockObject(GRAY_BRUSH));
	}
	
	if( ClientRect.bottom > m_SensorSize.cy )
	{
		RECT ClientRectBottom = ClientRect;
		ClientRectBottom.top = pt.y;
		dc.FillRect(&ClientRectBottom, (HBRUSH) GetStockObject(GRAY_BRUSH));
	} 
	
	return 0;    
}


BOOL CImageView::HitTestTrackerBorder()
{
	if ( ! m_bScalable )
		return FALSE;
	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	
	CRect rcTrueRect;
	
	m_RectTracker.GetTrueRect(rcTrueRect);	
	CRect rcTracker = m_RectTracker.m_rect;
	

	LogicalToClient(rcTracker);
	return rcTrueRect.PtInRect(point) && !rcTracker.PtInRect(point);
}



BOOL CImageView::MoveTrackerTo(CRect rcNew, CRect rcOld, BOOL fRepaint)
{
	if ( m_bScalable )
	{
		if (fRepaint)
		{
			Invalidate();
		}  
	}
	
	return TRUE;
}


void CImageView::AdjustRect(int nHandle, LPRECT lpRect)
{
	CRect rc(*lpRect);
	
	CRect rcLimits = m_SensorRect;
	
	if ( nHandle == CRectTracker::hitLeft ||
		nHandle == CRectTracker::hitTopLeft ||
		nHandle == CRectTracker::hitBottomLeft )
	{
		int width = m_PosInc.cx * ( rc.Width() / m_PosInc.cx );//m_SizeInc changed by xupx 2009.10.27
		
		rc.left = rc.right - width;
		if ( rc.left < 0 )
		{
			rc.left = 0;
			rc.right = m_PosInc.cx * ( rc.right / m_PosInc.cx ); //m_SizeInc changed by xupx 2009.10.27
		}
	}
	
	if ( nHandle == CRectTracker::hitRight ||
		nHandle == CRectTracker::hitTopRight ||
		nHandle == CRectTracker::hitBottomRight )
	{
		int width = m_PosInc.cx * ( rc.Width() / m_PosInc.cx );//m_SizeInc changed by xupx 2009.10.27
		
		rc.right = rc.left + width;
		if ( rc.right > rcLimits.right )
		{
			rc.right = rcLimits.right;
			rc.left = max(0, rc.right - m_PosInc.cx * ( rc.Width() / m_PosInc.cx ));//m_SizeInc changed by xupx 2009.10.27
		}
	}
	
	if ( nHandle == CRectTracker::hitTopLeft ||
		nHandle == CRectTracker::hitTop ||
		nHandle == CRectTracker::hitTopRight )
	{
		int height = m_PosInc.cy * ( rc.Height() / m_PosInc.cy );	//m_SizeInc changed by xupx 2009.10.27
		
		rc.top = rc.bottom - height;
		if ( rc.top < 0 )
		{
			rc.top = 0;
			rc.bottom = m_PosInc.cy * ( rc.bottom / m_PosInc.cy );	//m_SizeInc changed by xupx 2009.10.27
		}
	}
	
	if ( nHandle == CRectTracker::hitBottomLeft ||
		nHandle == CRectTracker::hitBottom ||
		nHandle == CRectTracker::hitBottomRight )
	{
		int height = m_PosInc.cy * ( rc.Height() / m_PosInc.cy ); //m_SizeInc changed by xupx 2009.10.27
		
		rc.bottom = rc.top + height;
		if ( rc.bottom > rcLimits.bottom )
		{
			rc.bottom = rcLimits.bottom;
			rc.top = max(0, rc.bottom - m_PosInc.cy * ( rc.Height() / m_PosInc.cy ));//changed by xupx 2009.10.27
		}
	}
	
	if ( nHandle == CRectTracker::hitMiddle)
	{ 
		int width = rc.Width();
		int height = rc.Height();
		
		rc.left = max(0, ( rc.left / m_PosInc.cx ) * m_PosInc.cx);
		rc.top = max(0, ( rc.top / m_PosInc.cy ) * m_PosInc.cy );
		rc.right = rc.left + ( width / m_SizeInc.cx ) * m_SizeInc.cx;
		rc.bottom = rc.top + ( height / m_SizeInc.cy ) * m_SizeInc.cy;
		if ( rc.right > rcLimits.right )
		{
			rc.right = rcLimits.right;
			rc.left = max(0, rc.right - ( width / m_SizeInc.cx ) * m_SizeInc.cx );
		}
		if ( rc.bottom > rcLimits.bottom )
		{
			rc.bottom = rcLimits.bottom;
			rc.top = max(0, rc.bottom - ( height / m_SizeInc.cy ) * m_SizeInc.cy );
		}
	}  
	
#ifdef DEBUG
    assert ( rc.left % m_PosInc.cx == 0 );
    assert ( rc.top % m_PosInc.cy == 0 );
    assert ( rc.Width() % m_PosInc.cx == 0 );	//  changed by xupx 2009.10.27
    assert ( rc.Height() % m_PosInc.cy == 0 );
    assert ( rc.right <= rcLimits.right );
    assert ( rc.left <= rcLimits.right );
    assert ( rc.Width() % 4 == 0 );
#endif
	
	// copy result back
	*lpRect = rc;
	
}


void CImageView::AdjustPoint(LPPOINT lpPoint)
{
	lpPoint->x = m_PosInc.cx * ( lpPoint->x / m_PosInc.cx);
	lpPoint->y = m_PosInc.cy * ( lpPoint->y / m_PosInc.cy);
}


LRESULT CImageView::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	//add by xupx 2008.06.27 使得选择任意大小窗口失效
	if( IS_READABLE_CAM(m_pDevice->m_pInfo->DeviceType()) && !IS_HV_CAM(m_pDevice->m_HVTYPE))//
	{
		return 0;
	}
	//end by xupx

	SetFocus();
	if ( m_bScalable )
	{
		assert( ! m_RectTracker.IsTracking() );
		
		CPoint Point = CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		
		CPoint tmp = Point;
		DPtoLP(&tmp);
		
		if (HitTestTrackerBorder())
		{
			m_RectTracker.Track(*this, Point, FALSE);
		}
		else
		{
			if ( m_SensorRect.PtInRect(tmp) )
			{
				m_RectTracker.TrackRubberBand(*this, Point, FALSE);
			}
		}
		
		if ( m_RectTracker.IsTracking() )
		{
			StartTimer();
		}
			
	}
	
	return 0;
}


void CImageView::SetNewAOI()
{
	
	try
	{
		m_pDevice->SetAOI(m_RectTracker.m_rect);
	}
	CATCH_REPORT();
	
}

LRESULT CImageView::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	
	if ( LOWORD(lParam) == HTCLIENT )
	{
		assert ( m_pDevice != NULL );
		
		ShowAOISize();
		
		if ( ! ShowCursorPos() )
		{
			m_MainFrame.m_Sbar.SetPaneText(ID_POS_PANE, "");
		}
		
		if (HitTestTrackerBorder() && m_RectTracker.SetCursor(m_hWnd, LOWORD(lParam)) )
		{
			// Handled.
			bHandled = TRUE;
		}
		
		// Set crosshair cursor and show pixel value
		CHVBitmap* pBitmap = m_pDevice->GetBitmap();
		if ( pBitmap != 0 )
		{
			CPoint pt;
			GetLogicalCursorPos(pt);
			if ( pBitmap->GetRect().PtInRect(pt) )
			{
				if ( ! bHandled ) 
				{
					SetCursor(m_hCursor);
					bHandled = TRUE;
				}
			}
			if ( ! ShowPixelValue(pBitmap) )
			{
				m_MainFrame.m_Sbar.SetPaneText(ID_VALUE_PANE, "");
			}
		} 
	} 
	return 0;
}


void CImageView::CenterCursor()
{	
	CPoint pt = m_SensorRect.CenterPoint();
	pt.x = floor(( pt.x - m_ptOffset.x ) * m_dZoomScale) + s_sizBorder.cx ;
    pt.y = floor(( pt.y - m_ptOffset.y ) * m_dZoomScale) + s_sizBorder.cy;
	ClientToScreen(&pt);
	SetCursorPos(pt.x, pt.y);
}


bool CImageView::ShowCursorPos()
{
	CPoint pt;
	GetLogicalCursorPos(pt);
	
	if ( m_SensorRect.PtInRect(pt)  )
	{
		CString str;
		str.Format(" ( %d, %3d )", pt.x, pt.y);
		m_MainFrame.m_Sbar.SetPaneText(ID_POS_PANE, str);
		return true;
	}
	return false;
}

// Show pixel value under the cursor.
bool CImageView::ShowPixelValue(CHVBitmap* pBitmap)
{
	if ( pBitmap == NULL )
		pBitmap = m_pDevice->GetBitmap();
	
	if ( pBitmap != NULL )
	{
		CPoint pt;
		GetLogicalCursorPos(pt);
		if ( m_RectTracker.GetTrackerHandle() == CRectTracker::hitNothing && pBitmap->GetRect().PtInRect(pt) )
		{
			CPoint origin = pBitmap->GetOrigin();
			CString str;
			RGBTRIPLE rgb = (*pBitmap)(pt.x - origin.x, pt.y - origin.y);
			str.Format("R: %-3u G: %-3u B: %-3u", (long) rgb.rgbtRed, (long) rgb.rgbtGreen, (long) rgb.rgbtBlue );
			m_MainFrame.m_Sbar.SetPaneText(ID_VALUE_PANE, str);
			return true;
		}
	}
	return false;
}

void CImageView::ShowAOISize()
{
	if ( ! m_bActiveChild )
		return;
	
	CSize size;
	//  if ( m_VideoFormat != DCS_Format7 )
	{
//		size = m_SensorSize;
	}
	//  else
	{
		size = m_RectTracker.m_rect.Size();
	}
	
	CString str;
	// if ( m_VideoFormat != DCS_Format7)
	{
		//   str.Format(" Image Size: %d x %d", size.cx, size.cy );
	}
	// else
	{
		str.Format(" AOI: (%d, %d): %d x %d", m_RectTracker.m_rect.left, m_RectTracker.m_rect.top, size.cx, size.cy);
	}
	m_MainFrame.m_Sbar.SetPaneText(ID_AOI_PANE, str);
}


void CImageView::StartTimer()
{
	m_nIdEvent = SetTimer(1, s_dwDragTimerInterval);
	ATLASSERT(m_nIdEvent);
}

void CImageView::StopTimer()
{
	KillTimer(m_nIdEvent);
}

LRESULT CImageView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Ensure tracker visibility.
	
	CRect rcClient;
	GetClientRect(rcClient);
	
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	
	if (!rcClient.PtInRect(ptCursor))
	{
		CSize offset;
		offset.cx = lcm(m_PosInc.cx, m_SizeInc.cx);
		offset.cy = lcm(m_PosInc.cy, m_SizeInc.cy);
		int ScrollCodeX = -1;
		int ScrollCodeY = -1;
		
		if (ptCursor.x > rcClient.right && m_ptOffset.x < m_sizeAll.cx - rcClient.Width())
		{
			ScrollCodeX = SB_LINEDOWN;
		}
		
		if (ptCursor.y > rcClient.bottom && m_ptOffset.y < m_sizeAll.cy - rcClient.Height())
		{
			ScrollCodeY = SB_LINEDOWN;
		}
		
		if (ptCursor.x < rcClient.left&& m_ptOffset.x > 0 )
		{
			ScrollCodeX = SB_LINEUP;
		}
		
		if (ptCursor.y < rcClient.top && m_ptOffset.y > 0)
		{
			ScrollCodeY = SB_LINEUP;
		}
		
		if ( ScrollCodeX != -1 || ScrollCodeY != -1 )
		{
			m_RectTracker.AdjustRect(m_RectTracker.GetTrackerHandle(), m_RectTracker.m_rect);
			if ( ScrollCodeX != -1 )
				DoScroll(SB_HORZ, ScrollCodeX, (int&) m_ptOffset.x, m_sizeAll.cx, m_sizePage.cx, m_sizeLine.cx );
			if ( ScrollCodeY != -1 )
				DoScroll(SB_VERT, ScrollCodeY, (int&) m_ptOffset.y, m_sizeAll.cy, m_sizePage.cy, m_sizeLine.cy );
			// Actualize rubbar band. This is done by sending a WM_MOUSEMOVE message, which will be processed by
			// the message loop of the TrackHandle function
			CRect rect;
			GetClientRect(&rect);
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			pt.x = max(pt.x, rect.left);
			pt.x = min(pt.x, rect.right);
			pt.y = max(pt.y, rect.top);
			pt.y = min(pt.y, rect.bottom);
			PostMessage(WM_MOUSEMOVE, MK_LBUTTON, pt.x + ( pt.y << 16 ) );
			
		} 		
	}


	ShowAOISize();
	ShowCursorPos();
	
	bHandled = FALSE;
	
	return 0;
	
}


bool CImageView::SaveLayout()
{
	
	if ( m_pDevice == NULL )
	{
		assert(false);
		return false;
	}
	
	bool closeDevice = false;
	
	bool ret = false;
	
	try
	{
		
		if ( ! m_pDevice->IsOpen() )
		{
			m_pDevice->Activate();
			closeDevice = true;
		}
		
		CString keyName = APPLICATION_KEY + m_pDevice->m_pInfo->DeviceName();
		CPropertyBagPtr ptrRootBag;
		try
		{
			ptrRootBag = CRegistryPropertyBag::Open(keyName);
		}
		catch ( HVBaseException& e)
		{
			if ( e.Error() == STATUS_INTERNAL_ERROR )
				ptrRootBag = CRegistryPropertyBag::Create(keyName);
			else
				throw e;
		}
		
		CPropertyBagPtr ptrWindowBag;
		try
		{
			ptrWindowBag = ptrRootBag->GetBag("ImageView");
		}
		catch ( HVBaseException& e)
		{
			if ( e.Error() == STATUS_INTERNAL_ERROR )
				ptrWindowBag = ptrRootBag->CreateBag("ImageView");
			else
				throw e;
		}
		
		
		WINDOWPLACEMENT p;
		p.length = sizeof(p);
		ret =  ( ::GetWindowPlacement(GetParent(), &p) == TRUE );
		CPoint center = GetLogicalCenter();
		if ( ret )
		{
			ptrWindowBag->WriteLong("left", p.rcNormalPosition.left);
			ptrWindowBag->WriteLong("bottom", p.rcNormalPosition.bottom);
			ptrWindowBag->WriteLong("top", p.rcNormalPosition.top);
			ptrWindowBag->WriteLong("right", p.rcNormalPosition.right);
			ptrWindowBag->WriteLong("showCmd", p.showCmd);
			ptrWindowBag->WriteFloat("zoom", GetZoomLevel());
			ptrWindowBag->WriteBool("fit", IsZoomedToFit());
			ptrWindowBag->WriteLong("scrollx", center.x);
			ptrWindowBag->WriteLong("scrolly", center.y);
		}
	}
	catch(HVBaseException& e)
	{
		ATLTRACE("Exception occured when saving window layout: %d (%s)\n", e.Error(), e.Description());
		ret = false;
	}
	
	if ( closeDevice )
	{
		try
		{
			m_pDevice->Deactivate();
		}
		catch ( HVBaseException &e )
		{
			ATLTRACE("Exception occured when closing the device: %d (%s)\n", e.Error(), e.Description());
		}
	}
	
	return ret;
	
}



bool CImageView::RestoreLayout()
{
	bool ret = false;
	bool closeDevice = false;
	
	if ( m_pDevice == NULL )
	{
		assert(false);
		return false;
	}
	
	try
	{
		
		
		if ( ! m_pDevice->IsOpen() )
		{
			m_pDevice->Activate();
			closeDevice = true;
		}
		
		CString keyName = APPLICATION_KEY + m_pDevice->m_pInfo->DeviceName();
		CPropertyBagPtr ptrBag = CRegistryPropertyBag::Open(keyName)->GetBag("ImageView");
		
		WINDOWPLACEMENT p;
		ZeroMemory(&p, sizeof(p));
		p.length = sizeof(p);
		p.rcNormalPosition.left = ptrBag->ReadLong("left");
		p.rcNormalPosition.right = ptrBag->ReadLong("right");
		p.rcNormalPosition.bottom = ptrBag->ReadLong("bottom");
		p.rcNormalPosition.top = ptrBag->ReadLong("top");
		p.showCmd = ptrBag->ReadLong("showCmd");
		double  zoom = ptrBag->ReadFloat("zoom");
		
		bool zoomToFit =  ptrBag->ReadBool("fit");	
		::SetWindowPlacement(GetParent(), &p);
		
		if ( ! zoomToFit )
		{
			SetZoomLevel(zoom);
			long spx = 0;
			long spy = 0;
			try
			{
				spx = ptrBag->ReadLong("scrollx");
				spy = ptrBag->ReadLong("scrolly");
			}
			catch ( HVBaseException& )
			{
				/* NOP */
			}
			UpdateViewPort(&CPoint(spx, spy));
			
		}
		if ( zoomToFit )
			ZoomToFit();
		
		
	}
	catch(HVBaseException& e)
	{
		ATLTRACE("Exception occured when restoring window layout: %d (%s)", e.Error(), e.Description());
		ret = false;
	}
	
	if ( closeDevice )
	{
		try
		{
			m_pDevice->Deactivate();
		}
		catch ( HVBaseException &e )
		{
			ATLTRACE("Exception occured when closing the device: %d (%s)", e.Error(), e.Description());
		}
	}
	
	return ret;
}


