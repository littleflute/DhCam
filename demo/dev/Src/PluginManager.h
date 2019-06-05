// PluginManager.h: interface for the CPluginManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGINMANAGER_H__2635ED11_7AA2_412D_B4AA_A39B60F50C1B__INCLUDED_)
#define AFX_PLUGINMANAGER_H__2635ED11_7AA2_412D_B4AA_A39B60F50C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChildFrame;
class CDevice;
class CMainFrame;

#include "Plugin.h"

class CPluginManager  
{
public:
	CPluginManager(CDevice *pDevice, CChildFrame &ChildFrame, CMainFrame &MainFrame);
	virtual ~CPluginManager();

	/// 
	std::vector<CPlugin *> m_PluginsSet;

	void LoadPlugins();
	void UnloadPlugins();

	/// 
	bool OnUserCommand(UINT id);
	/// 
	void OnManagerNotify(CPlugin *pPlugin, CImageInfo &info);
	/// 
	void OnStartGrabbing(CPlugin *pPlugin, CImageInfo &info);
	/// 
	void OnStopGrabbing(CPlugin *pPlugin, CImageInfo &info, CImageData &data);
	/// 
	void OnShowFrame(CPlugin *pPlugin, CImageInfo &info, CImageData &data);
	/// 
	void OnPaletteSetting(void *PalEntry);

	/// 
	void PluginActivate(CPlugin *pPlugin);
	void PluginDeactivate(CPlugin *pPlugin);
	/*----plugin v1.1 by xupx----*/
	DWORD dwPluginVersion;
	void OnManagerNotify(CPlugin *pPlugin, CConvertImageInfo *pImageInfo);
	void OnStartGrabbing(CPlugin *pPlugin, CConvertImageInfo *pImageInfo);
	void OnStopGrabbing(CPlugin *pPlugin, CConvertImageInfo *pImageInfo, CConvertImageData *pImageData);
	void OnShowFrame(CPlugin *pPlugin, CConvertImageInfo  *pImageInfo, CConvertImageData *pImageData);
	

private:
	std::vector<CPlugin *> PluginSet() { return m_PluginsSet; }
	/// 
	void RebuildPluginSet(std::vector<CPlugin *> &PluginSet) ;

private:
	CDevice				*m_pDevice; 
	/// reference to the camera's child frame
	CChildFrame          &m_ChildFrame;
	/// 
	CMainFrame			 &m_MainFrame;

	friend class CPluginManagerDlg;
};

#endif // !defined(AFX_PLUGINMANAGER_H__2635ED11_7AA2_412D_B4AA_A39B60F50C1B__INCLUDED_)
