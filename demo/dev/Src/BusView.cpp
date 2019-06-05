// BusView.cpp: implementation of the CBusView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DeviceManager.h"
#include "BusView.h"
#include "device.h"
#include "mainfrm.h"
#include "utility.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
// CBusView::CBusView(CDeviceManager& DeviceManager, CMainFrame& MainFrame) 
//------------------------------------------------------------------------------
/**
* Constructs a new bus viewer window
*
* \param     DeviceManager  Reference to the camera manager 
* \param     MainFrame Reference to the main frame
*/
//------------------------------------------------------------------------------
CBusView::CBusView(CDeviceManager& DeviceManager, CMainFrame& MainFrame) 
: m_DeviceManager(DeviceManager), m_MainFrame(MainFrame),  UNABLE_TO_OPEN("<Cannot open device>") 
{
}



LRESULT CBusView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_TreeView.Attach(GetDlgItem(IDC_BUSTREE));
	
	// Create Image List
	m_ImageList.Create(16, 16, ILC_MASK, 4, 4);
	CBitmap img;
	CBitmap mask;
	// insert image for "This PC"
	img.LoadBitmap(IDB_BITMAP1);
	mask.LoadBitmap(IDB_BITMAP2);
	m_ImageList.Add(img, mask);
	// insert image for "BCAM device"
	img.Detach();
	mask.Detach();
	img.LoadBitmap(IDB_BITMAP3);
	mask.LoadBitmap(IDB_BITMAP4);
	m_ImageList.Add(img, mask);
	
	m_TreeView.SetImageList(m_ImageList, TVSIL_NORMAL);
	
	// Insert root item
	m_RootItem = m_TreeView.InsertItem("This PC", 0, 0, TVI_ROOT, TVI_LAST);

    
	int nTotal = m_MainFrame.GetDeviceNum(); 	
	
	for(int i = 1; i <= nTotal; i++){
		AddNode(i);
	}
	
	m_TreeView.Expand(m_RootItem);
	
	return 0;
}




LRESULT CBusView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	NodeMap_t::iterator pEntry;
	for (pEntry = m_NodeMap.begin(); pEntry != m_NodeMap.end(); ++pEntry){
		delete pEntry->second;
	}
	
	bHandled = FALSE;
	return 0;
}




LRESULT CBusView::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	RECT rcClient;
	GetWindowRect(&rcClient);
	
	HWND hwnd;
	RECT rc;
	hwnd = GetDlgItem(IDC_BUSTREE);
	::GetWindowRect(hwnd, &rc);
	int w=rcClient.right - rc.left;
	int h=rcClient.bottom - rc.top;

	::SetWindowPos(hwnd, HWND_TOP, 
		0, 0, 
		w, h,
		SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	
	return 0;
}




LRESULT CBusView::OnContextMenu(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	UINT Flags;
	CPoint point = CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	m_TreeView.ScreenToClient(&point);
	HTREEITEM hItem = m_TreeView.HitTest(point, &Flags);
	if (hItem && (Flags & TVHT_ONITEM)){
		m_TreeView.SelectItem(hItem);
		if (m_NodeMap.find(hItem) != m_NodeMap.end()) {
			CBusNode *pNode = m_NodeMap[hItem];
			//			m_MainFrame->UpdateUI();
			CMenu menuContext;
			menuContext.LoadMenu(IDR_BUSVIEW_CONTEXT_MENU);
			CMenuHandle menuPop(menuContext.GetSubMenu(0));
			m_TreeView.ClientToScreen(&point);
			m_MainFrame.m_CmdBar.TrackPopupMenu(menuPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);
		}
	}
	
	return 0;
}





//------------------------------------------------------------------------------
// void CBusView::CurrentDeviceChanged(CDevice* pDevice)
// Author: 
//------------------------------------------------------------------------------
/**
* To be called if another device is to be selected (e.g. the user actiates an image 
* child window associated with a camera device which isn't selected in the tree view
*
* \param     pDevice Pointer to the camera device
* \return    void
*
*/
//------------------------------------------------------------------------------
void CBusView::CurrentDeviceChanged(CDevice* pDevice)
{
	if ( ! pDevice )
		return;
	int DeviceNumber = pDevice->m_pInfo->DeviceNumber();
	
	NodeMap_t::iterator it;
	for ( it = m_NodeMap.begin(); it != m_NodeMap.end(); ++it )
	{
		if ( it->second->GetDeviceNumber() == DeviceNumber )
		{
			m_TreeView.SelectItem(it->first);
			break;
		}
	}
}




//------------------------------------------------------------------------------
// LRESULT CBusView::OnLButtonDblClk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
//------------------------------------------------------------------------------
/**
* Called if the user performs a double click on one of the tree view's nodes
*
* \param     idCtrl
* \param     pnmh
* \param     bHandled
* \return    0
*
*/
//------------------------------------------------------------------------------

LRESULT CBusView::OnLButtonDblClk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	try
	{
		HTREEITEM hItem = m_TreeView.GetSelectedItem();
		if ( m_NodeMap.find(hItem) != m_NodeMap.end() )
		{
			CBusNode* pNode = m_NodeMap[hItem];
			
			// open new MDI child
			if ( m_DeviceManager.GetCurrentDevice() != NULL  && ! m_DeviceManager.ExistMDIChild() )
				m_DeviceManager.AddMDIChild();
			
		}
	} CATCH_REPORT();
	return 0;
}



