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
#pragma once

// Showpic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowpic window

class __JFINGER_EXPORT__ CShowpic : public CStatic
{
// Construction
protected:
	CString		m_szTitulo;
	CByteArray	m_pJPG;
public:
	CShowpic();

	int m_un32ImageWidth;
	int m_un32ImageHeight;
	int m_un32ImageBits;
	BYTE *m_pImageBytes;
	BYTE *m_pBytes;
	bool HasImage();
	void GetBitmap(CBitmap *bitmap);
	CSize GetTamano();
public:
	HBITMAP  m_hBitmap;
	BITMAPINFO* m_pBmp;
	BITMAP   m_bmInfo;
	unsigned char *GetCompress(int &tamano, int calidad=75);
	void SetWindowText(LPCTSTR lpszString);
	void SetTitulo(CString titulo="");

// Implementation
public:
	void ShowImage(BYTE *pImageByte);
	void MakeBMPHeader();
	void MakeBMPHeader(int alto,int ancho, int bits);
	BOOL SetBitmap(HBITMAP hBitmap);
	virtual ~CShowpic();
	bool SetBitmap(CByteArray &pImage);

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void OnGuardar();
	void OnCopiar();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

