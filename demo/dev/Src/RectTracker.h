//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  Subproject:  BCAM Viewer
//  $Revision: 2$
//  $Header: RectTracker.h, 2, 9/25/02 11:15:53 AM, Happe, A.$
//-----------------------------------------------------------------------------
/**
  \file     RectTracker.h
 *
  \brief   interface for the CRectTracker class.
 */
//-----------------------------------------------------------------------------


#if !defined(AFX_RECTTRACKER_H__A0E84F57_9DE8_11D5_9236_0090278E5E96__INCLUDED_)
#define AFX_RECTTRACKER_H__A0E84F57_9DE8_11D5_9236_0090278E5E96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//------------------------------------------------------------------------------
// class CRectTracker
// Author: 
// Date: 20.09.2002
//------------------------------------------------------------------------------
/**
 * \brief   simple rectangular tracking rectangle with resize handles
 *
 */
//------------------------------------------------------------------------------
class CRectTracker
{
public:
// Constructors
  CRectTracker(LPCRECT lpSrcRect, UINT nStyle);

// Style Flags
  enum StyleFlags
  {
    solidLine = 1, dottedLine = 2, hatchedBorder = 4,
    resizeInside = 8, resizeOutside = 16, hatchInside = 32,
  };

// Hit-Test codes
  enum TrackerHit
  {
    hitNothing = -1,
    hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
    hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
  };

// Attributes
  UINT m_nStyle;      // current state
  CRect m_rect;       // current position (always in pixels)
  CSize m_sizeMin;    // minimum X and Y size during track operation
  int m_nHandleSize;  // size of resize handles (default from WIN.INI)

// Operations
  void Draw(HDC hDC) const;
  void GetTrueRect(LPRECT lpTrueRect) const;
  int GetTrackerHandle() const { return m_nHandle; }
  BOOL IsTracking() const { return m_bIsTracking; }
  BOOL SetCursor(HWND hWnd, UINT nHitTest) const;
  BOOL Track(HWND hWnd, CPoint point, BOOL bAllowInvert = FALSE);
  BOOL TrackRubberBand(HWND hWnd, CPoint point, BOOL bAllowInvert = TRUE);
  void CancelTracking();
  int HitTest(CPoint point) const;
  int NormalizeHit(int nHandle) const;

// Overridables
  virtual void AdjustRect(int nHandle, LPRECT lpRect);
  virtual void AdjustPoint(LPPOINT lpPoint) {};
  virtual void OnChangedRect(const CRect& rectOld );
  virtual void OnTrackingFinished( const BOOL bCancelled );
  virtual UINT GetHandleMask() const;
  virtual void LPtoDP(LPPOINT lpPoints, int nCount = 1) const {};
  virtual void DPtoLP(LPPOINT lpPoints, int nCount = 1) const {};
  virtual BOOL MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam);

// Implementation
public:
  virtual ~CRectTracker();

protected:
  BOOL m_bAllowInvert;    // flag passed to Track or TrackRubberBand
  BOOL m_bIsTracking;     // TRUE if we are currently tracking 
  int m_nHandle;          // actual handle
  CRect m_rectLast;
  CSize m_sizeLast;
  CRect m_rectSave;
  static int m_cInstances;  // number of instances created of CRectTracker
  int * m_px, *m_py;
  CPoint m_Diff;
  BOOL m_bChanged;

  // implementation helpers
  int HitTestHandles(CPoint point) const;
  void GetHandleRect(int nHandle, CRect* pHandleRect) const;
  void GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py);
  virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
  BOOL TrackHandle(int nHandle, HWND hWnd, CPoint point);
  void Construct();
};

#endif // !defined(AFX_RECTTRACKER_H__A0E84F57_9DE8_11D5_9236_0090278E5E96__INCLUDED_)
