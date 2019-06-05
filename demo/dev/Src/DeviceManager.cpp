//-----------------------------------------------------------------------------
//  (c) 2004 by Daheng Imavision 
//  Author: ilovedrv
//  Data:  
//-----------------------------------------------------------------------------
/**
*
* \brief   Interface for the CDeviceManager class.
*
*/
//-----------------------------------------------------------------------------


#include "stdafx.h"
#include "DeviceManager.h"
#include "mainfrm.h"
#include "ChildFrm.h"

#include "Camera\HV1300FC.h"
#include "Camera\HV2000FC.h"
#include "Camera\HV3100FC.h"
#include "Camera\HV1300FM.h"
#include "Camera\SV1300FM.H"
#include "Camera\SV1300FC.H"
#include "Camera\SV1310FC.H"
#include "Camera\SV1310FM.H"
#include "Camera\SV1311FC.H"
#include "Camera\SV1311FM.H"
#include "Camera\SV400FC.H"
#include "Camera\SV400FM.H"
#include "Camera\SV1410FC.H"
#include "Camera\SV1410FM.H"
#include "Camera\SV1420FC.H"
#include "Camera\SV2000FC.H"
#include "Camera\SV1420FM.H"
#include "Camera\SV2000FM.H"
#include "Camera\SV1400FC.H"
#include "Camera\SV1400FM.H"

#include "Camera\ITS1410FC.H"
#include "Camera\ITS1420FC.H"
#include "Camera\ITS2000FC.H"
#include "Camera\ITS1394.H"

#include "Camera\GIGE.H"
#include "Camera\ENET.H"

/*
#include "Camera\HV1310UC.h"
#include "Camera\HV2010UC.h"
#include "Camera\HV3110UC.h"
#include "Camera\HV1310UM.h"
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeviceManager::CDeviceManager(CMainFrame &MainFrame)
: m_MainFrame(MainFrame),
m_pCurrentDevice(NULL),
m_SelectedDevice(0)
{
	
}

CDeviceManager::~CDeviceManager()
{	
	for ( DevNumber2Device_t::iterator it = m_DevNumber2Device.begin(); it != m_DevNumber2Device.end(); ++ it ){
		delete it->second;
	}		
}



//------------------------------------------------------------------------------
// CDevice* CDeviceManager::GetDevice(CString DeviceName)
//------------------------------------------------------------------------------
/**
* Returns the camera object for a given device name 
* \param     DeviceName
* \return the camera object. NULL is returned, if no object has been found  
*/
//------------------------------------------------------------------------------
CDevice* CDeviceManager::GetDevice(int nNumber)
{
	DevNumber2Device_t::iterator it = m_DevNumber2Device.find(nNumber);
	
	return it == m_DevNumber2Device.end() ? NULL : it->second;
}


CDevice* CDeviceManager::GetDevice(CChildFrame *pChild)
{
	CDevice* pDevice = NULL;
	
	for ( Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++it ){
		if (it->second == pChild){
			pDevice = it->first;			
			break;
		}
	} 
	assert (it != m_Device2ChildFrame.end());
	
	return pDevice;
}



DWORD CDeviceManager::CurrentDeviceChanged(int nNumber)
{
	CDevice* pOldDevice = m_pCurrentDevice;
	DWORD result = 0;
	
	m_SelectedDevice = nNumber;
	
	if (nNumber == 0) {
		// Actually in the bus view no BCAM object is selected. If there is no MDI associated
		// with m_pCurrentDevice, m_pCurrentDevice is set to NULL. Otherwise m_pCurrentDevice 
		// remains unchanged, i.e. m_pCurrentDevice corresponds to the MDI window.
		if ( GetCurrentChild() == NULL ){
			// no child window exists, set current device to NULL
			m_pCurrentDevice = NULL;
		}
		else{
			// current device remains unchanged
			return 0;
		}
	}
	else{
		CDevice *pDevice = GetDevice(nNumber);
		if (pDevice == m_pCurrentDevice) 
			return 0;   // nothing has changed
		m_pCurrentDevice = pDevice;
	}
	
	// open the new device
	try
	{
		if (m_pCurrentDevice != NULL)
		{
			m_pCurrentDevice->Activate();	
			// add by xupx 2009.03.14 
		}
		
		if ((pOldDevice != NULL) && pOldDevice->IsOpen() && (! pOldDevice->IsGrabActive()) ) 
		{
//			pOldDevice->Deactivate();		//delete 2008.11.22
		}
	}

	CATCH_REPORT();
	
	// inform the main frame
	m_MainFrame.CurrentDeviceChanged(m_pCurrentDevice);
	
	if (m_pCurrentDevice != NULL) {
		Device2ChildFrame_t::iterator it = m_Device2ChildFrame.find(m_pCurrentDevice);
		if ( it != m_Device2ChildFrame.end() )
		{
			// a MDI child exists, activate it
			CChildFrame* pChild = it->second;
			pChild->SetFocus();
		}
	}
	
	return result;
}




