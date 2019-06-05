//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: Image.cpp, 3, 10/31/02 3:02:26 PM, Albrecht M.$
//-----------------------------------------------------------------------------
/**
\file     Image.cpp
*
* Implementation of class CHVBitmap
*
*/
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "image.h"
#include "Dibsectn.h"


//////////////////////////////////////////////////////////////////////////
// CHVBitmap
//////////////////////////////////////////////////////////////////////////
RGBTRIPLE CHVBitmap::operator()(unsigned int x, unsigned int y)
{
	RGBTRIPLE rgb = { -1, -1, -1 };
		
	DIBSECTION DibSection;
	if ( ::GetObject(m_Bitmap, sizeof(DIBSECTION), &DibSection) == 0 )
		return rgb;
	
	if ( ! IsTopDown() )
	{
		y = DibSection.dsBmih.biHeight -1 - y ;
	}
	
	
	assert ( x >=0 && y >= 0 && x < DibSection.dsBmih.biWidth && DibSection.dsBmih.biHeight );
	
	if ( x< DibSection.dsBmih.biWidth && y < DibSection.dsBmih.biHeight )
	{   
		unsigned long RowLength = 4 * ( ( DibSection.dsBmih.biWidth * DibSection.dsBm.bmBitsPixel + 31 ) / 32 );
		unsigned char *pixel = (unsigned char*) DibSection.dsBm.bmBits + x * DibSection.dsBm.bmBitsPixel / 8 + y * RowLength;
		if(DibSection.dsBm.bmBitsPixel == 8)
		{
			rgb.rgbtRed = *pixel;
			rgb.rgbtBlue = *pixel;
			rgb.rgbtGreen = *pixel;
		}
		else if (DibSection.dsBm.bmBitsPixel == 24)
		{
			memcpy(&rgb, pixel, sizeof(RGBTRIPLE) );
		}
	}
	
	return rgb;
}



bool CHVBitmap::Save(CString FileName)
{
	HANDLE              hFile;
	BITMAPFILEHEADER    bfh;
	LPBITMAPINFO        pbmi;
	DWORD               dwTotalDIBBytes, dwSize, dwWritten;
	LPBYTE              pBits;
	
	// Open the file for writing (overwrites any previous version of that file)
	if( (hFile=CreateFile(FileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL)) == INVALID_HANDLE_VALUE )
		return false;
	
	// Get the BITMAPINFO for the DIBSection
	if( (pbmi = DSGetBITMAPINFOForDIBSection( m_Bitmap )) == NULL )
	{
		CloseHandle( hFile );
		return false;
	}
	

	if ( IsTopDown() )
	{
		// before saving image a top down image is converted into a bottom up image
		DWORD size = DSImageBitsSize( pbmi );
		pBits = new BYTE[size];
		if ( pBits == NULL )
		{
			CloseHandle( hFile );
			free( pbmi );
			return false;
		}
		
		LPBYTE pSource = DSGetPointerToDIBSectionImageBits( m_Bitmap );
		LPBYTE pDest = pBits;
		DWORD rowlength = 4 * ( ( pbmi->bmiHeader.biWidth*pbmi->bmiHeader.biBitCount + 31 ) / 32 );
		for ( int i = 0, m = abs(pbmi->bmiHeader.biHeight) - 1;  i < abs(pbmi->bmiHeader.biHeight); i ++, m-- )
			memcpy(pDest + m * rowlength,  pSource + i * rowlength, rowlength );
	}
	else
	{
		pBits = DSGetPointerToDIBSectionImageBits( m_Bitmap );
	}
	
	
	// What's the total size of the DIB information (not counting file header)?
	dwTotalDIBBytes = DSTotalBytesSize( pbmi );
	// Construct the file header
	ZeroMemory( &bfh, sizeof(BITMAPFILEHEADER) );
	bfh.bfType          = 0x4d42; // 'BM'
	bfh.bfSize          = dwTotalDIBBytes + sizeof(BITMAPFILEHEADER);
	bfh.bfReserved1     = 0;
	bfh.bfReserved2     = 0;
	bfh.bfOffBits       = (DWORD)(sizeof(BITMAPFILEHEADER) + pbmi->bmiHeader.biSize + DSColorTableSize(pbmi) );
	// Write the file header
	if( ( ! WriteFile( hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWritten, NULL ) ) || 
		( dwWritten != sizeof(BITMAPFILEHEADER) ) )
	{
		free( pbmi );
		CloseHandle( hFile );
		return false;
	}
	dwSize = sizeof(BITMAPINFOHEADER) + DSColorTableSize( pbmi );
	// Write the BITMAPINFO
	pbmi->bmiHeader.biHeight = abs(pbmi->bmiHeader.biHeight);
	if( ( ! WriteFile( hFile, pbmi, dwSize, &dwWritten, NULL ) ) || 
		( dwWritten != dwSize ) )
	{
		free( pbmi );
		CloseHandle( hFile );
		return false;
	}
	// Write the bits
	dwSize = DSImageBitsSize( pbmi );
	if( ( ! WriteFile( hFile, pBits, dwSize, &dwWritten, NULL ) ) || 
		( dwWritten != dwSize ) )
	{
		free( pbmi );
		CloseHandle( hFile );
		return false;
	}

	CloseHandle( hFile );

	// clean up and leave
	if ( IsTopDown() )
		delete[] pBits;
	free( pbmi );

	return true;
}

