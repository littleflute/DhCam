/*
 *-----------------------------------------------------------------------------
 *       Name                     HVUtil.h
 *       Purpose			
 *       Development Kit          Microsoft Win32 SDK, Visual C++ 6.00 
 *       Author                   Dh-Image
 *       Date                     2007/11/07
 *       Version                  1.3.0.0
 *
 *       Description
 *
 *       
 *-----------------------------------------------------------------------------
 */

#if !defined(HV_HVUTIL)         //
#define HV_HVUTIL


/* By C++ language compiler */
#ifdef __cplusplus
extern "C" {
#endif

HVSTATUS __stdcall HVSaveJPEG(char *lpFileName, BYTE *pBuffer, int nWidth, int nHeight, int nBitCount, BOOL bVerFlip, int nQuality);
HVSTATUS __stdcall HVLoadJPEG(char *lpFileName, BYTE *pBuffer, int *pWidth, int *pHeight, int *pBitCount, BOOL bVerFlip);		

/* extren "C" { */
#ifdef __cplusplus
}
#endif

#endif