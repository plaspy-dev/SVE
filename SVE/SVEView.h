// SVEView.h: interfaz de la clase CSVEView
//


#pragma once

#include "afxwin.h"
#include "afxctl.h"
#include "afxcmn.h"

class CVMR_Capture;

class CSVEView : public CFormView
{
protected: // Crear sólo a partir de serialización
	CSVEView();
	DECLARE_DYNCREATE(CSVEView)

public:
	enum { findcedula=1,findentrega, datos_huella, 
		query_huella, 
		query_foto1, 
		query_foto2, 
		query_foto3,
		query_huellasearch,
	};

	DWORD m_pTemporal;
	enum{ IDD = IDD_SVE_FORM };

	CString		m_szOld;
	CString		m_szOldCodigo;
	int			m_nAncho;
	int			m_nAlto;
	int			m_nTimer;
	int			m_nCamaras;
	BYTE		*m_pCamaras[2];
	int			m_nFrameLen[2];

	bool				m_bExiste;
	bool				m_bFound;
	COleRichEditCtrl	m_pRTF;
	CPrintRTF			*m_pPrint;

	// nuevos eliminar existe y found
	bool		m_bUsuario;
	bool		m_bIngreso;

	CConector	*m_pConector;
	CUsuario	*m_pUsuario;

	CVMR_Capture *m_VMRCap;
	CVMR_Capture *m_VMRCap2;
	CVMR_Capture *m_VMRCap3;
	bool			InitRTF();
	// CPrintSVE		*m_pPrint;
	
// Operaciones
public:

// Reemplazos
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnInitialUpdate();

// Implementación
public:
	virtual ~CSVEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
public:
	CSplashScreenEx *m_pSplash;
	CEditXP m_ECodigo;
	CEditXP m_EDocumento;
	CEditXP m_ENombre;
	CEditXP m_EApellido;
	CEditXP m_EFecha;
	CComboBoxXP m_CBCamara1;
	CComboBoxXP m_CBCamara2;
	CButtonXP m_BFoto1;
	CButtonXP m_BFoto2;
	CButtonXP m_BAceptar;
	CButtonXP m_BCancelar;
	CShowpic m_SFoto1;
	CShowpic m_SFoto2;
	CShowpic m_SFoto3;

	afx_msg void OnBnClickedBcapturar();
	afx_msg void OnBnClickedBcapturar2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnKillfocusEcodigo();
	afx_msg void OnEnKillfocusEcedula();
	void Clean(bool search=false);
	void Pause(int camara, bool pausa);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	LRESULT OnMysqlMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelendokCamaras();
	afx_msg void OnCbnSelendokCamaras2();

	CFingerBitmap	m_SHuella;

	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
public:
	CComboBoxXP m_CBCamara3;
	CButtonXP m_BCapturar3;
	afx_msg void OnBnClickedBcapturar3();
	afx_msg void OnCbnSelendokCamaras3();
	bool	CopyTo(int camara);
	void Refresh(int camara);
protected:
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnUpdatePrint(CCmdUI *pCmdUI);
public:
	CButtonXP m_BAgregar;
	afx_msg void OnBnClickedBagregar();
	CMyListCtrl m_LCMultiple;
	afx_msg void OnNMDblclkLmultiple(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickLmultiple(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownLmultiple(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnFingerPrint(WPARAM wParam, LPARAM lParam);

	void OnDeleteItem();
	void OnEditItem();
	afx_msg void OnBAgregar();
public:
	CBitmapPicture m_SLogo;
public:
	CStatic m_STemp;
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void	OnGuardar();
	bool ScanImage(int camara);
	HBITMAP GetBitmap(HBITMAP bmp);
public:
	afx_msg void OnEnChangeEcedula();
public:
	CStatic m_SMensaje;
};

