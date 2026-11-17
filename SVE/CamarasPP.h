#pragma once
#include "afxwin.h"


// CCamarasPP dialog

class CCamarasPP : public CPropertyPage
{
	DECLARE_DYNAMIC(CCamarasPP)

public:
	CCamarasPP();
	virtual ~CCamarasPP();

// Dialog Data
	enum { IDD = IDD_PCAMARAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_CReiniciar;
public:
	CEdit m_ETiempo;
public:
	afx_msg void OnBnClickedCreiniciar();
public:
	afx_msg void OnEnChangeEtiempo();
public:
	virtual BOOL OnSetActive();
public:
	virtual BOOL OnApply();
public:
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	bool Aceptar();
};
