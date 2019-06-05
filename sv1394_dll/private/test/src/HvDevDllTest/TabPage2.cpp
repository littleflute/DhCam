// TabPage2.cpp : implementation file
//

#include "stdafx.h"
#include "HvDevDllTest.h"
#include "TabPage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPage2 dialog


CTabPage2::CTabPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabPage2)
	m_strDllName = _T("");
	m_strFunName = _T(""); 
	m_strTimeStart = _T("");
	m_strTimeStop = _T("");
	m_strTimeElapse = _T("");
	m_strGUID = _T("");
	//}}AFX_DATA_INIT
}


void CTabPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabPage2)
	DDX_Control(pDX, IDC_LIST_ENUM_DEVICE, m_ctrlDeviceList);
	DDX_Text(pDX, IDC_STATIC_DLL_NAME, m_strDllName);
	DDX_Text(pDX, IDC_STATIC_FUN_NAME, m_strFunName); 
	DDX_Text(pDX, IDC_STATIC_TIME_START, m_strTimeStart);
	DDX_Text(pDX, IDC_STATIC_TIME_STOP, m_strTimeStop);
	DDX_Text(pDX, IDC_STATIC_TIME_ELAPSE, m_strTimeElapse);
	DDX_Text(pDX, IDC_EDIT_GUID, m_strGUID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabPage2, CDialog)
	//{{AFX_MSG_MAP(CTabPage2)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnTest)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENUM_DEVICE, OnClickListEnumDevice)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPage2 message handlers

void CTabPage2::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CStatic *pSt = (CStatic*)GetDlgItem(IDC_STATIC_BOX);
	if(pSt)
	pSt->MoveWindow(10,0,cx-10,cy,1);
	
	CListCtrl &lc = m_ctrlDeviceList;
	if(lc.m_hWnd){
		lc.MoveWindow(12,250,cx-20,cy-260,1);
	}
	 

}

void CTabPage2::_Set_Init(CString strDllName, CString strFunName)
{
	m_strDllName = strDllName;
	m_strFunName = strFunName;
	m_strTimeElapse = "0.0";
	m_strTimeStart = "";
	m_strTimeStop = "";
	UpdateData(false);
}

void CTabPage2::OnBtnTest() 
{
	// TODO: Add your control notification handler code here
	if(m_strFunName=="HVGuid")
	{
		v_Test_HVGuid();
		return;		
	}

	v_Test_Enumerate();

}

