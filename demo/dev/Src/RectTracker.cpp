//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: RectTracker.cpp, 2, 10/2/02 2:31:28 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     RectTracker.cpp
  \brief    implementation of the CRectTracker class.
*/
//////////////////////////////////////////////////////////////////////

// adopted from MFC CRectTrack
#include "stdafx.h"
#include "resource.h"
#include "RectTracker.h"

#ifndef _countof
#define _countof(s) (sizeof(s)/sizeof(*(s)))
#endif
#define CX_BORDER   1
#define CY_BORDER   1
static HCURSOR Cursors[10] = { 0, };
static HBRUSH HatchBrush = 0;
static HPEN BlackDottedPen = 0;
static int HandleSize = 0;


struct HANDLEINFO
{
  size_t nOffsetX;    // offset within RECT for X coordinate
  size_t nOffsetY;    // offset within RECT for Y coordinate
  int nCenterX;       // adjust X by Width()/2 * this number
  int nCenterY;       // adjust Y by Height()/2 * this number
  int nHandleX;       // adjust X by handle size * this number
  int nHandleY;       // adjust Y by handle size * this number
  int nInvertX;       // handle converts to this when X inverted
  int nInvertY;       // handle converts to this when Y inverted
};

// this array describes all 8 handles (clock-wise)
static const HANDLEINFO HandleInfo[] =
{
  // corner handles (top-left, top-right, bottom-right, bottom-left
  { offsetof(RECT, left), offsetof(RECT, top),        0, 0,  0,  0, 1, 3 },
  { offsetof(RECT, right), offsetof(RECT, top),       0, 0, -1,  0, 0, 2 },
  { offsetof(RECT, right), offsetof(RECT, bottom),    0, 0, -1, -1, 3, 1 },
  { offsetof(RECT, left), offsetof(RECT, bottom),     0, 0,  0, -1, 2, 0 },

  // side handles (top, right, bottom, left)
  { offsetof(RECT, left), offsetof(RECT, top),        1, 0,  0,  0, 4, 6 },
  { offsetof(RECT, right), offsetof(RECT, top),       0, 1, -1,  0, 7, 5 },
  { offsetof(RECT, left), offsetof(RECT, bottom),     1, 0,  0, -1, 6, 4 },
  { offsetof(RECT, left), offsetof(RECT, top),        0, 1,  0,  0, 5, 7 }
};

// the struct below gives us information on the layout of a RECT struct and
//  the relationship between its members
struct RECTINFO
{
  size_t nOffsetAcross;   // offset of opposite point (ie. left->right)
  int nSignAcross;        // sign relative to that point (ie. add/subtract)
};

// this array is indexed by the offset of the RECT member / sizeof(int)
static const RECTINFO RectInfo[] =
{
  { offsetof(RECT, right), +1 },
  { offsetof(RECT, bottom), +1 },
  { offsetof(RECT, left), -1 },
  { offsetof(RECT, top), -1 },
};

/////////////////////////////////////////////////////////////////////////////
// CRectTracker intitialization

int CRectTracker::m_cInstances = 0;

CRectTracker::CRectTracker(LPCRECT lpSrcRect, UINT nStyle) : m_bIsTracking(FALSE), m_nHandle(hitNothing)
{
  Construct();
  m_rect.CopyRect(lpSrcRect);
  m_nStyle = nStyle;
}

