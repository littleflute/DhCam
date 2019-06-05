// PluginManager.cpp: implementation of the CPluginManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hvperf.h"
#include "PluginManager.h"
#include "ChildFrm.h"
#include "Device.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPluginManager::CPluginManager(CDevice *pDevice, CChildFrame &ChildFrame, CMainFrame &MainFrame):  
	m_pDevice(pDevice),
	m_ChildFrame(ChildFrame),
	m_MainFrame(MainFrame)
{
}


CPluginManager::~CPluginManager()
{
	for ( std::vector<CPlugin *>::iterator it = m_PluginsSet.begin(); it != m_PluginsSet.end(); ++ it ){
		delete *it;
	}
}



void CPluginManager::PluginActivate(CPlugin *pPlugin)
{
	CCamera camera = {0};
	CManager manager = {0};
	
	CString szDeviceName	= m_pDevice->m_pInfo->DeviceName();
	camera.hDevice			= m_pDevice->m_hDevice;
	camera.pName			= (char *)(LPCTSTR)szDeviceName;
	
	CString keyName = APPLICATION_KEY + m_pDevice->m_pInfo->DeviceName();
	CString filename = pPlugin->GetFileName();
	int i = filename.ReverseFind( '\\' ); 	
	if ( i == -1) {
		i = filename.ReverseFind( ':' ); 
	}
	if (i >= 0 ) {
		filename = filename.Right(filename.GetLength() -1 - i ); 
	}
	
	CString szRegistryPath	= keyName + "\\Plugin\\" + filename;
	manager.hMenu			= m_MainFrame.GetPluginSubMenu();
	manager.hWndParent		= m_ChildFrame.m_hWnd;
	manager.pRegistryPath	= (char *)(LPCTSTR)szRegistryPath;
	pPlugin->Activate(camera, manager);
	
	dwPluginVersion = pPlugin->GetSpecVersion();
	if(dwPluginVersion == 0x00010000)
	{
		CImageInfo info = {0};
		m_pDevice->GetImageInfo(info);
		pPlugin->OnManagerNotify(info);
	}
	else										
	{
		CConvertImageInfo  *pImageInfo = new CConvertImageInfo ;	//v1.1
		m_pDevice->GetImageInfo(pImageInfo);	
		pPlugin->OnManagerNotify(pImageInfo);
		delete pImageInfo;
	}
	
}



void CPluginManager::PluginDeactivate(CPlugin *pPlugin)
{
	pPlugin->Deactivate();
	ATLTRACE("PluginDeactivate 2 ~~~~~~~~~~~~");
}



