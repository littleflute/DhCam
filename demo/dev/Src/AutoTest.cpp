// AutoTest.cpp: implementation of the CAutoTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvperf.h"
#include "AutoTest.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoTest::CAutoTest(CMainFrame &MainFrame) :
m_MainFrame(MainFrame)
{

}


CAutoTest::~CAutoTest()
{

}



void CAutoTest::Dispatch(int code, void *pContext)
{
	switch(code) {
	case TEST_GRAB_CONTINUOUS:
		TestGrabContinuous();
		break;
	default:
		break;
	}
}


void CAutoTest::TestGrabContinuous()
{
	CTreeViewCtrl &TreeView =  m_MainFrame.m_BusView.m_TreeView;
	
	HTREEITEM hItem = TreeView.GetFirstVisibleItem();
	hItem = TreeView.GetChildItem(hItem);
	do {
		TreeView.SelectItem(hItem);
		m_MainFrame.SendMessage(WM_COMMAND, ID_GRAB_CONTINUOUS, 0);
	}while (hItem = TreeView.GetNextItem(hItem, TVGN_NEXT));

}