//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: ImageView.h, 5, 10/31/02 3:02:26 PM, Albrecht M.$
//-----------------------------------------------------------------------------
/**
  \file     ImageView.h
 *
 * \brief Interface of the CImageView and CImageRectTracker classes
 *
 */
//-----------------------------------------------------------------------------


#if !defined(AFX_BCAMVIEWERVIEW_H__8C4B8AD4_7C21_11D5_920C_0090278E5E96__INCLUDED_)
#define AFX_BCAMVIEWERVIEW_H__8C4B8AD4_7C21_11D5_920C_0090278E5E96__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <math.h>
#include "controls\atlzoom.h"
#include "MainFrm.h"
#include "recttracker.h"

extern CAppModule _Module;

#define GUARD_HANDLER() \
    if ( m_RectTracker.MessageHandler(uMsg, wParam, lParam) ) \
      return TRUE;

class CImageView;

//------------------------------------------------------------------------------
// class CImageRectTracker
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief   Rubberband to adjust camera's AOI
 *
 * 
 * \todo      
 */
//------------------------------------------------------------------------------


class CImageRectTracker : public CRectTracker
{
private:
  CImageView& m_View;
  
public:
  CImageRectTracker(CImageView& view);
  virtual void AdjustRect(int nHandle, LPRECT lpRect);
  virtual void AdjustPoint(LPPOINT lpPoint);
  virtual void OnChangedRect(const CRect& rcOld );
  virtual void OnTrackingFinished( const BOOL bCancelled );

  virtual void LPtoDP(LPPOINT lpPoints, int nCount = 1) const;
  virtual void DPtoLP(LPPOINT lpPoints, int nCount = 1) const;
  
};

//------------------------------------------------------------------------------
// class CImageView
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief  CImageView: Window to display a bitmap.  
 * 
 *  Objects of type CImageView represent the child windows of the application's 
 *  MDI windows. The rubber band to adjust the camera's AOI is handled by the CImageView class. 
 *
 */
//------------------------------------------------------------------------------


class CImageView : public CZoomWindowImpl<CImageView>
{
public:
  DECLARE_WND_CLASS_EX(NULL, 0, -1);

  static const CSize s_sizBorder;

  CImageView::CImageView(CDevice* pDevice, CMainFrame& MainFrame) :
    m_pDevice(pDevice),
    m_MainFrame(MainFrame),
    m_SensorSize(0,0),
	m_SensorRect(0,0,0,0),
    m_VideoMode((HV_VIDEO_MODE) -1),
    m_SizeInc(1,1),
    m_PosInc(1,1),
    m_RectTracker(*this),
    m_bScalable(true),
    m_bFit(false)
  { 
    m_RectTracker.m_rect = m_RectTracker.m_rect = CRect(0, 0, 1, 1);	
    m_hCursor = ::LoadCursor(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDC_CROSSHAIR));
  }

  ~CImageView()
  {
    if (m_MainFrame.m_Sbar.IsWindow()) 
	{
		m_MainFrame.m_Sbar.SetPaneText(ID_AOI_PANE, CString(""));
		m_MainFrame.m_Sbar.SetPaneText(ID_POS_PANE, CString(""));
		m_MainFrame.m_Sbar.SetPaneText(ID_VALUE_PANE, CString(""));
		
	}
  }


  BOOL PreTranslateMessage(MSG* pMsg)
  {
    pMsg;
    return FALSE;
  }

  void SetSensorSize(CSize SensorSize);

  void ConfigurationChanged(HV_VIDEO_MODE, CSize SensorSize, CSize ImageSize, CPoint Origin);
  
  void BitBlt(CDCHandle dc, CHVBitmap* pBitmap);

  void DoPaint(CDCHandle dc);

  void ZoomToFit();

  bool IsZoomedToFit() { return m_bFit; }

  bool SaveLayout();

  bool RestoreLayout();

  void CenterCursor();

// message handler 

  BEGIN_MSG_MAP(CImageView)
    //if ( m_RectTracker.MessageHandler(uMsg, wParam, lParam) )