void CRectTracker::Construct()
{
  // do one-time initialization if necessary
  if ( m_cInstances == 0 )
  {
    // sanity checks for assumptions we make in the code
    ATLASSERT(sizeof(((RECT*)NULL)->left) == sizeof(int));
    ATLASSERT(offsetof(RECT, top) > offsetof(RECT, left));
    ATLASSERT(offsetof(RECT, right) > offsetof(RECT, top));
    ATLASSERT(offsetof(RECT, bottom) > offsetof(RECT, right));

    if ( HatchBrush == NULL)
    {
      // create the hatch pattern + bitmap
      WORD hatchPattern[8];
      WORD wPattern = 0x1111;
      for (int i = 0; i < 4; i++)
      {
        hatchPattern[i] = wPattern;
        hatchPattern[i+4] = wPattern;
        wPattern <<= 1;
      }
      HBITMAP hatchBitmap = CreateBitmap(8, 8, 1, 1, &hatchPattern);
      if (hatchBitmap == NULL)
      {
        throw ::GetLastError();
      }

      // create black hatched brush
      HatchBrush = CreatePatternBrush(hatchBitmap);
      DeleteObject(hatchBitmap);
      if (HatchBrush == NULL)
      {
        throw ::GetLastError();
      }
    }

    if (BlackDottedPen == NULL)
    {
      // create black dotted pen
      BlackDottedPen = CreatePen(PS_DOT, 0, RGB(0, 0, 0));
      if (BlackDottedPen == NULL)
      {
        throw ::GetLastError();
      }
    }

    // Note: all track cursors must live in same module
    HINSTANCE hInst = _Module.GetResourceInstance();

    // initialize the cursor array
    Cursors[0] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNWSE));
    Cursors[1] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNESW));
    Cursors[2] = Cursors[0];
    Cursors[3] = Cursors[1];
    Cursors[4] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKNS));
    Cursors[5] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACKWE));
    Cursors[6] = Cursors[4];
    Cursors[7] = Cursors[5];
    Cursors[8] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_TRACK4WAY));
    Cursors[9] = ::LoadCursor(hInst, MAKEINTRESOURCE(IDC_MOVE4WAY));

    // get default handle size from Windows profile setting
    static const TCHAR szWindows[] = _T("windows");
    static const TCHAR szInplaceBorderWidth[] =
      _T("oleinplaceborderwidth");
    HandleSize = GetProfileInt(szWindows, szInplaceBorderWidth, 4);
  }

  m_nStyle = 0;
  m_nHandleSize = HandleSize;
  m_sizeMin.cy = m_sizeMin.cx = m_nHandleSize*2;

  m_rectLast.SetRectEmpty();
  m_sizeLast.cx = m_sizeLast.cy = 0;
  m_cInstances ++;
}

CRectTracker::~CRectTracker()
{
  m_cInstances --;
  if ( m_cInstances == 0 )
  {
    // last instance has been erased. Release resources
    DeleteObject(HatchBrush);
    HatchBrush = NULL;
    DeleteObject(BlackDottedPen); 
    BlackDottedPen = NULL;
  }

}

/////////////////////////////////////////////////////////////////////////////
// CRectTracker operations