//------------------------------------------------------------------------------
// LRESULT CBusView::OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
//------------------------------------------------------------------------------
/**
* Called if the tree view's selection changes
*
* \param     idCtrl
* \param     pnmh
* \param     bHandled
* \return    0
*
*/
//------------------------------------------------------------------------------
LRESULT CBusView::OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	try{
		LPNMTREEVIEW pnmtv = (LPNMTREEVIEW) pnmh;
		HTREEITEM hItem = pnmtv->itemNew.hItem;
		if ( m_NodeMap.find(hItem) != m_NodeMap.end() ){
			CBusNode* pNode = m_NodeMap[hItem];
			int n = pNode->GetDeviceNumber();
			DWORD error = m_DeviceManager.CurrentDeviceChanged(n);
		}
		else{
			// inform the camera manager that currently now device is selected
			m_DeviceManager.CurrentDeviceChanged(0);
		}
	}
	CATCH_REPORT();
	
	
	return 0;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////
// Handling of removal / arrival of new devices

//------------------------------------------------------------------------------
// void CBusView::AddNode(CString devName, GUID guid)
// Author: 
//------------------------------------------------------------------------------
/**
* Add a new node to the bus view. Let the camera manager create a new camera object. 
* Try to restore the settings of a camera object from the registry
*
* \param     devName Device name of the new device
* \param     guid Device's interface GUID
* \return    void
*
*/
//------------------------------------------------------------------------------
void CBusView::AddNode(int nNumber)
{
	CBusNode *pNode = NULL;
	CString ModelName = UNABLE_TO_OPEN;
	HVTYPE type;
	
	if ( ContainsNode(nNumber) ){
		RemoveNode(nNumber);
	}
	
	try
	{
		// add a camera device
		CDevice *pDevice = NULL;
		m_DeviceManager.AddDevice(nNumber, m_hWnd);
		pDevice = m_DeviceManager.GetDevice(nNumber);
		assert ( pDevice != NULL );
		
		try
		{
			ModelName = pDevice->m_pInfo->DeviceName();
			type = pDevice->m_pInfo->DeviceType();
//			pDevice->Activate(false);
//			try		this part is put into the Activate() by xupx 2009.04.03
//			{
//				pDevice->Set_Default_AOI();
//				CPropertyBagPtr ptrBag = HVDevice::CRegistryPropertyBag::Open(APPLICATION_KEY + pDevice->m_pInfo->DeviceName() + "\\Device");
//				pDevice->RestoreConfiguration(ptrBag);
// 			}
//			catch ( HVBaseException& e)
//			{
//				if ( e.Error() != STATUS_INTERNAL_ERROR )
//					ReportError(e);
//			}
			pDevice->Deactivate();	
		}
		CATCH_REPORT();
	
		
		pNode = new CBusNode(ModelName, nNumber, type);
		if ( pNode == NULL )
		{
			throw HVBaseException(ERROR_OUTOFMEMORY, "CBusView::AddNode()");
			ATLTRACE("HVPerf: AddNode ........");
		}
	
		// 2009.04.13 by xupx 
//		CDevice *pDevice = NULL;
		HTREEITEM hItem;
		pDevice = m_DeviceManager.GetDevice(nNumber);

		if(pDevice->m_pInfo->DeviceType() == DRVIFGIGETYPE || pDevice->m_pInfo->DeviceType() == DRVIFENETTYPE)
		{
			HV_DEVICE_INTERFACE hdi;
			::HVGetDevInterfaceFromIndex(nNumber, &hdi);
			CString str;
			str = ModelName + "(" + hdi.szDevicePath + ")";
			hItem = m_TreeView.InsertItem(str, 1, 1, m_RootItem, TVI_LAST);
		}
		else
		{
			hItem = m_TreeView.InsertItem(ModelName, 1, 1, m_RootItem, TVI_LAST);		
		}

		m_TreeView.Expand(m_RootItem);
		m_NodeMap[hItem] = pNode;
	}
	CATCH_REPORT();
}

bool CBusView::ContainsNode(int nNumber)
{
	NodeMap_t::iterator pEntry;
	for ( pEntry = m_NodeMap.begin(); pEntry != m_NodeMap.end(); ++pEntry )
	{
		if ( pEntry->second->GetDeviceName() == nNumber )
		{
			return true;
		}
	}
	return false;
}


//------------------------------------------------------------------------------
// void CBusView::RemoveNode(CString devName)
// Author: 
//------------------------------------------------------------------------------
/**
* Remove the node for a specific device from the tree view
*
* \param     devName  The device's name
* \return    void
*
*/
//------------------------------------------------------------------------------
void CBusView::RemoveNode(int nNumber)
{
	NodeMap_t::iterator pEntry;
	for ( pEntry = m_NodeMap.begin(); pEntry != m_NodeMap.end(); ++pEntry )
	{
		if ( pEntry->second->GetDeviceNumber() == nNumber )
		{
			m_TreeView.DeleteItem(pEntry->first);
			// destroy node
			delete pEntry->second;
			// remove node from map
			m_NodeMap.erase(pEntry);
			if ( m_TreeView.GetCount() == 0 )
			{
				// The last device is removed, notify rest of the world.
				// This notification is necessary because removing the last device will not cause
				// TVN_SELCHANGED notification
				
				/*
				m_DeviceManager.CurrentDeviceChanged(NULL);*/
			}
			/*  m_MainFrame.DeviceRemoved(devName);*/
			
			break;
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Protected / private member functions
//

//------------------------------------------------------------------------------
// void CBusView::ReportError(BcamException& e) 
// Author: 
//------------------------------------------------------------------------------
/**
* Shows an error message
*
* \param     e Reference to a BcamException
* \return    void
*
*/
//------------------------------------------------------------------------------
void CBusView::ReportError(HVBaseException& e) 
{

	m_MainFrame.ReportError(e);
}
