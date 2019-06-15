// mainfrm.cpp : implementation of the CMainFrame class


#include "stdafx.h"
#include "bounce.h"

#include "mdi.h"

#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

char* GetDeviceType(HVTYPE nType)
{
	switch(nType) {
	case UNKNOWN_TYPE:
		return "UnKnownType";
	case HV1300UCTYPE:
		return "HV1300UC";
	case HV2000UCTYPE:
		return "HV2000UC";
	case HV1301UCTYPE:
		return "HV1301UC";
	case HV2001UCTYPE:
		return "HV2001UC";
	case HV3000UCTYPE:
		return "HV3000UC";
	case HV1300UMTYPE:
		return "HV1300UM";
	case HV1302UCTYPE:
		return "HV1302UC";
	case HV2002UCTYPE:
		return "HV2002UC";
	case HV3102UCTYPE:
		return "HV3102UC";
	case HV1302UMTYPE:
		return "HV1302UM";
	case HV1300FCTYPE:
		return "HV1300FC";
	case HV2000FCTYPE:
		return "HV2000FC";
	case HV3100FCTYPE:
		return "HV3100FC";
	case HV1300FMTYPE:
		return "HV1300FM";
	case HV1303UCTYPE:
		return "HV1303UC";
	case HV2003UCTYPE:
		return "HV2003UC";
	case HV3103UCTYPE:
		return "HV3103UC";
	case HV1303UMTYPE:
		return "HV1303UM";
	case SV1300FMTYPE:
		return "SV1300FM";
	case SV1300FCTYPE:
		return "SV1300FC";
	case SV1310FCTYPE:
		return "SV1310FC";
	case SV1310FMTYPE:
		return "SV1310FM";
	case SV1311FCTYPE:
		return "SV1311FC";
	case SV1311FMTYPE:
		return "SV1311FM";
	case SV400FCTYPE:
		return "SV400FC";
	case SV400FMTYPE:
		return "SV400FM";
	case DH1394FXTYPE:
		return "DH1394FX";
	case SV1410FCTYPE:
		return "SV1410FC";
	case SV1410FMTYPE:
		return "SV1410FC";
	case SV1420FCTYPE:
		return "SV1420FC";
	case SV1420FMTYPE:
		return "SV1420FM";
	case SV2000FCTYPE:
		return "SV2000FC";
	case SV2000FMTYPE:
		return "SV2000FM";
	case SV1400FCTYPE:
		return "SV1400FC";
	case SV1400FMTYPE:
		return "SV1400FM";
	case HV1350UCTYPE:
		return "HV1350UC";
	case HV2050UCTYPE:
		return "HV2050UC";
	case HV3150UCTYPE:
		return "HV3150UC";
	case HV1350UMTYPE:
		return "HV1350UM";
	case HV1360UCTYPE:
		return "HV1360UC";
	case HV2060UCTYPE:
		return "HV2060UC";
	case HV3160UCTYPE:
		return "HV3160UC";
	case HV1360UMTYPE:
		return "HV1360UM";
	case SV1410GCTYPE:
		return "SV1410GC";
	case SV2000GCTYPE:
		return "SV2000GC";
	case RESERVED_001:
		return "ALDSV1410FC";
	case PD1420EMTYPE:
		return "PD1420EM";
	case PD1420ECTYPE:
		return "PD1420EC";
	default:
		return "TypeError";
		
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_TYPE_BAR, OnViewTypeBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TYPE_BAR, OnUpdateViewTypeBar)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BUTTON_ENUMERATE_DEVICES, OnButtonEnumerateDevices)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	for (int i=0;i<CameraNumber;i++)
	{
		m_pDeviceWnd[i]=NULL;
		m_MIDhti[i]=NULL;
	}
	phead = NULL;
}

