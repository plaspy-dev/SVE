//////////////////////////////////////////////////////////////////////
//
// BitmapLib.h: Definition of class CBitmapLib
//
// Author:
// Gerd Esser
// Germany
// eMail: admin@gerdesser.de
// http://www.gerdesser.de
//
// History:
// 13.09.2000 - Creation
//
//////////////////////////////////////////////////////////////////////

#ifndef BITMAPLIB_H
#define BITMAPLIB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include <afxcmn.h>

//////////////////////////////////////////////////////////////////////
//
// The class CBitmapLib has basically 2 functions:
//
// + counting number of used colors within a bitmap
// + changing color depth of a bitmap
//
//////////////////////////////////////////////////////////////////////

class CBitmapLib  
{

public:

  // error index
  enum
  {
    blUnsupportedCompression = -1,
    blColorOverflow = -2,
    blNotEnoughMemory = -3,
  };

public:
	
  ////////////////////////////////////////////////////////////////////
  //
  // standard constructor / destructor
  //
  ////////////////////////////////////////////////////////////////////

  CBitmapLib();
	virtual ~CBitmapLib();

  ////////////////////////////////////////////////////////////////////
  //
  // static int CBitmapLib::GetUsedColors( BITMAPINFO const * pBmi,
  //                                       BYTE const *       pBits,
  //                                       UINT const         nStartScanLine,
  //                                       UINT const         nScanLines,
  //                                       RGBQUAD *          pColorTable, 
  //                                       WORD const         wMaxColorTableSize,
  //                                       WORD &             wUsedColors );
  //
  // Parameters:
  // pBmi - pointer to a BITMAPINFO structure of the bitmap
  // pBits - pointer to pixel data of the bitmap
  // nStartScanLine - number of pixel line, at which the 
  //                  scanning will start
  // nScanLines - number of pixel lines which will be scanned
  //              beginning from nStartScanLine
  // pColorTable - pointer to a color table
  // wMaxColorTableSize - max. entries within pColorTable
  // wUsedColors - number of colors within pColorTable
  //
  // GetUsedColors() counts the number of used colors within a
  // bitmap, which is stored in memory.
  //
  // The scanning starts at line position nStartScanLines and 
  // nScanLines will be scanned. This can be used for scanning
  // also great bitmaps by banding it into smaller parts.
  //
  // The colors that will be found are stored into a color
  // table pColorTable. If pColorTable is a blanc one,
  // wUsedColors has to be set to 0. In that case GetUsedColors()
  // indentifies new colors, add's them to pColorTable and 
  // returns the number of colors in wUsedColors. If the bitmap
  // contains more than wMaxColorTableSize colors, the method
  // aborts whith return code blColorOverflow.
  // 
  // If a prefilled pColorTable is given to GetUsedColord(),
  // the method will add only those colors, that are not
  // yet stored in pColorTable. The method returns the 
  // used colors in wUsedColors, but which is not
  // the number of colors in the present bitmap or
  // bitmap band. This makes sence in "banding" mode, where
  // the same bitmap is treated in a loop of bands, allways
  // giving the same color table to the method. The result
  // is the overall number of colors in the bitmap.
  // 
  // The method handles only bitmaps of compression type BI_RGB.
  //
  // The method returns the number of used colors in 
  // color table pColorTable (identical to wUsedColors).
  // In case of an error, the return value is negativ:
  // blUnsupportedCompression - compression format
  //                            of bitmap is something other than 
  //                            BI_RGB
  // blColorOverflow          - the number of colors is greater than
  //                            wMaxColorTableSize.
  //
  ////////////////////////////////////////////////////////////////////

  static int GetUsedColors( BITMAPINFO const * pBmi,
                            BYTE const *       pBits,
                            UINT const         nStartScanLine,
                            UINT const         nScanLines,
                            RGBQUAD *          pColorTable, 
                            WORD const         wMaxColorTableSize,
                            WORD &             wCurColorTableSize );

