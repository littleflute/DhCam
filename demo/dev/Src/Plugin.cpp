// Plugin.cpp: implementation of the CPlugin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvperf.h"
#include "Plugin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlugin::CPlugin(CString &filename):
  m_handle(NULL),
  m_filename(filename),
  m_hPlugin(NULL),
  m_fActive(false)
{
	m_handle = LoadLibrary(filename);
	if (m_handle) {
		CreateObject();
	}
}

CPlugin::~CPlugin()
{
	if (m_handle) {
		DestoryObject();
		FreeLibrary(m_handle);
	}
}


void CPlugin::CreateObject()
{
	assert(m_handle != NULL);
	FARPROC pFunc = GetProcAddress(m_handle, "PluginCreateObject");
	if (pFunc ) {
		m_hPlugin = (* (PLUGIN_CreateObject *)pFunc)(); 
	}
}


void CPlugin::DestoryObject()
{
	assert(m_handle != NULL);
	
	if (m_hPlugin == NULL)
		return;

	FARPROC pFunc = GetProcAddress(m_handle, "PluginDestoryObject");
	if (pFunc ) {
		(* (PLUGIN_DestoryObject *)pFunc)(m_hPlugin); 
		m_hPlugin = NULL;
	}
}



void CPlugin::Initialize(CCamera &camera, CManager &manager) const
{
	assert(m_handle != NULL);
	assert(m_hPlugin != NULL);
	FARPROC pFunc = GetProcAddress(m_handle, "PluginInitialize");
	if (pFunc ) {
		(* (PLUGIN_Initialize *)pFunc)(m_hPlugin, camera, manager); 
	}
}


void CPlugin::Uninitialize() const
{
	assert(m_handle != NULL);
	assert(m_hPlugin != NULL);
	FARPROC pFunc = GetProcAddress(m_handle, "PluginUninitialize");
	if (pFunc ) {
		(* (PLUGIN_Uninitialize *)pFunc)(m_hPlugin); 
	}
}



CString CPlugin::GetName()
{
	CString name;

	if (m_hPlugin == NULL)
		return name;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginGetName");
	if (pFunc ) {
		char buffer[256] = {0};
		(* (PLUGIN_GetName *)pFunc)(m_hPlugin, buffer); 
		buffer[255] = 0;
		name.Format("%s", buffer);
	}

	return name;
}





CString CPlugin::GetInfo()
{
	CString info;

	if (m_hPlugin == NULL)
		return info;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginGetInfo");
	if (pFunc ) {
		char buffer[256] = {0};
		(* (PLUGIN_GetInfo *)pFunc)(m_hPlugin, buffer); 
		buffer[255] = 0;
		info.Format("%s", buffer);
	}

	return info;
}





DWORD CPlugin::GetSpecVersion()
{
	DWORD dwVer = 0;
	
	if (m_hPlugin == NULL)
		return dwVer;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginGetSpecVersion");
	if (pFunc ) {
		dwVer = (* (PLUGIN_GetSpecVersion *)pFunc)(m_hPlugin); 
	}

	return dwVer;
}




void CPlugin::Activate(CCamera &camera, CManager &manager)
{	
	if (m_hPlugin == NULL || m_fActive)
		return;
	
	Initialize(camera, manager);
	m_fActive = true;
}




void CPlugin::Deactivate()
{	
	if (m_hPlugin == NULL || (! m_fActive))
		return;

	Uninitialize();
	m_fActive = false;
}



bool CPlugin::OnUserCommand(unsigned int id)
{
	bool flag = false;
	if (m_hPlugin == NULL)
		return false;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnUserCommand");
	if (pFunc ) {
		flag = (* (PLUGIN_OnUserCommand *)pFunc)(m_hPlugin, id); 
	}
	return flag;
}



void CPlugin::OnManagerNotify(CImageInfo &info)
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnManagerNotify");
	if (pFunc ) {
		(* (PLUGIN_OnManagerNotify *)pFunc)(m_hPlugin, info); 
	}
}

void CPlugin::OnManagerNotify(CConvertImageInfo  *pImageInfo)		//v1.1
{
	if (m_hPlugin == NULL)
		return ;
	
	ATLTRACE("HVPerf: Plugin OnManagerNotify 1");
					
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnManagerNotify");
	if (pFunc ) {
		(* (PLUGINV11_OnManagerNotify *)pFunc)(m_hPlugin, pImageInfo); //
	}
	ATLTRACE("HVPerf: Plugin OnManagerNotify 2");
}


void CPlugin::OnStartGrabbing(CImageInfo &info)
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnStartGrabbing");
	if (pFunc ) {
		(* (PLUGIN_OnStartGrabbing *)pFunc)(m_hPlugin, info); 
	}
}

void CPlugin::OnStartGrabbing(CConvertImageInfo  *pImageInfo)		//v1.1
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnStartGrabbing");
	if (pFunc ) {
		(* (PLUGINV11_OnStartGrabbing *)pFunc)(m_hPlugin, pImageInfo); //
	}
}


void CPlugin::OnStopGrabbing(CImageInfo &info, CImageData &data)
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnStopGrabbing");
	if (pFunc ) {
		(* (PLUGIN_OnStopGrabbing *)pFunc)(m_hPlugin, info, data); 
	}
}


void CPlugin::OnStopGrabbing(CConvertImageInfo  *pImageInfo, CConvertImageData *pImageData)		//0409	v1.1
{
	if (m_hPlugin == NULL)
		return ;
	
	ATLTRACE("HVPerf: Plugin OnStopGrabbing 1");
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnStopGrabbing");
	if (pFunc ) {
		(* (PLUGINV11_OnStopGrabbing *)pFunc)(m_hPlugin, pImageInfo, pImageData); 
	}
	ATLTRACE("HVPerf: Plugin OnStopGrabbing 1");
}



void CPlugin::OnShowFrame(CImageInfo &info, CImageData &data)
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnShowFrame");
	if (pFunc ) {
		(* (PLUGIN_OnShowFrame *)pFunc)(m_hPlugin, info, data); 
	}
}


void CPlugin::OnShowFrame(CConvertImageInfo  *pImageInfo, CConvertImageData *pImageData)				// 0409  v1.1
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnShowFrame");
	if (pFunc ) {
		(* (PLUGINV11_OnShowFrame *)pFunc)(m_hPlugin, pImageInfo, pImageData); 
	}
}


void CPlugin::OnPaletteSetting(void *PalEntry)
{
	if (m_hPlugin == NULL)
		return ;
	
	FARPROC pFunc = GetProcAddress(m_handle, "PluginOnPaletteSetting");
	if (pFunc ) {
		(* (PLUGIN_OnPaletteSetting *)pFunc)(m_hPlugin, PalEntry); 
	}
}