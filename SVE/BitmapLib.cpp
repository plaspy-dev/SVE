//////////////////////////////////////////////////////////////////////
//
// BitmapLib.cpp: Implementation of class CBitmapLib.
//
// Author:
// Gerd Esser
// Germany
// eMail: admin@gerdesser.de
// http://www.gerdesser.de
//
// History:
// 13.09.2000 - Creation
// 14.02.2002 - Error correction in GetRGB() case 16 (bit), expand ranges from 32 to 256
//              Thanks for comment of "ForJest"
// 
//////////////////////////////////////////////////////////////////////

# include "stdafx.h"
#include "BitmapLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LINES_PER_BAND  100

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

CBitmapLib::CBitmapLib()
{

}

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

CBitmapLib::~CBitmapLib()
{

}

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

int  CBitmapLib::GetUsedColors( CString const & stSrcBitmapFile,
                                RGBQUAD *       pColorTable,
                                WORD const      wMaxColorTableSize,
                                CProgressCtrl * pProgressCtrl )
{
  ASSERT( pColorTable );
  
  CFile  SrcFile;

  if ( !SrcFile.Open( stSrcBitmapFile, CFile::typeBinary | CFile::modeRead ) )
    return FALSE;

  // init progress control
  if ( pProgressCtrl )
  {
    pProgressCtrl->SetRange32( 0, SrcFile.GetLength( ) );
    pProgressCtrl->SetPos( 0 );
  }

  BITMAPFILEHEADER bmfhSrc;

  // read bitmapfileheader of source bitmap
  SrcFile.Read( &bmfhSrc, sizeof( bmfhSrc ) );

  if ( pProgressCtrl )
    pProgressCtrl->SetPos( SrcFile.GetPosition( ) );

  BITMAPINFO * pBmiSrc = ( BITMAPINFO * ) new BYTE[ bmfhSrc.bfOffBits - sizeof( bmfhSrc ) ];

  if ( !pBmiSrc )
  {
    SrcFile.Close( );
    return blNotEnoughMemory;
  }

  // read bitmapinfo header of source bitmap, including the color palette
  SrcFile.Read( pBmiSrc, bmfhSrc.bfOffBits - sizeof( bmfhSrc ) );

  if ( pProgressCtrl )
    pProgressCtrl->SetPos( SrcFile.GetPosition( ) );

  // only uncompressed bitmaps are possible, because its a bitwise operation
  if ( pBmiSrc->bmiHeader.biCompression != BI_RGB )
  {
    delete [] pBmiSrc;
    SrcFile.Close( );
    return blUnsupportedCompression;
  }

  WORD wUsedColors = 0;

  UINT  uSrcBytesPerLine = pBmiSrc->bmiHeader.biSizeImage / pBmiSrc->bmiHeader.biHeight;

  // only reserve memory for small bitmap bands, thus working also on fat sized bitmaps
  BYTE * pBitsSrc = new BYTE[ uSrcBytesPerLine * LINES_PER_BAND ];

  // perform each band in a loop
  for ( UINT i = 0; i < ( UINT ) pBmiSrc->bmiHeader.biHeight; i += LINES_PER_BAND )
  {
    // uCurLines may be smaller at the last loop
    UINT uCurLines = min( LINES_PER_BAND, pBmiSrc->bmiHeader.biHeight - i );

    // read source bitmap data
    SrcFile.Read( pBitsSrc, uCurLines * uSrcBytesPerLine );

    if ( pProgressCtrl )
      pProgressCtrl->SetPos( SrcFile.GetPosition( ) );

    int nReturn = GetUsedColors( pBmiSrc, pBitsSrc, 0, uCurLines, 
                                 pColorTable,
                                 wMaxColorTableSize,
                                 wUsedColors );
    
    if ( nReturn < 0 )
    {
      delete [] pBitsSrc;
      delete [] pBmiSrc;

      SrcFile.Close( );

      return nReturn;
    }
  }

  delete [] pBitsSrc;
  delete [] pBmiSrc;

  SrcFile.Close( );

  // reset progress control
  if ( pProgressCtrl )
    pProgressCtrl->SetPos( 0 );

  return wUsedColors;
}

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

