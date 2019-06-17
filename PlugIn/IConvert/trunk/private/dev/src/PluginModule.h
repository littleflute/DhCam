// PluginModule.h: interface for the CPluginModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_)
#define AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct CImageV11Info {
	unsigned long iV11Size;		//���ṹ�Ĵ�С�����ֽڼƣ�
	HV_COLOR_CODE iV11ColorCode;	//ͼ������ݸ�ʽ
	unsigned long iV11Width;		//ͼ��Ŀ��
	unsigned long iV11Height;	//ͼ��ĸ߶�
	unsigned long iV11BitCount;	//ͼ���ÿ��������ռλ��
	unsigned long iV11DataDepth;	//ͼ���ÿ��������ռ��Чλ��
	unsigned long iV11PixelNumber;	//ͼ���������
	unsigned long iV11SizeImage;	//ͼ��Ĵ�С�����ֽڼƣ�
};

struct CConvertImageInfo {
	unsigned long ciiSize;		//���ṹ�Ĵ�С�����ֽڼƣ�
	CImageV11Info ciiDest;	//Ŀ��ͼ���������Ϣ
	CImageV11Info ciiSrce;	//Դͼ���������Ϣ
};

struct CConvertImageData {
	unsigned long cidSize;		//���ṹ�Ĵ�С�����ֽڼƣ�
	void *pCidDest;	//ָ��Ŀ��ͼ������ݻ�����
	void *pCidSrce;	//ָ��Դͼ������ݻ�����
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