//      return TRUE;
    GUARD_HANDLER()
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
    MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    //MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)  //added by hujun
    MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
    MESSAGE_HANDLER(WM_SIZE, OnSize)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)

    CHAIN_MSG_MAP(CZoomWindowImpl<CImageView>)
  END_MSG_MAP()


  LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    ::SetFocus(GetParent());

    CMenu menuContext;
    menuContext.LoadMenu(IDR_IMAGEVIEW_CONTEXT_MENU);
    CMenuHandle menuPopup(menuContext.GetSubMenu(0));
    m_MainFrame.m_CmdBar.TrackPopupMenu(menuPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, LOWORD(lParam), HIWORD(lParam));

    return 0;
  }

  LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
  //LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

  void OnActivate(bool activated )
  {
    m_bActiveChild = activated;
    
    if ( activated  && m_pDevice != NULL )
    {
      ZoomLevelChanged();  // causes update of UI 
      ShowAOISize();
      ShowPixelValue(m_pDevice->GetBitmap());
    }
  }

  LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
  {
    bHandled = FALSE;

    m_bFit = false;
    return 0;
  }


  // overwritten, because we want to redraw the whole client area after doing the scrolling. Otherwise the rubber band
  // will not be drawn correctly
  void DoScroll(int nType, int nScrollCode, int& cxyOffset, int cxySizeAll, int cxySizePage, int cxySizeLine)
  {
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask = SIF_ALL;

    GetScrollInfo(nType, &si);
    int iPos = si.nPos;

    switch(nScrollCode)
    {
    case SB_TOP:    // top or all left
      si.nPos = 0;
      break;
    case SB_BOTTOM:   // bottom or all right
      si.nPos = si.nMax;
      break;
    case SB_LINEUP:   // line up or line left
      si.nPos -= cxySizeLine;
      break;
    case SB_LINEDOWN: // line down or line right
      si.nPos += cxySizeLine;
      break;
    case SB_PAGEUP:   // page up or page left
      si.nPos -= cxySizePage;
      break;
    case SB_PAGEDOWN: // page down or page right
      si.nPos += cxySizePage;
      break;
    case SB_THUMBTRACK:
      if(IsNoThumbTracking())
        break;
      // else fall through
    case SB_THUMBPOSITION:
      si.nPos = si.nTrackPos;
      break;
    case SB_ENDSCROLL:
    default:
      break;
    }
    si.fMask = SIF_POS;
    SetScrollInfo(nType, &si, TRUE);
    GetScrollInfo(nType, &si);

    if ( si.nPos != iPos )
    {
      if(nType == SB_VERT)
        ScrollWindowEx(0, iPos - si.nPos, m_uScrollFlags);
      else
        ScrollWindowEx(iPos - si.nPos, 0, m_uScrollFlags);
      cxyOffset = si.nPos;
      Invalidate();  // redraw the hole client are
      UpdateWindow();
    }
  }
    
  // show the value under the cursor
  bool ShowPixelValue(CHVBitmap* pBitmap = NULL);
  // show the position of the mosue cursor
  bool ShowCursorPos();

  BOOL ValidateRect(LPCRECT lpRect)
  {
    ATLASSERT(::IsWindow(m_hWnd));
    if ( lpRect != NULL )
    {
      CRect tmp = *lpRect;
      LPtoDP((LPPOINT) &tmp, 2);
      return ::ValidateRect(m_hWnd, lpRect);
    }
    else
      return ::ValidateRect(m_hWnd, lpRect);
  }



