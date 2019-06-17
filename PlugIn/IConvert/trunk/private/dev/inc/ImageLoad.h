#ifndef _INC_IMAGELOAD
#define _INC_IMAGELOAD
//保存选项
const WORD SAVE_OPTION_BMP=0x0001;  //存为BMP图象时的选项，有1，2，8，16，24，32位六个选项
const WORD SAVE_OPTION_JPG=0x0002;	//存为JPG图象时的选项，选择压缩品质
const WORD SAVE_OPTION_TIF=0x0004;	//存为TIF图象时的选项，选择压缩方式，LZW和不压缩

//函数声明
/******************************************************************************/

///////////////////////////////////////////////////////////////////
//载入图象
//lpszPathName 文件名
//返回图象句柄
//!!!特别注意：图像句柄用完后，用FreeDIBHandle()释放内存。
extern "C" HANDLE PASCAL EXPORT LoadImageFile(LPCTSTR lpszPathName);

///////////////////////////////////////////////////////////////////
//保存图象
//	lpszPathName 文件名
//	m_hDib 图象句柄
//	WORD nSaveOption 保存选项
//		可为SAVE_OPTION_BMP，SAVE_OPTION_JPG，WORD SAVE_OPTION_TIF
//		
//		的任意组合。为0则不提示保存选项。
//保存成功返回TRUE，否则返回FALSE
extern "C" BOOL PASCAL EXPORT SaveImageFile(HANDLE m_hDib,LPCTSTR lpszPathName,WORD nSaveOption=0);

///////////////////////////////////////////////////////////////////
//保存图象为JPG格式
//	lpszPathName 文件名
//	m_hDib 图象句柄
//	int nQuality 品质系数(0--100)
//	
//保存成功返回TRUE，否则返回FALSE
extern "C" BOOL PASCAL EXPORT SaveImageAsJPEG(HANDLE m_hDib,LPCTSTR lpszPathName,int nQuality=75);

///////////////////////////////////////////////////////////////////
//绘制图象到指定窗口
//hDC       显示窗口的设备句柄
//m_hDib    要显示的图象句柄
//rcDest	图象显示的目的区域，为NULL表明按照图象大小显示
//成功返回TRUE，否则返回FALSE
extern "C" BOOL PASCAL EXPORT DrawImage(HDC hDC,HANDLE m_hDib,LPRECT rcDest=NULL);

///////////////////////////////////////////////////////////////////
//取得当前图象的调色板
//m_hDib    当前图象句柄
//m_palDIB  返回的调色板指针
//成功返回TRUE，否则返回FALSE
extern "C" BOOL PASCAL EXPORT GetImagePalette(HANDLE m_hDib,CPalette *m_palDIB);

///////////////////////////////////////////////////////////////////
//取得当前图象的部分信息
//m_hDib            当前图象句柄
//m_ImageSize       图象尺寸
//pnPlanes		
//pnBitsPerPixel    象素位数
//pnNumColors		颜色数，24位为0
extern "C" void PASCAL EXPORT GetImageInfo(HANDLE m_hDib,CSize&  m_ImageSize,
						int *pnPlanes, int *pnBitsPerPixel, int *pnNumColors );
//
//用于得到位图句柄的数据指针
//lpbi:		lpbi=GlobalLock(HGLOBAL handle)
extern "C" LPSTR PASCAL EXPORT FindDIBBits(LPSTR lpbi);

///////////////////////////////////////////////////////////////////
//复制一个位图句柄
extern "C" HGLOBAL PASCAL EXPORT CopyHandle (HGLOBAL h);

///////////////////////////////////////////////////////////////////
//释放一个位图句柄，并把该句柄置为NULL
extern "C" void PASCAL EXPORT FreeDIBHandle(HGLOBAL& m_hDIB);

///////////////////////////////////////////////////////////////////
//设置位图句柄为给定的属性,包括大小、颜色位以及调色板信息,如果m_Handle!=NULL，
//先判断是否符合要求的属性，否则删除并重新创建该位图
extern "C" void PASCAL EXPORT SetHDIBProperty(HANDLE &m_Handle, int size_x, int size_y, int BitsPerPixel);

///////////////////////////////////////////////////////////////////
//2月1日增加函数
//取得/设置 位图中指定位置的象素值，适用于1，2，4，8，24bits的位图
//如果位图句柄为空、或者为RLE格式，返回FALSE
//ULONG& value 需要/已知 的指定位置象素值
extern "C" BOOL PASCAL EXPORT GetPixelData(HANDLE m_hDib,int x, int y, ULONG& value) ;
//
extern "C" BOOL PASCAL EXPORT SetPixelData(HANDLE m_hDib,int x, int y, const ULONG& value);

extern "C" HANDLE PASCAL EXPORT BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal);

extern "C" HBITMAP PASCAL EXPORT DIBToBitmap(HANDLE hDib, HPALETTE hPal);

///////////////////////////////////////////////////////////////////
//抽取24位图像得某一个通道
//m_hOldDIB:	原图
//m_hDIB：		处理得结果图,必须非空，并且属性与原图一样
//flag：	处理方式：
//			0：R分量
//			1：G分量
//			2：B分量
//			3：R分量BW方式显示
//			4：G分量BW方式显示
//			5：B分量BW方式显示
extern "C" BOOL PASCAL EXPORT SplitDIB(HANDLE m_hDIB,HANDLE m_hOldDIB,int flag);

///////////////////////////////////////////////////////////////////
//判断两个位图的属性是否一样：高度、宽度、颜色位数、调色板数目
extern "C" BOOL PASCAL EXPORT TwoDIBHaveSameProperty(HANDLE m_hDIB,HANDLE m_hOldDIB);

///////////////////////////////////////////////////////////////////
//产生全图的局部图
//hDIB:原图
//rect:	从位图hDIB中取出rect大小的区域放到返回位图中
//!!!特别注意：图像句柄用完后，用FreeDIBHandle()释放内存。
extern "C" HANDLE PASCAL EXPORT CutDIB(HANDLE hDIB,CRect& rect);


///////////////////////////////////////////////////////////////////
//从Jpeg缓冲区解压得到图像
//!!!特别注意：图像句柄用完后，用FreeDIBHandle()释放内存。
extern "C" HANDLE PASCAL EXPORT ReadJPEGFrombuf(unsigned char* jpeg_buf,int jpegdata_size);

///////////////////////////////////////////////////////////////////
//名称：GetJPEGBuf
//用途：把图像压缩到缓冲区
//参数：HANDLE hDib，输入的未压缩的原始图像,当前只支持24位图像
//		nQuality, 输入的压缩品质,1~100,缺省值是75
//		int * pJpegSize, 输出的压缩后的缓冲区大小
//返回：成功返回缓冲区指针，否则返回NULL.该缓冲区指针由用户负责释放.
//!!!特别注意：该缓冲区指针由用户负责释放,释放的方法：用下面的函数FreeJpegBuf释放。
extern "C" unsigned char * PASCAL EXPORT GetJPEGBuf(HANDLE hDib,int nQuality,int * pJpegSize);

///////////////////////////////////////////////////////////////////
//6月15日增加函数
//FreeJpegBuf
//用途：释放函数GetJPEGBuf的缓冲区
//参数：函数GetJPEGBuf的返回的缓冲区指针
//返回：成功返回TRUE
//!!!特别注意：用户自己调用，释放，否则会内存泄漏
extern "C" BOOL PASCAL EXPORT FreeJpegBuf(void  * pJpegBuf);

#endif //!_INC_IMAGELOAD