void CRectTracker::Draw(HDC hDC) const
{
  CDCHandle DC(hDC);
  // set initial DC state
  DC.SaveDC();
  DC.SetMapMode(MM_TEXT);
  DC.SetViewportOrg(0, 0);
  DC.SetWindowOrg(0, 0);  

  // get normalized rectangle
  CRect rect = m_rect;
  rect.NormalizeRect();
  LPtoDP((LPPOINT) &rect, 2);

  HPEN OldPen = NULL;
  HBRUSH OldBrush = NULL;
  int nOldROP;

  // draw lines
  if ((m_nStyle & (dottedLine|solidLine)) != 0)
  {
    if (m_nStyle & dottedLine)
      OldPen = DC.SelectPen(BlackDottedPen);
    else
      OldPen = DC.SelectStockPen(BLACK_PEN);
    OldBrush = DC.SelectStockBrush(NULL_BRUSH);
    nOldROP = DC.SetROP2(R2_COPYPEN);
    rect.InflateRect(+1, +1);   // borders are one pixel outside
    DC.Rectangle(rect.left, rect.top, rect.right, rect.bottom);
    DC.SetROP2(nOldROP);
  }

  // if hatchBrush is going to be used, need to unrealize it
  if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
    UnrealizeObject(HatchBrush);

  // hatch inside
  if ((m_nStyle & hatchInside) != 0)
  {
    HPEN TempPen = DC.SelectStockPen(NULL_PEN);
    if (OldPen == NULL)
      OldPen = TempPen;
    HBRUSH TempBrush = DC.SelectBrush(HatchBrush);
    if (OldBrush == NULL)
      OldBrush = TempBrush;
    DC.SetBkMode(TRANSPARENT);
    nOldROP = DC.SetROP2(R2_MASKNOTPEN);
    DC.Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
    DC.SetROP2(nOldROP);
  }

  // draw hatched border
  if ((m_nStyle & hatchedBorder) != 0)
  {
    HBRUSH TempBrush = DC.SelectBrush(HatchBrush);
    if (OldBrush == NULL)
      OldBrush = TempBrush;
    DC.SetBkMode(OPAQUE);
    CRect rectTrue;
    GetTrueRect(&rectTrue);
    DC.PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
      rect.top-rectTrue.top, 0x000F0001 /* Pn */);
    DC.PatBlt(rectTrue.left, rect.bottom,
      rectTrue.Width(), rectTrue.bottom-rect.bottom, 0x000F0001 /* Pn */);
    DC.PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
      rect.Height(), 0x000F0001 /* Pn */);
    DC.PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
      rect.Height(), 0x000F0001 /* Pn */);
  }

  // draw resize handles
  if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
  {
    UINT mask = GetHandleMask();
    for (int i = 0; i < 8; ++i)
    {
      if (mask & (1<<i))
      {
        GetHandleRect((TrackerHit)i, &rect);
        DC.FillSolidRect(rect, RGB(0, 0, 0));
      }
    }
  }

  // cleanup pDC state
  if (OldPen != NULL)
    DC.SelectPen(OldPen);
  if (OldBrush != NULL)
    DC.SelectBrush(OldBrush);
  DC.RestoreDC(-1);
}

BOOL CRectTracker::SetCursor(HWND hWnd, UINT nHitTest) const
{
  // trackers should only be in client area
  if (nHitTest != HTCLIENT)
    return FALSE;

  // convert cursor position to client co-ordinates
  CPoint point;
  GetCursorPos(&point);
  ScreenToClient(hWnd, &point);

  // do hittest and normalize hit
  int nHandle = HitTestHandles(point);
  if (nHandle < 0)
    return FALSE;

  // need to normalize the hittest such that we get proper cursors
  nHandle = NormalizeHit(nHandle);

  // handle special case of hitting area between handles
  //  (logically the same -- handled as a move -- but different cursor)
  if (nHandle == hitMiddle && !m_rect.PtInRect(point))
  {
    // only for trackers with hatchedBorder (ie. in-place resizing)
    if (m_nStyle & hatchedBorder)
      nHandle = (TrackerHit)9;
  }

  ATLASSERT(nHandle < _countof(Cursors));
  ::SetCursor(Cursors[nHandle]);
  return TRUE;
}

int CRectTracker::HitTest(CPoint point) const
{
  TrackerHit hitResult = hitNothing;

  CRect rectTrue;
  GetTrueRect(&rectTrue);
  ATLASSERT(rectTrue.left <= rectTrue.right);
  ATLASSERT(rectTrue.top <= rectTrue.bottom);
  if (rectTrue.PtInRect(point))
  {
    if ((m_nStyle & (resizeInside|resizeOutside)) != 0)
      hitResult = (TrackerHit)HitTestHandles(point);
    else
      hitResult = hitMiddle;
  }
  return hitResult;
}

int CRectTracker::NormalizeHit(int nHandle) const
{
  ATLASSERT(nHandle <= 8 && nHandle >= -1);
  if (nHandle == hitMiddle || nHandle == hitNothing)
    return nHandle;
  const HANDLEINFO* pHandleInfo = &HandleInfo[nHandle];
  if (m_rect.Width() < 0)
  {
    nHandle = (TrackerHit)pHandleInfo->nInvertX;
    pHandleInfo = &HandleInfo[nHandle];
  }
  if (m_rect.Height() < 0)
    nHandle = (TrackerHit)pHandleInfo->nInvertY;
  return nHandle;
}

