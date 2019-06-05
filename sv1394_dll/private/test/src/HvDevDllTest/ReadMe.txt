[HvDevDllTest]
--2009.3.26 Thu--hyl--
1. A: 
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

 
v1.0.0.5

============================================

--2009.3.25 Wed--hyl--
1. A: v_Test_HVGuid
 
v1.0.0.4

============================================

--2009.2.25 Tue--hyl--
1. M: v_Test_Enumerate
2. A: class CStopWatch
v1.0.0.3
============================================

--2009.2.23 Mon--hyl--
1. A: CTabPage1, CTabPage2
	void CTabPage2::v_Test1()

v1.0.0.2
=============================================

--2009.2.19 Thu--hyl--
1. 建立本工程。
v1.0.0.1
=============================================
