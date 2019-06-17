#ifndef _INC_IMAGELOAD
#define _INC_IMAGELOAD
//����ѡ��
const WORD SAVE_OPTION_BMP=0x0001;  //��ΪBMPͼ��ʱ��ѡ���1��2��8��16��24��32λ����ѡ��
const WORD SAVE_OPTION_JPG=0x0002;	//��ΪJPGͼ��ʱ��ѡ�ѡ��ѹ��Ʒ��
const WORD SAVE_OPTION_TIF=0x0004;	//��ΪTIFͼ��ʱ��ѡ�ѡ��ѹ����ʽ��LZW�Ͳ�ѹ��

//��������
/******************************************************************************/

///////////////////////////////////////////////////////////////////
//����ͼ��
//lpszPathName �ļ���
//����ͼ����
//!!!�ر�ע�⣺ͼ�����������FreeDIBHandle()�ͷ��ڴ档
extern "C" HANDLE PASCAL EXPORT LoadImageFile(LPCTSTR lpszPathName);

///////////////////////////////////////////////////////////////////
//����ͼ��
//	lpszPathName �ļ���
//	m_hDib ͼ����
//	WORD nSaveOption ����ѡ��
//		��ΪSAVE_OPTION_BMP��SAVE_OPTION_JPG��WORD SAVE_OPTION_TIF
//		
//		��������ϡ�Ϊ0����ʾ����ѡ�
//����ɹ�����TRUE�����򷵻�FALSE
extern "C" BOOL PASCAL EXPORT SaveImageFile(HANDLE m_hDib,LPCTSTR lpszPathName,WORD nSaveOption=0);

///////////////////////////////////////////////////////////////////
//����ͼ��ΪJPG��ʽ
//	lpszPathName �ļ���
//	m_hDib ͼ����
//	int nQuality Ʒ��ϵ��(0--100)
//	
//����ɹ�����TRUE�����򷵻�FALSE
extern "C" BOOL PASCAL EXPORT SaveImageAsJPEG(HANDLE m_hDib,LPCTSTR lpszPathName,int nQuality=75);

///////////////////////////////////////////////////////////////////
//����ͼ��ָ������
//hDC       ��ʾ���ڵ��豸���
//m_hDib    Ҫ��ʾ��ͼ����
//rcDest	ͼ����ʾ��Ŀ������ΪNULL��������ͼ���С��ʾ
//�ɹ�����TRUE�����򷵻�FALSE
extern "C" BOOL PASCAL EXPORT DrawImage(HDC hDC,HANDLE m_hDib,LPRECT rcDest=NULL);

///////////////////////////////////////////////////////////////////
//ȡ�õ�ǰͼ��ĵ�ɫ��
//m_hDib    ��ǰͼ����
//m_palDIB  ���صĵ�ɫ��ָ��
//�ɹ�����TRUE�����򷵻�FALSE
extern "C" BOOL PASCAL EXPORT GetImagePalette(HANDLE m_hDib,CPalette *m_palDIB);

///////////////////////////////////////////////////////////////////
//ȡ�õ�ǰͼ��Ĳ�����Ϣ
//m_hDib            ��ǰͼ����
//m_ImageSize       ͼ��ߴ�
//pnPlanes		
//pnBitsPerPixel    ����λ��
//pnNumColors		��ɫ����24λΪ0
extern "C" void PASCAL EXPORT GetImageInfo(HANDLE m_hDib,CSize&  m_ImageSize,
						int *pnPlanes, int *pnBitsPerPixel, int *pnNumColors );
//
//���ڵõ�λͼ���������ָ��
//lpbi:		lpbi=GlobalLock(HGLOBAL handle)
extern "C" LPSTR PASCAL EXPORT FindDIBBits(LPSTR lpbi);

///////////////////////////////////////////////////////////////////
//����һ��λͼ���
extern "C" HGLOBAL PASCAL EXPORT CopyHandle (HGLOBAL h);

///////////////////////////////////////////////////////////////////
//�ͷ�һ��λͼ��������Ѹþ����ΪNULL
extern "C" void PASCAL EXPORT FreeDIBHandle(HGLOBAL& m_hDIB);