BOOL CRectTracker::Track(HWND hWnd, CPoint point, BOOL bAllowInvert)
{
  // perform hit testing on the handles
  int nHandle = HitTestHandles(point);
  if (nHandle < 0)
  {
    // didn't hit a handle, so just return FALSE
    return FALSE;
  }

  // otherwise, call helper function to do the tracking
  m_bAllowInvert = bAllowInvert;
  m_rectSave = m_rect;
  return TrackHandle(nHandle, hWnd, point);
}

BOOL CRectTracker::TrackRubberBand(HWND hWnd, CPoint point, BOOL bAllowInvert)
{
  // simply call helper function to track from bottom right handle
  m_rectSave = m_rect;
  m_bAllowInvert = bAllowInvert;
  CPoint lpoint = point;
  DPtoLP(&lpoint);
  AdjustPoint(&lpoint);
  m_rect.SetRect(lpoint.x, lpoint.y, lpoint.x, lpoint.y);

  return TrackHandle(hitBottomRight, hWnd, point);
}


void CRectTracker::AdjustRect(int nHandle, LPRECT)
{
  if (nHandle == hitMiddle)
    return;


  // convert the handle into locations within m_rect
  int *px, *py;
  GetModifyPointers(nHandle, &px, &py, NULL, NULL);

  // enforce minimum width
  int nNewWidth = m_rect.Width();
  int nAbsWidth = m_bAllowInvert ? abs(nNewWidth) : nNewWidth;
  if (px != NULL && nAbsWidth < m_sizeMin.cx)
  {
    nNewWidth = nAbsWidth != 0 ? nNewWidth / nAbsWidth : 1;
    ATLASSERT((int*)px - (int*)&m_rect < _countof(RectInfo));
    const RECTINFO* pRectInfo = &RectInfo[(int*)px - (int*)&m_rect];
    *px = *(int*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
      nNewWidth * m_sizeMin.cx * -pRectInfo->nSignAcross;
  }

  // enforce minimum height
  int nNewHeight = m_rect.Height();
  int nAbsHeight = m_bAllowInvert ? abs(nNewHeight) : nNewHeight;
  if (py != NULL && nAbsHeight < m_sizeMin.cy)
  {
    nNewHeight = nAbsHeight != 0 ? nNewHeight / nAbsHeight : 1;
    ATLASSERT((int*)py - (int*)&m_rect < _countof(RectInfo));
    const RECTINFO* pRectInfo = &RectInfo[(int*)py - (int*)&m_rect];
    *py = *(int*)((BYTE*)&m_rect + pRectInfo->nOffsetAcross) +
      nNewHeight * m_sizeMin.cy * -pRectInfo->nSignAcross;
  }
}

void CRectTracker::GetTrueRect(LPRECT lpTrueRect) const
{

  CRect rect = m_rect;
  rect.NormalizeRect();
  LPtoDP((LPPOINT) &rect, 2);
  int nInflateBy = 0;
  if ((m_nStyle & (resizeOutside|hatchedBorder)) != 0)
    nInflateBy += GetHandleSize() - 1;
  if ((m_nStyle & (solidLine|dottedLine)) != 0)
    ++nInflateBy;
  rect.InflateRect(nInflateBy, nInflateBy);
  *lpTrueRect = rect;
}

void CRectTracker::OnChangedRect(const CRect& /*rectOld*/)
{
  // no default implementation, useful for derived classes
}

void CRectTracker::OnTrackingFinished(const BOOL bCancelled )
{
  // no default implementation, useful for derived classes
}

/////////////////////////////////////////////////////////////////////////////
// CRectTracker implementation helpers

void CRectTracker::GetHandleRect(int nHandle, CRect* pHandleRect) const
{
  ATLASSERT(nHandle < 8);

  // get normalized rectangle of the tracker
  CRect rectT = m_rect;
  rectT.NormalizeRect();
  LPtoDP((LPPOINT) &rectT, 2);
  if ((m_nStyle & (solidLine|dottedLine)) != 0)
    rectT.InflateRect(+1, +1);

  // since the rectangle itself was normalized, we also have to invert the
  //  resize handles.
  nHandle = NormalizeHit(nHandle);

  // handle case of resize handles outside the tracker
  int size = GetHandleSize();
  if (m_nStyle & resizeOutside)
    rectT.InflateRect(size-1, size-1);

  // calculate position of the resize handle
  int nWidth = rectT.Width();
  int nHeight = rectT.Height();
  CRect rect;
  const HANDLEINFO* pHandleInfo = &HandleInfo[nHandle];
  rect.left = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetX);
  rect.top = *(int*)((BYTE*)&rectT + pHandleInfo->nOffsetY);
  rect.left += size * pHandleInfo->nHandleX;
  rect.top += size * pHandleInfo->nHandleY;
  rect.left += pHandleInfo->nCenterX * (nWidth - size) / 2;
  rect.top += pHandleInfo->nCenterY * (nHeight - size) / 2;
  rect.right = rect.left + size;
  rect.bottom = rect.top + size;

  *pHandleRect = rect;
}

