// ImpresionPP.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "ImpresionPP.h"


// CImpresionPP dialog

IMPLEMENT_DYNAMIC(CImpresionPP, CPropertyPage)

CImpresionPP::CImpresionPP()
	: CPropertyPage(CImpresionPP::IDD)
{

}

CImpresionPP::~CImpresionPP()
{
}

void CImpresionPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ECOLUMNAS, m_EColumnas);
	DDX_Control(pDX, IDC_EFILAS, m_EFilas);
	DDX_Control(pDX, IDC_SENCABEZADO, m_SEncabezado);
	DDX_Control(pDX, IDC_BENCABEZADO, m_BEncabezado);
	DDX_Control(pDX, IDC_SPIE, m_SPie);
	DDX_Control(pDX, IDC_BPIE, m_BPie);
	DDX_Control(pDX, IDC_STITULOS, m_STitulos);
	DDX_Control(pDX, IDC_BTITULOS, m_BTitulos);
	DDX_Control(pDX, IDC_STEXTO, m_STexto);
	DDX_Control(pDX, IDC_BTEXTO, m_BTexto);
}


BEGIN_MESSAGE_MAP(CImpresionPP, CPropertyPage)
	ON_EN_CHANGE(IDC_ECOLUMNAS, &CImpresionPP::OnEnChangeEcolumnas)
	ON_EN_CHANGE(IDC_EFILAS, &CImpresionPP::OnEnChangeEfilas)
	ON_BN_CLICKED(IDC_BENCABEZADO, &CImpresionPP::OnBnClickedBencabezado)
	ON_BN_CLICKED(IDC_BPIE, &CImpresionPP::OnBnClickedBpie)
	ON_BN_CLICKED(IDC_BTITULOS, &CImpresionPP::OnBnClickedBtitulos)
	ON_BN_CLICKED(IDC_BTEXTO, &CImpresionPP::OnBnClickedBtexto)
END_MESSAGE_MAP()


// CImpresionPP message handlers

void CImpresionPP::OnEnChangeEcolumnas()
{
	SetModified();
}

void CImpresionPP::OnEnChangeEfilas()
{
	SetModified();
}

void CImpresionPP::OnBnClickedBencabezado()
{
	LOGFONT lfont;
	m_FEncabezado.GetLogFont(&lfont);
	CFontDialog dlg(&lfont,CF_SCREENFONTS);
	if( dlg.DoModal()==IDOK) {

		dlg.GetCurrentFont(&lfont);
		m_FEncabezado.DeleteObject();
		m_FEncabezado.CreateFontIndirect(&lfont);
		m_SEncabezado.SetFont(&m_FEncabezado);
		SetModified();
	}
}

void CImpresionPP::OnBnClickedBpie()
{
	LOGFONT lfont;
	m_FPie.GetLogFont(&lfont);
	CFontDialog dlg(&lfont,CF_SCREENFONTS);
	if( dlg.DoModal()==IDOK) {
		dlg.GetCurrentFont(&lfont);
		m_FPie.DeleteObject();
		m_FPie.CreateFontIndirect(&lfont);
		m_SPie.SetFont(&m_FPie);
		SetModified();
	}
}

void CImpresionPP::OnBnClickedBtitulos()
{
	LOGFONT lfont;
	m_FTitulo.GetLogFont(&lfont);
	CFontDialog dlg(&lfont,CF_SCREENFONTS);
	if( dlg.DoModal()==IDOK) {
		dlg.GetCurrentFont(&lfont);
		m_FTitulo.DeleteObject();
		m_FTitulo.CreateFontIndirect(&lfont);
		m_STitulos.SetFont(&m_FTitulo);
		SetModified();
	}
}

void CImpresionPP::OnBnClickedBtexto()
{
	LOGFONT lfont;
	m_FTexto.GetLogFont(&lfont);
	CFontDialog dlg(&lfont,CF_SCREENFONTS);
	if( dlg.DoModal()==IDOK) {
		dlg.GetCurrentFont(&lfont);
		m_FTexto.DeleteObject();
		m_FTexto.CreateFontIndirect(&lfont);
		m_STexto.SetFont(&m_FTexto);
		SetModified();
	}
}

