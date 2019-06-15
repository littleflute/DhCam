// AllCtrl.h: interface for the CAllCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALLCTRL_H__9691B68E_2E4D_4ADC_9348_9471E941B764__INCLUDED_)
#define AFX_ALLCTRL_H__9691B68E_2E4D_4ADC_9348_9471E941B764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgPrp.h"
class CDlgPrp;

class CAllCtrl  
{
public:
	void CreateGroupButtonCtrl(CDlgPrp *pwnd, CRect rect);
	void CreateComboBoxCtrl(CDlgPrp*pwnd,CRect rect);
	void CreateCheckButtonCtrl(CDlgPrp*pwnd,CRect rect);
	void CreateButtonCtrl(CDlgPrp* pwnd,CRect rect);
	void CreateEditCtrl(CDlgPrp*pwnd,CRect rect);
	void CreateStaticCtrl(CDlgPrp* pwnd,CRect rect);
	void CreateSliderCtrl(CDlgPrp*pwnd,CRect rect);
	void CreateMe(CDlgPrp* pwnd);
	CAllCtrl();
	virtual ~CAllCtrl();

	int nType,x,y,w,h,nID;///English:    ///Chinese: �ؼ����͡����ꡢID
	
	char ch[100];///English:    ///Chinese: ��;
	void *pCtrl; ///English:    ///Chinese: �ؼ����
	char chX[100];///English:    ///Chinese: �ؼ����Ե�һЩ����

	HFONT hFont;///English:    ///Chinese: �ؼ�������

};

#endif // !defined(AFX_ALLCTRL_H__9691B68E_2E4D_4ADC_9348_9471E941B764__INCLUDED_)
