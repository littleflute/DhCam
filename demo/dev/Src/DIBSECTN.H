//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: DIBSECTN.H, 2, 10/2/02 2:31:25 PM, Nebelung, H.$
//-----------------------------------------------------------------------------
/**
  \file     DIBSECTN.H
  \brief    Header file for sample source to demonstrate the use 
            of DIBSections

  Contains the following functions for Creating/Saving/Drawing:
    DSCreateDIBSection()
    DSLoadDIBSectionFromBMPFile()
    DSDrawDIBSectionOnDC()
    DSStoreDIBSectionInBMPFile()

  Contains the following helper functions:
    DSCreatePaletteForDIBSection()
    DSGetBITMAPINFOForDIBSection()
    DSColorTableSize()
    DSTotalBytesSize()
    DSImageBitsSize()
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


********************************************************************** */

// Create/Load/Store/Draw functions
HBITMAP DSCreateDIBSection( DWORD dwX, DWORD dwY, WORD wBits, BOOL Monochrome );
BOOL DSLoadDIBSectionFromBMPFile( LPTSTR szFileName, HBITMAP *phBitmap, HPALETTE *phPalette );
BOOL DSDrawDIBSectionOnDC( HDC hDC, HBITMAP hBitmap, LPRECT pRect );
BOOL DSStoreDIBSectionInBMPFile( LPTSTR szFileName, HBITMAP hBitmap );

// Helper Functions
HPALETTE DSCreatePaletteForDIBSection( HBITMAP hBitmap );
LPBITMAPINFO DSGetBITMAPINFOForDIBSection( HBITMAP hBitmap );
DWORD DSColorTableSize( LPBITMAPINFO pbmi );
DWORD DSTotalBytesSize( LPBITMAPINFO pbmi );
DWORD DSImageBitsSize( LPBITMAPINFO pbmi );
LPBYTE DSGetPointerToDIBSectionImageBits( HBITMAP hBitmap );
HPALETTE DSCreateSpectrumPalette( void );


