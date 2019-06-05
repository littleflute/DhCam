// Plugin.h: interface for the CPlugin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGIN_H__47285370_C048_41A9_A32F_D34D16131024__INCLUDED_)
#define AFX_PLUGIN_H__47285370_C048_41A9_A32F_D34D16131024__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "PluginSpec.h"


class CPlugin  
{
public:
	CPlugin(CString &filename);
	virtual ~CPlugin();

	/// get address of plugin module
	CString GetFileName() { return m_filename; }
	/// get name of user plugin
	CString GetName();
	/// get description of user plugin
	CString GetInfo();
	/// get version of plugin specification
	DWORD GetSpecVersion();

	/// open the plugin and do some initialization
	void Activate(CCamera &camera, CManager &manager);
	/// close the plugin and do some cleanup
	void Deactivate();
	/// get state of plugin
	bool IsActive() { return m_fActive; }

	/// 
	bool OnUserCommand(unsigned int id);
	/// 
	void OnManagerNotify(CImageInfo &info);
	///  
	void OnStartGrabbing(CImageInfo &info);
	/// 
	void OnStopGrabbing(CImageInfo &info, CImageData &data);
	/// 
	void OnShowFrame(CImageInfo &info, CImageData &data);

	/// 
	void OnPaletteSetting(void *PalEntry);
	/*===========v1.1 ==============*/
	void OnManagerNotify(CConvertImageInfo *pImageInfo);	//0409  v1.1
	void OnStartGrabbing(CConvertImageInfo *pImageInfo);	//0409  v1.1
	void OnStopGrabbing(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData);	//0409  v1.1
	void OnShowFrame(CConvertImageInfo  *pImageInfo, CConvertImageData *pImageData);
	
private:
	/// hide copy constructor and assignment operator
//	CPlugin();
	CPlugin( const CPlugin& );
	CPlugin& operator=( const CPlugin& );

	void CreateObject()  ; 
	void DestoryObject(); 

	void Initialize(CCamera &camera, CManager &manager) const;
	void Uninitialize() const;

	friend class CPluginManager;
	
private:
	/// 
	HMODULE				m_handle;
	/// name of plugin file
	CString				m_filename;
	//////////////////////////////////////////////////////////////////////////
	HANDLE				m_hPlugin;
	/// is AOI maximize 
	bool				m_fActive;
};

#endif // !defined(AFX_PLUGIN_H__47285370_C048_41A9_A32F_D34D16131024__INCLUDED_)