void CPluginManager::LoadPlugins()
{
	CString szSearchPath, szPluginPath;
	char szBuffer[MAX_PATH];
	::GetModuleFileName(NULL, szBuffer, MAX_PATH - 1);
	szPluginPath.Format("%s", szBuffer);
	int i = szPluginPath.ReverseFind( '\\' ); 	
	if ( i == -1) {
		i = szPluginPath.ReverseFind( ':' ); 
	}
	if (i >= 0 ) {
		szPluginPath = szPluginPath.Left(i); 
	}
	szPluginPath += "\\PlugIns\\";
	szSearchPath = szPluginPath + "*.plx";

	typedef map<CString, int> Name2Index_t;
	Name2Index_t Name2Index;

	WIN32_FIND_DATA wfd = {0};
	HANDLE handle = FindFirstFile(szSearchPath, &wfd);
	
	if (INVALID_HANDLE_VALUE == handle)
		return;

	bool fFirst = false;
	CPropertyBagPtr ptrRootBag, ptrPluginRootBag, ptrPluginBag;
	CString keyName = APPLICATION_KEY + m_pDevice->m_pInfo->DeviceName();
	try{
		ptrRootBag		= CRegistryPropertyBag::Open(keyName);
		ptrPluginRootBag	= ptrRootBag->GetBag("Plugin");
	}
	catch ( HVBaseException& e){
		if ( e.Error() == STATUS_INTERNAL_ERROR )
			fFirst = true;
		else
			throw e;
	}

	do {
		if (fFirst) {	
			bool fDefault = true;
			CString commonName;
			commonName.Format("%s%s%s", APPLICATION_KEY, "Plugin\\", wfd.cFileName);
			CPropertyBagPtr ptrCommonPluginBag;
			try{
				ptrCommonPluginBag = CRegistryPropertyBag::Open(commonName);
				Name2Index[wfd.cFileName] = ptrCommonPluginBag->ReadLong("index");;
			}
			catch ( HVBaseException& e){
				if ( e.Error() == STATUS_INTERNAL_ERROR )
					fDefault = false;
				else
					throw e;
			}
			if ( ! fDefault){
				Name2Index[wfd.cFileName] = -1;
			}
		}
		else {
			try{
				ptrPluginBag	= ptrPluginRootBag->GetBag(wfd.cFileName);
				i	= ptrPluginBag->ReadLong("index");
			}
			catch ( HVBaseException& e){
				if ( e.Error() == STATUS_INTERNAL_ERROR )
					i = -1;
				else
					throw e;
			}
			
			if ((i >= 256) || (i <= -1)) {
				i = -1;
			}
			else {
				for ( Name2Index_t::iterator it = Name2Index.begin(); it != Name2Index.end(); ++ it){
					if ( (it->second == i) ){
						i = -1;
						break;
					}
				}
			}
			Name2Index[wfd.cFileName] = i;
		}
	} while(FindNextFile(handle, &wfd));
	FindClose(handle);
	
	int oldnum = 0;
	for ( Name2Index_t::iterator it = Name2Index.begin(); it != Name2Index.end(); ++ it){
		if ( (it->second != -1) ){
			++oldnum;
		}
	}
	
	for (i = 0; i < oldnum; ++i){
		Name2Index_t::iterator it = NULL, next = NULL;
		for ( it = next = Name2Index.begin(); it != Name2Index.end(); ++ it){
			ATLTRACE("it->first : %s, it->second : %d\n", it->first, it->second);
			if ( (it->second == i) ){
				break;
			}
			else if ( it->second > i ) {			
				ATLTRACE("next->first : %s, next->second : %d\n", next->first, next->second);
				if ( (next->second == -1) || (next->second <= i) || (next->second > it->second) ){
					next = it;
				}
			}
		}
		if ( (it->second != i) ){
			ATLTRACE("the index of %s is changed from %d to %d \n", next->first, next->second, i);
			next->second = i;
		}
	}
	for (i = oldnum; i < Name2Index.size(); ++i){
		for ( Name2Index_t::iterator it = Name2Index.begin(); it != Name2Index.end(); ++ it){
			if ( (it->second == -1) ){
				it->second = i;
				break;
			}
		}
	}

	bool bIDisplay = false;
	for (i = 0; i < Name2Index.size(); ++i){
		for ( Name2Index_t::iterator it = Name2Index.begin(); it != Name2Index.end(); ++ it){
			ATLTRACE("it->second : %d\n", it->second);
			if ( (it->second == i) ){
				CPlugin *pPlugin = new CPlugin(szPluginPath + it->first);	
				if (pPlugin != NULL) {
					m_PluginsSet.push_back(pPlugin);
				
					bool fActive = false;
					CString filename = pPlugin->GetFileName();
					int i = filename.ReverseFind( '\\' ); 	
					if ( i == -1) {
					  i = filename.ReverseFind( ':' ); 
					}
					if (i >= 0 ) {
  						filename = filename.Right(filename.GetLength() -1 - i ); 
					}
					if(	filename=="_IConvert.plx" || filename=="IConvert.plx"){fActive = true; bIDisplay = true;}

					if ( fFirst){
						CString commonName;
						commonName.Format("%s%s%s", APPLICATION_KEY, "Plugin\\", filename);
						CPropertyBagPtr ptrCommonPluginBag;
						try{
							ptrCommonPluginBag = CRegistryPropertyBag::Open(commonName);
							fActive = ptrCommonPluginBag->ReadBool("active");
						}
						catch ( HVBaseException& e){
							if ( e.Error() != STATUS_INTERNAL_ERROR )
								throw e;
						}
					}
					else{
						try{	
							ptrPluginBag = ptrPluginRootBag->GetBag(filename);
							fActive = ptrPluginBag->ReadBool("active");

							if(filename=="_IConvert.plx" || filename=="IConvert.plx") {fActive = 1;}

						}
						catch ( HVBaseException& e){
							if ( e.Error() == STATUS_INTERNAL_ERROR )
								fActive = false;
							else
								throw e;
						}
					}
					
					if (fActive){
						PluginActivate(pPlugin);
					}
				}
				break;
			}
		}
	}
	if(bIDisplay == false){
		
        TCHAR   szTemp[64];
        wsprintf( szTemp, "没有找到 _IConvert.plx 或者 IConvert.plx的显示插件! \n\r请确认 [PlugIns] 目录下是否有此文件!\n\r IConvert.plx插件是Raw2RGB.plx的升级版" );
        MessageBox( NULL, szTemp, "Error", MB_OK );  //xupx
	}
}




