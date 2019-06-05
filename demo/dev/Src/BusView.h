//------------------------------------------------------------------------------
// class CBusView
// Author: ilovedrv
// Date: 2004.03.12
//------------------------------------------------------------------------------
/**
* \brief   Tree view which shows the devcies found on the usb bus
*
* This class implements the bus viewer window, 
* which allows the user to navigate the Bcam devices found on the usb bus.
* The bus viewer window is registered for Bcam related device notifications 
* sent by the plug and play manager. New devices are added to the tree, unplugged
* devices are removed.
* 
*/

#if !defined(AFX_BUSVIEW_H__F4CED1A3_9CED_4DF5_9C91_692B10F7E930__INCLUDED_)
#define AFX_BUSVIEW_H__F4CED1A3_9CED_4DF5_9C91_692B10F7E930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "dialogview.h"

class CMainFrame;
class CDeviceManager;

class CBusView : public CDialogView<CBusView>  
{
public:
	enum {IDD = IDD_BUSVIEW};
	
	BEGIN_MSG_MAP(CBusView)
		CHAIN_MSG_MAP(CDialogView<CBusView>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		NOTIFY_HANDLER(IDC_BUSTREE, TVN_SELCHANGED, OnSelChanged)
		NOTIFY_HANDLER(IDC_BUSTREE, NM_DBLCLK, OnLButtonDblClk)
		REFLECT_NOTIFICATIONS()
		END_MSG_MAP()
		
		// Handler prototypes (uncomment arguments if needed):
		//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
		//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
		//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
		
		CBusView(CDeviceManager& DeviceManager, CMainFrame& MainFrame);
	
public:
	const char* UNABLE_TO_OPEN;
	
protected:
	
	
	//------------------------------------------------------------------------------
	// class CBusView::CBusNode
	// Author: 
	// Date: 
	//------------------------------------------------------------------------------
	/**
	*  \brief Base class for bus nodes 
	*/
	//------------------------------------------------------------------------------
	class CBusNode 
	{
	public: 
		CBusNode(CString devName, int nNumber, HVTYPE type) :
		  m_DeviceName(devName),
			  m_nNumber(nNumber),
			  m_Type(type)
		  {};
		  virtual ~CBusNode() { ATLTRACE("~CBusNode()\n");};
		  
		  /// get Win32 device name
		  int GetDeviceNumber() const {return m_nNumber;}
		  /// get Win32 device name
		  CString GetDeviceName() const {return m_DeviceName;}
		  /// get type of node
		  HVTYPE GetType() const {return m_Type;}
		  
	protected:
		CString m_DeviceName;
		HVTYPE m_Type;
		int  m_nNumber;
	}; 
	
	/// map tree item handels to associated CBusNode objects
	typedef map<HTREEITEM, CBusNode*> NodeMap_t;
	
	public:
		/// A client informs us that the current device has changed ( e.g. the user activated an other 
		/// image view associated with an other device)
		void CurrentDeviceChanged(CDevice* pdevice);
		
	protected:
		// Message Handlers
		LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
		/// Another tree item is selected
		LRESULT OnSelChanged(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		/// User has double clicked on a tree item
		LRESULT OnLButtonDblClk(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
		
		/// Invoke the error message box
		void ReportError(HVBaseException& e);
		
		/// Add a new device to the tree.
		void AddNode(int nNumber);
		/// Remove a node from the tree
		void RemoveNode(int nNumber);
		/// bool check if the device list alread contains a node for a given device
		bool ContainsNode(int nNumber);
		/// Check device list for consistency
		void UpdateDeviceList();
		
		/// reference to the viewer's camera manager object
		CDeviceManager          &m_DeviceManager;
		/// reference to the application's main window
		CMainFrame              &m_MainFrame;
		/// tree view control
		CTreeViewCtrl           m_TreeView;
		/// list with icons displayed in the tree view
		CImageList              m_ImageList;
		/// root item of the tree
		CTreeItem               m_RootItem;
		/// map which maps the tree items to node objects
		NodeMap_t               m_NodeMap;
		
	private:
		friend class CAutoTest;
};

#endif // !defined(AFX_BUSVIEW_H__F4CED1A3_9CED_4DF5_9C91_692B10F7E930__INCLUDED_)
