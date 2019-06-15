#if !defined(AFX_DLGPRP_H__74FEA327_FCBC_4777_B225_052983EC011A__INCLUDED_)
#define AFX_DLGPRP_H__74FEA327_FCBC_4777_B225_052983EC011A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPrp.h : header file
//
#define ID_MYSLIDER         1821
#include "AllCtrl.h"
#include "bounce.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPrp dialog

class CDlgPrp : public CDialog
{
// Construction
public:
	int UpdateFriend(CString strName,int nPos);
	void SetExposureTime_HV1300(HHV hhv, int nWidth, int nUnit, int nSpeed);///English:    ///Chinese: HV1300FC/M曝光时间设置
	void SetExposureTime_HV3100FC(HHV hhv, int nWidth, int nUnit, int nSpeed);///English:    ///Chinese: HV3100FC曝光时间设置
	void SetExposureTime_HV2000FC(HHV hhv, int nWidth, int nUnit, int nSpeed);///English:    ///Chinese: HV2000FC曝光时间设置
	void Spec_Fun_Interface(HHV hhv, HV_INTERFACE1_ID Interface_ID, int value);
	
	HHV m_hhvCam;///English:    ///Chinese: 摄像机句柄
	CBounceWnd *m_bounceWnd;
	int m_nShutterSpeed; ///English:    ///Chinese: 快门速度
	int m_nShutterUnit;  
	int UpdateFriend(CAllCtrl *pCt,int nPos);
	
	void XOnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void XDefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void CreateAllCtrl();
	CDlgPrp(HHV hhv,CWnd* pParent = NULL);   // standard constructor
	~CDlgPrp();
// Dialog Data
	//{{AFX_DATA(CDlgPrp)
	enum { IDD = IDD_DLG_PRP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrp)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrp)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadLut();///English:    ///Chinese: 加载查找表文件
	BOOL m_bFreeze;
    BOOL m_bBlackEnable;
	
    int			m_nCtl;   ///English:    ///Chinese: 控件个数
	
    CAllCtrl    *m_pAllCtrl[100];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRP_H__74FEA327_FCBC_4777_B225_052983EC011A__INCLUDED_)