///////////////////////////////////////////////////////////////////
//����λͼ���Ϊ����������,������С����ɫλ�Լ���ɫ����Ϣ,���m_Handle!=NULL��
//���ж��Ƿ����Ҫ������ԣ�����ɾ�������´�����λͼ
extern "C" void PASCAL EXPORT SetHDIBProperty(HANDLE &m_Handle, int size_x, int size_y, int BitsPerPixel);

///////////////////////////////////////////////////////////////////
//2��1�����Ӻ���
//ȡ��/���� λͼ��ָ��λ�õ�����ֵ��������1��2��4��8��24bits��λͼ
//���λͼ���Ϊ�ա�����ΪRLE��ʽ������FALSE
//ULONG& value ��Ҫ/��֪ ��ָ��λ������ֵ
extern "C" BOOL PASCAL EXPORT GetPixelData(HANDLE m_hDib,int x, int y, ULONG& value) ;
//
extern "C" BOOL PASCAL EXPORT SetPixelData(HANDLE m_hDib,int x, int y, const ULONG& value);

extern "C" HANDLE PASCAL EXPORT BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal);

extern "C" HBITMAP PASCAL EXPORT DIBToBitmap(HANDLE hDib, HPALETTE hPal);

///////////////////////////////////////////////////////////////////
//��ȡ24λͼ���ĳһ��ͨ��
//m_hOldDIB:	ԭͼ
//m_hDIB��		����ý��ͼ,����ǿգ�����������ԭͼһ��
//flag��	����ʽ��
//			0��R����
//			1��G����
//			2��B����
//			3��R����BW��ʽ��ʾ
//			4��G����BW��ʽ��ʾ
//			5��B����BW��ʽ��ʾ
extern "C" BOOL PASCAL EXPORT SplitDIB(HANDLE m_hDIB,HANDLE m_hOldDIB,int flag);

///////////////////////////////////////////////////////////////////
//�ж�����λͼ�������Ƿ�һ�����߶ȡ���ȡ���ɫλ������ɫ����Ŀ
extern "C" BOOL PASCAL EXPORT TwoDIBHaveSameProperty(HANDLE m_hDIB,HANDLE m_hOldDIB);

///////////////////////////////////////////////////////////////////
//����ȫͼ�ľֲ�ͼ
//hDIB:ԭͼ
//rect:	��λͼhDIB��ȡ��rect��С������ŵ�����λͼ��
//!!!�ر�ע�⣺ͼ�����������FreeDIBHandle()�ͷ��ڴ档
extern "C" HANDLE PASCAL EXPORT CutDIB(HANDLE hDIB,CRect& rect);


///////////////////////////////////////////////////////////////////
//��Jpeg��������ѹ�õ�ͼ��
//!!!�ر�ע�⣺ͼ�����������FreeDIBHandle()�ͷ��ڴ档
extern "C" HANDLE PASCAL EXPORT ReadJPEGFrombuf(unsigned char* jpeg_buf,int jpegdata_size);

///////////////////////////////////////////////////////////////////
//���ƣ�GetJPEGBuf
//��;����ͼ��ѹ����������
//������HANDLE hDib�������δѹ����ԭʼͼ��,��ǰֻ֧��24λͼ��
//		nQuality, �����ѹ��Ʒ��,1~100,ȱʡֵ��75
//		int * pJpegSize, �����ѹ����Ļ�������С
//���أ��ɹ����ػ�����ָ�룬���򷵻�NULL.�û�����ָ�����û������ͷ�.
//!!!�ر�ע�⣺�û�����ָ�����û������ͷ�,�ͷŵķ�����������ĺ���FreeJpegBuf�ͷš�
extern "C" unsigned char * PASCAL EXPORT GetJPEGBuf(HANDLE hDib,int nQuality,int * pJpegSize);

///////////////////////////////////////////////////////////////////
//6��15�����Ӻ���
//FreeJpegBuf
//��;���ͷź���GetJPEGBuf�Ļ�����
//����������GetJPEGBuf�ķ��صĻ�����ָ��
//���أ��ɹ�����TRUE
//!!!�ر�ע�⣺�û��Լ����ã��ͷţ�������ڴ�й©
extern "C" BOOL PASCAL EXPORT FreeJpegBuf(void  * pJpegBuf);

#endif //!_INC_IMAGELOAD
