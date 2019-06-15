// AllCtrl.cpp: implementation of the CAllCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mdi.h"
#include "AllCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAllCtrl::CAllCtrl()
{
	pCtrl = NULL;
	
	hFont   =   CreateFont(12,   0,   0,   0,   400,   0,   0,   0,   ANSI_CHARSET,   
		OUT_DEFAULT_PRECIS,   CLIP_DEFAULT_PRECIS,   DEFAULT_QUALITY,   
		DEFAULT_PITCH   ||   FF_DONTCARE,   "宋体");   
		
}

CAllCtrl::~CAllCtrl()
{
	TRACE("======================~CAllCtrl");
//*
	if (pCtrl!=NULL)
	{
		
	
	}
//*/

}

void CAllCtrl::CreateMe(CDlgPrp* pwnd)
{

	
	CRect rect;
	rect=CRect(x,y,x+w,y+h);
	switch(nType) {
	case 1:///English:    ///Chinese: Slider控件
		CreateSliderCtrl(pwnd,rect);
		break;
	case 2:///English:    ///Chinese: Static控件
		CreateStaticCtrl(pwnd,rect);
		break;
	case 3:///English:    ///Chinese: edit控件
		CreateEditCtrl(pwnd,rect);
		break;
	case 4:///English:    ///Chinese: CButton控件
		CreateButtonCtrl(pwnd,rect);
		break;
	case 5:///English:    ///Chinese: check Button控件
		CreateCheckButtonCtrl(pwnd,rect);
		break;
	case 6:///English:    ///Chinese: CComboBox控件
		CreateComboBoxCtrl(pwnd,rect);
		break;
	case 7:///English:    ///Chinese: Group控件
		CreateGroupButtonCtrl(pwnd,rect);
		break;
	default:
		break;
		}
		
}

void CAllCtrl::CreateSliderCtrl(CDlgPrp *pwnd, CRect rect)
{
	int nMin,nMax,nCur,nLineSize;///English:    ///Chinese: 解析控件参数: 可调范围, 当前值, 步长
	sscanf(chX,"[%d,%d,%d,%d] ",&nMin,&nMax,&nCur,&nLineSize);
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CSliderCtrl);
		CSliderCtrl*p=(CSliderCtrl*)(pCtrl);
		
	  
		p->Create( TBS_HORZ|TBS_TOP|TBS_BOTTOM|TBS_NOTICKS,
			rect,
			pwnd,
			nID
			);
		p->ShowWindow(SW_SHOW);
		p->SetFocus();
		p->SetRange(nMin,nMax);
		p->SetLineSize(nLineSize);///English:    ///Chinese: 设置步长
		p->SetPos(nCur);
		if (strcmp(ch,"Shutter") == 0)
		{
			pwnd->m_nShutterSpeed=nCur;
			TRACE("CAllCtrl::CreateSliderCtrl m_nShutterSpeed=%d",pwnd->m_nShutterSpeed);
		}
		
		SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
	}
}





void CAllCtrl::CreateStaticCtrl(CDlgPrp *pwnd, CRect rect)
{

	
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CStatic);
		CStatic*p=(CStatic*)(pCtrl);
		p->Create(ch,WS_VISIBLE|WS_GROUP,
			rect,
			pwnd,
			nID
			);
		p->ShowWindow(SW_SHOW);
		p->SetFocus();

		SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
	
	}
}

void CAllCtrl::CreateEditCtrl(CDlgPrp *pwnd, CRect rect)
{
	int nMin,nMax,nCur;
	sscanf(chX,"[%d,%d,%d] ",&nMin,&nMax,&nCur);
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CEdit);
		CEdit*p=(CEdit*)(pCtrl);
		p->CreateEx(WS_EX_CLIENTEDGE, 
			_T("EDIT"), 
			NULL,
			ES_AUTOHSCROLL|
			ES_LEFT|ES_NOHIDESEL|
			WS_CHILD,
			rect, pwnd,
			nID);
		
		p->SetFocus();
		p->ShowWindow(SW_SHOW);

        SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
		
		CString strCur;
		strCur.Format("%d",nCur);
		p->SetWindowText(strCur);
	}
}

void CAllCtrl::CreateButtonCtrl(CDlgPrp *pwnd, CRect rect)
{
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CButton);
		CButton*p=(CButton*)(pCtrl);
		p->Create(ch,BS_DEFPUSHBUTTON|WS_VISIBLE|WS_CHILD,
			rect,
			pwnd,
			nID);
		//CFont *font=pwnd->GetFont();///English:    ///Chinese: 获取对话框字体
		//p->SetFont(font);///English:    ///Chinese: 设置控件字体
		
		SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
		
		if (strcmp(ch,"SoftTrigger") == 0)
		{
			p->EnableWindow(0);
		}
	}

}

void CAllCtrl::CreateCheckButtonCtrl(CDlgPrp *pwnd, CRect rect)
{
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CButton);
		CButton*p=(CButton*)(pCtrl);
		p->Create("",WS_CHILD|BS_AUTOCHECKBOX|BS_CHECKBOX|WS_VISIBLE,
			rect,
			pwnd,
			nID);

		SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
		
		if(strcmp(ch,"BlackLevelEnable") == 0)
		{
		   pwnd->CheckDlgButton(nID,1);
		   TRACE("CAllCtrl::CreateCheckButtonCtrl CheckDlgButton");
		}
	
	}

}

void CAllCtrl::CreateComboBoxCtrl(CDlgPrp *pwnd, CRect rect)
{
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CComboBox);
		CComboBox*p=(CComboBox*)(pCtrl);
		p->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_HSCROLL|WS_HSCROLL|CBS_AUTOHSCROLL |CBS_DROPDOWNLIST,//CBS_SORT|
			rect,
			pwnd,
			nID);


		SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
		
		chX;
		int n = 0;
		int idx = 0;
		
		//[4,1,continue,Trigger,TriggerLevel,TriggerEdge]
		
		char *pc=chX;
		
		pc++;
		
		int nn=0;
		char w[100];
		int nw=0;
		int dn=0;
		while((*pc!=']')){
			if(*pc==','){
				dn++;
				w[nw]='\0';
				if(dn==1){
					sscanf(w,"%d",&n);   // Item Num
					nw=0;
				}
				else if(dn==2){
					sscanf(w,"%d",&idx); // Item Idx
					nw=0;
				}
				else{
					p->AddString(w);
					nw=0;
				}
			}
			else{
				w[nw]=*pc;
				nw++;
			}
			pc++;
			
		}
		w[nw]='\0';
		p->AddString(w);
		p->SetCurSel(idx);
	}
	
	
}

void CAllCtrl::CreateGroupButtonCtrl(CDlgPrp *pwnd, CRect rect)
{
	if(pCtrl==NULL)
	{
		(pCtrl) = new (CButton);
		CButton*p=(CButton*)(pCtrl);
		p->Create(ch,BS_GROUPBOX|WS_CHILD|WS_VISIBLE,
			rect,
			pwnd,
			nID);
	    
		SendMessage(p->m_hWnd,WM_SETFONT,(DWORD)hFont,TRUE);  
		p->Invalidate(TRUE);
	}

}
