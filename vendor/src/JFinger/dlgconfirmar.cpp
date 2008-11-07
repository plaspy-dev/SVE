// DlgConfirmar.cpp : implementation file
//

#include "stdafx.h"
# include "JFinger.h"
#include "DlgConfirmar.h"

// CDlgConfirmar dialog

IMPLEMENT_DYNAMIC(CDlgConfirmar, CDialog)

CDlgConfirmar::CDlgConfirmar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfirmar::IDD, pParent)
{
	m_pFinger=NULL;
	m_pOldParent=NULL;
	m_nIntentos=0;
}

CDlgConfirmar::~CDlgConfirmar()
{
}

void CDlgConfirmar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_SMENSAJE, m_SMensaje);
	DDX_Control(pDX, IDCANCEL, m_BCancelar);
}


BEGIN_MESSAGE_MAP(CDlgConfirmar, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgConfirmar::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgConfirmar::OnBnClickedCancel)
	ON_MESSAGE(WM_FINGERPRINT,OnFingerPrint)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgConfirmar message handlers

void CDlgConfirmar::OnBnClickedOk()
{
	// OnOK();
}

void CDlgConfirmar::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CDlgConfirmar::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT(m_pFinger);
	if(m_pFinger==NULL)
		return FALSE;
	m_pOldParent= CFingerPrint::SetParent(this);
	ASSERT(m_pOldParent);
	VERIFY(m_pFinger->Prepare());
	m_SHuella.SetDefault();
	m_SHuella.HideInfo();
	return TRUE;
}

LRESULT CDlgConfirmar::OnFingerPrint(WPARAM wParam, LPARAM lParam) {

	if( wParam==GR_FINGER_UP) {

		int score=0;
		CFingerMessage *mess= m_SHuella.SetHuella(lParam);
		if(mess && m_pFinger->Compare((const char *)mess->GetTemplate(), &score)) {
			m_SMensaje.SetWindowText("Huella confirmada.");
			OnOK();
			return 0;
		}
		CString intento;
		m_nIntentos++;
		intento.Format("Por favor intente\r de nuevo(%d).",m_nIntentos);
		m_SMensaje.SetWindowText(intento);
	}
	return 0;
}

void CDlgConfirmar::OnDestroy()
{
	CDialog::OnDestroy();
	ASSERT(m_pOldParent);
	CFingerPrint::SetParent(m_pOldParent);
}