protected:

  void StartTimer();
  void StopTimer();


  BOOL HitTestTrackerBorder();
  BOOL MoveTrackerTo(CRect rcNew, CRect rcOld, BOOL fRepaint);
  void AdjustRect(int nHandle, LPRECT lpRect);
  void AdjustPoint(LPPOINT lpPoint);
  void ShowAOISize();

  void GetLogicalCursorPos(CPoint& pt)
  {
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    pt.x = floor(( pt.x - s_sizBorder.cx + m_ptOffset.x ) / m_dZoomScale);
    pt.y = floor(( pt.y - s_sizBorder.cy + m_ptOffset.y ) / m_dZoomScale);
  }

  void PrepareDC(HDC hDC)
  {

    SetMapMode(hDC, MM_ANISOTROPIC);
    SetWindowExtEx(hDC, m_SensorRect.Width(), m_SensorRect.Height(), NULL);  //Set up the logical window
    //xupxTest0627
    BOOL b = SetViewportExtEx(hDC, m_SensorRect.Width() * m_dZoomScale, m_SensorRect.Height() * m_dZoomScale, NULL); // in device coordinates
    SetViewportOrgEx(hDC, -m_ptOffset.x + s_sizBorder.cx, -m_ptOffset.y + s_sizBorder.cy, NULL); 
  }

  void UpdateViewPort(POINT* ptNewCenter)
  {
    if ( m_dZoomScale < 1.0 / 32.0 )
    {
      m_dZoomScale = 1.0 / 32.0; 
      MessageBeep(0);
    }
    else if ( m_dZoomScale > 32 )
    {
      MessageBeep(0);
      m_dZoomScale = 32;
    }
    CPoint ptCenter;
    if ( !ptNewCenter )
    {
      if ( ! m_bScalable )
      {
        ptCenter = GetLogicalCenter();  
      }
      else
      {
        ptCenter.x = 0.5 * ( m_RectTracker.m_rect.left + m_RectTracker.m_rect.right );
        ptCenter.y = 0.5 * ( m_RectTracker.m_rect.top + m_RectTracker.m_rect.bottom );
      }

    }
    else
    {
      ptCenter = *ptNewCenter;
    }
    
    m_sizeAll.cx = (long) (m_SensorRect.Width() * m_dZoomScale) + 2 * s_sizBorder.cx;
    m_sizeAll.cy = (long) (m_SensorRect.Height() * m_dZoomScale) + 2 * s_sizBorder.cy;
    RecalcBars(TRUE, FALSE);
    CenterOnLogicalPoint(ptCenter);
    ZoomLevelChanged();
    Invalidate();
    
  }

  void SetNewAOI();
    
    
  
  virtual void ZoomLevelChanged() 
  {
    // update main menu
    const double eps = 1e-4;
    bool enable25 = false;
    bool enable50 = false;
    bool enable100 = false;
    bool enable200 = false;
    bool enable400 = false;
    bool enableuser = true; 
    if ( fabs(m_dZoomScale - 0.25) <  eps )
    { enable25 = true; enableuser = false;}
    else if ( fabs(m_dZoomScale - 0.5 ) < eps)
    { enable50 = true; enableuser = false; }
    else if ( fabs(m_dZoomScale - 1.0 ) < eps )
    { enable100 = true; enableuser = false; }
    else if ( fabs(m_dZoomScale - 2.0 ) < eps )
    { enable200 = true; enableuser = false; }
    else if ( fabs(m_dZoomScale - 4.0 ) < eps )
    { enable400 = true; enableuser = false; }

    m_MainFrame.UISetCheck(ID_VIEW_ZOOM_25, enable25);
    m_MainFrame.UISetCheck(ID_VIEW_ZOOM_50, enable50);
    m_MainFrame.UISetCheck(ID_VIEW_ZOOM_100, enable100);
    m_MainFrame.UISetCheck(ID_VIEW_ZOOM_200, enable200);
    m_MainFrame.UISetCheck(ID_VIEW_ZOOM_400, enable400);
    m_MainFrame.UISetCheck(ID_VIEW_ZOOM_USER, enableuser);
    m_MainFrame.UpdateLayout();
  }

  void ReportError(HVBaseException& e)
  {
    m_MainFrame.ReportError(e);
  }

  void SetAOIRect(CRect rect)
  {
	  m_AOIRect = rect;
  }
  void RestorTrackRect()
  {
	  m_RectTracker.m_rect = m_AOIRect;
  }
  void SetTrackMinSize(CSize sz)
  {
	  m_TrackMinSize = sz;
  }
  void RestorTrackMinSize()
  {
	  m_RectTracker.m_sizeMin = m_TrackMinSize;
  }

private:
  CDevice*            m_pDevice;
  CSize               m_SensorSize;
  HV_VIDEO_MODE		  m_VideoMode;
  CRect               m_SensorRect;
  CMainFrame&         m_MainFrame;

  //Added by hujun
  CRect				  m_AOIRect;
  CSize               m_TrackMinSize;

  CImageRectTracker   m_RectTracker;
  bool                m_bScalable;      // Does the camera allow setting of AOI ?
  bool                m_bFit;           // Has the user performed ZoomToFit ?
  UINT                m_nIdEvent;
  CSize               m_SizeInc;  
  CSize               m_PosInc;
  CCriticalSection    m_CriticalSection;  // protect device context

  static              DWORD s_dwDragTimerInterval;
  HCURSOR             m_hCursor;
  bool                m_bActiveChild;




  friend class CImageRectTracker;
  friend class CDevice;
  friend class CChildFrame;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BCAMVIEWERVIEW_H__8C4B8AD4_7C21_11D5_920C_0090278E5E96__INCLUDED_)
