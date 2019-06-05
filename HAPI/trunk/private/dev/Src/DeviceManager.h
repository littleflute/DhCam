// DeviceManager.h: interface for the CDeviceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEVICEMANAGER_H__F0742292_E56B_4BFE_8B12_72A12EE03414__INCLUDED_)
#define AFX_DEVICEMANAGER_H__F0742292_E56B_4BFE_8B12_72A12EE03414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "local.h"
extern int CAMERA_KINDS;
class CDeviceManager  
{
public:
	void _Get_Device_Type_List(HMODULE hModule);
	bool Do_Enumerate(HV_DEVICE_INTERFACE *pDevIF,DWORD dwBytes, DWORD *pdwBytes);
	CDeviceManager();
	virtual ~CDeviceManager();

private:
	HMODULE v_Load_Dll(int n);
	 
};

#endif // !defined(AFX_DEVICEMANAGER_H__F0742292_E56B_4BFE_8B12_72A12EE03414__INCLUDED_)
