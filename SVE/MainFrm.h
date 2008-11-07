// MainFrm.h: interfaz de la clase CMainFrame
//
#pragma once

# include "PropiedadesBar.h"
#include "wia/EventCallback.h"

class CMainFrame : public CMDIFrameWndXP
{
	DECLARE_DYNAMIC(CMainFrame)
private:
	// varios
	bool	Messages( MySQLMessage *contenido);
	bool	DeleteHilos( MySQLMessage *hilo);
	bool	LoadDocumentos();

protected:
	CToolBarXP		m_wndToolBar;
// Reemplazos
public:
	bool			Conectar();
	CUsuario		*m_pUsuario;
	void			ResetToolbars();

	CPropiedadesBar	m_PropBar;
	CHelpBar		m_wndHelpBar;

	CConector		m_pConector;
	CFingerPrint	*m_pFinger;

	CSplashScreenEx *m_pSplash;

	virtual void	SaveState();
	virtual void	LoadState();

	CMainFrame();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateVerNavegador(CCmdUI *pCmdUI);
	afx_msg void OnConfiguracionServidores();
	afx_msg void OnUpdateVerBarradenavegacion(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg void OnUpdateFileNew(CCmdUI *pCmdUI);
	afx_msg LRESULT OnQueryConector(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryUsuario(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySQL(WPARAM wParam, LPARAM lParam);
	int OnMySQLThreadEnd(MySQLMessage *hilo);
	int OnMySQLQueryResult(MySQLMessage *message);
	int OnQueryEmpty(int owner);
	int OnQueryTitle(MySQLMessage *message);

	afx_msg LRESULT OnInsertODS(WPARAM wParam, LPARAM lParam);
	void OnVerPropiedades();
	void OnUpdateVerPropiedades(CCmdUI *pCmdUI);

	LRESULT OnQueryNavigate(WPARAM wParam, LPARAM lParam);
	LRESULT OnQueryFinger(WPARAM wParam, LPARAM lParam);
	LRESULT OnFingerPrint(WPARAM wParam, LPARAM lParam);

	afx_msg void OnAyudaIndice();
	afx_msg void OnAyudaBuscar();
	afx_msg void OnUpdateAyudaIndice(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAyudaBuscar(CCmdUI *pCmdUI);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnHerramientasPersonalizar();
protected:
	CComPtr<WiaWrap::CEventCallback>  m_pEventCallback;
public:
	int GetDevicesCount() const;
};
