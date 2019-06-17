#if !defined(AFX_IMAGEVIEW_H__0196F3F2_13B5_4A97_84E7_114CDDD5D538__INCLUDED_)
#define AFX_IMAGEVIEW_H__0196F3F2_13B5_4A97_84E7_114CDDD5D538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageView.h : header file
//

#include "ImageRectTracker.h"
#include "CDib.h"

/////////////////////////////////////////////////////////////////////////////
// CImageView window

class CImageView : public CStatic
{
// Construction
public:
	CImageView(BYTE * pImage = NULL, int nWid = 0, int nHei = 0);

// Attributes
public:
	CArray<CImageRectTracker, CImageRectTracker&> m_arrayRect;
	BITMAPINFO	*m_pBmpInfo;
	int			m_nNowDepth;
	SCImage*	m_pSCImage;
	
// Operations
public:
// 	void Draw(CDC & dc, void * pImage, int nWid, int nHei);
	void OnDraw(CDC *pDC);
	void OnPrepareDC(CDC *pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageView)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	BYTE		* m_pImage;
	int			  m_nWid;
	int			  m_nHei;

	BOOL          m_bUpdate;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEW_H__0196F3F2_13B5_4A97_84E7_114CDDD5D538__INCLUDED_)