//------------------------------------------------------------------------------
// void CDeviceManager::AddMDIChild()
//------------------------------------------------------------------------------
/**
* Opens a new window associated with the current device. 
* If the registry key for the current device contains information about the window
* layout and zoom factor, restore these information
*/
//------------------------------------------------------------------------------
void CDeviceManager::AddMDIChild()
{
	assert ( m_pCurrentDevice != NULL );
	assert ( m_Device2ChildFrame.find(m_pCurrentDevice) == m_Device2ChildFrame.end() );  // only one window per device is allowed
	
	CChildFrame* pChild = new CChildFrame(m_pCurrentDevice, *this, m_MainFrame);
	
	pChild->m_WinName = m_pCurrentDevice->m_pInfo->DeviceName() + 
		CString(" Viewer (") + 
		m_pCurrentDevice->m_pInfo->NodeId() + 
		CString(" )");

	pChild->CreateEx(m_MainFrame.m_hWndMDIClient,NULL, pChild->m_WinName);
	
	m_pCurrentDevice->SetMDIChild(pChild);
	m_Device2ChildFrame[m_pCurrentDevice] = pChild;
	
	// try to restore the window layout
	pChild->m_View.RestoreLayout();
	
	BOOL bMaximized = FALSE;
	m_MainFrame.MDIGetActive(&bMaximized);
	if ( bMaximized )
		pChild->ShowWindow(SW_MAXIMIZE);
		
	// load all plugin from the default folder
	pChild->m_PluginManager.LoadPlugins();
}




//------------------------------------------------------------------------------
// CChildFrame* CDeviceManager::GetCurrentChild()
// Author: 
//------------------------------------------------------------------------------
/**
* Retrieve a pointer to the window associated with the current device
*
*/
//------------------------------------------------------------------------------
CChildFrame* CDeviceManager::GetCurrentChild()
{
	if ( m_pCurrentDevice == NULL )
		return NULL;
	Device2ChildFrame_t::iterator it = m_Device2ChildFrame.find(m_pCurrentDevice);
	if ( it != m_Device2ChildFrame.end() ) 
		return it->second;
	else 
		return NULL;
}




//------------------------------------------------------------------------------
// bool CDeviceManager::ExistMDIChild()
//------------------------------------------------------------------------------
/**
* Is there already a MDI window associated with the current device
*
*/
//------------------------------------------------------------------------------
bool CDeviceManager::ExistMDIChild()
{
	assert ( m_pCurrentDevice != NULL );
	return ( m_Device2ChildFrame.find(m_pCurrentDevice) != m_Device2ChildFrame.end() );
}


//------------------------------------------------------------------------------
// bool CDeviceManager::IsDeviceValid()
//------------------------------------------------------------------------------
/**
* Is a given device valid, i.e. do we still know the device? 
*
*/
//------------------------------------------------------------------------------
bool CDeviceManager::IsDeviceValid(CDevice* pDevice)
{
	if ( pDevice == NULL ) 
		return false;
	DevNumber2Device_t::iterator it = m_DevNumber2Device.begin();
	while ( it != m_DevNumber2Device.end() )
	{
		if ( it->second == pDevice )
			return true;
		it++;
	}
	return false;
}




//------------------------------------------------------------------------------
// bool CDeviceManager::IsGrabActive()
// Author: 
//------------------------------------------------------------------------------
/**
* Is the current device grabbing?
*
*/
//------------------------------------------------------------------------------
bool CDeviceManager::IsGrabActive()
{
	assert ( m_pCurrentDevice != NULL );
	if ( m_pCurrentDevice != NULL )
		return ( m_pCurrentDevice->IsGrabActive() );
	else
		return false;
}


//------------------------------------------------------------------------------
// void CDeviceManager::MDIChildActivated(CChildFrame* pChild)
//------------------------------------------------------------------------------
/**
* A MDI child informs us that it has been activated by the user.
* \param     pChild Pointer to the child which has been activated
*/
//------------------------------------------------------------------------------
void CDeviceManager::MDIChildActivated(CChildFrame* pChild)
{
	for ( Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++it )
	{
		if ( it->second == pChild )
		{
			CDevice* pDevice = it->first;
			CurrentDeviceChanged(pDevice->m_pInfo->DeviceNumber());
			break;
		}
	}
	
}



