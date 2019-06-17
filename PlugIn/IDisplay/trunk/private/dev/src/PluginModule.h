// PluginModule.h: interface for the CPluginModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_)
#define AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/**The different image storage formats supported.*/
enum IMAGEFORMAT
{
    FORMAT_Mono8 = 0,     ///< Y component has 8bit data
	FORMAT_YUV8_4_1_1,    ///< 4:1:1 YUV 8 format, each component has 8bit data
	FORMAT_YUV8_4_2_2,    ///< 4:2:2 YUV 8 format, each component has 8bit data
	FORMAT_YUV8_4_4_4,    ///< 4:4:4 YUV 8 format, each component has 8bit data
	FORMAT_RGB8,          ///< RGB 8 format, each component has 8bit data
	FORMAT_Mono16,        ///< Y component has 16bit data
	FORMAT_RGB16          ///< RGB 16 format, each component has 16bit data
};


struct CImageInfo {
	unsigned long ImageWidth;       // Image width in pixel
    unsigned long ImageHeight;      // Image height in pixel
    unsigned long ImageBitDepth;    // Image depth in bits (8,16,24,32)
    unsigned long ImageBitDepthReal;// Precise Image depth (x bits)
    unsigned long ImageSizeBytes;   // Size used to store one image.
    IMAGEFORMAT ImageFormat;       // Planar, Packed, color or mono

};



struct CImageData {
	void *pRawBuffer;
	void *pImageBuffer;
};

struct CCamera {
	HANDLE	hDevice;
	char   *pName;
};


struct CManager {
	HANDLE	hWndParent;
	HANDLE	hMenu;
	char   *pRegistryPath;
};

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
	virtual void OnManagerNotify(CImageInfo info) {}
	///
	virtual void OnStartGrabbing(CImageInfo info) {}
	///
	virtual void OnStopGrabbing(CImageInfo info, CImageData data) {}
	///
	virtual void OnShowFrame(CImageInfo info, CImageData data) {}
};

#endif // !defined(AFX_PLUGINMODULE_H__C0B706B7_0577_46D4_87ED_A1FDC6E74C82__INCLUDED_)
