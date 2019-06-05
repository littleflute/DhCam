//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: DIBSECTN.CPP, 2, 10/2/02 2:31:25 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     DIBSECTN.CPP
 *
  \brief   Sample source to demonstrate the use of DIBSections
 *

  Contains the following functions for Creating/Saving/Drawing:
    DSCreateDIBSection()
    DSLoadDIBSectionFromBMPFile()
    DSDrawDIBSectionOnDC()
    DSStoreDIBSectionInBMPFile()

  Contains the following helper functions:
    DSCreatePaletteForDIBSection()
    DSGetBITMAPINFOForDIBSection()
    DSColorTableSize()
    DSImageBitsSize()
    DSTotalBytesSize()
    DSGetPointerToDIBSectionImageBits()
    DSCreateSpectrumPalette() 


  Notes:
    * Most of the functions in this module which are passed an HBITMAP
      as a parameter assume the HBITMAP is not selected into a device
      context at the time of the call. Since the underlying APIs usually
      require this, so do these functions.
    * Throughout this module, code is in place to handle color tables
      in DIBSections (in the BITMAPINFO) even when it is not necessary.
      This helps the re-usability of the code, since it is often desirable
      to include a "suggested" color table when storing 16bpp or higher
      BMP files.
    * 

********************************************************************** */

#include "stdafx.h"
#include <Windows.h>
#include "DibSectn.h"


// Macro to determine the number of bytes per line in the DIB bits. This
//  accounts for DWORD alignment by adding 31 bits, then dividing by 32,
//  then rounding up to the next highest count of 4-bytes. Then, we 
//  multiply by 4 to get the total byte count.
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * (DWORD)(BPP) + 31) >> 5)) << 2)


