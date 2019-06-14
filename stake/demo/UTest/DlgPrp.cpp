// DlgPrp.cpp : implementation file
//

#include "stdafx.h"
#include "mdi.h"
#include "DlgPrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "math.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPrp dialog

CDlgPrp::CDlgPrp(HHV hhv,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_hhvCam=hhv;
	m_bounceWnd =(CBounceWnd*)pParent;
	m_nShutterUnit=1000;
	
    m_bFreeze=FALSE;
    m_bBlackEnable=FALSE;

	for(int j = 0; j<100; j ++)
		m_pAllCtrl[j] = NULL;
}

CDlgPrp::~CDlgPrp()
{
	TRACE("CDlgPrp::~CDlgPrp() ");
	
	
	for (int i=0;i<100;i++)
    {
		if(NULL!=m_pAllCtrl[i]){

			if(m_pAllCtrl[i]->nType==1&&m_pAllCtrl[i]->pCtrl!=NULL){
				CSliderCtrl *pSlider = (CSliderCtrl *)m_pAllCtrl[i]->pCtrl;
				pSlider->DestroyWindow();
				
				delete pSlider;
				pSlider=NULL;
			}
			else if (m_pAllCtrl[i]->nType==2&&m_pAllCtrl[i]->pCtrl!=NULL)
			{
				CStatic *pStatic = (CStatic *)m_pAllCtrl[i]->pCtrl;
				pStatic->DestroyWindow();
				delete pStatic;
				pStatic=NULL;
			}
			else if (m_pAllCtrl[i]->nType==3&&m_pAllCtrl[i]->pCtrl!=NULL)
			{
				CEdit *pEdit = (CEdit *)m_pAllCtrl[i]->pCtrl;
				pEdit->DestroyWindow();
				delete pEdit;
				pEdit=NULL;
			}
			else if (m_pAllCtrl[i]->nType==4&&m_pAllCtrl[i]->pCtrl!=NULL)
			{
				CButton *pBut = (CButton *)m_pAllCtrl[i]->pCtrl;
				pBut->DestroyWindow();
				delete pBut;
				pBut=NULL;
			}
			else if (m_pAllCtrl[i]->nType==5&&m_pAllCtrl[i]->pCtrl!=NULL)
			{
				CButton *pBut = (CButton *)m_pAllCtrl[i]->pCtrl;
				pBut->DestroyWindow();
				delete pBut;
				pBut=NULL;
			}
			else if (m_pAllCtrl[i]->nType==6&&m_pAllCtrl[i]->pCtrl!=NULL)
			{
				CComboBox *pCombox = (CComboBox *)m_pAllCtrl[i]->pCtrl;
				pCombox->DestroyWindow();
				delete pCombox;
				pCombox=NULL;
			}
			else if (m_pAllCtrl[i]->nType==7&&m_pAllCtrl[i]->pCtrl!=NULL)
			{
				CButton *pBut = (CButton *)m_pAllCtrl[i]->pCtrl;
				pBut->DestroyWindow();
				delete pBut;
				pBut=NULL;
			}
			
			
			delete m_pAllCtrl[i];
			m_pAllCtrl[i] = NULL;
		}
    }
	
}


void CDlgPrp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrp, CDialog)
	//{{AFX_MSG_MAP(CDlgPrp)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrp message handlers

int CDlgPrp::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	if (m_bounceWnd)
	{
	  CString strTitle;
//	  strTitle.Format("%s%d",m_bounceWnd->m_strCamerName,m_bounceWnd->m_nListNum);
	  strTitle.Format("%s",m_bounceWnd->m_strCamerDispName);
	  SetWindowText(strTitle); ///English:    ///Chinese: 属性页的标题栏
	}
	
	CreateAllCtrl();          ///English:    ///Chinese: 创建并初始化属性页中的控件
    
	
	return 0;
}

BOOL CDlgPrp::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

void CDlgPrp::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

LRESULT CDlgPrp::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	XDefWindowProc(message,wParam,lParam);///English:    ///Chinese: 响应对摄像机属性页中控件的操作
	
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}


