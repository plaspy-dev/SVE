#pragma once
#include "afxwin.h"


// CEscanerPP dialog

class CEscanerPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CEscanerPP)

public:
	CEscanerPP();
	virtual ~CEscanerPP();

// Dialog Data
	enum { IDD = IDD_ESCANERPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_BColor;
public:
	CButton m_BRecortar;
public:
	CButton m_BMostrar;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedCcolor();
public:
	afx_msg void OnBnClickedCrecortar();
public:
	afx_msg void OnBnClickedCdialogo();
public:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedCcalidad();
public:
	CButton m_BCalidad;
public:
	int m_nOrientacion;
public:
	afx_msg void OnBnClickedRvertical();
public:
	afx_msg void OnBnClickedRhorizontal();
};