/* **********************************************************************
  HBITMAP DSCreateDIBSection( DWORD dwX, DWORD dwY, WORD wBits )

  PARAMETERS:
    DWORD dwX    - Desired width of the DIBSection
    DWORD dwY    - Desired height of the DIBSection
    WORD  wBits    - Desired bit-depth of the DIBSection
  BOOL  Monochrome - If wBits=8 and Monochrome=true the palette will contain grey values only

  RETURNS:
    HBITMAP      - a DIBSection HBITMAP of the desired size and bit-depth
                   NULL on failure
    
  REMARKS:
    Creates a DIBSection based on the criteria passed in as parameters.

    The DIBSection is created with a default color table - for 8bpp and
    above, this is a spectrum palette. For 4bpp, it is a stock 16 color
    table, and for 1bpp it is black and white.
********************************************************************** */
HBITMAP DSCreateDIBSection( DWORD dwX, DWORD dwY, WORD wBits, BOOL Monochrome )
{
    HBITMAP         hBitmap;
    LPBYTE          pBits;
    int             nInfoSize;
    LPBITMAPINFO    pbmi;
    HDC             hRefDC;

    nInfoSize = sizeof( BITMAPINFOHEADER );
    if( wBits <= 8 )
        nInfoSize += sizeof(RGBQUAD) * (1 << wBits);
    if( ( wBits == 16 ) || ( wBits == 32 ) )
        nInfoSize += 3 * sizeof(DWORD);
    
    // Create the header big enough to contain color table and bitmasks if needed
    pbmi = (LPBITMAPINFO)malloc( nInfoSize );
    ZeroMemory( pbmi, nInfoSize );
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = dwX;
    pbmi->bmiHeader.biHeight = dwY;
    pbmi->bmiHeader.biPlanes = 1; 
    pbmi->bmiHeader.biBitCount = wBits;
    pbmi->bmiHeader.biCompression = BI_RGB; // override below for 16 and 32bpp

    switch( wBits )
    {
        case 24:
            // 24bpp requires no special handling
        break;
        case 16:
        {   // if it's 16bpp, fill in the masks and override the compression
            // these are the default masks - you could change them if needed
            LPDWORD pMasks = (LPDWORD)(pbmi->bmiColors);
            pMasks[0] = 0x00007c00;
            pMasks[1] = 0x000003e0;
            pMasks[2] = 0x0000001f;
            pbmi->bmiHeader.biCompression = BI_BITFIELDS;
        }
        break;
        case 32:
        {   // if it's 32bpp, fill in the masks and override the compression
            // these are the default masks - you could change them if needed
            LPDWORD pMasks = (LPDWORD)(pbmi->bmiColors);
            pMasks[0] = 0x00ff0000;
            pMasks[1] = 0x0000ff00;
            pMasks[2] = 0x000000ff;
            pbmi->bmiHeader.biCompression = BI_BITFIELDS; 
        }
        break;
        case 8:  
    {
      if(Monochrome)
      {
        int             i;
        for(i=0;i<256;i++)
        {
          pbmi->bmiColors[i].rgbRed = i;
          pbmi->bmiColors[i].rgbGreen = i;
          pbmi->bmiColors[i].rgbBlue = i;
          pbmi->bmiColors[i].rgbReserved = 0;
        }
        pbmi->bmiHeader.biClrUsed = 256;
      }
      else
      {
        HPALETTE        hPal;
        PALETTEENTRY    pe[256];
        int             i;
        // at this point, prgb points to the color table, even 
        // if bitmasks are present
        hPal = DSCreateSpectrumPalette();
        GetPaletteEntries( hPal, 0, 256, pe );
        for(i=0;i<256;i++)
        {
          pbmi->bmiColors[i].rgbRed = pe[i].peRed;
          pbmi->bmiColors[i].rgbGreen = pe[i].peGreen;
          pbmi->bmiColors[i].rgbBlue = pe[i].peBlue;
          pbmi->bmiColors[i].rgbReserved = 0;
        }
        DeleteObject( hPal );
        pbmi->bmiHeader.biClrUsed = 256;
      }
        }
    break;
        case 4:
        {   // Use a default 16 color table for 4bpp DIBSections
            RGBTRIPLE       rgb[16] = { { 0x00, 0x00, 0x00 }, // black 
                                        { 0x80, 0x00, 0x00 }, // dark red 
                                        { 0x00, 0x80, 0x00 }, // dark green 
                                        { 0x80, 0x80, 0x00 }, // dark yellow 
                                        { 0x00, 0x00, 0x80 }, // dark blue 
                                        { 0x80, 0x00, 0x80 }, // dark magenta 
                                        { 0x00, 0x80, 0x80 }, // dark cyan 
                                        { 0xC0, 0xC0, 0xC0 }, // light gray 
                                    //  { 0xC0, 0xDC, 0xC0 }, // money green 
                                    //  { 0xA6, 0xCA, 0xF0 }, // sky blue 
                                    //  { 0xFF, 0xFB, 0xF0 }, // cream 
                                    //  { 0xA0, 0xA0, 0xA4 }, // light gray 
                                        { 0x80, 0x80, 0x80 }, // medium gray 
                                        { 0xFF, 0x00, 0x00 }, // red 
                                        { 0x00, 0xFF, 0x00 }, // green 
                                        { 0xFF, 0xFF, 0x00 }, // yellow 
                                        { 0x00, 0x00, 0xFF }, // blue 
                                        { 0xFF, 0x00, 0xFF }, // magenta 
                                        { 0x00, 0xFF, 0xFF }, // cyan 
                                        { 0xFF, 0xFF, 0xFF } }; // white 
            int i;

            for(i=0;i<16;i++)
            {
                pbmi->bmiColors[i].rgbRed = rgb[i].rgbtRed;
                pbmi->bmiColors[i].rgbGreen = rgb[i].rgbtGreen;
                pbmi->bmiColors[i].rgbBlue = rgb[i].rgbtBlue;
                pbmi->bmiColors[i].rgbReserved = 0;
            }
            pbmi->bmiHeader.biClrUsed = 16;
        }
        break;
        case 1: // BW
            pbmi->bmiColors[0].rgbRed = pbmi->bmiColors[0].rgbGreen = pbmi->bmiColors[0].rgbBlue = 0;
            pbmi->bmiColors[1].rgbRed = pbmi->bmiColors[1].rgbGreen = pbmi->bmiColors[1].rgbBlue = 255;
            pbmi->bmiColors[0].rgbReserved = pbmi->bmiColors[1].rgbReserved = 0;
        break;
    }
    hRefDC = GetDC( NULL );
    hBitmap = CreateDIBSection( hRefDC, pbmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0 );
    nInfoSize = GetLastError();
    ReleaseDC( NULL, hRefDC );
    free( pbmi );
    if( hBitmap == NULL )
    {
        TCHAR   szTemp[64];
        wsprintf( szTemp, "Error %d", nInfoSize );
        MessageBox( NULL, szTemp, "Error", MB_OK );
    }
    return hBitmap;
}
/* ******************************************************************* */




