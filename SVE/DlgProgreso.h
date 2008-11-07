#pragma once
#include "afxwin.h"
#include "afxcmn.h"
# include <Componentes/Componentes.h>
# include "resource.h"
// CDlgProgreso dialog

class CDlgProgreso : public CDialogXP, public CThreadAuto
{
protected:
	virtual bool	Init();
	static CDlgProgreso *m_pCurrent;
	virtual ~CDlgProgreso();

	DECLARE_DYNAMIC(CDlgProgreso)
public:
	CDlgProgreso(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool	m_bCancelado;
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_SMensaje;
public:
	CTextProgressCtrl m_PCProgreso;
public:
	CButtonXP m_BCancelar;
public:
	bool IsCancelled() const;
public:
	virtual BOOL OnInitDialog();
	void Cerrar();
	static CDlgProgreso *GetDlgActual();
protected:
	virtual void OnCancel();
};
