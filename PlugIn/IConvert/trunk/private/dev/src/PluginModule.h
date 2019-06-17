// PluginModule.h: interface for the CPluginModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_)
#define AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct CImageV11Info {
	unsigned long iV11Size;		//本结构的大小（以字节计）
	HV_COLOR_CODE iV11ColorCode;	//图像的数据格式
	unsigned long iV11Width;		//图像的宽度
	unsigned long iV11Height;	//图像的高度
	unsigned long iV11BitCount;	//图像的每个像素所占位数
	unsigned long iV11DataDepth;	//图像的每个像素所占有效位数
	unsigned long iV11PixelNumber;	//图像的像素数
	unsigned long iV11SizeImage;	//图像的大小（以字节计）
};

struct CConvertImageInfo {
	unsigned long ciiSize;		//本结构的大小（以字节计）
	CImageV11Info ciiDest;	//目标图像的描述信息
	CImageV11Info ciiSrce;	//源图像的描述信息
};

struct CConvertImageData {
	unsigned long cidSize;		//本结构的大小（以字节计）
	void *pCidDest;	//指向目标图像的数据缓存区
	void *pCidSrce;	//指向源图像的数据缓存区
};

struct CState {
	int		Index;
	bool	Active;
};
////////////////////////////////////////////////////////////////////////////////

struct CCamera {
	HANDLE	hDevice;
	char   *pName;
};


struct CManager {
	HANDLE	hWndParent;
	HANDLE	hMenu;
	char   *pRegistryPath;
};

///////////////////////////////////////////////////////////////////////////////
class CPluginModule  
{
public:
	CPluginModule() {}
	virtual ~CPluginModule() {}

public:
	/// get name of user plugin
	virtual void GetName(char *) = 0;
	/// get description of user plugin
	virtual void GetInfo(char *) = 0;
	/// get version of plugin specification
	virtual DWORD GetSpecVersion() = 0;
	/// 
	virtual void Initialize(CCamera, CManager) {}
	///
	virtual void Uninitialize() {}
	/// 
	virtual bool OnUserCommand(unsigned int id) {return false;}
	/// 
	virtual void OnManagerNotify(CConvertImageInfo *pImageInfo) {}	//Ver1.1
	///
	virtual void OnStartGrabbing(CConvertImageInfo *pImageInfo) {}	//Ver1.1
	///
	virtual void OnStopGrabbing(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData) {}	//Ver1.1
	///
	virtual void OnShowFrame(CConvertImageInfo *pImageInfo, CConvertImageData *pImageData) {}	//Ver1.1
};

#endif // !defined(AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_)