int CBitmapLib::GetUsedColors( BITMAPINFO const * pBmi,
                               BYTE const *       pBits,
                               UINT const         nStartScanLine,
                               UINT const         nScanLines,
                               RGBQUAD *          pColorTable,
                               WORD const         wMaxColorTableSize,
                               WORD &             wUsedColors )
{
  ASSERT( pBmi );
  ASSERT( pBits );
  ASSERT( pColorTable );

  BITMAPINFOHEADER const * pBmih = &pBmi->bmiHeader;
  RGBQUAD rgb;

  if ( pBmih->biCompression != BI_RGB )
    return blUnsupportedCompression;  // unsupported bitmap format, no compression

  for ( UINT i = nStartScanLine; i < nStartScanLine + nScanLines; i++ )
  {
    for ( UINT j = 0; j < ( UINT ) pBmih->biWidth; j++ )
    {
      rgb = GetRGB( pBmi, i, j, pBits );

      // search color table for current color
	  WORD k = 0;
      for ( k = 0; k < wUsedColors; k++ )
        if ( pColorTable[k].rgbBlue == rgb.rgbBlue
             && pColorTable[k].rgbGreen == rgb.rgbGreen
             && pColorTable[k].rgbRed == rgb.rgbRed )
          break;

      // if color could not be found
      if ( k == wUsedColors )
      {
        // table full, no more entries and thus color count possible
        if ( wUsedColors == wMaxColorTableSize )
          return blColorOverflow;            // error: color table overflow
        else
        {
          // append new color at color table
          pColorTable[wUsedColors] = rgb;
          wUsedColors++;
        }
      }
    }
  }

  return wUsedColors;
} // CBitmapLib::GetUsedColors( )
  
//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