bool CHVBitmap::CopyToClipboard(HWND hWnd)
{
	// What's the total size of the DIB information (not counting file header)?
	
	LPBITMAPINFO pbmi;
	if( (pbmi = DSGetBITMAPINFOForDIBSection( m_Bitmap )) == NULL )
	{
		return false;
	}
	DWORD dwTotalDIBBytes = DSTotalBytesSize( pbmi );
	
	HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, dwTotalDIBBytes);
	
	BITMAPINFO* pDest = (BITMAPINFO*) GlobalLock(hGlobal);
	if ( pDest == NULL )
		return false;
	
	// copy the information header and the color table
	DWORD dwHeaderSize = sizeof(BITMAPINFOHEADER) + DSColorTableSize( pbmi );
	pbmi->bmiHeader.biHeight = abs(pbmi->bmiHeader.biHeight);
	CopyMemory(pDest, &pbmi->bmiHeader, dwHeaderSize);
	
	LPBYTE pBits;
	DWORD dwSize = DSImageBitsSize( pbmi );
	if ( IsTopDown() )
	{
		// before saving image a top down image is converted into a bottom up image
		pBits = new BYTE[dwSize];
		if ( pBits == NULL )
		{
			free( pbmi );
			return false;
		}
		
		LPBYTE pSource = DSGetPointerToDIBSectionImageBits( m_Bitmap );
		LPBYTE pDest = pBits;
		DWORD rowlength = 4 * ( ( pbmi->bmiHeader.biWidth*pbmi->bmiHeader.biBitCount + 31 ) / 32 );
		for ( int i = 0, m = abs(pbmi->bmiHeader.biHeight) - 1;  i < abs(pbmi->bmiHeader.biHeight); i ++, m-- )
			memcpy(pDest + m * rowlength,  pSource + i * rowlength, rowlength );
	}
	else
	{
		pBits = DSGetPointerToDIBSectionImageBits( m_Bitmap );
	}
	
	CopyMemory(((PBYTE) pDest) + dwHeaderSize, pBits, dwSize);
	
	GlobalUnlock(hGlobal);
	if ( IsTopDown() ) delete [] pBits;
	free(pbmi);
	
	// copy to clipboard
	
	OpenClipboard(hWnd);
	EmptyClipboard();
	SetClipboardData(CF_DIB, hGlobal);
	CloseClipboard();
	return true;
	
}






