int CRectTracker::GetHandleSize(LPCRECT lpRect) const
{
  if (lpRect == NULL)
    lpRect = &m_rect;

  int size = m_nHandleSize;
  if (!(m_nStyle & resizeOutside))
  {
    // make sure size is small enough for the size of the rect
    int sizeMax = min(abs(lpRect->right - lpRect->left),
      abs(lpRect->bottom - lpRect->top));
    if (size * 2 > sizeMax)
      size = sizeMax / 2;
  }
  return size;
}

int CRectTracker::HitTestHandles(CPoint point) const
{
  CRect rect;
  UINT mask = GetHandleMask();

  // see if hit anywhere inside the tracker
  GetTrueRect(&rect);
  if (!rect.PtInRect(point))
    return hitNothing;  // totally missed

  // see if we hit a handle
  for (int i = 0; i < 8; ++i)
  {
    if (mask & (1<<i))
    {
      GetHandleRect((TrackerHit)i, &rect);
      if (rect.PtInRect(point))
        return (TrackerHit)i;
    }
  }

  // last of all, check for non-hit outside of object, between resize handles
  if ((m_nStyle & hatchedBorder) == 0)
  {
    CRect rect = m_rect;
    rect.NormalizeRect();
    if ((m_nStyle & dottedLine|solidLine) != 0)
      rect.InflateRect(+1, +1);
    if (!rect.PtInRect(point))
      return hitNothing;  // must have been between resize handles
  }
  return hitMiddle;   // no handle hit, but hit object (or object border)
}

BOOL CRectTracker::TrackHandle(int nHandle, HWND hWnd, CPoint point)
{

  if ( m_bIsTracking )
    return FALSE;  // we are already tracking;
  // don't handle if capture already set
  if (::GetCapture() != NULL)
    return FALSE;

  m_nHandle = nHandle;
  CWindow Wnd(hWnd);

  ATLASSERT(nHandle >= 0);
  ATLASSERT(nHandle <= 8);   // handle 8 is inside the rect

  DPtoLP(&point);


  // set capture to the window which received this message
  Wnd.SetCapture();
  ATLASSERT(Wnd.m_hWnd == ::GetCapture());
  Wnd.UpdateWindow();

  // find out what x/y coords we are supposed to modify
  CPoint pt;
  CPoint Diff;
  GetModifyPointers(nHandle, &m_px, &m_py, (int*) &m_Diff.x, (int*) &m_Diff.y);
  m_Diff.x = point.x - m_Diff.x;
  m_Diff.y = point.y - m_Diff.y;

  m_bIsTracking = TRUE;
  m_bChanged = FALSE;


  AdjustRect(m_nHandle, &m_rect);


  return TRUE;


}

// a client  has to call this message handler 

