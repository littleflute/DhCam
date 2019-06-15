// mainfrm.h : interface of the CMainFrame class

//#include "afxext.h"
//#include "afxcmn.h"
#include"XdTreeCtrl.h"
#include"bounce.h"
#define CameraNumber  20
#define RELEASEBUFANDDESTROYWINDOW(p)	if(p) {/*p->DestroyWindow();*/delete[]p; p=NULL;}


typedef struct _Node {
	int            nbegin;
    char           szDevicePath[128];    
	char           szDeviceName[20];
	char           szDeviceSN[20];
	CBounceWnd     *pDeviceWnd;
	struct _Node   *pNext;
} DeviceNode, *PDeviceNode;


//------------------------------------------------------------------------------
// class CStopWatch
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
 * \brief   Measurement of time intervalls
 *
 */
//------------------------------------------------------------------------------


class CStopWatch
{
public:
  //////////////////////////////////////////////////////////////////////////////////////////
  // cconstructor, starts time measurement
  CStopWatch()
  {
    Start();
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Start the stop watch 
  void Start()
  {
     QueryPerformanceCounter(&m_StartTime);
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Stop. The elapsed time is returned. The stop watch may be started againn neuen Zeitpunkt
  double Stop(bool StartAgain)
  {
    QueryPerformanceCounter(&m_StopTime);
    double theElapsedTime = ElapsedTime();
    if(StartAgain)
      m_StartTime = m_StopTime; 
    return theElapsedTime;
  }

  //////////////////////////////////////////////////////////////////////////////////////////
  // Return the elapsed time in seconds between start() and stop()
  double ElapsedTime()
  {
    LARGE_INTEGER timerFrequency;
    QueryPerformanceFrequency(&timerFrequency);
  
    __int64 oldTicks = ((__int64)m_StartTime.HighPart << 32) + (__int64)m_StartTime.LowPart;
    __int64 newTicks = ((__int64)m_StopTime.HighPart << 32) + (__int64)m_StopTime.LowPart;
    long double timeDifference = (long double) (newTicks - oldTicks);

    long double ticksPerSecond = (long double) (((__int64)timerFrequency.HighPart << 32) 
				  + (__int64)timerFrequency.LowPart);

    return (double)(timeDifference / ticksPerSecond);
  }

protected:
  // zero-point for time measurment
  LARGE_INTEGER m_StartTime;

  // last time stamp
  LARGE_INTEGER m_StopTime;
};

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	
	CToolBar   m_wndToolBar;
	CStatusBar m_wndStatusBar;
    
    CCoolBar    m_TypeToolBar; ///English:    ///Chinese: 摄像机类型工具条
	CXdTreeCtrl m_wndTree;
	HTREEITEM   m_hti;         ///English:    ///Chinese: 树根句柄
	HTREEITEM   m_MIDhti[CameraNumber];///English:    ///Chinese: 纪录树中子项的句柄
	int  m_nDevTotal;          ///English:    ///Chinese: 设备总数

	DeviceNode* phead;         ///English:    ///Chinese: 设备链表的头
// Operations
public:

// Implementation

public:
	void f_RefreshTree_Insert(DeviceNode *pInsertNode);
	void f_GetDeviceSN(int i, char*DeviceSN);
	DeviceNode* f_CreateList(DeviceNode *&h,char *szDevicePath,char *szDeviceSN,HVTYPE type,int nNumber);
	void f_DeleteList(DeviceNode *head);
	void f_GetDeviceInfo(int nDevice, HVTYPE *pType, char *pDevicePath);
	BOOL f_RemoveDeviceNode(DeviceNode *&h, char *szDevicePath);
	DeviceNode* f_GetDeviceNodeFromPath(DeviceNode *head,char *szDevicePath);
	DeviceNode* f_GetDeviceNodeFromIndex(DeviceNode *head,int nIndex);
	char* f_MakeLower(char *str);
	void f_RefreshTree();
	char * f_GetRemoveDevPath();
	CBounceWnd *m_pDeviceWnd[CameraNumber];///English:    ///Chinese: 升级要去掉
	void EnumDevice(int nNumber);
	virtual ~CMainFrame();

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewTypeBar();
	afx_msg void OnUpdateViewTypeBar(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnDeviceChange(UINT wParam, DWORD lParam);//JiaPeng
	afx_msg void OnButtonEnumerateDevices();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void v_Xd_Enumerate();
	BOOL RegPnpGUID(HANDLE h, GUID guid);

	CString		m_strTitle;
};

/////////////////////////////////////////////////////////////////////////////

