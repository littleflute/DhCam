#if !defined(AFX_DLGSETPARAMETER_H__8243213D_B318_4888_BC7F_84D013B26269__INCLUDED_)
#define AFX_DLGSETPARAMETER_H__8243213D_B318_4888_BC7F_84D013B26269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetParameter.h : header file
//

#include "resource.h"
#include "Parameter.h"
#include "SetParamImageView.h"
#include "PluginModule.h"
#include "Placement.h"
#include <list>
//////////////////////////////////////////////////////////////////////////

#define COLOR_KEY					_T("Software\\Daheng\\Color")
#define RATIOTOINTEGER(ratio)		(int)((ratio) * 100)
#define INTEGERTORATIO(value)       (double)(value) / 100      

/////////////////////////////////////////////////////////////////////////////
// CDlgSetParameter dialog

class CDlgSetParameter : public CDialog, public CPluginModule
{
// Construction
public:	
	CDlgSetParameter(CWnd* pParent = NULL);
	~CDlgSetParameter();

	void RawDatato8(BYTE *pInRaw, BYTE *pOutRaw, int nWidth, int nHeigh, int n);
	
// Dialog Data
	//{{AFX_DATA(CDlgSetParameter)
	enum { IDD = IDD_DLG_SETPARAMETER };
	CSliderCtrl	m_SliderSharpen;
	CSliderCtrl	m_SliderContrast;
	CSliderCtrl	m_SliderBrightness;
	CComboBox	m_BitCombo;
	CSliderCtrl	m_SliderRed;
	CSliderCtrl	m_SliderGama;
	CSliderCtrl	m_SliderBlue;
	CSetParamImageView	m_ImageView;
	int		m_EditBlue;
	int		m_EditGama;
	int		m_EditRed;
	int		m_Bayer;
	BOOL	m_bCorrect;
	int		m_Light;
	int		m_Quality;
	BOOL	m_bDoColor;
	int		m_EditBrightness;
	int		m_EditContrast;
	float	m_EditSharpen;
	BOOL	m_bEnableGamma;
	BOOL	m_bEnableBrightness;
	BOOL	m_bEnableContrast;
	BOOL	m_bEnableSharpen;
	//}}AFX_DATA

	BITMAPINFO		*m_pBmpInfo;
	int				m_nNowDepth;
	int				m_nLastDepth;
	int				m_nLastColor;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetParameter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	CParameter   m_pParam;
	CParameter   m_sParam;
	CPlacement   m_Place;
	BOOL         m_bBalance;
	BOOL		 m_bSavePara;	//add by xupx 2008.10.06
	int			 m_ShiftMax;		//Raw data need to be shifted m_nBit. by xupx 2009.01.06
	int			 m_CurrentShift;
//	int			 m_DataDepth;		//real data depth, 8,10,12...
	std::list<CString> strlist; // save the list;

	CString      m_strInfo;
	CString      m_strInfo1;
	HMENU        m_hMenu;
	BYTE       * m_pDispBuff;
	UINT         m_nDispSize;
	CString      m_KeyPath;
	UINT		 m_nTempSize;
	BYTE		*m_pTempBuff;
	BOOL		m_bShowDlg;
	int			m_nSrcDepth;
	int         m_nDestDepth;

	// Generated message map functions
	//{{AFX_MSG(CDlgSetParameter)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusRatioB();
	afx_msg void OnKillfocusRatioR();
	afx_msg void OnKillfocusRatioGama();
	afx_msg void OnCheckCorrect();
	afx_msg void OnBalance();
	afx_msg void OnWbReset();
	afx_msg void OnColor();
	afx_msg void OnDefaultGamma();
	afx_msg void OnSelchangeComboBit();
	afx_msg void OnKillfocusEditBrightness();
	afx_msg void OnKillfocusEditContrast();
	afx_msg void OnKillfocusEditSharpen();
	afx_msg void OnDefaultBrightness();
	afx_msg void OnDefaultContrast();
	afx_msg void OnDefaultSharpen();
	afx_msg void OnEnableBrightness();
	afx_msg void OnEnableGamma();
	afx_msg void OnEnableContrast();
	afx_msg void OnEnableSharpen();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void	OnBayerChange(UINT nID);
	afx_msg void	OnQualityChange(UINT nID);
	afx_msg void	OnLightChange(UINT nID);
	afx_msg LRESULT OnUpdateView(WPARAM, LPARAM);
	afx_msg LRESULT OnShowDlg(WPARAM, LPARAM);	
	DECLARE_MESSAGE_MAP()

protected:
	void EnableSharpenControl(BOOL bEnable);
	void EnableContrastControl(BOOL bEnable);
	BOOL isSupportedColorCode(HV_COLOR_CODE code);
	void InitializeControl();
	void EnableControl(BOOL bEnable);
	int  BayerToControl(HV_BAYER_LAYOUT layout);
	int  ConvertToControl(HV_BAYER_CONVERT_TYPE type);
	HV_BAYER_LAYOUT       ControlToBayer(int control);
	HV_BAYER_CONVERT_TYPE ControlToConvert(int control);
	
	void WhiteBalance(void *pDst, void *pSrc, int nWid, int nHei);
	void ConvertBayer2Rgb(void *pDst, void *pSrc, int nWid, int nHei);
	void UpdatePreView(void *pDst, void *pSrc, int nWid, int nHei);
	void EnableFullControl(BOOL bEnable);
	void EnableGammaControl(BOOL bEnable);
	void EnableBrightnessControl(BOOL bEnable);
	
public:
	///
	virtual void OnManagerNotify(CConvertImageInfo *pImageInfo);
	///
	virtual void GetName(char * buffer);
	///
	virtual void GetInfo(char * buffer);
	///
	virtual DWORD GetSpecVersion();
	/// 
	virtual void Initialize(CCamera hCamera, CManager hManager);
	///
	virtual void Uninitialize();
	///
	virtual bool OnUserCommand(unsigned int id);
	///
	virtual void OnStartGrabbing(CConvertImageInfo *pImageInfo);
	///
	virtual void OnStopGrabbing(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData);
	///
	virtual void OnShowFrame(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETPARAMETER_H__8243213D_B318_4888_BC7F_84D013B26269__INCLUDED_)