BOOL CRectTracker::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

  CPoint pt;
  CRect rectOld;
  BOOL bHandled = FALSE;
  int nWidth, nHeight;

  if ( ! m_bIsTracking )
    return bHandled;
 
  switch (uMsg)
  {
    // handle movement/accept messages
  case WM_MOUSEMOVE:
  case WM_LBUTTONUP:
    bHandled = TRUE;
    rectOld = m_rect;
    // save original width & height in pixels
    nWidth = m_rect.Width();
    nHeight = m_rect.Height();
    
    // handle resize cases (and part of move)
    pt.x = (int)(short)LOWORD(lParam);
    pt.y = (int)(short)HIWORD(lParam);
    DPtoLP(&pt);
    if (m_px != NULL)
      *m_px = pt.x - m_Diff.x;
    if (m_py != NULL)
      *m_py = pt.y - m_Diff.y;
    
    // handle move case
    if (m_nHandle == hitMiddle)
    {
      m_rect.right = m_rect.left + nWidth;
      m_rect.bottom = m_rect.top + nHeight;
    }
    // allow caller to adjust the rectangle if necessary
    AdjustRect(m_nHandle, &m_rect);
    
    // only callback if the rect actually changed
    if (!rectOld.EqualRect(&m_rect)  )
    {
      OnChangedRect(rectOld);
	  
      m_bChanged = TRUE;
	  
    }
	if ( uMsg == WM_LBUTTONUP )
	{
		// finish tracking
		if ( m_bChanged ) 
		{
			m_bIsTracking = false;
			ReleaseCapture();
			m_nHandle = hitNothing;
			OnTrackingFinished( FALSE ); 
		}
		else
		{
			CancelTracking();
		}
	}
    break;
    // handle cancel messages
  case WM_KEYDOWN:
    if (wParam != VK_ESCAPE)
    {
      break;
    }
  case WM_CAPTURECHANGED:
    // we lost the mouse capture -> cancel
  case WM_RBUTTONDOWN:
    CancelTracking();
    bHandled = TRUE;
    break;
  }

  return bHandled;
}

void CRectTracker::CancelTracking()
{
  if ( m_bIsTracking )
  {
    m_rect = m_rectSave;
    m_bIsTracking = false;
    m_nHandle = hitNothing;
    OnTrackingFinished(TRUE);
    ReleaseCapture();
  }
}


void CRectTracker::GetModifyPointers(
  int nHandle, int** ppx, int** ppy, int* px, int* py)
{
  ATLASSERT(nHandle >= 0);
  ATLASSERT(nHandle <= 8);

  if (nHandle == hitMiddle)
    nHandle = hitTopLeft;   // same as hitting top-left

  *ppx = NULL;
  *ppy = NULL;

  // fill in the part of the rect that this handle modifies
  //  (Note: handles that map to themselves along a given axis when that
  //   axis is inverted don't modify the value on that axis)

  const HANDLEINFO* pHandleInfo = &HandleInfo[nHandle];
  if (pHandleInfo->nInvertX != nHandle)
  {
    *ppx = (int*)((BYTE*)&m_rect + pHandleInfo->nOffsetX);
    if (px != NULL)
      *px = **ppx;
  }
  else
  {
    // middle handle on X axis
    if (px != NULL)
      *px = m_rect.left + abs(m_rect.Width()) / 2;
  }
  if (pHandleInfo->nInvertY != nHandle)
  {
    *ppy = (int*)((BYTE*)&m_rect + pHandleInfo->nOffsetY);
    if (py != NULL)
      *py = **ppy;
  }
  else
  {
    // middle handle on Y axis
    if (py != NULL)
      *py = m_rect.top + abs(m_rect.Height()) / 2;
  }
}

UINT CRectTracker::GetHandleMask() const
{
  UINT mask = 0x0F;   // always have 4 corner handles
  int size = m_nHandleSize*3;
  if (abs(m_rect.Width()) - size > 4)
    mask |= 0x50;
  if (abs(m_rect.Height()) - size > 4)
    mask |= 0xA0;
  return mask;
}

/////////////////////////////////////////////////////////////////////////////
