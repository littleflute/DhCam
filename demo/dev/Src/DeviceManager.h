//-----------------------------------------------------------------------------
//  (c) 2004 by Daheng Imavision 
//  Author: ilovedrv
//  Data: 2004.03.15
//-----------------------------------------------------------------------------
/**
*
* \brief   Interface for the CDeviceManager class.
*
*/
//-----------------------------------------------------------------------------

#if !defined(AFX_DEVICEMANAGER_H__AFBE39EB_B98A_400C_89B2_EC36A2AF5EDA__INCLUDED_)
#define AFX_DEVICEMANAGER_H__AFBE39EB_B98A_400C_89B2_EC36A2AF5EDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainFrame;
class CChildFrame;
#include "Device.h"

#define	PLUGIN_MENU_POSITION		4

//------------------------------------------------------------------------------
// class CDeviceManager
// Author: 
// Date: 
//------------------------------------------------------------------------------
/**
* \brief   Manages CDevice objects  and their associated MDI child windows
*
* The device manager holds a map, which maps the device names of Bcam devices 
* attached to the 1394 bus to CDevice objects representing the devices. CDevice 
* inherits from CBcam. 
* The device manager holds a second map, which maps the CDevice objects to associated
* Image views, showing live images etc.
*
* The device knows which device is the "current device", i.e. the device selected 
* in the bus viewer's tree view. The application's main frame communicates with the
* device manager to start stop continuous grabs, single grabs etc.
*
* The device manager is also repsonsible for the creation of image windows.
*/
//------------------------------------------------------------------------------
class CDeviceManager  
{
public:
	CDeviceManager(CMainFrame &MainFrame);
	virtual ~CDeviceManager();
	/// Returns the device object for a given device name
	CDevice* GetDevice(int nNumber);
	/// Returns the device object for a given child frame
	CDevice* GetDevice(CChildFrame *pChild);
	/// The device manager gets informed that the current device has been changed
	DWORD CurrentDeviceChanged(int nNumber);
	/// Returns the current device
	CDevice *GetCurrentDevice() { return m_pCurrentDevice; }
	/// The device manager gets informed that an image window has been closed
	void MDIChildClosed(CChildFrame* pChild);
	/// The device manager gets informed that image window has been activated
	void  MDIChildActivated(CChildFrame* pChild);
	/// Add a new device to the device manager's list of devices
	BOOL AddDevice(int nNumber, HWND hWndNotify);
	/// Delete a device which has been removed from the system
	BOOL RemoveDevice(int nNumber);
	/// Create a new image window associated with the current device
	void AddMDIChild();
	/// Returns the image window assiciated with the current device 
	CChildFrame *GetCurrentChild();
	/// Is there already a image window for the current device
	bool ExistMDIChild();
	/// Check if a given device object is in the device manager's list of devices
	bool IsDeviceValid(CDevice* pDevice);
	
	/// Is the current device grabbing?
	bool IsGrabActive();
	
	/// Let the current device grag an image
	void GrabSingle();
	/// Let the current device grab continuosly images
	void GrabContinuous();
	/// Cancel the current device's grab
	void GrabCancel();

	/// Load an image from a file show it in the image window associated with the current device
	void GrabFromFile(CString FileName);
	/// Shows the pixel value under the cursor in the main frame's status bar
	void ShowPixelValue();
	/// Retrieves the current frame rate
	void GetFrameRate(double& fps_displayed);
	
	/// The device manager get's informed that the application is to be closed. The settings of the current device will be saved to the registry
	void AppExit(bool b);
	
    void OnSoftTrigger();

	void XD_Show_All_FPS();
	
private:
	/// Display an error message
	void ReportError(HVBaseException& e);
	
	/// maps device names to device objects
	typedef map<int, CDevice*> DevNumber2Device_t;
	DevNumber2Device_t      m_DevNumber2Device;
	/// maps device objects to child Device
	typedef map<CDevice*, CChildFrame*> Device2ChildFrame_t;
	Device2ChildFrame_t   m_Device2ChildFrame;
	
	/// reference to the application's main frame
	CMainFrame          &m_MainFrame;
	/// pointer to the current device
	CDevice				*m_pCurrentDevice;
	/// The name of the device which is currently selected. In general this should be
	/// the name of the current device. But if the user has selected a non Bcam device in the
	/// bus view, the m_pSelectedDeviceName is "", but the current device might not be NULL 
	/// (i.e. a child window is active for the current device)
	int					m_SelectedDevice;
};

#endif // !defined(AFX_DEVICEMANAGER_H__AFBE39EB_B98A_400C_89B2_EC36A2AF5EDA__INCLUDED_)