/* **********************************************************************
  BOOL DSLoadDIBSectionFromBMPFile( LPTSTR, HBITMAP *, HPALETTE * )

  PARAMETERS:
    LPTSTR     - Name of (BMP) file from which to obtain DIBSection
    HBITMAP *  - Pointer to handle which receives HBITMAP DIBSection
    HPALETTE * - Pointer to handle which receives Palette

  RETURNS:
    BOOL    - TRUE for success, FALSE for failure
    
  REMARKS:
    Creates a DIBSection based on the contents of a BMP file.

    If the HPALETTE pointer is NULL, no palette is retreived.

    If the HPALETTE pointer is non-NULL, the palette is created by
    calling DSCreatePaletteForDIBSection. See its comments for 
    information regarding its functionality.

    The LoadImage() API is used to load the bitmap.
********************************************************************** */
BOOL DSLoadDIBSectionFromBMPFile( LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette )
{
    if( phBitmap == NULL )
        return FALSE;
    *phBitmap = NULL;
    if( phPalette != NULL )
        *phPalette = NULL;

    // Use LoadImage() to get the image loaded into a DIBSection
    *phBitmap = (HBITMAP)LoadImage( NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
    if( *phBitmap == NULL )
        return FALSE;
    // if they want a palette, get one
    if( phPalette != NULL )
        *phPalette = DSCreatePaletteForDIBSection( *phBitmap );
    return TRUE;
}
/* ******************************************************************* */




/* **********************************************************************
  BOOL DSDrawDIBSectionOnDC( HDC, HBITMAP, LPRECT )

  PARAMETERS:
    HDC     - HDC on which to draw the DIBSection
    HBITMAP - the DIBSection to be drawn
    LPRECT  - pointer to a rectangle containing the target coordinates

  RETURNS:
    BOOL    - TRUE for success, FALSE for failure
    
  REMARKS:
    Draws a DIBSection on a target DC.

    The bitmap is drawn using StretchBlt() if the target DC is a screen
    DC and StretchDIBits() otherwise.

    A pallete is used, only if the target DC is a screen DC. A more 
    robust implementation might check RASTERCAPS to see if the target
    device is a palettized device.

    Also, the palette is selected as a background palette. If it is
    desirable to have the palette as a foreground palette, simply 
    obtain and realize the palette in the foreground before calling
    this function.
********************************************************************** */
BOOL DSDrawDIBSectionOnDC( HDC hDC, HBITMAP hBitmap, LPRECT pRect )
{
    DIBSECTION  ds;
    HPALETTE    hPal, hOldPal;
    BOOL        bRes;

    GetObject( hBitmap, sizeof(DIBSECTION), &ds );
    if( GetDeviceCaps( hDC, RASTERCAPS ) & RC_PALETTE )
    {   // if it's a palette device, select and realize a palette
        // as a background palette (won't cause a problem is the
        // palette was not selected in the foreground in the main app
        hPal = DSCreatePaletteForDIBSection( hBitmap );
        hOldPal = SelectPalette( hDC, hPal, TRUE );
        RealizePalette( hDC );
    }
    if( GetDeviceCaps( hDC, TECHNOLOGY ) == DT_RASDISPLAY )
    { // going to the screen, use StretchBlt()
        HDC hMemDC;
        HBITMAP hOldBitmap;

        hMemDC = CreateCompatibleDC( hDC  );
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
        bRes = StretchBlt( hDC, pRect->left, pRect->top, 
                pRect->right-pRect->left, pRect->bottom-pRect->top, 
                hMemDC, 0, 0, ds.dsBm.bmWidth, ds.dsBm.bmHeight, SRCCOPY );
        SelectObject( hMemDC, hOldBitmap );
        DeleteDC( hMemDC );
    }
    else
    { // going to the printer, use StretchDIBits()
        LPBITMAPINFO    pbmi;

        // if the printer is palettized, 
        pbmi = DSGetBITMAPINFOForDIBSection( hBitmap );
        bRes = (GDI_ERROR != StretchDIBits( hDC, pRect->left, pRect->top, 
                pRect->right-pRect->left, pRect->bottom-pRect->top,
                0, 0, ds.dsBm.bmWidth, ds.dsBm.bmHeight, ds.dsBm.bmBits, 
                pbmi, DIB_RGB_COLORS, SRCCOPY ));
        free( pbmi );
    }
    if( GetDeviceCaps( hDC, RASTERCAPS ) & RC_PALETTE )
    {
        SelectPalette( hDC, hOldPal, TRUE );
        DeleteObject( hPal );
    }
    return bRes;
}
/* ******************************************************************* */




/* **********************************************************************
  BOOL DSStoreDIBSectionInBMPFile( LPTSTR, HBITMAP )

  PARAMETERS:
    LPTSTR     - Name of (BMP) file in which to store the DIBSection
    HBITMAP    - handle for HBITMAP DIBSection to be stored

  RETURNS:
    BOOL    - TRUE for success, FALSE for failure
    
  REMARKS:
    Stores the DIBSection in a BMP file.
********************************************************************** */
BOOL DSStoreDIBSectionInBMPFile( LPTSTR szFileName, HBITMAP hBitmap )
{
    HANDLE              hFile;
    BITMAPFILEHEADER    bfh;
    LPBITMAPINFO        pbmi;
    DWORD               dwTotalDIBBytes, dwSize, dwWritten;
    LPBYTE              pBits;

    // Open the file for writing (overwrites any previous version of that file)
    if( (hFile=CreateFile(szFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL)) == INVALID_HANDLE_VALUE )
        return FALSE;
    // Get the BITMAPINFO for the DIBSection
    if( (pbmi = DSGetBITMAPINFOForDIBSection( hBitmap )) == NULL )
    {
        CloseHandle( hFile );
        return FALSE;
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
        return FALSE;
    }
    // Write the BITMAPINFO
    dwSize = sizeof(BITMAPINFOHEADER) + DSColorTableSize( pbmi );
    if( ( ! WriteFile( hFile, pbmi, dwSize, &dwWritten, NULL ) ) || 
        ( dwWritten != dwSize ) )
    {
        free( pbmi );
        CloseHandle( hFile );
        return FALSE;
    }
    // Write the bits
    pBits = DSGetPointerToDIBSectionImageBits( hBitmap );
    dwSize = DSImageBitsSize( pbmi );
    if( ( ! WriteFile( hFile, pBits, dwSize, &dwWritten, NULL ) ) || 
        ( dwWritten != dwSize ) )
    {
        free( pbmi );
        CloseHandle( hFile );
        return FALSE;
    }
    // clean up and leave
    free( pbmi );
    CloseHandle( hFile );
    return TRUE;
}
/* ******************************************************************* */




/* **********************************************************************
  HPALETTE DSCreatePaletteForDIBSection( HBITMAP )

  PARAMETERS:
    HBITMAP - a DIBSection for which to get a palette

  RETURNS:
    HPALETTE - a palette which can be used to display the DIBSection
               on palettized devices

  REMARKS:
    Creates a palette to be used to display the DIBSection on a 
    palettized display device.

    If the DIBSection contains a color table, those colors are used 
    to create the palette. However, if the DIBSection is greater than
    8bpp (ie sans color table), then a suitable palette is derived
    from either CreateHalftonePalette() or GetStockObject() depending
    on the video mode.
********************************************************************** */
HPALETTE DSCreatePaletteForDIBSection( HBITMAP hBitmap )
{
    BITMAP      bm;
    HPALETTE    hPalette;

    // Get the color depth of the DIBSection
    GetObject( hBitmap, sizeof(BITMAP), &bm );
    // If the DIBSection is 256 color or less, it has a color table
    if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
    {
        HDC             hMemDC;
        HBITMAP         hOldBitmap;
        RGBQUAD         rgb[256];
        LPLOGPALETTE    pLogPal;
        WORD            i;
        int             nColors;

        // Find out how many colors are in the color table
        nColors = 1 << ( bm.bmBitsPixel * bm.bmPlanes );
        // Create a memory DC and select the DIBSection into it
        hMemDC = CreateCompatibleDC( NULL );
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
        // Get the DIBSection's color table
        GetDIBColorTable( hMemDC, 0, nColors, rgb );
        // Create a palette from the color table
        pLogPal = (LOGPALETTE*)malloc( sizeof(LOGPALETTE) + (nColors*sizeof(PALETTEENTRY)) );
        pLogPal->palVersion = 0x300;
        pLogPal->palNumEntries = nColors;
        for(i=0;i<nColors;i++)
        {
            pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
            pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
            pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
            pLogPal->palPalEntry[i].peFlags = 0;
        }
        hPalette = CreatePalette( pLogPal );
        // Clean up
        free( pLogPal );
        SelectObject( hMemDC, hOldBitmap );
        DeleteDC( hMemDC );
     }
     else   // It has no color table, so make one up
     {
         hPalette = DSCreateSpectrumPalette();
     }
     return hPalette;
}
/* ******************************************************************* */




/* **********************************************************************
  LPBITMAPINFO DSGetBITMAPINFOForDIBSection( HBITMAP )

  PARAMETERS:
    HBITMAP - a DIBSection for which to get the BITMAPINFO

  RETURNS:
    LPBITMAPINFO - a pointer to a newly allocated memory block containing
                   the BITMAPINFO structure which describes the DIBSection.

  REMARKS:
    Allocates and returns a pointer to a BITMAPINFO for the DIBSection.

    The returned BITMAPINFO contains a color table, even if the 
    bit-depth does not require one - this way the caller can override
    the optional color table if need be.

    The caller is responsible for de-allocating the memory block using
    the CRT function free().
********************************************************************** */
LPBITMAPINFO DSGetBITMAPINFOForDIBSection( HBITMAP hBitmap )
{
    LPBITMAPINFO    pbmi;
    DIBSECTION      ds;
    LPRGBQUAD       prgb;

    GetObject( hBitmap, sizeof(DIBSECTION), &ds );
    // load the header and the bitmasks if present
    // per function comments above, we allocate space for a color 
    // table even if it is not needed 
    if( ds.dsBmih.biCompression == BI_BITFIELDS )
    {   // has a bitmask - be sure to allocate for and copy them
        pbmi = (BITMAPINFO*)malloc( sizeof(BITMAPINFOHEADER) + (3*sizeof(DWORD)) + (256*sizeof(RGBQUAD)) );
        CopyMemory( &(pbmi->bmiHeader), &(ds.dsBmih), sizeof(BITMAPINFOHEADER) + (3*sizeof(DWORD)) );
        prgb = (LPRGBQUAD)&(pbmi->bmiColors[3]);
    }
    else
    {   // no bitmask - just the header and color table
        pbmi = (BITMAPINFO*)malloc( sizeof(BITMAPINFOHEADER) + (256*sizeof(RGBQUAD)) );
        CopyMemory( &(pbmi->bmiHeader), &(ds.dsBmih), sizeof(BITMAPINFOHEADER) );
        prgb = pbmi->bmiColors;
    }
    // at this point, prgb points to the color table, even 
    // if bitmasks are present

    // Now for the color table
    if( ( ds.dsBm.bmBitsPixel * ds.dsBm.bmPlanes ) <= 8 )
    {   // the DIBSection is 256 color or less (has color table)
        HDC             hMemDC;
        HBITMAP         hOldBitmap;
        hMemDC = CreateCompatibleDC( NULL );
        hOldBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
        GetDIBColorTable( hMemDC, 0, 1<<(ds.dsBm.bmBitsPixel*ds.dsBm.bmPlanes), prgb );
        SelectObject( hMemDC, hOldBitmap );
        DeleteDC( hMemDC );
    }
    else
    {   // the DIBSection is >8bpp (has no color table) so make one up
        HPALETTE        hPal;
        PALETTEENTRY    pe[256];
        int             i;

        // where are we going to get the colors? from a spectrum palette
        hPal = DSCreateSpectrumPalette();
        GetPaletteEntries( hPal, 0, 256, pe );
        for(i=0;i<256;i++)
        {
            prgb[i].rgbRed = pe[i].peRed;
            prgb[i].rgbGreen = pe[i].peGreen;
            prgb[i].rgbBlue = pe[i].peBlue;
            prgb[i].rgbReserved = 0;
        }
        DeleteObject( hPal );
        pbmi->bmiHeader.biClrUsed = 256;
    }
    return pbmi;
}
/* ******************************************************************* */




/* **********************************************************************
  DWORD DSColorTableSize( LPBITMAPINFO pbmi )

  PARAMETERS:
    LPBITMAPINFO - pointer to a BITMAPINFO describing a DIB

  RETURNS:
    DWORD    - the size, in bytes, of the color table for the DIB
    
  REMARKS:
    Calculates and returns the size, in bytes, of the color table for
    the DIB described by the BITMAPINFO.
********************************************************************** */
DWORD DSColorTableSize( LPBITMAPINFO pbmi )
{
    DWORD dwColors;

    dwColors = 0;
    if( pbmi->bmiHeader.biClrUsed != 0 )
        dwColors = pbmi->bmiHeader.biClrUsed;
    else if( pbmi->bmiHeader.biBitCount > 8 )
        dwColors = 0;
    else
        dwColors = 1 << (pbmi->bmiHeader.biBitCount * pbmi->bmiHeader.biPlanes);

    if( pbmi->bmiHeader.biCompression == BI_BITFIELDS )
        return (sizeof(DWORD)*3) + (dwColors*sizeof(RGBQUAD));
    return (dwColors*sizeof(RGBQUAD));
}
/* ******************************************************************* */




/* **********************************************************************
  DWORD DSImageBitsSize( LPBITMAPINFO pbmi )

  PARAMETERS:
    LPBITMAPINFO - pointer to a BITMAPINFO describing a DIB

  RETURNS:
    DWORD    - the size, in bytes, of the DIB's image bits
    
  REMARKS:
    Calculates and returns the size, in bytes, of the image bits for
    the DIB described by the BITMAPINFO.
********************************************************************** */
DWORD DSImageBitsSize( LPBITMAPINFO pbmi )
{
    switch( pbmi->bmiHeader.biCompression )
    {
        case BI_RLE8:
        case BI_RLE4:
            return pbmi->bmiHeader.biSizeImage;
        break;
        default: // should not have to use "default"
        case BI_RGB:
        case BI_BITFIELDS:
            return abs(BYTESPERLINE(pbmi->bmiHeader.biWidth,pbmi->bmiHeader.biBitCount*pbmi->bmiHeader.biPlanes)*pbmi->bmiHeader.biHeight);
        break;
    }
    return 0;
}
/* ******************************************************************* */




/* **********************************************************************
  DWORD DSTotalBytesSize( LPBITMAPINFO pbmi )

  PARAMETERS:
    LPBITMAPINFO - pointer to a BITMAPINFO describing a DIB

  RETURNS:
    DWORD    - the size, in bytes, of the DIB's memory footprint
    
  REMARKS:
    Calculates and returns the size, in bytes, of the entire CF_DIB 
    bitmap described by the BITMAPINFO - including header, color table,
    and image bits.
********************************************************************** */
DWORD DSTotalBytesSize( LPBITMAPINFO pbmi )
{
    return DSImageBitsSize(pbmi) + sizeof(BITMAPINFOHEADER) + DSColorTableSize(pbmi);
}
/* ******************************************************************* */




/* **********************************************************************
  LPBYTE DSGetPointerToDIBSectionImageBits( HBITMAP hBitmap )

  PARAMETERS:
    HBITMAP    - handle for HBITMAP DIBSection

  RETURNS:
    LPBYTE     - pointer to the bitmap's bits
    
  REMARKS:
    Returns a pointer to the DIBSection's image bits.
********************************************************************** */
// get a pointer which points to the image bits for a DIBSection
LPBYTE DSGetPointerToDIBSectionImageBits( HBITMAP hBitmap )
{
    DIBSECTION  ds;

    GetObject( hBitmap, sizeof(DIBSECTION), &ds );
    return (LPBYTE)ds.dsBm.bmBits;
}
/* ******************************************************************* */




/* **********************************************************************
  HPALETTE DSCreateSpectrumPalette( void ) 

  PARAMETERS:
    void

  RETURNS:
    HPALETTE - a handle to a spectrum palette - NULL on failure

  REMARKS:
    This function was stolen from Mike Irvine's SEEDIB sample. There's
    no source code comments in his version either :)

    This function will build a palette with a spectrum of colors.  It is
    useful when you want to display a number of DIBs each with a different
    palette yet still have an a good selection of colors to which the 
    DIBs' colors will be mapped.
********************************************************************** */
HPALETTE DSCreateSpectrumPalette( void ) 
{                 
    HPALETTE hPal;
    LPLOGPALETTE lplgPal;
    BYTE red, green, blue;
    int i; 
    
    lplgPal = (LPLOGPALETTE)GlobalAlloc( GPTR, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 256 ); 
    if (!lplgPal)
      return NULL;
    
    lplgPal->palVersion = 0x300;
    lplgPal->palNumEntries = 256;
     
    red = green = blue = 0;
    for (i = 0; i < 256; i++) {
        lplgPal->palPalEntry[i].peRed   = red;
        lplgPal->palPalEntry[i].peGreen = green;
        lplgPal->palPalEntry[i].peBlue  = blue;
        lplgPal->palPalEntry[i].peFlags = (BYTE)0;

        if (!(red += 32))
            if (!(green += 32))
                blue += 64;
    }
    hPal = CreatePalette(lplgPal);
    GlobalFree(lplgPal);
    return hPal;
}
/* ******************************************************************* */