CMainFrame::~CMainFrame()
{
	TRACE("===CMainFrame::~CMainFrame===");
	f_DeleteList(phead);
	
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame commands

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

    if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	///English:    ///Chinese: 状态栏
	m_wndStatusBar.SetPaneInfo(0,ID_OUTPUTWIN_PARAM,SBPS_NORMAL,300);
	m_wndStatusBar.SetPaneInfo(1,ID_OUTPUTWIN_PARAM,SBPS_NORMAL,200);	
	m_wndStatusBar.SetPaneInfo(2,ID_AOI_INFO,SBPS_NORMAL,200);
	m_wndStatusBar.SetPaneInfo(3,ID_MOUSE_POS,SBPS_NORMAL,200);

	//////////////English:    ///Chinese: 创建控制条///////////////
	
	if (!m_TypeToolBar.Create(_T("Be controlled Camera"), this,CSize(200,100),TRUE,124))
	{
		TRACE0("Failed to create mybar\n");
		return -1;
	}

    
   ////////////////English:    ///Chinese: 停靠控制///////////////////
	
	m_TypeToolBar.SetBarStyle(m_TypeToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);//
	
	m_TypeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	RecalcLayout();
	CRect rect;
	
	rect.OffsetRect(0, 1);
	DockControlBar(&m_TypeToolBar, AFX_IDW_DOCKBAR_LEFT,rect);///English:    ///Chinese: 停靠在左边


    ///English:    ///Chinese: 创建树型控件并放入左边摄像机类型控制条窗口中 
    if (!m_wndTree.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|
		TVS_SHOWSELALWAYS,
		CRect(0, 0, 0, 0), &m_TypeToolBar, 100))
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;
	}
	m_wndTree.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
 
     
	m_strTitle = this->GetTitle();
//	v_Xd_Enumerate();
 
     
	 
	DWORD cb;
	GUID *pDH_CameraGUIDs = NULL;
	int nCamNum;
	HVSTATUS status = HVGetDeviceGUID(NULL,0,&cb);
	if (HV_SUCCESS(status)) {
		nCamNum = cb/sizeof(GUID);
		pDH_CameraGUIDs = new GUID[nCamNum];
		status = HVGetDeviceGUID(pDH_CameraGUIDs,cb,&cb);
	}
	
	for(int i = 0; i < nCamNum; i++)
	{
		RegPnpGUID(m_hWnd, pDH_CameraGUIDs[i]);///English:    ///Chinese: 注册GUID,实现接收插拔消息
	}

	if (pDH_CameraGUIDs != NULL) 
	{
		delete[]pDH_CameraGUIDs;
	}
	
	return 0;
}

