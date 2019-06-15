// bounce.h : Declares the class interfaces for the Bounce child window.

#ifndef __BOUNCE_H__
#define __BOUNCE_H__
#include "afxext.h"
#define RELEASEBUFFER(p)	if(p) { delete []p; p=NULL;}

typedef enum  tagHV_IMAGE_MODE
{
	HV_COLOR = 0,
	HV_BW    = 1
}HV_IMAGE_MODE;

#define WM_GET_RATIO		(WM_USER + 101)
/////////////////////////////////////////////////////////////////////////////

class CBounceWnd : public CMDIChildWnd
{
public:
	CBounceWnd();
    ~CBounceWnd();
	BOOL Create(LPCTSTR szTitle, LONG style = 0,
				const RECT& rect = rectDefault,
				CMDIFrameWnd* pParent = NULL);
public:
	BOOL m_bBegin;
	BOOL m_bTrigger;
	BOOL BMPBufBottomToUp(BYTE* pInBuffer,BYTE* pOutBuffer, int nWidth, int nHeight);
	void Calculatefps();
	void Spec_Fun_Interface(HHV hhv, HV_INTERFACE1_ID Interface_ID, int value);
	void TestSetDefaultValue();
	void SetCMOSVideoMode2();
	void SetCMOSVideoMode1();
	void SetCMOSVideoMode0();
	void AdjustExposureTime();
	void SetScrollInfoX();
	void ShowImage();
	
	void Init(int n);
	HVSTATUS Open();
	HVSTATUS Start();
	HVSTATUS Stop();
	HVSTATUS Close();
	
	static int CALLBACK Snap_Callback(HV_SNAP_INFO *pInfo);///English:    ///Chinese: 回调函数声明
	
    BOOL SetLutTable(double dRatioR,double dRatioG,double dRatioB);
	
	int     m_nListNum;		 ///English:    ///Chinese: 树形控件中设备序号
	int     m_nCamerNumber;  ///English:    ///Chinese: 摄像机查询序号
    CString m_strCamerName;  ///English:    ///Chinese: 摄像机型号
	CString m_strCamerDispName;///English:    ///Chinese: 摄像机表现名（型号-序列号）
	CDialog *m_pDlgPrp;      ///English:    ///Chinese: 指向属性页对话框 

    CRectTracker m_tracker; ///English:    ///Chinese: 可移动窗口类
    BOOL m_bAllowInvert;
	CRect m_rcClientRect;   ///English:    ///Chinese: 当前客户区
	BOOL  m_bShowAOI;
	CRect m_rcRemberTrackRect;

	int   m_nXStart;   ///English:    ///Chinese: 输出窗口起始X坐标            
    int   m_nYStart;   ///English:    ///Chinese: 输出窗口起始Y坐标     
    int   m_nWidth;    ///English:    ///Chinese: 输出窗口宽
    int   m_nHeight;   ///English:    ///Chinese: 输出窗口高
    
    BOOL  m_bTestALLMode;
	BOOL  m_IsLButtonDown;
    float  m_fOptwScale;///English:    ///Chinese: 输出窗口缩放比例
    CRect m_rcOutputWindows;///English:    ///Chinese: 输出窗口区域对应背景区域
    CRect m_rcShowView;///English:    ///Chinese: 图像显示绘图区域
	CRect m_rcAOI;///English:    ///Chinese: 图像绘图区域相对于最大输出窗口的四边形区域
    CRect m_rcTestOutput;
    
    DWORD m_T1,m_T2,m_C;///English:    ///Chinese: 帧率计算
    CString m_strFPS;

	int m_nMaxWidth;    ///English:    ///Chinese: 最大输出窗口宽
	int m_nMaxHeight;   ///English:    ///Chinese: 最大输出窗口高
	
	HHV			m_hhv;				///English:    ///Chinese: 数字摄像机句柄	
	BOOL		m_bOpen;			///English:    ///Chinese: 打开设备标志
	BOOL		m_bStart;			///English:    ///Chinese: 启动采集标志	
	BYTE		*m_pRawBuffer;		///English:    ///Chinese: 采集图像原始数据缓冲区
	BYTE		*m_pRGBBuffer;	    ///English:    ///Chinese: Bayer转换后图像数据缓冲区
    char		m_chBmpBuf[2048];   ///English:    ///Chinese: BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区		
	BITMAPINFO	*m_pBmpInfo;		///English:    ///Chinese: BITMAPINFO 结构指针，显示图像时使用
	
	///English:    ///Chinese: 颜色查找表
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;

	HPALETTE m_hPalette; 
	HPALETTE m_hOldPal;

	HV_BAYER_LAYOUT m_Layout;///English:    ///Chinese: Bayer 数据类型
	HV_RESOLUTION m_Resolution;
	HV_IMAGE_MODE m_ImageMode;

	///English:    ///Chinese: 白平衡系数	
	double m_dRatioR;
	double m_dRatioG;
	double m_dRatioB;

	char          m_szDevicePath[128];  //Add 2008.9.27


// Implementation
protected:
	static CMenu NEAR menu;     // menu for all BOUNCE windows

	// message handlers
	//{{AFX_MSG(CBounceWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOpenDlgPrp();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnOutputwinScale50();
	afx_msg void OnUpdateOutputwinScale50(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnAoiMachingBest();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnOutputwinScale100();
	afx_msg void OnUpdateOutputwinScale100(CCmdUI* pCmdUI);
	afx_msg void OnOutputwinScale150();
	afx_msg void OnUpdateOutputwinScale150(CCmdUI* pCmdUI);
	afx_msg void OnOutputwinScale200();
	afx_msg void OnUpdateOutputwinScale200(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnGrabContinuous();
	afx_msg void OnGrabCancel();
	afx_msg void OnUpdateGrabContinuous(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrabCancel(CCmdUI* pCmdUI);
	afx_msg void OnGrabSingle();
	afx_msg void OnUpdateGrabSingle(CCmdUI* pCmdUI);
	afx_msg void OnImagemode();
	afx_msg void OnUpdateImagemode(CCmdUI* pCmdUI);
	afx_msg void OnWhiteBalance();
	afx_msg void OnUpdateWhiteBalance(CCmdUI* pCmdUI); 
	//}}AFX_MSG

    LRESULT OnGetRatioMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif // __BOUNCE_H__