void CTabPage2::v_Test_HVGuid()
{
	CListCtrl &lc = m_ctrlDeviceList;
	lc.DeleteAllItems();
	if(0==m_strDllName.GetLength())
	{
		AfxMessageBox("Dll名称为空");
		return;
	}
	if(0==m_strFunName.GetLength())
	{
		AfxMessageBox("函数名称为空");
		return;
	}
	
	//to test... 
	char szDllFileName[256];
	sprintf(szDllFileName, "%s%s", m_strDllName.GetBuffer(m_strDllName.GetLength()), ".dll");
 
	HMODULE hDll = LoadLibrary(szDllFileName); 
	if(hDll)
	{		 
		DWORD dwNeeded = 0;

		FUNC_Guid pFunc = (FUNC_Guid)::GetProcAddress(hDll,
			m_strFunName.GetBuffer(m_strFunName.GetLength())); 
		if(NULL==pFunc){
			AfxMessageBox(m_strDllName + ".dll 库中不存在函数:" + m_strFunName);
			return;
		}		
		 
		SYSTEMTIME   u;
		GetLocalTime(&u);
		m_strTimeStart.Format("%d.%d.%02d-%02d:%02d:%02d-%04d",u.wYear,u.wMonth,u.wDay,u.wHour,u.wMinute,u.wSecond,u.wMilliseconds);
	 		
		CStopWatch t;    
		t.Start();	

		GUID guid;
		pFunc(&guid);//HVGuid
		
		double theElapsedTime = t.Stop(true);			
		m_strTimeElapse.Format("%f",theElapsedTime);
		GetLocalTime(&u);
		m_strTimeStop.Format("%d.%d.%02d-%02d:%02d:%02d-%04d",u.wYear,u.wMonth,u.wDay,u.wHour,u.wMinute,u.wSecond,u.wMilliseconds);
	
				
		lc.InsertItem(0,"");
		CString strGUID;
		strGUID.Format("%x-%x-%x_%x-%x-%x-%x-%x-%x-%x-%x",
					guid.Data1,guid.Data2,guid.Data3,
					guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],
					guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]); 
		lc.SetItemText(0,1,strGUID);

		FreeLibrary(hDll);
	}
	else{		
		AfxMessageBox(m_strDllName + ".dll 加载失败");
	}
}
void CTabPage2::v_Test_Enumerate()
{
	CListCtrl &lc = m_ctrlDeviceList;
	lc.DeleteAllItems();
	if(0==m_strDllName.GetLength())
	{
		AfxMessageBox("Dll名称为空");
		return;
	}
	if(0==m_strFunName.GetLength())
	{
		AfxMessageBox("函数名称为空");
		return;
	}
	
	//to test... 
	char szDllFileName[256];
	sprintf(szDllFileName, "%s%s", m_strDllName.GetBuffer(m_strDllName.GetLength()), ".dll");
 
	HMODULE hDll = LoadLibrary(szDllFileName); 
	if(hDll)
	{		 
		DWORD dwNeeded = 0;

		FUNC_Enumerate pFunc = (FUNC_Enumerate)::GetProcAddress(hDll,
			m_strFunName.GetBuffer(m_strFunName.GetLength())); 
		if(NULL==pFunc){
			AfxMessageBox(m_strDllName + ".dll 库中不存在函数:" + m_strFunName);
			return;
		}		
		 
		SYSTEMTIME   u;
		GetLocalTime(&u);
		m_strTimeStart.Format("%d.%d.%02d-%02d:%02d:%02d-%04d",u.wYear,u.wMonth,u.wDay,u.wHour,u.wMinute,u.wSecond,u.wMilliseconds);
	 		
		CStopWatch t;    
		t.Start();	
		pFunc(NULL,0,&dwNeeded);//查总个数

		int n = dwNeeded/sizeof(HV_DEVICE_INTERFACE);
		if(n>0)//有设备存在
		{
			HV_DEVICE_INTERFACE *pHvDevIF = new HV_DEVICE_INTERFACE[n];
			pFunc(pHvDevIF,dwNeeded,&dwNeeded);//取出所有设备的列表
			
			double theElapsedTime = t.Stop(true);			
			m_strTimeElapse.Format("%f",theElapsedTime);
			GetLocalTime(&u);
			m_strTimeStop.Format("%d.%d.%02d-%02d:%02d:%02d-%04d",u.wYear,u.wMonth,u.wDay,u.wHour,u.wMinute,u.wSecond,u.wMilliseconds);
			 
			UpdateData(false);
			
			for(int i = 0; i < n; i++)
			{ 
				GUID guid = pHvDevIF[i].ClassGUID;  
				
				lc.InsertItem(i,"");
				CString strIdx;
				strIdx.Format("%d",i);
				lc.SetItemText(i,0,strIdx);
				CString strGUID;
				strGUID.Format("%x-%x-%x_%x-%x-%x-%x-%x-%x-%x-%x",
					guid.Data1,guid.Data2,guid.Data3,
					guid.Data4[0],guid.Data4[1],guid.Data4[2],guid.Data4[3],
					guid.Data4[4],guid.Data4[5],guid.Data4[6],guid.Data4[7]); 
				lc.SetItemText(i,1,strGUID);
				CString strDevPath;
				strDevPath.Format("%s",pHvDevIF[i].szDevicePath); 
				lc.SetItemText(i,2,strDevPath);

			}

			delete []pHvDevIF;
		}
		else{//没有设备的场景
			double theElapsedTime = t.Stop(true);			
			m_strTimeElapse.Format("%f",theElapsedTime);
			GetLocalTime(&u);
			m_strTimeStop.Format("%d.%d.%02d-%02d:%02d:%02d-%04d",u.wYear,u.wMonth,u.wDay,u.wHour,u.wMinute,u.wSecond,u.wMilliseconds);
			 
			UpdateData(false);
			
		}
		FreeLibrary(hDll);
	}
	else{		
		AfxMessageBox(m_strDllName + ".dll 加载失败");
	}
}

BOOL CTabPage2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CListCtrl &lc = m_ctrlDeviceList;
	
	lc.SetExtendedStyle(LVS_EX_FLATSB|
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|
		LVS_EX_HEADERDRAGDROP|LVS_EX_ONECLICKACTIVATE);
	
	lc.InsertColumn(0,"设备编号",LVCFMT_LEFT,80,0);
	lc.InsertColumn(1,"GUID",LVCFMT_LEFT,280,1);
	lc.InsertColumn(2,"设备路径",LVCFMT_LEFT,800,1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabPage2::v_UpdateUI(bool b)
{
	UpdateData(b);
}

void CTabPage2::OnClickListEnumDevice(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	CListCtrl &lc = m_ctrlDeviceList;
	int pos = lc.GetSelectionMark();
	TRACE("pos=%d",pos);
	if(pos==-1) return;

	m_strGUID = lc.GetItemText(pos,1);
	UpdateData(false);
	
	*pResult = 0;
}
