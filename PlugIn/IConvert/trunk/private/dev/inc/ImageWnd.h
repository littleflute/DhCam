/**
	@file  ImageWnd.h
	@brief CImageWnd class header

	Copyright (c) 2009, Daheng
  
	@author  xupx
	@date 2009-05-21
	@version 1.0.1.0
*/



#if !defined(AFX_IMAGEWND_H__B2988DDD_796E_4D1B_8818_AEDBC0A97986__INCLUDED_)
#define AFX_IMAGEWND_H__B2988DDD_796E_4D1B_8818_AEDBC0A97986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageWnd.h : header file
//
#include "CDib.h"


/**
* \brief   CImageWnd: Display image in several ways.
*
* This class used to display, transfer, or zoom image.
*
* 
*/
//------------------------------------------------------------------------------

class AFX_EXT_CLASS  CImageWnd : public CWnd
{
// Construction
public:
	CImageWnd();

// Attributes
public:
	
// Operations
public:

	int		Create(CWnd * pParent, DWORD dwStyle, CRect *rc);

	// ���������б����а���ͼ������
	int		LoadImage(int width, int height, int bitDepth, void *pSrc);

	// ��Ӳ���ж�ȡͼ�������ļ���ͼ�����⻺����
	int		LoadImage(char *FileName);

	// �ѵ�ǰͼ�����ͼ�񻺳�������ΪӲ���ļ�.
	int		SaveImage(char *FileName);

	// ���õ�ǰͼ����ʾģʽ
	void		 SetMode(OPERATE_MODE mode);

	OPERATE_MODE GetMode ();
	
	// �������ű���
	void		SetZoom(double scale);

	// �������ŷ�Χ
	void		SetZoomRange(double min, double max);

	// �õ�ģʽ
	double		GetZoom();

	// ���ԴӴ˴��õ���ǰ�����ɫ������
	COLORREF	GetPixelValue();

	CPoint		GetPos();

	SCImage *	GetImage();
	
	 void	SetBkColor(COLORREF color);

	 void	FitToWin();

	 // �õ��豸����dc
	 void	OnPrepareDC(CDC* pdc);
	 
	 virtual void OnDraw(CDC *pDC);

private:
	void DoZoomUp(CPoint point);
	void DoZoomDown(CPoint point);

protected:
	
	virtual void  RedrawEmpty();
	
protected:

	COLORREF	m_BKColor;
	
	// ���ű���
	double		m_fZoomScale;
	double		m_fZoomScaleY;

	// X�᷽���ƶ�ͼ��
	int			m_TransferX;

	// Y�᷽���ƶ�ͼ��
	int			m_TransferY;

	// ��ק�����ľ��ο�
	CRect		m_trackerRect;

	SCImage*	m_pSCImage;

	// ͼ��ģʽ
	OPERATE_MODE	m_ImageMode;

	// �������������
	CPoint		m_LastPoint;

	// ���������Сֵ
	double		m_fZoomMax;
	double		m_fZoomMin;

	HCURSOR		m_hTransCur;
	HCURSOR		m_hZoomUpCur;
	HCURSOR		m_hZoomDownCur;
	HCURSOR		m_hMagnifierCur;

	CRect		m_OldRect;

	CRITICAL_SECTION  m_Lock;

	BOOL		m_CtrlDown;

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEWND_H__B2988DDD_796E_4D1B_8818_AEDBC0A97986__INCLUDED_)





















