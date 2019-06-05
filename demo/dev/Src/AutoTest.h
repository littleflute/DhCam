// AutoTest.h: interface for the CAutoTest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOTEST_H__BCB15A32_F666_4B9D_A71C_56EFE771D417__INCLUDED_)
#define AFX_AUTOTEST_H__BCB15A32_F666_4B9D_A71C_56EFE771D417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame;

#define	TEST_GRAB_CONTINUOUS	0


class CAutoTest  
{
public:
	CAutoTest(CMainFrame &MainFrame);
	virtual ~CAutoTest();

	void Dispatch(int code, void *pContext);

private:
	void TestGrabContinuous();

	CMainFrame &m_MainFrame;
};

#endif // !defined(AFX_AUTOTEST_H__BCB15A32_F666_4B9D_A71C_56EFE771D417__INCLUDED_)
