// XdTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "mdi.h"
#include "XdTreeCtrl.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXdTreeCtrl

CXdTreeCtrl::CXdTreeCtrl()
{
   m_strName="";
	
}

CXdTreeCtrl::~CXdTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CXdTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXdTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXdTreeCtrl message handlers

void CXdTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrame);
	HTREEITEM hitem=pNMTreeView->itemNew.hItem;
    CString strName=GetItemText(hitem);        //取下树型控件中的字符串
    m_strName=strName;	
	
	if(strName!="DH CameraType")
	{
		int num;    
		sscanf(strName.GetBuffer(strName.GetLength()),"%d",&num);

		DeviceNode *pCurrentNode = NULL;
		if(pCurrentNode = pFrame->f_GetDeviceNodeFromIndex(pFrame->phead,num))
		{
			if (pCurrentNode->pDeviceWnd != NULL)
			{
				pCurrentNode->pDeviceWnd->MDIActivate();//设置子窗口的焦点
			}
		}
	}

   *pResult = 0;
}



void CXdTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	ASSERT(pFrame);    
	
    if(m_strName!="DH types of cameras")
	{		
		int num;    
		CString strName="";		
		DeviceNode *pCurrentNode = NULL;
		sscanf(m_strName.GetBuffer(m_strName.GetLength()),"%d %[^-]",&num,strName);

		if(pCurrentNode = pFrame->f_GetDeviceNodeFromIndex(pFrame->phead,num))
		{
			if (pCurrentNode->pDeviceWnd == NULL)
			{
				pCurrentNode->pDeviceWnd = new CBounceWnd;
				pCurrentNode->pDeviceWnd->m_nCamerNumber = pCurrentNode->nbegin;				
				pCurrentNode->pDeviceWnd->m_nListNum=num;//add 081013_lm
				pCurrentNode->pDeviceWnd->m_strCamerName.Format("%s",strName);
				sprintf(pCurrentNode->pDeviceWnd->m_szDevicePath,"%s",pCurrentNode->szDevicePath);
				CString strNameL;
//				strNameL.Format("%s%d",strName,num);

				/******add 081020_lm begin******/
				sscanf(m_strName.GetBuffer(m_strName.GetLength()),"%*d%s",strName);
				pCurrentNode->pDeviceWnd->m_strCamerDispName.Format("%s",strName);
				strNameL.Format("%s",strName);
				/**************end*************/
				
				if (!pCurrentNode->pDeviceWnd->Create(_T(strNameL),
					WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, pFrame->rectDefault, pFrame))
					return;
			}
		}
		
	}
	*pResult = 0;
}
