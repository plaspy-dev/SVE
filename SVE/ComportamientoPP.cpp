// ComportamientoPP.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "ComportamientoPP.h"
# include <atlbase.h>

// CComportamientoPP dialog

IMPLEMENT_DYNAMIC(CComportamientoPP, CPropertyPage)

CComportamientoPP::CComportamientoPP()
	: CPropertyPage(CComportamientoPP::IDD)
{

}

CComportamientoPP::~CComportamientoPP()
{
}

void CComportamientoPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBLOQUEAR, m_BBloquear);
	DDX_Control(pDX, IDC_ETIEMPO, m_ETiempo);
	DDX_Control(pDX, IDC_CEFECTO, m_CEfecto);
	DDX_Control(pDX, IDC_CTOOLTIP, m_CToolTip);
	DDX_Control(pDX, IDC_CMENSAJE, m_CMessageBox);
	DDX_Control(pDX, IDC_CTEXTO, m_CTexto);
	DDX_Control(pDX, IDC_CENTER, m_CEnter);
	DDX_Control(pDX, IDC_CFLECHAS, m_CFlechas);
}


BEGIN_MESSAGE_MAP(CComportamientoPP, CPropertyPage)
	ON_BN_CLICKED(IDC_CBLOQUEAR, &CComportamientoPP::OnBnClickedCbloquear)
	ON_BN_CLICKED(IDC_CEFECTO, &CComportamientoPP::OnBnClickedCefecto)
	ON_BN_CLICKED(IDC_CTOOLTIP, &CComportamientoPP::OnBnClickedCtooltip)
	ON_BN_CLICKED(IDC_CMENSAJE, &CComportamientoPP::OnBnClickedCmensaje)
	ON_BN_CLICKED(IDC_CTEXTO, &CComportamientoPP::OnBnClickedCtexto)
	ON_BN_CLICKED(IDC_CENTER, &CComportamientoPP::OnBnClickedCenter)
	ON_BN_CLICKED(IDC_CFLECHAS, &CComportamientoPP::OnBnClickedCflechas)
	ON_EN_CHANGE(IDC_ETIEMPO, &CComportamientoPP::OnEnChangeEtiempo)
END_MESSAGE_MAP()


// CComportamientoPP message handlers

void CComportamientoPP::OnBnClickedCbloquear()
{
	m_CEfecto.EnableWindow(m_BBloquear.GetCheck());
	m_ETiempo.EnableWindow(m_BBloquear.GetCheck());
	SetModified();
}

BOOL CComportamientoPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_BBloquear.SetCheck(AfxGetApp()->GetProfileInt("control", "timeout", 300));
	m_CEfecto.EnableWindow(m_BBloquear.GetCheck());
	m_ETiempo.EnableWindow(m_BBloquear.GetCheck());

	CString temp;
	temp.Format("%d" ,AfxGetApp()->GetProfileInt("control", "timeout", 300)/60);
	m_ETiempo.SetWindowText(temp);
	m_CEfecto.SetCheck(AfxGetApp()->GetProfileInt("control", "efecto", 1));
	
	m_CToolTip.SetCheck(AfxGetApp()->GetProfileInt("componentes","tooltip", 1));

	m_CTexto.SetCheck(AfxGetApp()->GetProfileInt("toolbar", "showtext",false));

	m_CFlechas.SetCheck(AfxGetApp()->GetProfileInt("editctrl", "sflechas", 1));

	m_CEnter.SetCheck(AfxGetApp()->GetProfileInt("componentes","enter", 0));

	return TRUE;
}

bool CComportamientoPP::Aceptar() {
	
	CString temp;
	if(m_BBloquear.GetCheck()) {
		m_ETiempo.GetWindowText(temp);
		if(temp.IsEmpty()) {
			MessageBox("Por favor ingrese un valor para el tiempo.");
			m_ETiempo.SetFocus();
			return false;
		}
		AfxGetApp()->WriteProfileInt("control", "timeout", atoi(temp)*60);
	} else {
		AfxGetApp()->WriteProfileInt("control", "timeout", 0);
	}
	AfxGetApp()->WriteProfileInt("control", "efecto", m_CEfecto.GetCheck());
	AfxGetApp()->WriteProfileInt("componentes","tooltip", m_CToolTip.GetCheck());
	AfxGetApp()->WriteProfileInt("toolbar", "showtext",m_CTexto.GetCheck());
	AfxGetApp()->WriteProfileInt("editctrl", "sflechas", m_CFlechas.GetCheck());
	AfxGetApp()->WriteProfileInt("componentes","enter", m_CEnter.GetCheck());
	if(m_CMessageBox.GetCheck()) {
		CRegKey mikey;
		if( mikey.Open(HKEY_CURRENT_USER, "SoftWare\\SDG\\SVE")==ERROR_SUCCESS) {

			if( mikey.DeleteSubKey("MessageBox")!=ERROR_SUCCESS)
				mikey.RecurseDeleteKey("MessageBox");
			mikey.Close();
		}
	}
	return true;
}

void CComportamientoPP::OnOK()
{
	if(Aceptar())
		CPropertyPage::OnOK();
}

BOOL CComportamientoPP::OnApply()
{
	return Aceptar() ? CPropertyPage::OnApply() : false;
}

void CComportamientoPP::OnBnClickedCefecto()
{
	SetModified();
}

void CComportamientoPP::OnBnClickedCtooltip()
{
	SetModified();
}

void CComportamientoPP::OnBnClickedCmensaje()
{
	SetModified();
}

void CComportamientoPP::OnBnClickedCtexto()
{
	SetModified();
}

void CComportamientoPP::OnBnClickedCenter()
{
	SetModified();
}

void CComportamientoPP::OnBnClickedCflechas()
{
	SetModified();
}

void CComportamientoPP::OnEnChangeEtiempo()
{
	SetModified();
}
