// SetParamImageView.h: interface for the CSetParamImageView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETPARAMIMAGEVIEW_H__51F3360B_AFE1_4792_9BAD_D47DB2B7980A__INCLUDED_)
#define AFX_SETPARAMIMAGEVIEW_H__51F3360B_AFE1_4792_9BAD_D47DB2B7980A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageView.h"

//////////////////////////////////////////////////////////////////////////

class CSetParamImageView : public CImageView  
{
public:
	CSetParamImageView();
	virtual ~CSetParamImageView();

	
	// Generated message map functions
protected:
	//{{AFX_MSG(CSetParamImageView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
		
protected:
	void GetValidRect(CRect * rect);
	BOOL IsValidClick(CPoint point);
	BOOL HitRectBorder(CPoint point);
	void LogicPtToImagePt(CPoint * point, int nWid, int nHei);
	void LogicRcToImageRc(CRect * rect, int nWid, int nHei);
	void VerticalInvertRect(CRect * rect, int nHei);

public:
	void InitialRect(BITMAPINFO *pBmpInfo, int nNowDepth);
	void UpdateImage(void * pImage, int nWid, int nHei);
	void GetSelectRect(CRect * rect, int nWid, int nHei);
	void GetCursorPoint(CPoint * point, int nWid, int nHei);
	void GetRectInfo(CString * info, void *pSrc, int nWid, int nHei);
	void GetPointInfo(CString * info,  void *pSrc, int nWid, int nHei);
};

#endif // !defined(AFX_SETPARAMIMAGEVIEW_H__51F3360B_AFE1_4792_9BAD_D47DB2B7980A__INCLUDED_)