  ////////////////////////////////////////////////////////////////////
  //
  // static int GetUsedColors( CString const & stSrcBitmapFile,
  //                           RGBQUAD *       pColorTable,
  //                           WORD const      wMaxColorTableSize,
  //                           CProgressCtrl * pProgressCtrl = NULL );
  //
  // Parameters:
  // stSrcBitmapFile - name of source bitmap file
  // pColorTable - pointer to a color table
  // wMaxColorTableSize - max. entries within pColorTable
  // pProgressCtrl - pointer of standard MFC progress control as
  //                 part of a dialog box. The progress control
  //                 will initialized and stepped by the method.
  //                 If pProgressCtrl is NULL, no progress control
  //                 will be managed.
  //
  // GetUsedColors() counts the number of used colors within a
  // file based bitmap.
  //
  // The colors that will be found are stored into a color
  // table pColorTable. If the bitmap
  // contains more than wMaxColorTableSize colors, the method
  // aborts whith return code blColorOverflow.
  // 
  // The method also handles great bitmaps. For this case a 
  // progress control within a dialog box can be managed, because
  // the operation than needs a certain time.
  //
  // The method handles only bitmaps of compression type BI_RGB.
  //
  // The method returns the number of used colors in 
  // color table pColorTable.
  // In case of an error, the return value is negativ:
  // blUnsupportedCompression - compression format
  //                            of bitmap is something other than 
  //                            BI_RGB
  // blColorOverflow          - the number of colors is greater than
  //                            wMaxColorTableSize.
  // blNotEnoughMemory        - not enough memory for reading a bitmap
  //                            band
  //
  ////////////////////////////////////////////////////////////////////

  static int GetUsedColors( CString const & stSrcBitmapFile,
                            RGBQUAD *       pColorTable,
                            WORD const      wMaxColorTableSize,
                            CProgressCtrl * pProgressCtrl = NULL );

  ////////////////////////////////////////////////////////////////////
  //
  // static BOOL  ChangeColorDepth( CString const & stSrcBitmapFile,
  //                                CString const & stDstBitmapFile,
  //                                WORD const      wDstBitCount,
  //                                CProgressCtrl * pProgressCtrl = NULL );
  //
  // stSrcBitmapFile - name of source bitmap file
  // stDstBitmapFile - name of destination bitmap file
  // wDstBitCount - number of bits per pixel (color depth) of 
  //                the destination bitmap
  // pProgressCtrl - pointer of standard MFC progress control as
  //                 part of a dialog box. The progress control
  //                 will initialized and stepped by the method.
  //                 If pProgressCtrl is NULL, no progress control
  //                 will be managed.
  //
  // ChangeColorDepth() changes the bits per pixel of a bitmap, by 
  // increasing or decreasing the number of bits per pixel. Valid
  // values for wDstBitCount are 1, 4, 8, 16, 24 and 32. By doing this
  // the method also changes the bitmap from a device independent
  // bitmap (wDstBitCount 1 to 8) to a device dependent bitmap
  // (wDstBitCount 16 to 32) or the way back.
  //
  // ChangeColorDepth() maps the bitmap into a memory DC. If the 
  // DC is not sufficient enough, the colors will be slightly 
  // affected at operations decreasing the color depth.
  //
  // The method also handles great bitmaps. For this case a 
  // progress control within a dialog box can be managed, because
  // the operation than needs a certain time.
  //
  // The method handles only bitmaps of compression type BI_RGB.
  //
  // The method returns FALSE in case of error, otherwise TRUE.
  // 
  ////////////////////////////////////////////////////////////////////

  static BOOL  ChangeColorDepth( CString const & stSrcBitmapFile,
                                 CString const & stDstBitmapFile,
                                 WORD const      wDstBitCount,
                                 CProgressCtrl * pProgressCtrl = NULL );

private:

  typedef struct tagColorEntry
  {
    WORD     wIndex;
    RGBQUAD  rgb;
  } COLORENTRY;

private:

  ////////////////////////////////////////////////////////////////////
  //
  // Gets a RGB value out of a bitmap (device independent or dependent).
  //
  ////////////////////////////////////////////////////////////////////

  static RGBQUAD  GetRGB( BITMAPINFO const * pBmi,
                          UINT const         uLine,
                          UINT const         uCol,
                          BYTE const *       pBits );

  ////////////////////////////////////////////////////////////////////
  //
  // Sets a pixel into a device independent bitmap.
  //
  ////////////////////////////////////////////////////////////////////

  static void  SetDIBPixel( BITMAPINFO const * pBmi,
                            UINT const         uLine,
                            UINT const         uCol,
                            BYTE *             pBits,
                            BYTE const         sPixelVal );

  ////////////////////////////////////////////////////////////////////
  //
  // Sets a pixel into a device dependent bitmap.
  //
  ////////////////////////////////////////////////////////////////////

  static void  SetRGBPixel( BITMAPINFO const * pBmi,
                            UINT const         uLine,
                            UINT const         uCol,
                            BYTE *             pBits,
                            RGBQUAD            rgbPixel );

}; // class CBitmapLib

#endif // BITMAPLIB_H
