//////////////////////////////////////////////////////////////////////
//
//  This class is designed to provide simple interface for 
//  simultaneous Video Capture & Preview using DirectShow
//
//////////////////////////////////////////////////////////////////////
//
//	References: MS DirectShow Samples
//
//		
//////////////////////////////////////////////////////////////////////
//
//	This class was written by Sagar K.R . 
//  Use of this class is not restricted in any
//	way whatsoever.Please report the bugs to krssagar@firsteccom.co.kr
//
//	Special thanks to all the members at The Code Project! 
//	(www.codeproject.com)
//
//////////////////////////////////////////////////////////////////////
// Showpic.cpp : implementation file
//

#include "stdafx.h"
# include "jfinger.h"
#include "Showpic.h"
# include <atlimage.h>


DEFINE_GUID(ImageFormatJPEG, 0xb96b3cae,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowpic

CShowpic::CShowpic()
{
	m_un32ImageWidth=AfxGetApp()->GetProfileInt("bitmap", "ancho", 320);
	m_un32ImageHeight=AfxGetApp()->GetProfileInt("bitmap", "alto", 240);
	m_un32ImageBits=24;
	m_pImageBytes=NULL;
	m_hBitmap=NULL;
	MakeBMPHeader();
	m_pBytes=NULL;
}

CShowpic::~CShowpic()
{
	if(m_pBmp)
	delete []m_pBmp;
	m_pBmp=NULL;
	if (m_hBitmap) ::DeleteObject(m_hBitmap);
	if( m_pBytes) {
		delete m_pBytes;
		m_pBytes=NULL;
	}
}


BEGIN_MESSAGE_MAP(CShowpic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(2000, OnGuardar)
	ON_COMMAND(2001, OnCopiar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowpic message handlers

void CShowpic::MakeBMPHeader(int alto,int ancho, int bits) {
	if(m_pBmp)
		delete m_pBmp;
	m_pBmp=NULL;
	m_un32ImageHeight= alto;
	m_un32ImageBits= bits;
	m_un32ImageWidth= ancho;
	MakeBMPHeader();
	Invalidate(false);
}

CSize CShowpic::GetTamano() {

	return CSize(m_un32ImageWidth, m_un32ImageHeight);
}

void CShowpic::SetTitulo(CString titulo/*=""*/) {

	m_szTitulo=titulo;
}

void CShowpic::MakeBMPHeader()
{	

	DWORD  dwBitmapInfoSize;
	dwBitmapInfoSize = sizeof(BITMAPINFO);
	m_pBmp    = (BITMAPINFO *)new BYTE [dwBitmapInfoSize];

	m_pBmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBmp->bmiHeader.biWidth = m_un32ImageWidth;
    m_pBmp->bmiHeader.biHeight = m_un32ImageHeight*-1;	
    m_pBmp->bmiHeader.biPlanes = 1;
    m_pBmp->bmiHeader.biBitCount =(unsigned short) m_un32ImageBits;
    m_pBmp->bmiHeader.biCompression = BI_RGB;
    m_pBmp->bmiHeader.biSizeImage = 0;
    m_pBmp->bmiHeader.biXPelsPerMeter = 0;
    m_pBmp->bmiHeader.biYPelsPerMeter = 0;
    m_pBmp->bmiHeader.biClrUsed = 0;
    m_pBmp->bmiHeader.biClrImportant = 0;

	if( m_un32ImageBits==1) {

#ifndef _DEBUG
		m_pBmp->bmiColors[0].rgbBlue=255;
		m_pBmp->bmiColors[0].rgbGreen=255;
		m_pBmp->bmiColors[0].rgbRed=255;
# endif
	}
}

bool CShowpic::HasImage() {

	return m_hBitmap || m_pImageBytes;
}

void CShowpic::SetWindowText(LPCTSTR lpszString) {
	CStatic::SetWindowText(lpszString);
	Invalidate();
}

unsigned char *CShowpic::GetCompress(int &tamano,  int calidad/*=75*/) {

	unsigned char *resultado= new unsigned char[tamano];

	CTonyJpegEncoder encoder(calidad);
	tamano=0;

	if(encoder.CompressImage(m_pImageBytes, resultado, m_un32ImageWidth, m_un32ImageHeight, tamano)) {
		
		return resultado;
	} else {
		
		delete []resultado;
		return 0;
	}
}

void CShowpic::OnPaint() 
{
	CRect rect;
    GetClientRect(rect);
	CPaintDC dc(this); // device context for painting

    // If no bitmap selected, simply erase the background as per normal and return
    if (!m_hBitmap)
    {
		// TODO: Add your message handler code here
		if(m_pImageBytes!=NULL)
		{
			bool espejo= AfxGetApp()->GetProfileInt("camaras", "espejo", 0);
			dc.RealizePalette();
			SetStretchBltMode(dc.GetSafeHdc(), COLORONCOLOR);
			int nResult = ::StretchDIBits(
									dc.GetSafeHdc(),
									espejo ? rect.left : rect.right,
									rect.top,
									espejo ? rect.Width() : -rect.Width(),
									rect.Height(),
									0,
									0,
									m_un32ImageWidth, 
									m_un32ImageHeight,
									m_pImageBytes, 
									m_pBmp,
									DIB_RGB_COLORS,
									SRCCOPY);
		} else {
			Default();
		}
		return;
	} else if( m_hBitmap) {

		// We have a bitmap - draw it.

		// Create compatible memory DC using the controls DC
		CDC dcMem;
		VERIFY( dcMem.CreateCompatibleDC(&dc));
	    
		// Select bitmap into memory DC.
		HBITMAP* pBmpOld = (HBITMAP*) ::SelectObject(dcMem.m_hDC, m_hBitmap);

		dc.SetStretchBltMode(COLORONCOLOR);

		// StretchBlt bitmap onto static's client area
		dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
						&dcMem, 0, 0, m_bmInfo.bmWidth-1, m_bmInfo.bmHeight-1,
						SRCCOPY);
		::SelectObject(dcMem.m_hDC, pBmpOld);
	}
}

BOOL CShowpic::SetBitmap(HBITMAP hBitmap)
{
	if( m_pBytes) {
		delete m_pBytes;
		m_pBytes=NULL;
	}
	m_pImageBytes=NULL;
    ::DeleteObject(m_hBitmap);
    m_hBitmap = hBitmap;
	bool retorno=false;
	if( m_hBitmap)
		retorno= ::GetObject(m_hBitmap, sizeof(BITMAP), &m_bmInfo);
	Invalidate();
	return retorno;
}

void CShowpic::ShowImage(BYTE *pImageByte)
{
	if (m_hBitmap) 
		::DeleteObject(m_hBitmap);
	m_hBitmap=NULL;
	m_pImageBytes=pImageByte;
	this->Invalidate();
}

void CShowpic::GetBitmap(CBitmap *bitmap) {

	ASSERT(false);
	return;
}

bool CShowpic::SetBitmap(CByteArray &pImage) {

	if(pImage.IsEmpty())
		return false;
	m_pJPG.RemoveAll();
	m_pJPG.Copy(pImage);
	m_pImageBytes=NULL;
	HBITMAP bmp= CEnBitmap::LoadImageBuffer(m_pJPG);
	if(bmp && AfxGetApp()->GetProfileInt("camaras", "espejo", 0)==0) {

		CEnBitmap cbmp;

		VERIFY(cbmp.Attach(bmp));
		VERIFY(cbmp.FlipImage(true, false));
		bmp= (HBITMAP)cbmp.Detach();
	}
	return SetBitmap(bmp);
}

void CShowpic::OnGuardar() {

	if(!HasImage())
		return;
	CFileDialog dlg( false, "jpg", m_szTitulo, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, "Archivos JPG(*.jpg)|*.jpg| Todos los archivos (*.*)|*.*||");
	if(dlg.DoModal()==IDOK) {

		CWaitCursor wait;

		::DeleteFile(dlg.GetPathName());

		if(AfxGetApp()->GetProfileInt("camaras", "espejo", 0)==0) {
			CImage imagen;
			imagen.Attach(m_hBitmap);
			if(FAILED(imagen.Save(dlg.GetPathName(), ImageFormatJPEG))) {

				AfxMessageBox("No se puede obtener acceso a \"" + dlg.GetPathName() + "\".\nCompruebe que el archivo no este siendo utilizado por otro proceso.", MB_OK | MB_ICONERROR);
				return;
			}
			imagen.Detach();
		} else {
			CFile archivo;
			if(!archivo.Open(dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate)) {
				AfxMessageBox("No se puede obtener acceso a \"" + dlg.GetPathName() + "\".\nCompruebe que el archivo no este siendo utilizado por otro proceso.", MB_OK | MB_ICONERROR);
				return;
			}
			archivo.Write(m_pJPG.GetData(), m_pJPG.GetCount());
			archivo.Close();
		}
		if( MyMessageBox("¿Desea ver el archivo?", MB_ICONQUESTION | MB_YESNOCANCEL, "showfile")==IDYES) 
			ShellExecute(NULL, "open", dlg.GetPathName(), NULL,NULL,SW_SHOWNORMAL);
	}
}

void CShowpic::OnCopiar() {

	CWaitCursor wait;
	if(!HasImage())
		return;
	if(!OpenClipboard())
		return;
	if(!EmptyClipboard())
		return;

	CEnBitmap bmp;
	if(!bmp.CopyImage(m_hBitmap))
		return;
	HBITMAP bmptemp=  (HBITMAP)bmp.Detach();
	SetClipboardData(CF_BITMAP, bmptemp);
	CloseClipboard();
}

void CShowpic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_szTitulo.IsEmpty()) {
		OnGuardar();
	}
	CStatic::OnLButtonDblClk(nFlags, point);
}

void CShowpic::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(!m_szTitulo.IsEmpty()) {
		CNewMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 2000, "Guardar Como");
		menu.AppendMenu(MF_STRING, 2001, "Copiar");
		menu.SetDefaultItem(2000);

		POINT point;
		GetCursorPos( &point );
		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	}
	CStatic::OnRButtonDown(nFlags, point);
}
