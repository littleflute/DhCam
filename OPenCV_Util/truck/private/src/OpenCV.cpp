/*
 *	
 */


#include "stdafx.h"

#include "..\inc\private\hvobj.h"
#include "..\inc\hvdailt.h"

#include "..\CV\cv.h"
#include "..\CV\highgui.h"


extern "C" HVSTATUS __stdcall HVSaveJPEG(char *lpFileName, BYTE *pBuffer, int nWidth, int nHeight, int nBitCount, BOOL bVerFlip, int nQuality)
{
	HVSTATUS status		= STATUS_OK;

	DWORD dwImageSize	= 0;
	
	CvSize cv_size;
	int nChannels		= 0;
	IplImage *pImage	= NULL; 

	int nLength			= 0;
	char cFileExt[MAX_STRING] = {0};

	if ((lpFileName == NULL) || (nQuality < 1) || (nQuality > 100) ||
		(pBuffer == NULL)) {
		return STATUS_PARAMETER_INVALID;
	}
	
	nLength = strlen(lpFileName);
	CopyMemory(cFileExt, (lpFileName + nLength - 4), 4);
	strupr(cFileExt);
	if (strcmp(cFileExt, ".JPG")) {
		return STATUS_PARAMETER_INVALID;
	}

	cv_size.width	= nWidth;
	cv_size.height	= nHeight;
	
	nChannels	= (nBitCount == 8 ? 1 : 3);
	
	dwImageSize	= WIDTH_BYTES(cv_size.width * PIXEL_BITS(nChannels * 8)) * nHeight;
	
	if (dwImageSize <= 0) {
		status = STATUS_PARAMETER_INVALID;
	}
	else{
		pImage = ::cvCreateImage(cv_size, IPL_DEPTH_8U, nChannels);	
		if (NULL == pImage) {
			status = STATUS_NOT_ENOUGH_SYSTEM_MEMORY;
		}
		else{
			CopyMemory(pImage->imageDataOrigin, pBuffer, dwImageSize);
			
			pImage->origin = bVerFlip;
			
			if ( !(::cvSaveImage(lpFileName, pImage, &nQuality))) {
				status = STATUS_FILE_CREATE_ERROR;
			}
			
			::cvReleaseImage(&pImage);
		}	
	}

	return status;
}




extern "C" HVSTATUS __stdcall HVLoadJPEG(char *lpFileName, BYTE *pBuffer, int *pWidth, int *pHeight, int *pBitCount, BOOL bVerFlip)
{
	HVSTATUS status		= STATUS_OK;

	DWORD dwImageSize	= 0;
	DWORD dwWidthBytes = 0, dwSrcOffset = 0, dwDestOffset = 0;
	
	int nChannels		= 0;
	IplImage *pImage	= NULL; 
  
	int nY =0;

	int nLength			= 0;
	char cFileExt[MAX_STRING] = {0};

	if ((lpFileName == NULL) || (pWidth == NULL) || 
		(pHeight == NULL) || (pBitCount == NULL)) {
		return STATUS_PARAMETER_INVALID;
	}
	
	nLength = strlen(lpFileName);
	CopyMemory(cFileExt, (lpFileName + nLength - 4), 4);
	strupr(cFileExt);
	if (strcmp(cFileExt, ".JPG")) {
		return STATUS_PARAMETER_INVALID;
	}

	pImage = ::cvLoadImage(lpFileName, -1);
	if (NULL == pImage) {
		status = STATUS_FILE_INVALID;
	}
	else{
		*pBitCount	= pImage->nChannels * pImage->depth;	
		*pWidth		= pImage->width;
		*pHeight	= pImage->height;
  
		if (pBuffer)
		{
			dwWidthBytes = WIDTH_BYTES((pImage->width) * (*pBitCount));
			
			if(bVerFlip){
				for(nY = 0; nY < pImage->height; nY++){
					dwSrcOffset	= nY * dwWidthBytes;
					dwDestOffset = (pImage->height - nY -1) * dwWidthBytes;
					
					CopyMemory(pBuffer + dwDestOffset, pImage->imageDataOrigin + dwSrcOffset, dwWidthBytes);
				}
			}
			else{
				CopyMemory(pBuffer, pImage->imageDataOrigin, pImage->imageSize);
			}
		}
		
		::cvReleaseImage(&pImage);	
	}
	
	return status;
}