bool CImpresionPP::Aceptar() {

	LOGFONT lfont;
	CString temp;
	CDC *hDC= GetDC();

	m_EColumnas.GetWindowText(temp);
	if(temp.IsEmpty()) {
		MessageBox("Por favor ingrese un valor para columnas");
		m_EColumnas.SetFocus();
		return false;
	}
	AfxGetApp()->WriteProfileInt("printer", "cellpadding", atol(temp));
	m_EFilas.GetWindowText(temp);
	if(temp.IsEmpty()) {
		MessageBox("Por favor ingrese un valor para filas");
		m_EFilas.SetFocus();
		return false;
	}
	AfxGetApp()->WriteProfileInt("printer", "rowpadding", atol(temp));


	m_FEncabezado.GetLogFont(&lfont);
	AfxGetApp()->WriteProfileInt("printer\\header","tamano",-MulDiv(lfont.lfHeight, 72,GetDeviceCaps(*hDC, LOGPIXELSY)));
	AfxGetApp()->WriteProfileInt("printer\\header", "weight", lfont.lfWeight),
		AfxGetApp()->WriteProfileInt("printer\\header","italic",lfont.lfItalic);
	AfxGetApp()->WriteProfileString("printer\\header", "font",lfont.lfFaceName);

	m_FPie.GetLogFont(&lfont);
	AfxGetApp()->WriteProfileInt("printer\\footer","tamano",-MulDiv(lfont.lfHeight, 72,GetDeviceCaps(*hDC, LOGPIXELSY)));
	AfxGetApp()->WriteProfileInt("printer\\footer", "weight", lfont.lfWeight),
		AfxGetApp()->WriteProfileInt("printer\\footer","italic",lfont.lfItalic);
	AfxGetApp()->WriteProfileString("printer\\footer", "font",lfont.lfFaceName);

	m_FTitulo.GetLogFont(&lfont);
	AfxGetApp()->WriteProfileInt("printer\\column","tamano",-MulDiv(lfont.lfHeight, 72,GetDeviceCaps(*hDC, LOGPIXELSY)));
	AfxGetApp()->WriteProfileInt("printer\\column", "weight", lfont.lfWeight),
		AfxGetApp()->WriteProfileInt("printer\\column","italic",lfont.lfItalic);
	AfxGetApp()->WriteProfileString("printer\\column", "font",lfont.lfFaceName);

	m_FTexto.GetLogFont(&lfont);
	AfxGetApp()->WriteProfileInt("printer\\body","tamano",-MulDiv(lfont.lfHeight, 72,GetDeviceCaps(*hDC, LOGPIXELSY)));
	AfxGetApp()->WriteProfileInt("printer\\body", "weight", lfont.lfWeight),
		AfxGetApp()->WriteProfileInt("printer\\body","italic",lfont.lfItalic);
	AfxGetApp()->WriteProfileString("printer\\body", "font",lfont.lfFaceName);

	return true;
}

void CImpresionPP::OnOK()
{
	if(Aceptar())
		CPropertyPage::OnOK();
}

BOOL CImpresionPP::OnApply()
{
	return Aceptar() ? CPropertyPage::OnApply() : false;
}

BOOL CImpresionPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	CString temp;
	temp.Format("%d", AfxGetApp()->GetProfileInt("printer", "cellpadding", 1));
	m_EColumnas.SetWindowText(temp);
	temp.Format("%d", AfxGetApp()->GetProfileInt("printer", "rowpadding", 1));
	m_EFilas.SetWindowText(temp);


	CDC *hDC= GetDC();
	// fuente para el encabezado

	m_FEncabezado.DeleteObject();
	VERIFY(m_FEncabezado.CreateFont(
		-MulDiv(AfxGetApp()->GetProfileInt("printer\\header","tamano",12), GetDeviceCaps(*hDC, LOGPIXELSY), 72),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		AfxGetApp()->GetProfileInt("printer\\header", "weight", FW_BOLD),                 // nWeight
		AfxGetApp()->GetProfileInt("printer\\header","italic",false),                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		AfxGetApp()->GetProfileString("printer\\header", "font",_T("Arial"))));                 // lpszFacename

	m_SEncabezado.SetFont(&m_FEncabezado);

	m_FPie.DeleteObject();
	//// fuente para el pie de pagina
	VERIFY(m_FPie.CreateFont(
		-MulDiv(AfxGetApp()->GetProfileInt("printer\\footer","tamano",10), GetDeviceCaps(*hDC, LOGPIXELSY), 72),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		AfxGetApp()->GetProfileInt("printer\\footer", "weight", FW_NORMAL),                 // nWeight
		AfxGetApp()->GetProfileInt("printer\\footer","italic",false),                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		AfxGetApp()->GetProfileString("printer\\footer", "font",_T("Arial"))));                 // lpszFacename

	m_SPie.SetFont(&m_FPie);

	////// fuente para las columnas

	m_FTitulo.DeleteObject();
	VERIFY(m_FTitulo.CreateFont(
		-MulDiv(AfxGetApp()->GetProfileInt("printer\\column","tamano",9), GetDeviceCaps(*hDC, LOGPIXELSY), 72),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		AfxGetApp()->GetProfileInt("printer\\column", "weight", FW_BOLD),                 // nWeight
		AfxGetApp()->GetProfileInt("printer\\column","italic",false),                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		AfxGetApp()->GetProfileString("printer\\column", "font",_T("Arial"))));                 // lpszFacename

	m_STitulos.SetFont(&m_FTitulo);


	////// fuente para el texto de la pagina

	m_FTexto.DeleteObject();
	VERIFY(m_FTexto.CreateFont(
		-MulDiv(AfxGetApp()->GetProfileInt("printer\\body","tamano",9), GetDeviceCaps(*hDC, LOGPIXELSY), 72),                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		AfxGetApp()->GetProfileInt("printer\\body", "weight", FW_NORMAL),                 // nWeight
		AfxGetApp()->GetProfileInt("printer\\body","italic",false),                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		AfxGetApp()->GetProfileString("printer\\body", "font",_T("Times New Roman"))));                 // lpszFacename

	m_STexto.SetFont(&m_FTexto);

	return TRUE;
}