BOOL  CBitmapLib::ChangeColorDepth( CString const & stSrcBitmapFile,
                                    CString const & stDstBitmapFile,
                                    WORD const      wDstBitCount,
                                    CProgressCtrl * pProgressCtrl )
{
  ASSERT( stSrcBitmapFile != stDstBitmapFile );
  
  CFile  SrcFile;
  CFile  DstFile;

  if ( !SrcFile.Open( stSrcBitmapFile, CFile::typeBinary | CFile::modeRead ) )
    return FALSE;

  // init progress control
  if ( pProgressCtrl )
  {
    pProgressCtrl->SetRange32( 0, SrcFile.GetLength( ) );
    pProgressCtrl->SetPos( 0 );
  }

  // if bitcount is greater than 8 bits, no device independent bitmap is requested
  BOOL bDstIsDIB = TRUE;
  if ( wDstBitCount > 8 )
    bDstIsDIB = FALSE;

  BITMAPFILEHEADER bmfhSrc;

  // read bitmapfileheader of source bitmap
  SrcFile.Read( &bmfhSrc, sizeof( bmfhSrc ) );

  if ( pProgressCtrl )
    pProgressCtrl->SetPos( SrcFile.GetPosition( ) );

  BITMAPINFO * pBmiSrc = ( BITMAPINFO * ) new BYTE[ bmfhSrc.bfOffBits - sizeof( bmfhSrc ) ];

  if ( !pBmiSrc )
  {
    SrcFile.Close( );
    return FALSE;
  }

  // read bitmapinfo header of source bitmap, including the color palette
  SrcFile.Read( pBmiSrc, bmfhSrc.bfOffBits - sizeof( bmfhSrc ) );

  if ( pProgressCtrl )
    pProgressCtrl->SetPos( SrcFile.GetPosition( ) );

  // only uncompressed bitmaps are possible, because its a bitwise operation
  if ( pBmiSrc->bmiHeader.biCompression != BI_RGB )
  {
    delete [] pBmiSrc;
    SrcFile.Close( );
    return FALSE;
  }

  if ( !DstFile.Open( stDstBitmapFile, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite ) )
  {
    delete [] pBmiSrc;
    SrcFile.Close( );
    return FALSE;
  }

  BITMAPINFO * pBmiDst;

  // create destination bitmapinfo header, due to wether it's a DIB or not (with or without color palette)
  if ( bDstIsDIB )
  {
    pBmiDst = ( BITMAPINFO * ) new BYTE[ sizeof( BITMAPINFOHEADER )
                                         + ( 1 << wDstBitCount ) * sizeof( RGBQUAD ) ];
    pBmiDst->bmiHeader = pBmiSrc->bmiHeader;
    pBmiDst->bmiHeader.biClrUsed = ( 1 << wDstBitCount );
    pBmiDst->bmiHeader.biClrImportant = ( 1 << wDstBitCount );
  }
  else
  {
    pBmiDst = ( BITMAPINFO * ) new BYTE[ sizeof( BITMAPINFOHEADER ) ];
    pBmiDst->bmiHeader = pBmiSrc->bmiHeader;
    pBmiDst->bmiHeader.biClrUsed = 0;
    pBmiDst->bmiHeader.biClrImportant = 0;
  }

  pBmiDst->bmiHeader.biBitCount = wDstBitCount;
  pBmiDst->bmiHeader.biSizeImage = ( ( ( ( pBmiDst->bmiHeader.biWidth 
                                           * pBmiDst->bmiHeader.biBitCount ) + 31 ) & ~31 ) >> 3 )
                                   * pBmiDst->bmiHeader.biHeight;

  // init color table in case of a DIB
  if ( bDstIsDIB )
    for ( WORD j = 0; j < pBmiDst->bmiHeader.biClrUsed; j++ )
    {
      pBmiDst->bmiColors[j].rgbBlue     = 255;
      pBmiDst->bmiColors[j].rgbGreen    = 255;
      pBmiDst->bmiColors[j].rgbRed      = 255;
      pBmiDst->bmiColors[j].rgbReserved = 0;
    }

  // create a bitmapfileheader of destination bitmap
  BITMAPFILEHEADER bmfhDst = bmfhSrc;

  bmfhDst.bfOffBits = sizeof( BITMAPFILEHEADER ) 
                      + pBmiDst->bmiHeader.biSize
                      + pBmiDst->bmiHeader.biClrUsed * sizeof( RGBQUAD );
  bmfhDst.bfSize = bmfhDst.bfOffBits + pBmiDst->bmiHeader.biSizeImage;

  // write bitmapfileheader of destination bitmap
  DstFile.Write( &bmfhDst, sizeof( BITMAPFILEHEADER ) );
  // write bitmapinfo struct of destination bitmap including a real space for the color 
  // palette, which will be detected and written later
  DstFile.Write( pBmiDst, pBmiDst->bmiHeader.biSize + pBmiDst->bmiHeader.biClrUsed * sizeof( RGBQUAD ) );

  WORD wColorTableSize = 0;     // only used for bDstIsDIB = TRUE

  UINT  uSrcBytesPerLine = pBmiSrc->bmiHeader.biSizeImage / pBmiSrc->bmiHeader.biHeight;
  UINT  uDstBytesPerLine = pBmiDst->bmiHeader.biSizeImage / pBmiDst->bmiHeader.biHeight;

  // only reserve memory for small bitmap bands, thus working also on fat sized bitmaps
  BYTE * pBitsSrc = new BYTE[ uSrcBytesPerLine * LINES_PER_BAND ];
  BYTE * pBitsDst = new BYTE[ uDstBytesPerLine * LINES_PER_BAND ];

  // perform each band in a loop
  for ( UINT i = 0; i < ( UINT ) pBmiSrc->bmiHeader.biHeight; i += LINES_PER_BAND )
  {
    // uCurLines may be smaller at the last loop
    UINT uCurLines = min( LINES_PER_BAND, pBmiSrc->bmiHeader.biHeight - i );

    // read source bitmap data
    SrcFile.Read( pBitsSrc, uCurLines * uSrcBytesPerLine );

    if ( pProgressCtrl )
      pProgressCtrl->SetPos( SrcFile.GetPosition( ) );

    // build up destination color palette at a DIB
    if ( bDstIsDIB )
      if ( GetUsedColors( pBmiSrc, pBitsSrc, 0, uCurLines, 
                          pBmiDst->bmiColors, ( WORD ) pBmiDst->bmiHeader.biClrUsed, wColorTableSize ) < 0 )
      {
        delete [] pBitsDst;
        delete [] pBitsSrc;
        delete [] pBmiSrc;
        delete [] pBmiDst;

        DstFile.Close( );
        SrcFile.Close( );

        return FALSE;
      }

    // init destination buffer (needed for bitmaps smaller 8bit bitcount)
    memset( pBitsDst, 0, uCurLines * uDstBytesPerLine );

    // loops over each pixel
    for ( UINT m = 0; m < uCurLines; m++ )
    {
      for ( UINT n = 0; n < ( UINT ) pBmiSrc->bmiHeader.biWidth; n++ )
      {
        RGBQUAD rgb = GetRGB( pBmiSrc, m, n, pBitsSrc );
        
        if ( bDstIsDIB )
        {
		  WORD k = 0; 
          for ( k = 0; k < wColorTableSize; k++ )
          {
            if ( pBmiDst->bmiColors[k].rgbBlue == rgb.rgbBlue
                 && pBmiDst->bmiColors[k].rgbGreen == rgb.rgbGreen
                 && pBmiDst->bmiColors[k].rgbRed == rgb.rgbRed )
            {
              // sets a DIB pixel
              SetDIBPixel( pBmiDst, m, n, pBitsDst, ( BYTE ) k );
              break;
            }
          }

          ASSERT( k < wColorTableSize );
        }
        else
          // sets a high or true color pixel
          SetRGBPixel( pBmiDst, m, n, pBitsDst, rgb );
      }
    }

    // write destination bitmap band
    DstFile.Write( pBitsDst, uCurLines * uDstBytesPerLine );
  }

  // write color table to file
  if ( bDstIsDIB )
  {
    DstFile.Seek( sizeof( BITMAPFILEHEADER ) + pBmiDst->bmiHeader.biSize, CFile::begin );
    DstFile.Write( pBmiDst->bmiColors, pBmiDst->bmiHeader.biClrUsed * sizeof( RGBQUAD ) );
  }

  delete [] pBitsDst;
  delete [] pBitsSrc;
  delete [] pBmiSrc;
  delete [] pBmiDst;

  DstFile.Close( );
  SrcFile.Close( );

  // reset progress control
  if ( pProgressCtrl )
    pProgressCtrl->SetPos( 0 );

  return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

RGBQUAD  CBitmapLib::GetRGB( BITMAPINFO const * pBmi,
                             UINT const         uLine,
                             UINT const         uCol,
                             BYTE const *       pBits )
{
  BITMAPINFOHEADER const * pBmih = &pBmi->bmiHeader;
  RGBQUAD const * pRgb = pBmi->bmiColors;
  RGBQUAD rgb = { 0, 0, 0, 0 };
  
  DWORD dwBytesPerLine = pBmih->biSizeImage / pBmih->biHeight;

  switch ( pBmih->biBitCount )
  {
    case 1:
      {
        DWORD dwBytePos = uLine * dwBytesPerLine + ( uCol >> 3 );

        WORD wIndex = pBits[ dwBytePos ];
        wIndex = ( WORD ) ( ( wIndex >> ~( ( BYTE ) uCol & 0x7 ) ) & 0x1 );

        rgb = pRgb[wIndex];
      }
    break;

    case 4:
      {
        DWORD dwBytePos = uLine * dwBytesPerLine + ( uCol >> 1 );

        WORD wIndex = pBits[ dwBytePos ];
        wIndex = ( WORD ) ( ( wIndex >> ( ( ( ( BYTE ) uCol & 0x1 ) ^ 0x1 ) << 2 ) ) & 0xf );

        rgb = pRgb[wIndex];
      }
    break;

    case 8:
      rgb = pRgb[ pBits[ uLine * dwBytesPerLine + uCol ] ];
    break;

    case 16:
      {
        WORD wCol = ( ( WORD * ) pBits )[ uLine * dwBytesPerLine / 2 + uCol ];

        rgb.rgbRed   = ( BYTE ) ( ( ( wCol >> 10 ) & 0x1f ) << 3 );
        rgb.rgbGreen = ( BYTE ) ( ( ( wCol >> 5 ) & 0x1f ) << 3 );
        rgb.rgbBlue  = ( BYTE ) ( ( wCol & 0x1f ) << 3 );
        rgb.rgbReserved = 0;
      }
    break;

    case 24:
      rgb.rgbRed   = pBits[ uLine * dwBytesPerLine + uCol * 3 + 2 ];
      rgb.rgbGreen = pBits[ uLine * dwBytesPerLine + uCol * 3 + 1 ];
      rgb.rgbBlue  = pBits[ uLine * dwBytesPerLine + uCol * 3 ];
      rgb.rgbReserved = 0;
    break;

    case 32:
      {
        DWORD dwCol = ( ( DWORD * ) pBits )[ uLine * dwBytesPerLine / 4 + uCol ];

        rgb.rgbRed   = ( BYTE ) ( ( dwCol >> 16 ) & 0xff );
        rgb.rgbGreen = ( BYTE ) ( ( dwCol >> 8 ) & 0xff );
        rgb.rgbBlue  = ( BYTE ) ( dwCol & 0xff );
        rgb.rgbReserved = 0;
      }
    break;

    default:
      ASSERT( FALSE );
      // error: unsupported bitmap format, bitcount
    break;
  }

  return rgb;
} // CBitmapLib::GetRGB( )

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

void  CBitmapLib::SetDIBPixel( BITMAPINFO const * pBmi,
                               UINT const         uLine,
                               UINT const         uCol,
                               BYTE *             pBits,
                               BYTE const         sPixelVal )
{
  BITMAPINFOHEADER const * pBmih = &pBmi->bmiHeader;
  
  DWORD dwBytesPerLine = pBmih->biSizeImage / pBmih->biHeight;

  switch ( pBmih->biBitCount )
  {
    // device independent bitmaps
    case 1:
      {
        DWORD dwBytePos = uLine * dwBytesPerLine + ( uCol >> 3 );

        pBits[ dwBytePos ] |= ( sPixelVal & 0x1 ) << ~( ( BYTE ) uCol & 0x7 );
      }
    break;

    case 4:
      {
        DWORD dwBytePos = uLine * dwBytesPerLine + ( uCol >> 1 );

        if ( uCol % 2 == 1 )
          pBits[ dwBytePos ] |= ( sPixelVal & 0xf );
        else
          pBits[ dwBytePos ] |= ( sPixelVal & 0xf ) << 4;
      }
    break;

    case 8:
      pBits[ uLine * dwBytesPerLine + uCol ] = ( BYTE ) ( sPixelVal & 0xff );
    break;

    // device dependent bitmaps
    case 16:
    case 24:
    case 32:
      ASSERT( FALSE );
    break;

    default:
      ASSERT( FALSE );
      // error: unsupported bitmap format, bitcount
    break;
  }
} // CBitmapLib::SetDIBPixel( )

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////

void  CBitmapLib::SetRGBPixel( BITMAPINFO const * pBmi,
                               UINT const         uLine,
                               UINT const         uCol,
                               BYTE *             pBits,
                               RGBQUAD            rgbPixel )
{
  BITMAPINFOHEADER const * pBmih = &pBmi->bmiHeader;
  
  DWORD dwBytesPerLine = pBmih->biSizeImage / pBmih->biHeight;

  switch ( pBmih->biBitCount )
  {
    // device independent bitmaps
    case 1:
    case 4:
    case 8:
      ASSERT( FALSE );
    break;

    // device dependent bitmaps
    case 16:
      {
        // set rgb ranges to 5 bit, reduce ranges from 256 to 32
        rgbPixel.rgbBlue  >>= 3; 
        rgbPixel.rgbGreen >>= 3;
        rgbPixel.rgbRed   >>= 3;

        WORD wColVal = ( WORD ) (  ( WORD ) rgbPixel.rgbBlue << 10
                                 | ( WORD ) rgbPixel.rgbGreen << 5
                                 | ( WORD ) rgbPixel.rgbRed );
        ( ( WORD * ) pBits )[ uLine * dwBytesPerLine / 2 + uCol ] = wColVal;
      }
    break;

    case 24:
      pBits[ uLine * dwBytesPerLine + uCol * 3 + 2 ] = rgbPixel.rgbRed;
      pBits[ uLine * dwBytesPerLine + uCol * 3 + 1 ] = rgbPixel.rgbGreen;
      pBits[ uLine * dwBytesPerLine + uCol * 3 ]     = rgbPixel.rgbBlue;
    break;

    case 32:
      {
        DWORD dwColVal =   ( DWORD ) rgbPixel.rgbRed << 16
                         | ( DWORD ) rgbPixel.rgbGreen << 8
                         | ( DWORD ) rgbPixel.rgbBlue;
        ( ( DWORD * ) pBits )[ uLine * dwBytesPerLine / 4 + uCol ] = dwColVal;
      }
    break;

    default:
      ASSERT( FALSE );
      // error: unsupported bitmap format, bitcount
    break;
  }
} // CBitmapLib::SetRGBPixel( )

//////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////