void CDlgPrp::CreateAllCtrl()
{

	char chName[100];
    sprintf(chName,"%s.txt",m_bounceWnd->m_strCamerName);
	
	FILE *pRead =fopen(chName,"r");

	if(pRead!=NULL)
	{
		fscanf(pRead,"%d",&m_nCtl);
		
		for(int i = 0; i<m_nCtl ; i++){
			if (m_pAllCtrl[i]==NULL)
			{
				m_pAllCtrl[i]=new CAllCtrl;
			}
			
			fscanf(pRead,"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
				&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
				&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
		}
		fclose(pRead);
	
	}
	else
	{
		if (m_bounceWnd->m_strCamerName=="SV1311FC")
		{
			m_nCtl=Ctrl_NUMBER_SV1311FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1311FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				
			}
		}
        else if (m_bounceWnd->m_strCamerName=="SV1311FM")
		{
			m_nCtl=Ctrl_NUMBER_SV1311FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1311FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1310FC")
		{
			m_nCtl=Ctrl_NUMBER_SV1310FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1310FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1310FM")
		{
			m_nCtl=Ctrl_NUMBER_SV1310FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1310FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1300FC")
		{
			m_nCtl=Ctrl_NUMBER_SV1300FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1300FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1300FM")
		{
			m_nCtl=Ctrl_NUMBER_SV1300FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1300FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="HV2000FC")
		{
			m_nCtl=Ctrl_NUMBER_HV2000FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_HV2000FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="HV3100FC")
		{
			m_nCtl=Ctrl_NUMBER_HV3100FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_HV3100FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="HV1300FC")
		{
			m_nCtl=Ctrl_NUMBER_HV1300FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_HV1300FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="HV1300FM")
		{
			m_nCtl=Ctrl_NUMBER_HV1300FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_HV1300FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1410FC")
		{
			m_nCtl=Ctrl_NUMBER_SV1410FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1410FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1400FC")
		{
			m_nCtl=Ctrl_NUMBER_SV1400FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1400FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1400FM")
		{
			m_nCtl=Ctrl_NUMBER_SV1400FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1400FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV2000FC")
		{
			m_nCtl=Ctrl_NUMBER_SV2000FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV2000FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV2000FM")
		{
			m_nCtl=Ctrl_NUMBER_SV2000FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV2000FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1420FC")
		{
			m_nCtl=Ctrl_NUMBER_SV1420FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1420FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1420FM")
		{
			m_nCtl=Ctrl_NUMBER_SV1420FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1420FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV1410FM")
		{
			m_nCtl=Ctrl_NUMBER_SV1410FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV1410FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV400FC")
		{
			m_nCtl=Ctrl_NUMBER_SV400FC;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV400FC[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
				}
			
		}
		else if (m_bounceWnd->m_strCamerName=="SV400FM")
		{
			m_nCtl=Ctrl_NUMBER_SV400FM;
			for(int i = 0; i<m_nCtl ; i++){
				if (m_pAllCtrl[i]==NULL)
				{
					m_pAllCtrl[i]=new CAllCtrl;
				}
				
				sscanf(g_strPreCtrl_SV400FM[i],"%d [%d,%d,%d,%d] %d %s %s ",&m_pAllCtrl[i]->nType,&m_pAllCtrl[i]->x,
					&m_pAllCtrl[i]->y,&m_pAllCtrl[i]->w,&m_pAllCtrl[i]->h,
					&m_pAllCtrl[i]->nID,&m_pAllCtrl[i]->ch,&m_pAllCtrl[i]->chX);
			}
		}


	}
	
	
    
    for(int j = 0; j<m_nCtl; j++){
		
		m_pAllCtrl[j]->CreateMe(this);
		
	}

}

void CDlgPrp::XDefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int k;
	switch(message){

	case WM_HSCROLL:
		
		TRACE("wParam = 0x%x(%d)  lParam = 0x%x(%d)",wParam,lParam,wParam,lParam);
	    switch(wParam){
		case ID_MYSLIDER:
			TRACE(" __  ID_MYSLIDER ___ \n"); 
			break;

		}
		break;
	case WM_COMMAND:
	
		for (k=0;k<100;k++)
		{
			if (m_pAllCtrl[k]!=NULL)
			{
				if(m_pAllCtrl[k]->nID==LOWORD(wParam)&&m_pAllCtrl[k]->nType==3)///English:    ///Chinese: edit控件
				{
					///English:    ///Chinese: 此处代码需要完善
					CString str;
					GetDlgItem(m_pAllCtrl[k]->nID)->GetWindowText(str);
					int v;
					sscanf(str.GetBuffer(str.GetLength()),"%d",&v);
					TRACE(str);
					//m_pAllCtrl[k]->GetMV(minV,maxV);
					if(v>=0&&v<1024){
					//UpdateFriend(m_pAllCtrl[k],v);
					}
					
				}
				else if (m_pAllCtrl[k]->nID==LOWORD(wParam)&&m_pAllCtrl[k]->nType==6)///English:    ///Chinese: ComboBox控件
				{
					if( HIWORD(wParam) == CBN_SELCHANGE)
					{
						long lTemp=::SendMessage( (HWND)lParam, CB_GETCURSEL,WPARAM(0),LPARAM(0)); 
						if (strcmp(m_pAllCtrl[k]->ch, "SnapMode") == 0)
						{
							TRACE("SnapMode %d",lTemp);
							HVSetSnapMode(m_hhvCam,(HV_SNAP_MODE)lTemp);
							if (lTemp==1) {
								m_bounceWnd->m_bTrigger=TRUE;
							}
							UpdateFriend("SoftTrigger",lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "TestImage") == 0)
						{
							TRACE("TestImage  %d",lTemp);
							Spec_Fun_Interface(m_hhvCam,TEST_IMAGE,lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "ShutterUnit") == 0)
						{
							TRACE("ShutterUnit %d",lTemp);
							    ///English:    ///Chinese: 区别CCD、CMOS摄像机
							if (m_bounceWnd->m_strCamerName=="HV2000FC")
							{
								TRACE("%s",m_bounceWnd->m_strCamerName);
								m_nShutterUnit=pow(1000, 2 - lTemp);
								///English:    ///Chinese: 最好能从快门速度滑动条上取
								SetExposureTime_HV2000FC(m_hhvCam,m_bounceWnd->m_nWidth,m_nShutterUnit,m_nShutterSpeed);
								
							}
							else if (m_bounceWnd->m_strCamerName=="HV3100FC")
							{
								TRACE("%s",m_bounceWnd->m_strCamerName);
								m_nShutterUnit=pow(1000, 2 - lTemp);
								SetExposureTime_HV3100FC(m_hhvCam,m_bounceWnd->m_nWidth,m_nShutterUnit,m_nShutterSpeed);
								
							}
							else if (m_bounceWnd->m_strCamerName=="HV1300FC"||\
								m_bounceWnd->m_strCamerName=="HV1300FM")
							{
								TRACE("%s",m_bounceWnd->m_strCamerName);
								m_nShutterUnit=pow(1000, 2 - lTemp);
								SetExposureTime_HV1300(m_hhvCam,m_bounceWnd->m_nWidth,m_nShutterUnit,m_nShutterSpeed);
							}
							else//CCD摄像机
								HVAECControl(m_hhvCam, AEC_SHUTTER_UNIT,lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "ExposureDelay") == 0)
						{
							TRACE("ExposureDelay %d",lTemp);
							Spec_Fun_Interface(m_hhvCam,TRIGGER_DELAY_ON_OFF,lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "StrobeOnOff") == 0)
						{
							TRACE("StrobeOnOff %d",lTemp);
							Spec_Fun_Interface(m_hhvCam,STROBE_ON_OFF,lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "StrobePolarity") == 0)
						{
							TRACE("StrobePolarity %d",lTemp);
							HVSetStrobePolarity(m_hhvCam, (HV_POLARITY)lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "ADCLevel") == 0)
						{
							TRACE("ADCLevel %d",lTemp);
							HVADCControl(m_hhvCam,ADC_BITS,(HV_ADC_LEVEL)lTemp);
						}
						else if (strcmp(m_pAllCtrl[k]->ch, "VideoMode") == 0)
						{
							TRACE("VideoMode %d",lTemp);
							switch(lTemp) {
							case 0:
								m_bounceWnd->SetCMOSVideoMode0();
								m_bounceWnd->m_Resolution=(HV_RESOLUTION)0;
								break;
							case 1:
								m_bounceWnd->SetCMOSVideoMode1();
								m_bounceWnd->m_Resolution=(HV_RESOLUTION)1;
								break;
							case 2:
								m_bounceWnd->SetCMOSVideoMode2();
								m_bounceWnd->m_Resolution=(HV_RESOLUTION)2;
								break;
								
							default:
								break;
							}
						}
						
					}
					
				}
				else if (m_pAllCtrl[k]->nID==LOWORD(wParam)&&m_pAllCtrl[k]->nType==4)//Button控件
				{
					if (strcmp(m_pAllCtrl[k]->ch, "SoftTrigger") == 0)
					{
						TRACE("HVTriggerShot");
						HVTriggerShot(m_hhvCam);
					}
					if (strcmp(m_pAllCtrl[k]->ch, "LUT") == 0)
					{
						TRACE("LoadLut");
						LoadLut();///English:    ///Chinese: 查找表文件
					}
					

				}
				else if (m_pAllCtrl[k]->nID==LOWORD(wParam)&&m_pAllCtrl[k]->nType==5)///English:    ///Chinese: CheckButton控件
				{
					if (strcmp(m_pAllCtrl[k]->ch, "FrameFreeze") == 0)
					{
						if (m_bFreeze==0)
						{
							TRACE("CMD_FRAME_SHOT");
							HVCommand(m_hhvCam, CMD_FRAME_SHOT,0,0);
							m_bFreeze=1;
						}
						else if (m_bFreeze==1)
						{
							TRACE("CMD_FRAME_SHOT_FINISH");
							HVCommand(m_hhvCam,CMD_FRAME_SHOT_FINISH,0,0);
							m_bFreeze=0;
						}
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "BlackLevelEnable") == 0)
					{
						if (m_bBlackEnable==0)
						{
							TRACE("BlackEnable 0");
							HVADCControl(m_hhvCam,ADC_BLKLEVEL_CAL,0);
							m_bBlackEnable=1;
						}
						else if (m_bBlackEnable==1)
						{
							TRACE("BlackEnable 1");
							HVADCControl(m_hhvCam,ADC_BLKLEVEL_CAL,1);
							m_bBlackEnable=0;
						}
						UpdateFriend("BlackLevel",!m_bBlackEnable);
					}
					

				}
			}
		}
		

		break;
	case WM_MOUSEACTIVATE:
	
		break;
	}


}

void CDlgPrp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
    XOnHScroll(nSBCode,nPos,pScrollBar);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgPrp::XOnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int k;
	for (k=0;k<100;k++)
		{
			if (m_pAllCtrl[k]!=NULL)
			{
				int nid=m_pAllCtrl[k]->nID;
				CSliderCtrl *pSlider=(CSliderCtrl*)GetDlgItem(nid);
				if (pSlider==(CSliderCtrl *)pScrollBar)
				{
					int nPos=pSlider->GetPos(); 
					TRACE("============================================%d",k);
					if (strcmp(m_pAllCtrl[k]->ch, "GainAdjust") == 0)
					{
						TRACE("HVCommand(SetGain,%d)",nPos);
						HVAGCControl(m_hhvCam,AGC_GAIN,nPos);
						UpdateFriend(m_pAllCtrl[k],nPos);

					}
					else if (strcmp(m_pAllCtrl[k]->ch, "RedChannel") == 0)
					{
						TRACE("HVCommand(SetRedGain,%d)",nPos);
						HVAGCControl(m_hhvCam,AGC_GAIN_RED_CHANNEL,nPos);
						UpdateFriend(m_pAllCtrl[k],nPos);
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "BlueChannel") == 0)
					{
						TRACE("HVCommand(SetBlueGain,%d)",nPos);
						HVAGCControl(m_hhvCam,AGC_GAIN_BLUE_CHANNEL,nPos);
						UpdateFriend(m_pAllCtrl[k],nPos);
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "Shutter") == 0)
					{
						if (m_bounceWnd->m_strCamerName=="HV2000FC")
						{
						  TRACE("%s %d",m_bounceWnd->m_strCamerName,nPos);
						  SetExposureTime_HV2000FC(m_hhvCam,m_bounceWnd->m_nWidth,m_nShutterUnit,nPos);
						  m_nShutterSpeed=nPos;///English:    ///Chinese: 纪录快门速度滑动条上的当前值
							
						}
						else if (m_bounceWnd->m_strCamerName=="HV3100FC")
						{
						  TRACE("%s %d",m_bounceWnd->m_strCamerName,nPos);
						  SetExposureTime_HV3100FC(m_hhvCam,m_bounceWnd->m_nWidth,m_nShutterUnit,nPos);
						  m_nShutterSpeed=nPos;
						  
						}
						else if (m_bounceWnd->m_strCamerName=="HV1300FC"||\
							     m_bounceWnd->m_strCamerName=="HV1300FM")
						{
						  TRACE("%s %d",m_bounceWnd->m_strCamerName,nPos);
						  SetExposureTime_HV1300(m_hhvCam,m_bounceWnd->m_nWidth,m_nShutterUnit,nPos);
						  m_nShutterSpeed=nPos;
						  
						}
						else ///English:    ///Chinese: CCD摄像机
						{
							TRACE("HVAECControl(SetShutterSpeed)",nPos);
							HVAECControl(m_hhvCam, AEC_SHUTTER_SPEED,nPos);
							
						}
						UpdateFriend(m_pAllCtrl[k],nPos);
						
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "Brightness") == 0)
					{
						TRACE("HVADCControl(SetBrightness)",nPos);
						HVADCControl(m_hhvCam,ADC_BRIGHTNESS,nPos);
						UpdateFriend(m_pAllCtrl[k],nPos);
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "DelayTime") == 0)
					{
						TRACE("HVCommand(SetTriggerDelayValue)",nPos);
						Spec_Fun_Interface(m_hhvCam,TRIGGER_DELAY_VALUE,nPos);
						UpdateFriend(m_pAllCtrl[k],nPos);
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "PackageSize") == 0)
					{
						nPos=nPos/4*4;
						TRACE("HVCommand(SetPacket)",nPos);
						//
						HV_CMD_PACKET_SIZE sz;
						sz.dwSize=nPos;
						HVCommand(m_hhvCam,CMD_SET_BYTE_PER_PACKET,&sz,0);
						UpdateFriend(m_pAllCtrl[k],nPos);
					}
					else if (strcmp(m_pAllCtrl[k]->ch, "BlackLevel") == 0)
					{
						TRACE("HVADCControl(SetBlackLevel)",nPos);
						for (int Channel = ADC_BLKLEVEL_CAL_REDCHANNEL; Channel <= ADC_BLKLEVEL_CAL_BLUECHANNEL; ++Channel )
						{
							HVADCControl(m_hhvCam,Channel,nPos);
						}	
						UpdateFriend(m_pAllCtrl[k],nPos);
					}
				}
			//	TRACE("m_pAllCtrl[%d]=====%s",k,m_pAllCtrl[k]->ch);
			}
		}	

}


int CDlgPrp::UpdateFriend(CAllCtrl *pCt, int nPos)
{
	if (pCt==NULL)
	     return 0;
	
	int k;
	int nId;
	CString str;
	int OldV;
	for (k=0;k<100;k++)
	{
		if (m_pAllCtrl[k]!=NULL)
		{
			if ((strcmp(m_pAllCtrl[k]->ch, pCt->ch) == 0)&&m_pAllCtrl[k]->nType==3&&m_pAllCtrl[k]!=pCt)
			{
				nId=m_pAllCtrl[k]->nID;
				CEdit *pEd = (CEdit *)GetDlgItem(nId);
				pEd->GetWindowText(str);
				sscanf(str.GetBuffer(str.GetLength()),"%d",&OldV);
			
				if(OldV!=nPos){
					str.Format(" %d", nPos);
					pEd->SetWindowText(str);
					//HVAGCControl(m_hhvCam,AGC_GAIN,nPos);
				}
				
			}
			else if ((strcmp(m_pAllCtrl[k]->ch, pCt->ch) == 0)&&m_pAllCtrl[k]->nType==1&&m_pAllCtrl[k]!=pCt)
			{	
				nId=m_pAllCtrl[k]->nID;
				CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(nId);
				OldV = pSlider->GetPos();
				if(OldV!=nPos){
					pSlider->SetPos(nPos);
					//HVAGCControl(m_hhvCam,AGC_GAIN,nPos);
				}
				
				str.Format("asdf %d", nPos);
				TRACE(str);
			}
			
		}
	}
	
	return nId;

}

void CDlgPrp::Spec_Fun_Interface(HHV hhv, HV_INTERFACE1_ID Interface_ID, int value)
{
	HV_INTERFACE1_CONTEXT pInfcectx;
	pInfcectx.ID = Interface_ID;
	pInfcectx.dwVal=(DWORD)value;
	HVSTATUS status = HVCommand(hhv,CMD_SPEC_FUN_INTERFACE1,&pInfcectx,0);

}

void CDlgPrp::LoadLut()
{
	CFileDialog dlg(TRUE, "Lut", NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"TextFiles (*.lut)|*.lut|All Files(*.*)|*.*||",
		AfxGetMainWnd());
	   
	   if(dlg.DoModal() == IDOK) {
		   
		   char buf[1280];
		   CString s;
		   s=dlg.GetFileName();
		   
		   strcpy(buf,s.GetBuffer(s.GetLength()));
		   
		   DWORD* p = new DWORD[4*1024];
		   DWORD Readed = 0;
		   HANDLE hFile = CreateFile(buf, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		   ReadFile(hFile, p, 16*1024, &Readed, NULL);
		   CloseHandle(hFile);
		   if (Readed == 16*1024) {
			   
			   HV_CMD_LUT_DWENTRIES lut;
			     
			   lut.dwOffset=0    ;
			   lut.pBuffer=p     ;
			   lut.dwEntries=16*1024    ;
			   
			   HVCommand(m_hhvCam,CMD_SET_LUT_DWENTRIES,&lut,0);
		   }
		   
		   delete p;
	   }

}

void CDlgPrp::SetExposureTime_HV2000FC(HHV hhv, int nWidth, int nUnit, int nSpeed)
{
	///English:    ///Chinese: 时钟频率
	long lClockFreq = 24000000;
	lClockFreq = lClockFreq / nUnit;  ///English:    ///Chinese: 毫秒或微秒
	long lTb = 0;
	lTb += 53;
	
	if(lTb < 19) lTb = 19;
	unsigned long Register = 0;
	int w=nWidth;
    long lTrow = max(617, w + 305.0 + lTb);
	float fReg = ( (nSpeed * lClockFreq) + 180.0) / lTrow + 1;
	if((fReg - (int)fReg) > 0.5){	///English:    ///Chinese: 对数据 fReg 进行四舍五入
		fReg += 1.0;
	}
	Register = (unsigned long)fReg;
	
	if (Register <= 0)
		Register = 1;
	else if (Register > 16383) 
		Register = 16383;				
				
	
	HVSTATUS status = HVAECControl(hhv, AEC_EXPOSURE_TIME, Register);
	
	if ( ! HV_SUCCESS(status)){
		AfxMessageBox("HVAECControl Error!!!");
	}

}

void CDlgPrp::SetExposureTime_HV3100FC(HHV hhv, int nWidth, int nUnit, int nSpeed)
{
	///English:    ///Chinese: 时钟频率
	long lClockFreq = 24000000;
	lClockFreq = lClockFreq / nUnit;  ///English:    ///Chinese: 毫秒或微秒
	long lTb = 0;
	lTb += 142;             ///English:    ///Chinese: 确保数据为正值
	if (lTb < 21)			///English:    ///Chinese: 使得lTb（即P3）为大于21的数值
		lTb = 21;
	long lP1 = 331, lP2 = 38; 
	long lP4 = 316;	
	if(m_bounceWnd->m_Resolution == RES_MODE1){
		lP1 = 673;
		lP2 = 22;
		lP4 = 316 * 2;
	}
	int w=nWidth;
	long lAQ = w + lP1 + lP2 + lTb;
	long lTrow = max(lAQ, (lP1 + lP4));
	float fReg = ((nSpeed * lClockFreq) + lP1 - 132.0) / lTrow;
	if((fReg - (int)fReg) > 0.5){				///English:    ///Chinese: 对数据 fReg 进行四舍五入
		fReg += 1.0;
	}
	unsigned long Register;
	Register = (unsigned long)fReg;
	
	if (Register <= 0)
		Register = 1;
	else if (Register > 1048575) 
		Register = 1048575;
	
	HVSTATUS status = HVAECControl(hhv, AEC_EXPOSURE_TIME, Register);
	
	if ( ! HV_SUCCESS(status)){
		AfxMessageBox("HVAECControl Error!!!");
	}

}

void CDlgPrp::SetExposureTime_HV1300(HHV hhv, int nWidth, int nUnit, int nSpeed)
{
	long lClockFreq = 24000000;
	lClockFreq = lClockFreq / nUnit;  ///English:    ///Chinese: 毫秒或微秒
	long lTb = 0;
	lTb += 9;
	lTb -= 19;	
	if(lTb <= 0) lTb =0;
	
	unsigned long Register = 0;
	int w=nWidth;
	Register = (unsigned long)(((nSpeed * lClockFreq)+ 180.0) / (w + 244.0 + lTb));
	if (Register <= 0)
		Register = 1;
	else if (Register > 16383) 
		Register = 16383;				
				
	
	HVSTATUS status = HVAECControl(hhv, AEC_EXPOSURE_TIME, Register);
	
	if ( ! HV_SUCCESS(status)){
		AfxMessageBox("HVAECControl Error!!!");
	}

}

int CDlgPrp::UpdateFriend(CString strName, int nPos)
{
	if (strName=="")
	return 0;

	int k;
	int nId;
	CString str;
	
	for (k=0;k<100;k++)
	{
		if (m_pAllCtrl[k]!=NULL)
		{
			if ((strcmp(m_pAllCtrl[k]->ch, strName) == 0)&&m_pAllCtrl[k]->nType==4)///English:    ///Chinese: 软触发按钮
			{
				nId=m_pAllCtrl[k]->nID;
				CButton *But = (CButton *)GetDlgItem(nId);
				if (nPos!=0)
				{
					But->EnableWindow(TRUE);
				}
				else
					But->EnableWindow(FALSE);
			}
			else if ((strcmp(m_pAllCtrl[k]->ch, strName) == 0)&&m_pAllCtrl[k]->nType==1)//
			{
				nId=m_pAllCtrl[k]->nID;
				CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(nId);
				if (nPos!=0)
				{
					pSlider->EnableWindow(TRUE);
				}
				else
					pSlider->EnableWindow(FALSE);
			}
			else if ((strcmp(m_pAllCtrl[k]->ch, strName) == 0)&&m_pAllCtrl[k]->nType==3)//
			{
				nId=m_pAllCtrl[k]->nID;
				CEdit *pEdt = (CEdit *)GetDlgItem(nId);
				if (nPos!=0)
				{
					pEdt->EnableWindow(TRUE);
				}
				else
					pEdt->EnableWindow(FALSE);
			}
			
			
		}
	}
	

	return 1;
}
