// CMDIMenuControl : taken from the codeproject's WTL donut sample (http://www.codeproject.com/wtl/donut.asp)

#pragma once

#include "atlcrack.h"

class CMDIMenuControl : CWindowImpl<CMDIMenuControl>
{
public:
// Data members
	CContainedWindowT<CMDIWindow> m_wndMDIClient;
	CRect m_arrCaption[3];
	BOOL m_bPushed;
	int m_nTracking;
	bool m_bVisible;

	enum { s_kcyWastefulReBarGap = 0 };

// Constants
	enum _MDIMenuControlConstants
	{
		s_kcxGap = 2
	};

// Constructor
	CMDIMenuControl() :
		m_wndMDIClient(this, 1),
		m_bPushed(false),
		m_nTracking(-1),
		m_bVisible(true)
	{
	}

	bool Install(HWND hWndCmdBar, HWND hWndMDIClient)
	{
		CRect rc;
		::GetClientRect(hWndCmdBar, &rc);
		_UpdateLayout(rc.Size(), false);// now, no need to fix

		SubclassWindow(hWndCmdBar);
		m_wndMDIClient.SubclassWindow(hWndMDIClient);
		m_wndMDIClient.m_hWndMDIClient = hWndMDIClient;

		return true;
	}

// Message map and handlers
	BEGIN_MSG_MAP_EX(CMDIMenuControl)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_WINDOWPOSCHANGED(OnWindowPosChanged)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONUP(onLButtonUp)
	ALT_MSG_MAP(1)
		MSG_WM_PAINT(OnPaintMDIClient)
	END_MSG_MAP()

	void OnLButtonDown(UINT nFlags, CPoint pt)
	{
		SetMsgHandled(_OnMouseMsg(WM_LBUTTONDOWN, nFlags, pt));
	}

	void OnMouseMove(UINT nFlags, CPoint pt)
	{
		SetMsgHandled(_OnMouseMsg(WM_MOUSEMOVE, nFlags, pt));
	}

	void onLButtonUp(UINT nFlags, CPoint pt)
	{
		SetMsgHandled(_OnMouseMsg(WM_LBUTTONUP, nFlags, pt));
	}

	void OnPaint(HDC hDC)
	{
//		ATLTRACE2(atlTraceUser, 4, _T("CMDIMenuControl::OnPaint\n"));
		if (!m_bVisible) {
			SetMsgHandled(FALSE);
			return;
		}
//
//		CDCHandle dcOld(hDC);
//		CRect rc;
//		dcOld.GetClipBox(&rc);

		DefWindowProc();
//		CRect rcCaptions(m_arrCaption[2].left, m_arrCaption[2].top,
//			m_arrCaption[0].right, m_arrCaption[0].bottom);

//		CRect rcTmp;
//		if (::IntersectRect(&rcTmp, rc, rcCaptions)) {
			CClientDC dc(m_hWnd);
			_DoPaint(dc.m_hDC);
//		}
	}

	void OnWindowPosChanged(LPWINDOWPOS lpwndpos)
	{
		ATLTRACE2(atlTraceUser, 4, _T("CMDIMenuControl::OnWindowPosChanged (%d, %d)\n"), lpwndpos->cx, lpwndpos->cy);
		SetMsgHandled(FALSE);

		if (!m_bVisible) {
			CSize size(lpwndpos->cx, lpwndpos->cy);
			_UpdateLayout(size);			
			return;
		}

		// erase controls on previous position
		_InvalidateControls();

		CSize size(lpwndpos->cx, lpwndpos->cy);
		_UpdateLayout(size);

		CClientDC dc(m_hWnd);
		_DoPaint(dc.m_hDC);
	}

