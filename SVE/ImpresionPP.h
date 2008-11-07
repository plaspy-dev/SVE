#pragma once
#include "afxwin.h"


// CImpresionPP dialog

class CImpresionPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CImpresionPP)

public:
	CImpresionPP();
	virtual ~CImpresionPP();

// Dialog Data
	enum { IDD = IDD_PPIMPRESION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EColumnas;
public:
	CEdit m_EFilas;
public:
	CStatic m_SEncabezado;
public:
	CButton m_BEncabezado;
public:
	CStatic m_SPie;
public:
	CButton m_BPie;
public:
	CStatic m_STitulos;
public:
	CButton m_BTitulos;
public:
	CStatic m_STexto;
public:
	CButton m_BTexto;
public:
	afx_msg void OnEnChangeEcolumnas();
public:
	afx_msg void OnEnChangeEfilas();
public:
	afx_msg void OnBnClickedBencabezado();
public:
	afx_msg void OnBnClickedBpie();
public:
	afx_msg void OnBnClickedBtitulos();
public:
	afx_msg void OnBnClickedBtexto();
public:
	virtual void OnOK();
public:
	virtual BOOL OnApply();
public:
	virtual BOOL OnInitDialog();
	bool Aceptar();
	CFont		m_FEncabezado;
	CFont		m_FPie;
	CFont		m_FTitulo;
	CFont		m_FTexto;
};
