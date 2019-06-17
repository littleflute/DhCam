/************************************************************************/
/*                                                                      */
/*  Copyright (C) 2005 by DaHeng Image Vision Company.                  */
/*  All rights reserved. Property of DaHeng Image Vision Company.       */
/*  Restricted rights to use, duplicate or disclose this code are       */
/*  granted through contract.                                           */
/*                                                                      */
/*----------------------------------------------------------------------*/
/*                                                                      */
/*  Name:			Raw2Rgb_Plug.cpp                                    */
/*  Description:	This file defines the interface of color plug       */
/*  Author:         Michael Chang				                        */
/*  Date:			2005-04-025                                         */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "DlgSetParameter.h"


//////////////////////////////////////////////////////////////////////////

extern "C" HANDLE __stdcall PluginCreateObject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule * pPlugin = new CDlgSetParameter;
	return pPlugin;
}


extern "C" void __stdcall PluginDestoryObject(HANDLE handle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	delete pPlugin;
}



extern "C" void __stdcall PluginGetName(HANDLE handle, char *buffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL || buffer == NULL)
		return;

	pPlugin->GetName(buffer);
}


extern "C" void __stdcall PluginGetInfo(HANDLE handle, char *buffer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL || buffer == NULL)
		return;

	pPlugin->GetInfo(buffer);
}




extern "C" DWORD __stdcall PluginGetSpecVersion(HANDLE handle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return 0;

	return pPlugin->GetSpecVersion();
}




extern "C" void __stdcall PluginInitialize(HANDLE handle, CCamera hCamera, CManager hManager)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return ;

	pPlugin->Initialize(hCamera, hManager);
}



extern "C" void __stdcall PluginUninitialize(HANDLE handle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return ;

	pPlugin->Uninitialize();
}

extern "C" bool __stdcall PluginOnUserCommand(HANDLE handle, unsigned int id)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return false ;
	
	return pPlugin->OnUserCommand(id);
}

extern "C" void __stdcall PluginOnManagerNotify(HANDLE handle, CImageInfo info)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return ;
	
	pPlugin->OnManagerNotify(info);
}


extern "C" void __stdcall PluginOnStartGrabbing(HANDLE handle, CImageInfo info)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return ;

	pPlugin->OnStartGrabbing(info);
}




extern "C" void __stdcall PluginOnStopGrabbing(HANDLE handle,CImageInfo info, CImageData data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState);
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return ;

	pPlugin->OnStopGrabbing(info, data);
}




extern "C" void __stdcall PluginOnShowFrame(HANDLE handle, CImageInfo info, CImageData data)
{
	CPluginModule *pPlugin = (CPluginModule *)handle;
	if (pPlugin == NULL )
		return ;
		
	pPlugin->OnShowFrame(info, data);
}
