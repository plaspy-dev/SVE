// CamarasPP.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "CamarasPP.h"

// CCamarasPP dialog

IMPLEMENT_DYNAMIC(CCamarasPP, CPropertyPage)

CCamarasPP::CCamarasPP()
	: CPropertyPage(CCamarasPP::IDD)
{

}

CCamarasPP::~CCamarasPP()
{
}

void CCamarasPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CREINICIAR, m_CReiniciar);
	DDX_Control(pDX, IDC_ETIEMPO, m_ETiempo);
}


BEGIN_MESSAGE_MAP(CCamarasPP, CPropertyPage)
	ON_BN_CLICKED(IDC_CREINICIAR, &CCamarasPP::OnBnClickedCreiniciar)
	ON_EN_CHANGE(IDC_ETIEMPO, &CCamarasPP::OnEnChangeEtiempo)
END_MESSAGE_MAP()


// CCamarasPP message handlers

void CCamarasPP::OnBnClickedCreiniciar()
{
	if(m_CReiniciar.GetCheck()) {
		if(MyMessageBox("¿Esta seguro de habilitar esta opcion?.\rEsto ralentizara el programa.", MB_ICONQUESTION | MB_YESNOCANCEL)!=IDYES) {
			m_CReiniciar.SetCheck(false);
			return;
		}
	}
	SetModified();
}

void CCamarasPP::OnEnChangeEtiempo()
{
	SetModified();
}

BOOL CCamarasPP::OnSetActive()
{
	MyMessageBox("Los ajustes de camaras se recomiendan para usuarios experimentados.", MB_OK, "avanzado");
	return CPropertyPage::OnSetActive();
}

BOOL CCamarasPP::OnApply()
{
	return Aceptar() ? CPropertyPage::OnApply() : false;
}

void CCamarasPP::OnOK()
{
	if(Aceptar())
		CPropertyPage::OnOK();
}

bool CCamarasPP::Aceptar() {

	CString temp;
	m_ETiempo.GetWindowText(temp);
	if( temp.IsEmpty()) {
		MessageBox("Por favor coloque un tiempo de actualización");
		m_ETiempo.SetFocus();
		return false;
	}
	if(atoi(temp)<100) {
		if(MessageBox("¿Esta seguro de colocar el tiempo de actualización en '"+ temp + "'.\rEsto ralentizara el programa.?", NULL, MB_ICONQUESTION | MB_YESNOCANCEL)!=IDYES) {
			return false;
		}
	}
	AfxGetApp()->WriteProfileInt("camaras","refresh", atoi(temp));
	AfxGetApp()->WriteProfileInt("local", "reiniciar", m_CReiniciar.GetCheck());
	return true;
}

BOOL CCamarasPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_CReiniciar.SetCheck(AfxGetApp()->GetProfileInt("local", "reiniciar", 0));
	CString temp;
	temp.Format("%d", AfxGetApp()->GetProfileInt("camaras","refresh", 100));
	m_ETiempo.SetWindowText(temp);
	return TRUE;
}
