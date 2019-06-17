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

	// 接收数据列表，其中包括图像数据
	int		LoadImage(int width, int height, int bitDepth, void *pSrc);

	// 从硬盘中读取图像数据文件到图像界面库缓冲区
	int		LoadImage(char *FileName);

	// 把当前图像操作图像缓冲区保存为硬盘文件.
	int		SaveImage(char *FileName);

	// 设置当前图像显示模式
	void		 SetMode(OPERATE_MODE mode);

	OPERATE_MODE GetMode ();
	
	// 设置缩放比例
	void		SetZoom(double scale);

	// 设置缩放范围
	void		SetZoomRange(double min, double max);

	// 得到模式
	double		GetZoom();

	// 可以从此处得到当前点的颜色分量。
	COLORREF	GetPixelValue();

	CPoint		GetPos();

	SCImage *	GetImage();
	
	 void	SetBkColor(COLORREF color);

	 void	FitToWin();

	 // 得到设备环境dc
	 void	OnPrepareDC(CDC* pdc);
	 
	 virtual void OnDraw(CDC *pDC);

private:
	void DoZoomUp(CPoint point);
	void DoZoomDown(CPoint point);

protected:
	
	virtual void  RedrawEmpty();
	
protected:

	COLORREF	m_BKColor;
	
	// 缩放比例
	double		m_fZoomScale;
	double		m_fZoomScaleY;

	// X轴方向，移动图像
	int			m_TransferX;

	// Y轴方向，移动图像
	int			m_TransferY;

	// 拖拽出来的矩形框
	CRect		m_trackerRect;

	SCImage*	m_pSCImage;

	// 图像模式
	OPERATE_MODE	m_ImageMode;

	// 按下左键的坐标
	CPoint		m_LastPoint;

	// 缩放最大、最小值
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





