void CPluginManager::UnloadPlugins()
{
	CPropertyBagPtr ptrRootBag, ptrPluginRootBag, ptrPluginBag;
	CString keyName = APPLICATION_KEY + m_pDevice->m_pInfo->DeviceName();
	
	try{
		ptrRootBag = CRegistryPropertyBag::Open(keyName);
	}
	catch ( HVBaseException& e){
		if ( e.Error() == STATUS_INTERNAL_ERROR )
			ptrRootBag = CRegistryPropertyBag::Create(keyName);
		else
			throw e;
	}
	try{
		ptrPluginRootBag = ptrRootBag->GetBag("Plugin");
	}
	catch ( HVBaseException& e){
		if ( e.Error() == STATUS_INTERNAL_ERROR )
			ptrPluginRootBag = ptrRootBag->CreateBag("Plugin");
		else
			throw e;
	}

	CPlugin *pPlugin = NULL;
	std::vector<CPlugin *>::iterator it;
	for ( it = m_PluginsSet.begin(); it != m_PluginsSet.end(); ++ it){
		if ( pPlugin = *it ){
			CString filename = pPlugin->GetFileName();
			int i = filename.ReverseFind( '\\' ); 	
			if ( i == -1) {
			  i = filename.ReverseFind( ':' ); 
			}
			if (i >= 0 ) {
  				filename = filename.Right(filename.GetLength() -1 - i ); 
			}
			try{	
				ptrPluginBag = ptrPluginRootBag->GetBag(filename);
			}
			catch ( HVBaseException& e)
			{
				if ( e.Error() == STATUS_INTERNAL_ERROR )
					ptrPluginBag = ptrPluginRootBag->CreateBag(filename);
				else
					throw e;
			}
			ptrPluginBag->WriteLong("index", it - m_PluginsSet.begin());
			ptrPluginBag->WriteBool("active", pPlugin->IsActive());
			if (pPlugin->IsActive()){
				PluginDeactivate(pPlugin);
			}
			delete pPlugin;
		}
	}

	m_PluginsSet.clear();
}



bool CPluginManager::OnUserCommand(UINT id)
{
	bool flag = false;
	CPlugin *pPlugin = NULL;
	for ( std::vector<CPlugin *>::iterator it = m_PluginsSet.begin(); it != m_PluginsSet.end(); ++ it ){
		if ((pPlugin = *it) && (pPlugin->IsActive())) {
			flag = pPlugin->OnUserCommand( id );
			if (flag){
				break;
			}
		}
	}
	return flag;
}




void CPluginManager::OnManagerNotify(CPlugin *pPlugin, CImageInfo &info)
{
		if (pPlugin->IsActive()) {
			pPlugin->OnManagerNotify(info);
		}
}

void CPluginManager::OnManagerNotify(CPlugin *pPlugin, CConvertImageInfo *pImageInfo)		//v1.1  
{
	if (pPlugin->IsActive()) {
		pPlugin->OnManagerNotify(pImageInfo);
	}
}



void CPluginManager::OnStartGrabbing(CPlugin *pPlugin, CImageInfo &info)
{
	if (pPlugin->IsActive()) {
		pPlugin->OnStartGrabbing(info);
	}
}


void CPluginManager::OnStartGrabbing(CPlugin *pPlugin, CConvertImageInfo *pImageInfo)		//v1.1  0409
{
	if (pPlugin->IsActive()) {
		pPlugin->OnStartGrabbing(pImageInfo);
	}
}




void CPluginManager::OnStopGrabbing(CPlugin *pPlugin, CImageInfo &info, CImageData &data)
{
	if (pPlugin->IsActive()) {
		pPlugin->OnStopGrabbing(info, data);
	}
}




void CPluginManager::OnStopGrabbing(CPlugin *pPlugin, CConvertImageInfo *pImageInfo, CConvertImageData *pImageData)	//v1.1  0409
{
	if (pPlugin->IsActive()) {
		pPlugin->OnStopGrabbing(pImageInfo, pImageData);
	}
}





void CPluginManager::OnShowFrame(CPlugin *pPlugin, CImageInfo &info, CImageData &data)
{
	if (pPlugin->IsActive()) {
		pPlugin->OnShowFrame(info, data);
	}
}



void CPluginManager::OnShowFrame(CPlugin *pPlugin, CConvertImageInfo  *pImageInfo, CConvertImageData *pImageData)	//v1.1  0409
{
	if (pPlugin->IsActive()) {
		pPlugin->OnShowFrame(pImageInfo, pImageData);
	}
}




void CPluginManager::RebuildPluginSet(std::vector<CPlugin *> &PluginSet)
{
	CDevice *pDevice = m_ChildFrame.m_DeviceManager.GetDevice(&m_ChildFrame);
	if (pDevice){
		if (pDevice->ReconfigurationRequest()){
			m_PluginsSet = PluginSet;
			pDevice->ConfigurationChanged();
		}
	}
}



void CPluginManager::OnPaletteSetting(void *PalEntry)
{
	CPlugin *pPlugin = NULL;
	for ( std::vector<CPlugin *>::iterator it = m_PluginsSet.begin(); it != m_PluginsSet.end(); ++ it ){
		if ((pPlugin = *it) && (pPlugin->IsActive())) {
			pPlugin->OnPaletteSetting(PalEntry);
		}
	}
}



