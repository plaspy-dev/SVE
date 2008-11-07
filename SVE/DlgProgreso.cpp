// DlgProgreso.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "DlgProgreso.h"

CDlgProgreso *CDlgProgreso::m_pCurrent=NULL;

// CDlgProgreso dialog

IMPLEMENT_DYNAMIC(CDlgProgreso, CDialogXP)

CDlgProgreso::CDlgProgreso(CWnd* pParent /*=NULL*/)
	: CDialogXP(CDlgProgreso::IDD, pParent)
{
	m_pCurrent= this;
	m_bCancelado=false;
	Run();
}

CDlgProgreso::~CDlgProgreso()
{
	m_pCurrent=NULL;
}

void CDlgProgreso::DoDataExchange(CDataExchange* pDX)
{
	CDialogXP::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_SMensaje);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_PCProgreso);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
}


BEGIN_MESSAGE_MAP(CDlgProgreso, CDialogXP)
END_MESSAGE_MAP()


// CDlgProgreso message handlers

void CDlgProgreso::Cerrar() {

	EndDialog(IDCANCEL);
	m_pCurrent=NULL;
	delete this;
}

bool CDlgProgreso::IsCancelled() const {

	return m_bCancelado;
}

BOOL CDlgProgreso::OnInitDialog()
{
	CDialogXP::OnInitDialog();
	m_PCProgreso.SetRange(0,100);
	return TRUE;
}

bool CDlgProgreso::Init() {

	DoModal();
	return CThreadAuto::Init();
}

CDlgProgreso *CDlgProgreso::GetDlgActual() {

	return m_pCurrent;
}

void CDlgProgreso::OnCancel()
{
	m_bCancelado=true;
	m_BCancelar.SetWindowText("Cancelando...");
	// CDialogXP::OnCancel();
}