void CMainFrame::EnumDevice(int nNumber)
{
	HVTYPE type;
	int nIndex = 0;
	::HVGetTypeFromIndex(nNumber, &type, &nIndex);
	CString strName;
	
	switch(type) {
	case SV1310FCTYPE:
		strName.Format( "%d %s ",nNumber,"SV1310FC");
		m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1310FMTYPE:
		strName.Format( "%d %s ",nNumber,"SV1310FM");
		m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1300FCTYPE:
		strName.Format( "%d %s ",nNumber,"SV1300FC");
		m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1300FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV1300FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1311FCTYPE:
        strName.Format( "%d %s ",nNumber,"SV1311FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1311FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV1311FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1400FCTYPE:
        strName.Format( "%d %s ",nNumber,"SV1400FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1400FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV1400FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1410FCTYPE:
        strName.Format( "%d %s ",nNumber,"SV1410FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1410FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV1410FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1420FCTYPE:
        strName.Format( "%d %s ",nNumber,"SV1420FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV1420FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV1420FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV2000FCTYPE:
        strName.Format( "%d %s ",nNumber,"SV2000FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV2000FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV2000FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV400FCTYPE:
        strName.Format( "%d %s ",nNumber,"SV400FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case SV400FMTYPE:
        strName.Format( "%d %s ",nNumber,"SV400FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
    case HV1300FCTYPE:
		strName.Format( "%d %s ",nNumber,"HV1300FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case HV1300FMTYPE:
		strName.Format( "%d %s ",nNumber,"HV1300FM");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case HV2000FCTYPE:
		strName.Format( "%d %s ",nNumber,"HV2000FC");
         m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case HV3100FCTYPE:
		strName.Format( "%d %s ",nNumber,"HV3100FC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case HV1302UCTYPE:
		strName.Format( "%d %s ",nNumber,"HV1302UC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	case PD1420ECTYPE:
		strName.Format( "%d %s ",nNumber,"PD1420EC");
        m_MIDhti[nNumber-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		break;
	default:
		break;
	}

}

void CMainFrame::OnViewTypeBar() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_TypeToolBar, !m_TypeToolBar.IsVisible(), FALSE);

}

void CMainFrame::OnUpdateViewTypeBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_TypeToolBar.IsVisible());
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("============CMainFrame::OnClose==========");
    
  	for (int i=1;i <= m_nDevTotal;i++)
  	{
		DeviceNode *pNode = NULL;
		if(pNode = f_GetDeviceNodeFromIndex(phead,i))
		{
			if (pNode->pDeviceWnd)
			{
				if (pNode->pDeviceWnd->m_bStart)
				   pNode->pDeviceWnd->Close();
				pNode->pDeviceWnd = NULL;
			}
		}

  	}
    
	CMDIFrameWnd::OnClose();
}
BOOL CMainFrame::OnDeviceChange(UINT wParam, DWORD lParam)
{
	//TRACE("HVMViewer::  wParam=%x,lParam=%x\n",wParam,lParam);
	switch(wParam) 
	{
	case DBT_DEVICEARRIVAL:     
		{
			TRACE("DBT_DEVICEARRIVAL");
			DEV_BROADCAST_DEVICEINTERFACE_A* devHdr;
			devHdr = (DEV_BROADCAST_DEVICEINTERFACE_A*)lParam;
			
			switch ( devHdr->dbcc_devicetype )
			{
			case DBT_DEVTYP_DEVICEINTERFACE :  
				TRACE("CameraInsert");
				TRACE("HVMViewer::DevicePath:%s",devHdr->dbcc_name);
				{					
					m_nDevTotal = 0;
					::HVGetDeviceTotal(&m_nDevTotal);
					for(int i = 1; i <= m_nDevTotal; i++)
					{
						char szDevicePath[128];
						HVTYPE type = UNKNOWN_TYPE;
						ZeroMemory(szDevicePath,sizeof(szDevicePath));
						f_GetDeviceInfo(i,&type,szDevicePath);
						
						DeviceNode *pNode = NULL;
						if (pNode = f_GetDeviceNodeFromPath(phead,szDevicePath))
						{
							pNode->nbegin = i;
						}
						else{//...........
							char InSertDevPath[128];
							char InSertDevSN[20];
							ZeroMemory(InSertDevSN,sizeof(InSertDevSN));
							sprintf(InSertDevPath,"%s",f_MakeLower(devHdr->dbcc_name));
							DeviceNode *pNodeInSert = f_CreateList(phead,InSertDevPath,InSertDevSN,UNKNOWN_TYPE,-1);
							
							f_GetDeviceSN(i,InSertDevSN);
							pNodeInSert->nbegin = i;
							sprintf(pNodeInSert->szDeviceName,"%s",GetDeviceType(type));
							sprintf(pNodeInSert->szDevicePath,"%s",f_MakeLower(szDevicePath));
							sprintf(pNodeInSert->szDeviceSN,"%s",InSertDevSN);
							f_RefreshTree_Insert(pNodeInSert);
						}
					}
									
				}
				break;
			default:
				break;
			}
		}
		break;
		
	case DBT_DEVICEREMOVECOMPLETE:      
		{
			TRACE("DBT_DEVICEREMOVECOMPLETE");
			DEV_BROADCAST_DEVICEINTERFACE_A* devHdr;
			devHdr = (DEV_BROADCAST_DEVICEINTERFACE_A*)lParam;
			
			switch ( devHdr->dbcc_devicetype )
			{
			case DBT_DEVTYP_DEVICEINTERFACE :  
				TRACE("CameraRemove");
                TRACE("HVMViewer::DevicePath:%s",devHdr->dbcc_name);
				{
					char RemoveDevPath[128];
					sprintf(RemoveDevPath,"%s",f_MakeLower(devHdr->dbcc_name));
					m_nDevTotal = 0;
					::HVGetDeviceTotal(&m_nDevTotal);
					
					if(!f_RemoveDeviceNode(phead,RemoveDevPath))
					{   
						f_RemoveDeviceNode(phead,f_GetRemoveDevPath());
					}

					for(int i = 1; i <= m_nDevTotal; i++)
					{
						char szDevicePath[128];
						HVTYPE type = UNKNOWN_TYPE;
						ZeroMemory(szDevicePath,sizeof(szDevicePath));
						f_GetDeviceInfo(i,&type,szDevicePath);
						
						DeviceNode *pNode = NULL;
						if (pNode = f_GetDeviceNodeFromPath(phead,szDevicePath))
						{
							pNode->nbegin = i;
						}
					}
					f_RefreshTree();
				}
				
			default:
				break;
			}
		}
		break;
		
	default:
		break;
	}
	
	return TRUE;
} 

BOOL CMainFrame::RegPnpGUID(HANDLE h, GUID guid)
{
	BOOL bReturn=FALSE;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = guid; 
	HDEVNOTIFY hDevInterfaceNotify = ::RegisterDeviceNotification( h, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE) ;
	if ( hDevInterfaceNotify ) 
		bReturn=TRUE;	
	return bReturn;

}

void CMainFrame::f_GetDeviceInfo(int nDevice, HVTYPE *pType, char *pDevicePath)
{
	if (pType == NULL|| pDevicePath == NULL)  return;
	::HVGetTypeFromIndex(nDevice, pType, NULL);
	HV_DEVICE_INTERFACE *pDevInterface = new(HV_DEVICE_INTERFACE);
	HVSTATUS status = ::HVGetDevInterfaceFromIndex(nDevice,pDevInterface);
	if (status==STATUS_OK) {
		sprintf(pDevicePath,"%s",pDevInterface->szDevicePath);
	}	
	delete pDevInterface;
}

DeviceNode* CMainFrame::f_CreateList(DeviceNode *&h, char *szDevicePath,char *szDeviceSN, HVTYPE type,int nNumber)
{
	DeviceNode *p = h;
	DeviceNode *s = new DeviceNode;
	s->nbegin = nNumber;
	sprintf(s->szDevicePath,"%s",f_MakeLower(szDevicePath));
	sprintf(s->szDeviceName,"%s",GetDeviceType(type));
	sprintf(s->szDeviceSN,"%s",szDeviceSN);
	
	s->pNext = NULL;
    s->pDeviceWnd = NULL;	

	if(!p)
	{
		h = s;
		return s;
	}

	for(; p->pNext; p = p->pNext);
	p->pNext = s;

	return s;

}

void CMainFrame::f_DeleteList(DeviceNode *head)
{
	DeviceNode *Delem, *next;
	Delem = head;
	while(Delem){
		next = Delem->pNext;
		delete[]Delem;         ///English:    ///Chinese: 内部指针的释放?
		Delem = next;     
	}
}

DeviceNode* CMainFrame::f_GetDeviceNodeFromIndex(DeviceNode *head, int nIndex)
{
	DeviceNode *pNode;
	pNode = head;
	int dex = 0;
	while (pNode){
		dex++;
		if (dex == nIndex)
			return pNode;
		pNode = pNode->pNext;
	}
    return NULL;
}

DeviceNode* CMainFrame::f_GetDeviceNodeFromPath(DeviceNode *head, char *szDevicePath)
{
	DeviceNode *pNode;
	pNode = head;
	while (pNode)
	{
		if (strcmp(pNode->szDevicePath,f_MakeLower(szDevicePath)) == 0)
			return  pNode;
		pNode = pNode->pNext;
	}
    return NULL;
}

void CMainFrame::f_RefreshTree()
{
	m_wndTree.DeleteAllItems();
	m_hti = m_wndTree.InsertItem(_T("DH CameraType"));
	
	for (int dex = 1;dex <= m_nDevTotal;dex++)
	{
		CString strName = "UnKnown";
		DeviceNode *pNode = NULL;
		if(pNode = f_GetDeviceNodeFromIndex(phead,dex))
		{
		    strName.Format( "%d %s-%s",dex,pNode->szDeviceName,pNode->szDeviceSN);     
			m_MIDhti[dex-1]=m_wndTree.InsertItem(_T(strName),m_hti);
		}
	}
	m_wndTree.Expand(m_hti,TVE_EXPAND);
}

char * CMainFrame::f_MakeLower(char *str)
{
	int  len = strlen(str);   
	for(int i=0;i<len;i++)   
	{   
		if(str[i]>='A' && str[i]<='Z')   
		{   
			str[i]=str[i]-'A'+'a';			
		}   
	}   
	
	return  str;  
}

BOOL CMainFrame::f_RemoveDeviceNode(DeviceNode *&h, char *szDevicePath)
{
	DeviceNode *pNode = h;
	
	if(!pNode) return 0;
	
	if(strcmp(pNode->szDevicePath,f_MakeLower(szDevicePath)) == 0 )
	{
		h = pNode->pNext;
		delete pNode;
		return 1;
	}
	
	for(;pNode->pNext;pNode=pNode->pNext)
	{
		if(strcmp(pNode->pNext->szDevicePath,f_MakeLower(szDevicePath)) == 0)
		{
			DeviceNode *temp=pNode->pNext;
			pNode->pNext=pNode->pNext->pNext;
			pNode=pNode->pNext;

			if (temp->pDeviceWnd)
			{
	//			delete[]temp->pDeviceWnd;///English:    ///Chinese: 关闭子窗口 ???
				temp->pDeviceWnd = NULL;
			}
			delete temp;						
			return 1;
		}
	}
		
	return 0;
}

char * CMainFrame::f_GetRemoveDevPath()
{
	DeviceNode *pNode =NULL;
	int ndex = 1;
	
	while(pNode = f_GetDeviceNodeFromIndex(phead,ndex))
	{
		BOOL b = FALSE;
		
		for(int i = 1; i <= m_nDevTotal; i++)
		{
			char szDevicePath[128];
			HVTYPE type = UNKNOWN_TYPE;
			ZeroMemory(szDevicePath,sizeof(szDevicePath));
			f_GetDeviceInfo(i,&type,szDevicePath);
			
			if (strcmp(pNode->szDevicePath,f_MakeLower(szDevicePath)) == 0)
			{
				b = TRUE;
				break;
			}
		}
		
		if (b == FALSE)
			return pNode->szDevicePath;
		ndex++;
	}
 
	return "No Found Remove Device";

}

void CMainFrame::f_GetDeviceSN(int i, char*DeviceSN)
{
	HHV m_localhhv;
	int nBuffSize = 0;
	BYTE *pbContext=NULL;
	DWORD *pdContext=NULL;	
	BeginHVDevice(i,&m_localhhv);
	HVGetDeviceInfo(m_localhhv,DESC_DEVICE_SERIESNUM,NULL,&nBuffSize);
	pbContext = new BYTE[nBuffSize];
	pdContext = (DWORD *)pbContext;
	HVGetDeviceInfo(m_localhhv,DESC_DEVICE_SERIESNUM,pdContext,&nBuffSize);	
	sprintf(DeviceSN,"%.14s",pdContext);
	EndHVDevice(m_localhhv);
	delete pbContext;
}

void CMainFrame::f_RefreshTree_Insert(DeviceNode *pInsertNode)
{
	CString strName = "UnKnown";
	strName.Format( "%d %s-%s",m_nDevTotal,pInsertNode->szDeviceName,pInsertNode->szDeviceSN);     
	m_MIDhti[m_nDevTotal-1]=m_wndTree.InsertItem(_T(strName),m_hti);
	m_wndTree.Expand(m_hti,TVE_EXPAND);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return TRUE;
	return CMDIFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::v_Xd_Enumerate()
{
	f_DeleteList(phead);
	phead = NULL;

	m_nDevTotal = 0;
	::HVGetDeviceTotal(&m_nDevTotal);          ///English:    ///Chinese: 获取当前系统中摄像机的总数
	
	for (int i = 1;i <= m_nDevTotal;i++)
	{
		char szDevicePath[128];
		char szDeviceSN[20];
		HVTYPE type = UNKNOWN_TYPE;
		ZeroMemory(szDevicePath,sizeof(szDevicePath));
		ZeroMemory(szDeviceSN,sizeof(szDeviceSN));
        f_GetDeviceInfo(i,&type,szDevicePath);
		f_GetDeviceSN(i,szDeviceSN);
		f_CreateList(phead,szDevicePath,szDeviceSN,type,i);	
	}
	
	f_RefreshTree();

}

void CMainFrame::OnButtonEnumerateDevices() 
{
	// TODO: Add your command handler code here
	
	CStopWatch t;    
	t.Start();	
	v_Xd_Enumerate();
	double theElapsedTime = t.Stop(true);	
	CString strTimeElapse;
	strTimeElapse.Format("%f",theElapsedTime);
	
	CString strTitle = m_strTitle + " " + strTimeElapse;

	this->SetWindowText(strTitle);

}