//------------------------------------------------------------------------------
// void CDeviceManager::AddDevice(CString DeviceName, HWND hWndNotify)
// 
// 
//------------------------------------------------------------------------------
/**
* The bus view informs us about a new device found on the bus. 
* Create a camera object for the new device. If the registry contains settings 
* for the device, restore them.
*
* \param     DeviceName name of the device
* \param     hWndNotify handle of window which receives the pnp notifications
*/
//------------------------------------------------------------------------------
BOOL CDeviceManager::AddDevice(int nNumber, HWND hWndNotify)
{
	CDevice* pDevice = NULL;
	
	HVTYPE type;
	int nIndex = 0;
	::HVGetTypeFromIndex(nNumber, &type, &nIndex);
	ATLTRACE("HVPerf: AddDevice type  %d", type );
	switch(type){

	case HV1300FCTYPE: //11
		pDevice = new CHV1300FC(nNumber, nIndex, hWndNotify, m_MainFrame);
		break;
	case HV2000FCTYPE: //12
		pDevice = new CHV2000FC(nNumber, nIndex, hWndNotify, m_MainFrame);
		break;
	case HV3100FCTYPE: //13
		pDevice = new CHV3100FC(nNumber, nIndex, hWndNotify, m_MainFrame);
		break;
	case HV1300FMTYPE: //14
		pDevice = new CHV1300FM(nNumber, nIndex, hWndNotify, m_MainFrame);
		break;
    case SV1300FMTYPE: //19
        pDevice = new CSV1300FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
    case SV1300FCTYPE: //20
        pDevice = new CSV1300FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1310FCTYPE: //21
        pDevice = new CSV1310FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1310FMTYPE: //22
        pDevice = new CSV1310FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1311FCTYPE: //23
        pDevice = new CSV1311FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1311FMTYPE: //24
        pDevice = new CSV1311FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV400FCTYPE: //25
        pDevice = new CSV400FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV400FMTYPE: //26
        pDevice = new CSV400FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1410FCTYPE: //28
        pDevice = new CSV1410FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1410FMTYPE: //29
        pDevice = new CSV1410FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
      break;
	case SV1420FCTYPE: //30
        pDevice = new CSV1420FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1420FMTYPE: //31
        pDevice = new CSV1420FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV2000FCTYPE: //32
        pDevice = new CSV2000FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV2000FMTYPE: //33
        pDevice = new CSV2000FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1400FCTYPE: //34
        pDevice = new CSV1400FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case SV1400FMTYPE: //35
        pDevice = new CSV1400FM(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case DRVIFGIGETYPE: 
        pDevice = new CGIGE(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case DRVIFENETTYPE: 
        pDevice = new CENET(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;

	case ITS1410FCTYPE: 
        pDevice = new CITS1410FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case ITS1420FCTYPE: 
        pDevice = new CITS1420FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;
	case ITS2000FCTYPE: 
        pDevice = new CITS2000FC(nNumber, nIndex, hWndNotify, m_MainFrame);
        break;		

	case DRVIF1394TYPE:
		pDevice = new CITS1394(nNumber, nIndex, hWndNotify, m_MainFrame);
		break;

	default:
		throw HVBaseException(STATUS_PARAMETER_INVALID, "CDeviceManager::AddDevice()");
	}
	
	if ( pDevice == NULL ) return FALSE;	
	m_DevNumber2Device[nNumber] = pDevice;
	
	return TRUE;
}

//------------------------------------------------------------------------------
// void CDeviceManager::RemoveDevice(CString DeviceName)
// Author: 
//------------------------------------------------------------------------------
/**
* Tell the camera manager to remove a device. This method is called for devices
* which were not opened when they have been removed
*
* \param     DeviceName
* \return    
*
* 
* \see       <delete line if not used>
* \todo      
*/
//------------------------------------------------------------------------------
BOOL CDeviceManager::RemoveDevice(int nNumber)
{
	CDevice* pDevice = GetDevice(nNumber);
	if ( pDevice != NULL )
	{
	}
	
	return TRUE;
}




//------------------------------------------------------------------------------
// void CDeviceManager::MDIChildClosed(CChildFrame* pChild)
//------------------------------------------------------------------------------
/**
* A MDI child window informs us that it has been closed.
*
* \param     pChild
* \return    
*
* <type Return description here>
* 
* \see       <delete line if not used>
* \todo      
*/
//------------------------------------------------------------------------------
void CDeviceManager::MDIChildClosed(CChildFrame* pChild)
{
	//	Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin();
	for ( Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++it ){
		if (it->second == pChild){
			CDevice* pDevice = it->first;
			// cancel an active grab
			if ( pDevice->IsGrabActive() )
			{
				try
				{
					pDevice->GrabCancel();
					// If the camera is no longer selected, close it to allow other applications to access it
					if ( pDevice != m_pCurrentDevice && pDevice->IsOpen() )
					{
						pDevice->Deactivate();
					}
				}
				CATCH_REPORT();
			}
			m_Device2ChildFrame.erase(it);
			pDevice->SetMDIChild(NULL);

			pChild->m_PluginManager.UnloadPlugins();

			break;
		}
	} 
	assert ( it != m_Device2ChildFrame.end() );
	
	// If the user has selected a non BCAM node in the BusView window and no other 
    // child frames exist, there is no current device. In this case call the CurrentDeviceChanged()
    // method to update the DeviceManager's status
    if ( m_Device2ChildFrame.size() == 0 && m_SelectedDevice == 0)
		CurrentDeviceChanged(NULL);
}





//------------------------------------------------------------------------------
// void CDeviceManager::GrabSingle()
// Author: 
// Date: 20.09.2002
//------------------------------------------------------------------------------
/**
* Let the current device grab a single image
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::GrabSingle()
{
	assert ( m_pCurrentDevice != NULL );
	if ( m_pCurrentDevice == NULL )
		return;
	assert ( ! m_pCurrentDevice->IsGrabActive() );
	if ( m_pCurrentDevice->IsGrabActive() )
		return;
	if ( ! ExistMDIChild() )
		AddMDIChild();
	m_pCurrentDevice->GrabSingle();
	
}



//------------------------------------------------------------------------------
// void CDeviceManager::GrabContinuous()
// Author: 
//------------------------------------------------------------------------------
/**
* Let the current device continously grabbing images
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::GrabContinuous()
{
	assert ( m_pCurrentDevice != NULL );
	if ( m_pCurrentDevice == NULL )
		return;
//	assert ( ! m_pCurrentDevice->IsGrabActive() );
	if ( m_pCurrentDevice->IsGrabActive() )
		return;
	if ( ! ExistMDIChild() )
		AddMDIChild();
	m_pCurrentDevice->GrabContinuous();
}


void CDeviceManager::OnSoftTrigger()
{
    //assert ( m_pCurrentDevice != NULL );
    if ( m_pCurrentDevice == NULL )
        return;
    //assert ( m_pCurrentDevice->IsGrabActive() );
    m_pCurrentDevice->SoftTrigger();
}



void CDeviceManager::XD_Show_All_FPS()
{
	for ( Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++ it)
	{
		CChildFrame* pFrm = it->second;
		CDevice* pDevice = it->first;
		if(pFrm&&pDevice){
			double fps;
			if(it->first->XD_Get_FPS(fps)){			
				CString str;
				str.Format("FPS:%3.1f ", fps);
				pFrm->SetWindowText(str+pFrm->m_WinName);
			}
			else{
				pFrm->SetWindowText(pFrm->m_WinName);
			}
		}
	}
}


//------------------------------------------------------------------------------
// void CDeviceManager::GrabCancel()
// Author: 
//------------------------------------------------------------------------------
/**
* Let the current device cancel image acquisition
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::GrabCancel()
{
	assert ( m_pCurrentDevice != NULL );
	if ( m_pCurrentDevice == NULL )
		return;
//	assert ( m_pCurrentDevice->IsGrabActive() );
	if(!m_pCurrentDevice->IsGrabActive())
		return;				// add by xupx 2009.03.03  for TotalCancel
	else
		m_pCurrentDevice->GrabCancel();
}




//------------------------------------------------------------------------------
// void CDeviceManager::GrabFromFile(CString FileName)
// Author: 
//------------------------------------------------------------------------------
/**
* Load an image from file and show it in the MDI child window associated with the current device
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::GrabFromFile(CString FileName)
{
	assert ( m_pCurrentDevice != NULL );
	if ( m_pCurrentDevice == NULL )
		return;
	if ( ! ExistMDIChild() )
		AddMDIChild();
	m_pCurrentDevice->GrabFromFile( FileName);
}




//------------------------------------------------------------------------------
// void CDeviceManager::ShowPixelValue()
// Author: 
//------------------------------------------------------------------------------
/**
* Show the pixel value under the cursor in the main frame's status bar. 
* The z-ordering of the windows is considered.
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::ShowPixelValue()
{
	
	// Iterate through the MDI child windows in descending z-order until the window is found which
	// is under the cursor
	
	// retrieve handle to first MDI child
	HWND hWndChild = GetWindow(m_MainFrame.m_hWndMDIClient, GW_CHILD);
	
	CPoint pt;
	GetCursorPos(&pt);
	
	bool eraseValuePane = true;
	bool erasePosPane = true;
	
	while ( hWndChild != NULL )
	{
		CRect rect;
		GetWindowRect(hWndChild, &rect);
		if ( rect.PtInRect(pt) )
		{
			// window found 
			// search the object which represents the child window
			for ( Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++ it)
			{
				if ( it->second->m_hWnd == hWndChild )
				{
					eraseValuePane = ! it->second->m_View.ShowPixelValue() ; 
					break;
				}
			}
			for ( it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++it )
			{
				if ( it->second->m_hWnd == hWndChild )
				{
					erasePosPane = ! it->second->m_View.ShowCursorPos();
					break;
				}
			}
			break;
		}
		hWndChild = GetWindow(hWndChild, GW_HWNDNEXT);
	}
	if ( eraseValuePane )
	{
		m_MainFrame.m_Sbar.SetPaneText(ID_VALUE_PANE, CString("") );
	}
	if ( erasePosPane )
	{
		m_MainFrame.m_Sbar.SetPaneText(ID_POS_PANE, CString("") );
	}
}



//------------------------------------------------------------------------------
// void CDeviceManager::GetFrameRate(double& fps_acquired, double& fps_displayed)
// Author: 
//------------------------------------------------------------------------------
/**
* Get the frame rate for the device associated with the child window having the focus.
*
* \param     fps_acquired
* \param     fps_displayed
*/
//------------------------------------------------------------------------------
void CDeviceManager::GetFrameRate(double& fps_displayed)
{
	
	CPoint pt;
	GetCursorPos(&pt);
	//	fps_acquired = -1;
	fps_displayed = -1;
	
	HWND hWndChild = m_MainFrame.MDIGetActive();
	if ( hWndChild != NULL )
	{
		for ( Device2ChildFrame_t::iterator it = m_Device2ChildFrame.begin(); it != m_Device2ChildFrame.end(); ++ it)
		{
			if ( it->second->m_hWnd == hWndChild && it->first->IsContinuousGrabActive() )
			{
				it->first->GetFrameRate(fps_displayed);
				break;
			}
		}
	}
}



//------------------------------------------------------------------------------
// void CDeviceManager::AppExit()
// Author: 
//------------------------------------------------------------------------------
/**
* The main frame informs us that the application is to be closed. 
* Store the settings of the camera objects and do the cleanup
*
* \return    
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::AppExit(bool b)
{	
	ATLTRACE("CDeviceManager::AppExit() \n");

	m_pCurrentDevice = NULL;
	for ( DevNumber2Device_t::iterator it = m_DevNumber2Device.begin(); it != m_DevNumber2Device.end(); ++ it )
	{
		
		CDevice* pDevice = it->second;
		if ( pDevice != NULL )
		{
			// try to save the current configuration and to the registry
			try
			{
//				modified by xupx 2009.03.20
				if ( ! pDevice->IsOpen() )	{
//					pDevice->Activate();
					delete pDevice;
					pDevice = NULL;
					continue;
				}
				// If we are still grabbing, cancel the grabbing
				if ( pDevice->IsGrabActive() ) {
					ATLTRACE("pDevice->GrabCancel() \n");
					pDevice->GrabCancel();
				}
				if(!b){
					CPropertyBagPtr cameraBag = CRegistryPropertyBag::Create(APPLICATION_KEY + pDevice->m_pInfo->DeviceName() + "\\Device");
					pDevice->SaveConfiguration(cameraBag);
				}
				else{
					CString keyName = APPLICATION_KEY;
					SHDeleteKey(HKEY_CURRENT_USER,keyName);
				}
				
			}
			catch ( HVBaseException &e) 
			{
				ATLTRACE("Exception occured when saving device's configuration: %d (%s)\n", e.Error(), e.Description());
			}

			// Save window Layout  
			Device2ChildFrame_t::iterator wnd = m_Device2ChildFrame.find(pDevice);
			if ( wnd != m_Device2ChildFrame.end() ){
				wnd->second->m_View.SaveLayout();
				
				// delete plugins and free resources
				wnd->second->m_PluginManager.UnloadPlugins();
				
			}

			// delete device and cancel all operations
			delete pDevice;
			pDevice = NULL;
		}     
	}
	m_DevNumber2Device.clear();
}




//------------------------------------------------------------------------------
// void CDeviceManager::ReportError(BcamException& e)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
* Convenience function to show an error message
*
*/
//------------------------------------------------------------------------------
void CDeviceManager::ReportError(HVBaseException& e)
{
	m_MainFrame.ReportError(e);
}