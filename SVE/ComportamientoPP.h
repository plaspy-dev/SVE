#pragma once
#include "afxwin.h"


// CComportamientoPP dialog

class CComportamientoPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CComportamientoPP)

public:
	CComportamientoPP();
	virtual ~CComportamientoPP();

// Dialog Data
	enum { IDD = IDD_PPCOMPORTAMIENTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_BBloquear;
public:
	CEdit m_ETiempo;
public:
	CButton m_CEfecto;
public:
	CButton m_CToolTip;
public:
	CButton m_CMessageBox;
public:
	CButton m_CTexto;
public:
	CButton m_CEnter;
public:
	CButton m_CFlechas;
public:
	afx_msg void OnBnClickedCbloquear();
public:
	virtual BOOL OnInitDialog();
public:
	virtual void OnOK();
public:
	virtual BOOL OnApply();
public:
	afx_msg void OnBnClickedCefecto();
public:
	afx_msg void OnBnClickedCtooltip();
public:
	afx_msg void OnBnClickedCmensaje();
public:
	afx_msg void OnBnClickedCtexto();
public:
	afx_msg void OnBnClickedCenter();
public:
	afx_msg void OnBnClickedCflechas();
	bool Aceptar();
public:
	afx_msg void OnEnChangeEtiempo();
};
