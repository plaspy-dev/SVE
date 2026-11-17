#pragma once
#include "afxdtctl.h"
#include "afxwin.h"

// CEntregaView form view

class CVMR_Capture;

class CEntregaView : public CFormView
{
	DECLARE_DYNCREATE(CEntregaView)

protected:
	CEntregaView();           // protected constructor used by dynamic creation
	virtual ~CEntregaView();
	CConector *m_pConector;
	CUsuario  *m_pUsuario;
	CPrintRTF   *m_pPrint;
	CString		m_szOldGuia;
	CString		m_szOldCedula;
	CString		m_szCurrentId;
	CSplashScreenEx *m_pSplash;
	bool		m_bExiste;
	bool		m_bFound;
	COleRichEditCtrl	m_pRTF;
	int			m_nAncho;
	int			m_nAlto;
	int			m_nCamaras;

	CVMR_Capture *m_VMRCap;

public:
	enum { IDD = IDD_FENTREGA };
	enum { finddocumento=1, findguia, findfoto};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	CEditXP m_EGuia;
public:
	CEditXP m_EOficina;
public:
	CEditXP m_ECiudad;
public:
	CDateTimeCtrl m_DTFecha;
public:
	CComboBoxXP m_CBTipoD;
public:
	CEditXP m_EDocumento;
public:
	CEditXP m_ENombre;
public:
	CEditXP m_EApellido;
public:
	CEditXP m_EDireccion;
public:
	CEditXP m_ETelefono;
public:
	CEditXP m_ECantidad;
public:
	CEditXP m_EPeso;
public:
	CButtonXP m_BAceptar;
public:
	CButtonXP m_BCancelar;
public:
	CBitmapPicture m_SLogo;
public:
	CFingerBitmap m_SHuella;
public:
	afx_msg void OnEnChangeEguia();
public:
	afx_msg void OnEnChangeEnodocumento();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedBcancelar();
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnKillfocusEguia();
public:
	afx_msg void OnEnKillfocusEnodocumento();
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	afx_msg void OnDestroy();
	LRESULT OnFingerPrint(WPARAM wParam, LPARAM lParam);
	void Clean();
protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnUpdatePrint(CCmdUI *pCmdUI);
	void OnTemp();
	bool InitRTF(bool foto=false);
public:
	CEditXP m_EDescripcion;
public:
	CEditXP m_EObservaciones;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	CComboBoxXP m_CBCamaras;
public:
	CButtonXP m_BFoto;
public:
	CShowpic m_SFoto;
public:
	CStatic m_SCamara;
public:
	afx_msg void OnBnClickedBcapturar();
public:
	afx_msg void OnCbnSelendokCamaras();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Refresh();
	void Pause(bool pause);
	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);
};


