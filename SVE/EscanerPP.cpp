// EscanerPP.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "EscanerPP.h"


// CEscanerPP dialog

IMPLEMENT_DYNAMIC(CEscanerPP, CPropertyPage)

CEscanerPP::CEscanerPP()
	: CPropertyPage(CEscanerPP::IDD)
	, m_nOrientacion(0)
{

}

CEscanerPP::~CEscanerPP()
{
}

void CEscanerPP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CCOLOR, m_BColor);
	DDX_Control(pDX, IDC_CRECORTAR, m_BRecortar);
	DDX_Control(pDX, IDC_CDIALOGO, m_BMostrar);
	DDX_Control(pDX, IDC_CCALIDAD, m_BCalidad);
	DDX_Radio(pDX, IDC_RVERTICAL, m_nOrientacion);
}


BEGIN_MESSAGE_MAP(CEscanerPP, CPropertyPage)
	ON_BN_CLICKED(IDC_CCOLOR, &CEscanerPP::OnBnClickedCcolor)
	ON_BN_CLICKED(IDC_CRECORTAR, &CEscanerPP::OnBnClickedCrecortar)
	ON_BN_CLICKED(IDC_CDIALOGO, &CEscanerPP::OnBnClickedCdialogo)
	ON_BN_CLICKED(IDC_CCALIDAD, &CEscanerPP::OnBnClickedCcalidad)
	ON_BN_CLICKED(IDC_RVERTICAL, &CEscanerPP::OnBnClickedRvertical)
	ON_BN_CLICKED(IDC_RHORIZONTAL, &CEscanerPP::OnBnClickedRhorizontal)
END_MESSAGE_MAP()


// CEscanerPP message handlers

BOOL CEscanerPP::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_BColor.SetCheck(AfxGetApp()->GetProfileInt("escaner", "color", 1));
	m_BRecortar.SetCheck(AfxGetApp()->GetProfileInt("escaner", "cortar", 1));
	m_BMostrar.SetCheck(AfxGetApp()->GetProfileInt("escaner", "showUI", false));
	m_BCalidad.SetCheck(AfxGetApp()->GetProfileInt("escaner", "calidad", 0));
	OnBnClickedCdialogo();
	m_nOrientacion= AfxGetApp()->GetProfileInt("escaner", "orientacion", 1);
	UpdateData(false);
	return TRUE;
}

void CEscanerPP::OnBnClickedCcolor()
{
	SetModified();
}

void CEscanerPP::OnBnClickedCrecortar()
{
	SetModified();
}

void CEscanerPP::OnBnClickedCdialogo()
{
	m_BCalidad.EnableWindow(!m_BMostrar.GetCheck());
	m_BColor.EnableWindow(!m_BMostrar.GetCheck());
	SetModified();
}

void CEscanerPP::OnOK()
{
	AfxGetApp()->WriteProfileInt("escaner", "color", m_BColor.GetCheck());
	AfxGetApp()->WriteProfileInt("escaner", "cortar", m_BRecortar.GetCheck());
	AfxGetApp()->WriteProfileInt("escaner", "showUI", m_BMostrar.GetCheck());
	AfxGetApp()->WriteProfileInt("escaner", "calidad", m_BCalidad.GetCheck());
	UpdateData();
	AfxGetApp()->WriteProfileInt("escaner", "orientacion", m_nOrientacion);
	CPropertyPage::OnOK();
}

void CEscanerPP::OnBnClickedCcalidad()
{
	SetModified();
}

void CEscanerPP::OnBnClickedRvertical()
{
	SetModified();
}

void CEscanerPP::OnBnClickedRhorizontal()
{
	SetModified();
}