	void OnPaintMDIClient(HDC hDC)
	{
//		ATLTRACE2(atlTraceUser, 4, _T("CMDIMenuControl::OnPaintMDIClient\n"));
		SetMsgHandled(FALSE);

		bool bPrev = m_bVisible;
		m_bVisible = _IsChildWindowMaximized();
		if (bPrev != m_bVisible)
			_InvalidateControls();
	}

// Implementation
	BOOL _OnMouseMsg(UINT msg, UINT nFlags, CPoint pt)
	{
		if (!m_bVisible)
			return FALSE;

		if (msg == WM_LBUTTONDOWN) {
			//LTRACE(_T("    WM_LBUTTONDOWN\n"));
			m_nTracking = _HitTest(pt);
			if (m_nTracking >= 0) {
				CClientDC dc(m_hWnd);
				_DrawFrameControl(dc.m_hDC, m_nTracking, TRUE);
				m_bPushed = TRUE;
				SetCapture();				  // grab mouse input
				return TRUE;
			}
		}
		else if ((msg == WM_MOUSEMOVE) && m_nTracking >= 0) {
			// mouse moved, and I am tracking: possibly draw button up/down
			BOOL bOldDown = m_bPushed;
			m_bPushed = m_arrCaption[m_nTracking].PtInRect(pt);
			if (bOldDown != m_bPushed) {
				// up/down state changed: need to redraw button
				CClientDC dc(m_hWnd);
				_DrawFrameControl(dc.m_hDC, m_nTracking, m_bPushed);
			}
			return TRUE; // handled
		}
		else if (msg == WM_LBUTTONUP && m_nTracking >= 0) {
			// user released the mouse and I am tracking: do button command
			::ReleaseCapture(); // let go the mouse
			if (m_bPushed) {
				// if button was down when released: draw button up, and do system cmd
				CClientDC dc(m_hWnd);
				_DrawFrameControl(dc.m_hDC, m_nTracking, FALSE);
				CWindow wnd = m_wndMDIClient.MDIGetActive();
				if (wnd.m_hWnd) {
					static int syscmd[3] = { /*SC_MOUSEMENU,*/ SC_CLOSE, SC_RESTORE, SC_MINIMIZE };
					// SendMessage occur access violation!
					wnd.PostMessage(WM_SYSCOMMAND, syscmd[m_nTracking]);
				}
			}
			m_nTracking = -1; // stop tracking
			return TRUE;	  // handled (eat)
		}

		return FALSE;
	}

	void _DoPaint(CDCHandle hDC)
	{
		for (int i = 0; i < 3; ++i) {
			_DrawFrameControl(hDC, i, FALSE);
		}	
	}

	void _UpdateLayout(const CSize& size, bool bGapValid = true)
	{
//		ATLTRACE2(atlTraceUser, 4, _T("CMDIMenuControl::_UpdateLayout : size(%d, %d)\n"), size.cx, size.cy);		

		CSize sizeCaption = _GetCaptionSize();
		int cxCaption = sizeCaption.cx;
		int cyCaption = sizeCaption.cy;
/*
		CRect rcCaption;
		rcCaption.right = size.cx;
		rcCaption.top = 0;
		rcCaption.bottom = 0 + cyCaption;
		rcCaption.left = rcCaption.right - cxCaption;

		m_arrCaption[0] = rcCaption;

		rcCaption -= CPoint(cxCaption + s_kcxGap, 0);
		m_arrCaption[1] = rcCaption;

		rcCaption -= CPoint(cxCaption, 0);
		m_arrCaption[2] = rcCaption;
*/

		CRect rcCaption;
		rcCaption.right = size.cx;
		rcCaption.bottom = size.cy - s_kcxGap;
		rcCaption.left = rcCaption.right - cxCaption;
		rcCaption.top = rcCaption.bottom - cyCaption;

		if (bGapValid)
			rcCaption += CPoint(0, s_kcyWastefulReBarGap*2);

		m_arrCaption[0] = rcCaption;

		rcCaption -= CPoint(cxCaption + s_kcxGap, 0);
		m_arrCaption[1] = rcCaption;

		rcCaption -= CPoint(cxCaption, 0);
		m_arrCaption[2] = rcCaption;
	}

	void _InvalidateControls()
	{
		for (int i = 0; i < 3; ++i) {
			InvalidateRect(m_arrCaption[i]);
		}
	}

	void _DrawFrameControl(CDCHandle hDC, int nIndex, BOOL bPushed)
	{
		// draw frame controls
		CRect& rc = m_arrCaption[nIndex];
		const UINT dfcs[3] = { DFCS_CAPTIONCLOSE, DFCS_CAPTIONRESTORE, DFCS_CAPTIONMIN };
		UINT uState = dfcs[nIndex];
		if (bPushed)
			uState |= DFCS_PUSHED;
		hDC.DrawFrameControl(rc, DFC_CAPTION, uState);
	}

	int _HitTest(CPoint point)
	{
		for (int i = 0; i < 3; ++i) {
			if (m_arrCaption[i].PtInRect(point))
				return i;
		}

		return -1;
	}

	CSize _GetCaptionSize()
	{
		int cxCaption = ::GetSystemMetrics(SM_CXSIZE) - s_kcxGap;
		int cyCaption = ::GetSystemMetrics(SM_CYSIZE) - s_kcxGap*2;

		int cyMenu = ::GetSystemMetrics(SM_CYMENUSIZE);
		if (cyCaption > cyMenu) {
			cyCaption = cyMenu;
			cxCaption = cyMenu + s_kcxGap;
		}

		return CSize(cxCaption, cyCaption);
	}

	bool _IsChildWindowMaximized()
	{
		BOOL bMaximized = FALSE;
		m_wndMDIClient.MDIGetActive(&bMaximized);
		return (bMaximized == TRUE);
	}

};