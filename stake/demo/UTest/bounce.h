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
	
	static int CALLBACK Snap_Callback(HV_SNAP_INFO *pInfo);///English:    ///Chinese: �ص���������
	
    BOOL SetLutTable(double dRatioR,double dRatioG,double dRatioB);
	
	int     m_nListNum;		 ///English:    ///Chinese: ���οؼ����豸���
	int     m_nCamerNumber;  ///English:    ///Chinese: �������ѯ���
    CString m_strCamerName;  ///English:    ///Chinese: ������ͺ�
	CString m_strCamerDispName;///English:    ///Chinese: ��������������ͺ�-���кţ�
	CDialog *m_pDlgPrp;      ///English:    ///Chinese: ָ������ҳ�Ի��� 

    CRectTracker m_tracker; ///English:    ///Chinese: ���ƶ�������
    BOOL m_bAllowInvert;
	CRect m_rcClientRect;   ///English:    ///Chinese: ��ǰ�ͻ���
	BOOL  m_bShowAOI;
	CRect m_rcRemberTrackRect;

	int   m_nXStart;   ///English:    ///Chinese: ���������ʼX����            
    int   m_nYStart;   ///English:    ///Chinese: ���������ʼY����     
    int   m_nWidth;    ///English:    ///Chinese: ������ڿ�
    int   m_nHeight;   ///English:    ///Chinese: ������ڸ�
    
    BOOL  m_bTestALLMode;
	BOOL  m_IsLButtonDown;
    float  m_fOptwScale;///English:    ///Chinese: ����������ű���
    CRect m_rcOutputWindows;///English:    ///Chinese: ������������Ӧ��������
    CRect m_rcShowView;///English:    ///Chinese: ͼ����ʾ��ͼ����
	CRect m_rcAOI;///English:    ///Chinese: ͼ���ͼ������������������ڵ��ı�������
    CRect m_rcTestOutput;
    
    DWORD m_T1,m_T2,m_C;///English:    ///Chinese: ֡�ʼ���
    CString m_strFPS;

	int m_nMaxWidth;    ///English:    ///Chinese: ���������ڿ�
	int m_nMaxHeight;   ///English:    ///Chinese: ���������ڸ�
	
	HHV			m_hhv;				///English:    ///Chinese: ������������	
	BOOL		m_bOpen;			///English:    ///Chinese: ���豸��־
	BOOL		m_bStart;			///English:    ///Chinese: �����ɼ���־	
	BYTE		*m_pRawBuffer;		///English:    ///Chinese: �ɼ�ͼ��ԭʼ���ݻ�����
	BYTE		*m_pRGBBuffer;	    ///English:    ///Chinese: Bayerת����ͼ�����ݻ�����
    char		m_chBmpBuf[2048];   ///English:    ///Chinese: BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����		
	BITMAPINFO	*m_pBmpInfo;		///English:    ///Chinese: BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	
	///English:    ///Chinese: ��ɫ���ұ�
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;

	HPALETTE m_hPalette; 
	HPALETTE m_hOldPal;

	HV_BAYER_LAYOUT m_Layout;///English:    ///Chinese: Bayer ��������
	HV_RESOLUTION m_Resolution;
	HV_IMAGE_MODE m_ImageMode;

	///English:    ///Chinese: ��ƽ��ϵ��	
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
