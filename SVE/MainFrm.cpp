// MainFrm.cpp: implementación de la clase CMainFrame
//

#include "stdafx.h"

#include "SVE.h"
# include "atlbase.h"
# include "afxole.h"
#include "HtmlHelp.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

# include "SVEView.h"
# include "informesview.h"
#include "EntregaView.h"

# include "OpcionesSheet.h"

# define	ANIMATION_TIMER			300
# define	TIMER_EVENT				32000

// LoadFunciones
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndXP)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndXP)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_VER_NAVEGADOR, OnUpdateVerNavegador)
	ON_COMMAND(ID_CONFIGURACION_SERVIDORES, OnConfiguracionServidores)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_MESSAGE(WM_QUERYCONECTOR,OnQueryConector)
	ON_MESSAGE(WM_QUERYUSUARIO,OnQueryUsuario) 
	ON_MESSAGE(WM_MYSQL,OnMySQL)
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
	ON_COMMAND(ID_VER_PROPIEDADES, OnVerPropiedades)
	ON_UPDATE_COMMAND_UI(ID_VER_PROPIEDADES, OnUpdateVerPropiedades)
	ON_MESSAGE(WM_HHELP, OnQueryNavigate)
	ON_COMMAND(ID_HELP_INDEX, OnAyudaIndice)
	ON_COMMAND(ID_HELP_FINDER, OnAyudaBuscar)
	ON_UPDATE_COMMAND_UI(ID_HELP_INDEX, OnUpdateAyudaIndice)
	ON_UPDATE_COMMAND_UI(ID_HELP_FINDER, OnUpdateAyudaBuscar)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_QUERYFINGER,OnQueryFinger)
	ON_MESSAGE(WM_FINGERPRINT,OnFingerPrint)
	ON_COMMAND(ID_HERRAMIENTAS_PERSONALIZAR, &CMainFrame::OnHerramientasPersonalizar)
	ON_MESSAGE(WM_QUERYFINGER, OnQueryFinger)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_EXT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};

CMainFrame::CMainFrame()
{
	m_pUsuario= NULL;
	m_pFinger=NULL;
	m_pEventCallback = new WiaWrap::CEventCallback;

	if (m_pEventCallback != NULL)
	{
		m_pEventCallback->Register();
	}
}

CMainFrame::~CMainFrame()
{
	if(m_pUsuario)
		delete m_pUsuario;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CSplashScreenEx m_pSplash;

	m_pSplash.Create(this,"SVE splash",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	m_pSplash.SetBitmap(IDB_ACERCADE,255,0,255);
	m_pSplash.SetTextFont("Arial",100,CSS_TEXT_NORMAL);
	m_pSplash.SetTextRect(CRect(100,300,600,350));
	m_pSplash.SetTextColor(RGB(0,0,0));
	m_pSplash.SetTextFormat(DT_RIGHT | DT_BOTTOM);
	m_pSplash.Show();
	CString szTemp;
	szTemp.Format("SVE Versión %s\r\n", SVE_VERSION);
	m_pSplash.SetText(szTemp);

	if (CMDIFrameWndXP::OnCreate(lpCreateStruct) == -1)
		return -1;
	// barra de herramientas principal
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("No se pudo crear la barra de herramientas\n");
		return -1;      // No se pudo crear
	}
	// barra de estado
	if (!m_wndStatusBar.CreateStatusBar(this, indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("No se pudo crear la barra de estado\n");
		return -1;      // Fehler beim Erzeugen
	}
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CREATE_JDOCK(this);

	// barras de herramientas
	if (!m_pNavBar.Create(_T("Navegador"),WS_CHILD, this, 200))
	{
		TRACE0("Failed to create my bar2\n");
		return -1;
	}

	if (!m_PropBar.Create(_T("Propiedades"), WS_CHILD,this, 202))
	{
		TRACE0("Failed to create my ODS Bar\n");
		return -1;
	}

	if(!m_wndHelpBar.Create("Ayuda", WS_CHILD, this, 201)) {
		TRACE0("Fallo al crear la barra de ayuda\n");
		return -1;
	}
	
	m_pNavBar.SetBarStyle(m_pNavBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_PropBar.SetBarStyle(m_PropBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_pNavBar.EnableDocking(CBRS_ALIGN_ANY);
	m_PropBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_pNavBar, AFX_IDW_DOCKBAR_RIGHT);
    DockControlBar(&m_PropBar, AFX_IDW_DOCKBAR_LEFT);

	m_wndHelpBar.SetBarStyle(m_wndHelpBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndHelpBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndHelpBar, AFX_IDW_DOCKBAR_LEFT);

	// los tabs
	DWORD style = MT_TOP | MT_IMAGES;
	style|= AfxGetApp()->GetProfileInt("control", "showtab", 1)==2 ? MT_HIDEWLT2VIEWS : 0;
	m_mdiTabs.Create(this, style);
	
	// el conector
	m_pConector.SetMainFrame( this);
	m_pConector.Cargar();
	
	// barra de estado
	m_wndStatusBar.SetMode(1, XSB_PROGRESS | XSB_SMOOTH);
	m_wndStatusBar.SetRange(1, 0, 100);
	m_wndStatusBar.SetWidth(1, 100);
	m_wndStatusBar.GetProgressCtrl(1)->SetShowText(false);
	m_wndStatusBar.SetMode(2, XSB_TEXT | DT_CENTER);
	m_wndStatusBar.SetText(2, " ");
	m_wndStatusBar.SetWidth( 2, 100);
	
	if( AfxGetApp()->GetProfileInt("control", "animation", 1)) {
		m_wndStatusBar.SetMode(4, XSB_BITMAP | DT_CENTER);
		CString temp;
		temp.Format("%d", IDB_BLOADING1);
		m_nAnimation=IDB_BLOADING1;
		m_wndStatusBar.SetBitmap(4, temp, temp);
		SetTimer(ANIMATION_TIMER, AfxGetApp()->GetProfileInt("control", "animationtimer", 200), NULL);
	} else {
		m_wndStatusBar.SetMode(4, XSB_TEXT | DT_CENTER);
		m_wndStatusBar.SetNumber(4, 0, 0);
	}

	m_wndStatusBar.SetMode(3, XSB_TEXT | DT_CENTER);
	m_wndStatusBar.SetText(3, " ", " ");
	m_wndStatusBar.SetWidth(3, 100);
	
	m_DefaultNewMenu.LoadToolBar(IDR_MAINFRAME, NULL);
#ifdef ENABLE_LICENSE_CHECK
	SetTimer(4, 30*60*1000, NULL); 
#endif
	
	szTemp.Format("SVE Versión %s\r\nIniciando Dispositivo Biométrico", SVE_VERSION);
	m_pSplash.SetText(szTemp);

	m_pFinger= new CFingerPrint(this);

	m_LogoMdiClientWnd.SetFlash(CApplicacion::GetAppPath( AfxGetApp()->GetProfileString("swfop", "archivo", "logo.swf")), 
		AfxGetApp()->GetProfileInt("swfop", "ancho", 456),
		AfxGetApp()->GetProfileInt("swfop", "alto", 238),
		AfxGetApp()->GetProfileInt("swfop", "bkcolor", -1)
		);

	return 0;
}

void CMainFrame::ResetToolbars() {
    
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	m_pNavBar.OnCerrar();
	m_PropBar.OnCerrar();

	DockControlBar(&m_pNavBar, AFX_IDW_DOCKBAR_RIGHT);
	DockControlBar(&m_PropBar, AFX_IDW_DOCKBAR_LEFT);

	m_pNavBar.LoadStateBar("Barras\\Barras");
	m_PropBar.LoadStateBar("Barras\\Barras");

	m_pNavBar.ShowWindow(SW_SHOW);
	m_PropBar.m_nLastAlingDocking= CBRS_ALIGN_LEFT;
	m_PropBar.SetAutoHide(true);
	m_PropBar.InitAutoHide();

	RecalcLayout();
	AfxGetApp()->WriteProfileInt("control", "notstore", 1);
}

void CMainFrame::LoadState() {

	CWaitCursor wait;

	LoadBarState("barras\\Barras");
	m_pNavBar.LoadStateBar("Barras\\Barras");
	m_PropBar.LoadStateBar("Barras\\Barras");
	m_wndHelpBar.LoadStateBar("Barras\\Barras");

	if( !AfxGetApp()->GetProfileInt("barras\\Barras", "store",0)) {

		m_pNavBar.ShowWindow(SW_SHOW);
		m_PropBar.m_nLastAlingDocking= CBRS_ALIGN_LEFT;
		m_PropBar.SetAutoHide(true);
		m_PropBar.InitAutoHide();
		m_wndHelpBar.m_nLastAlingDocking= CBRS_ALIGN_LEFT;
		m_wndHelpBar.SetAutoHide(true);
		m_wndHelpBar.InitAutoHide();
	}
	CMDIFrameWndXP::LoadState();
}

void CMainFrame::SaveState() {

	if( !m_pUsuario || !m_pUsuario->GetID())

		return;

	CMDIFrameWndXP::SaveState();
	m_PropBar.SaveBar("Barras\\Barras");
	m_wndHelpBar.SaveBar("Barras\\Barras");
}

bool CMainFrame::Conectar() {

	FUNCTION_STACK_TRACE("AAAAAAAA");
	bool existe=false;
	if( m_pUsuario) 
		if( m_pUsuario->GetID())
			existe=true;	

	if( !m_pConector.GetDatabasesCount()) {

		CBitmap asistente;
		asistente.LoadMappedBitmap(IDB_ASISTENTE);
		InitAsistente(&asistente, &asistente);
		m_pConector.SetMainFrame( this);
		m_pConector.Cargar();
		if(!m_pConector.GetDatabasesCount()) {
			OnConfiguracionServidores();
			if( !m_pConector.GetDatabasesCount())
				return false;
		}
	}
	
	KillTimer(50000);
	m_bConectando= true;
	CStringArray	datos;

	if( GetDlgIngreso(existe, datos)==IDOK) {

		if( !existe)
			m_pNavBar.m_Tree.DeleteAllItems();

		m_pConector.SetPasswd( datos[0]);
		m_pConector.SetUser(datos[1]);

		if( !m_pUsuario)
			m_pUsuario= new CUsuario( datos[2], atoi(datos[3]), atoi(datos[4]));
		else {
			m_pUsuario->SetUser(datos[2]);
			m_pUsuario->SetId(atoi(datos[3]));
			m_pUsuario->SetGrupo(atoi(datos[4]));
		}

		m_wndStatusBar.SetText(2, datos[2]);
		m_PropBar.SetConector(&m_pConector);

		// registrarse en el chat
		SendMessage(WM_REGISTRARUSUARIO, 0, -2);
		if( !existe) {
			AfxGetApp()->WriteProfileInt("control", "loadstate", 1);
			LoadDocumentos();
			LoadState();
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		}
		SendMessageBroadCast(WM_QUERYUSUARIO, 0, (LPARAM)m_pUsuario);
		SendMessageBroadCast(WM_QUERYCONECTOR, 0, (LPARAM)&m_pConector);
		if( AfxGetApp()->GetProfileInt("control", "timeout", 300))
			SetTimer(50000, 1000, NULL);
		SAFE_DELETE(m_pFadeWnd);
		SendMessageBroadCast(WM_INACTIVIDAD, 0,1);
		AfxGetApp()->WriteProfileInt("control", "loadstate", 0);
		m_bConectando= false;
		return true;
	}
	return false;
}

bool CMainFrame::LoadDocumentos() {

	FUNCTION_STACK_TRACE("AAAAAAAB");

	HTREEITEM hRoot;

	hRoot= m_pNavBar.m_Tree.InsertItem( "SVE");
	m_pNavBar.m_Tree.SetItemState(hRoot, TVIS_BOLD,TVIS_BOLD);

	CNewMenu *mimenu= new CNewMenu();

	mimenu->CreateMenu();

	LoadDocument(mimenu, hRoot, "Entrega", IDR_SVETYPE,RUNTIME_CLASS(CSVEView));
	// LoadDocument(mimenu, hRoot, "Internacional", IDR_SVETYPE, RUNTIME_CLASS(CEntregaView));
	LoadDocument(mimenu, hRoot, "Informes", IDR_SVETYPE,RUNTIME_CLASS(CInformesView));

	m_pMenus.Add("SVE", (DWORD)mimenu);

	m_pNavBar.m_Tree.Expand(hRoot, TVE_EXPAND);

	return CMDIFrameWndXP::LoadDocumentos(m_pUsuario->GetGrupo());;
}

// Diagnósticos de CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndXP::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndXP::Dump(dc);
}

#endif //_DEBUG

LRESULT CMainFrame::OnQueryConector(WPARAM wParam, LPARAM lParam) {

	CWnd *temp= (CWnd *) lParam;
	if(temp)
		temp->SendMessage( WM_QUERYCONECTOR, 0, (LPARAM) &m_pConector);
	return (LRESULT) &m_pConector;
}

LRESULT CMainFrame::OnQueryUsuario(WPARAM wParam, LPARAM lParam) {

	CWnd *temp= (CWnd *) lParam;
	if(temp)
		temp->SendMessage( WM_QUERYUSUARIO, 1, (LPARAM) m_pUsuario);
	return (LRESULT) m_pUsuario;
} 

int CMainFrame::OnMySQLThreadEnd(MySQLMessage *hilo) {

	PostMessage(WM_STATUSBAR, 2);
	if( hilo->GetID()>49000 || TIMER_EVENT==hilo->GetID()) {
		return 0;
	} else if( hilo->GetID()>99) {
	} else DeleteHilos(hilo);
	return 0;
}

int CMainFrame::OnMySQLQueryResult(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
			{
				CMyTime tiempo(message->GetAt(0));
				m_pTServidor=tiempo;
				m_pTLocal= CTime::GetCurrentTime();
				m_psDiferencia= m_pTLocal-m_pTServidor;
				SetTimer(TIMER_EVENT, 1000, NULL);
				SendMessageBroadCast(WM_QUERYTIME, 0, (LPARAM)&m_pTServidor);
			}
		case 50001: // no se hace nada
			return 0;
	}
	if( message->GetID()>99) {
		PostMessage(WM_STATUSBAR, 1);
	} else Messages(message); // SOLO en el caso de NEW_XML
	return 0;
}

int CMainFrame::OnQueryEmpty(int owner) {

	switch( owner) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
			return 0;
	}
	return 0;
}

int CMainFrame::OnQueryTitle(MySQLMessage *message) {

	switch( message->GetID()) {
		case TIMER_EVENT:
		case 50001: // no se hace nada
		case 50000: // chat
			return 0;
	}
	return 0;
}

LRESULT CMainFrame::OnMySQL(WPARAM wParam, LPARAM lParam) {

	CMySQLMessage *mensaje= (CMySQLMessage *)lParam;
	switch( wParam) 
	{
	case THREAD_END:
		if(mensaje->GetParent()==this)
			OnMySQLThreadEnd(mensaje);
		else 
			PostMessage( WM_STATUSBAR, 2);
		break;
	case QUERY_COUNT2:
		PostMessage( WM_STATUSBAR, 0, mensaje->GetCount());
		break;
	case QUERY_RESULT:
		if(mensaje->GetParent()==this)
			OnMySQLQueryResult(mensaje);
		else 
			PostMessage( WM_STATUSBAR, 1);
		break;
	case QUERY_TITLE:
		if(mensaje->GetParent()==this)
			OnQueryTitle(mensaje);
		break;
	case QUERY_EMPTY:
		if(mensaje->GetParent()==this)
			OnQueryEmpty(mensaje->GetID());
		break;
	case QUERY_ERROR:
		{
			SetErrorEx(NULL, mensaje->GetAt(0)+ " \r\nMientras se realizaba la consulta\r\n" + mensaje->GetAt(1), 3, "mysql");
		}
		break;
	}
	return 0;
}

bool CMainFrame::DeleteHilos( MySQLMessage *hilo) {
	return true;
}

bool CMainFrame::Messages( MySQLMessage *contenido) {

	FUNCTION_STACK_TRACE("AAAAAAAE");
	PostMessage( WM_STATUSBAR, 1);
	return true;
}

void CMainFrame::OnUpdateVerNavegador(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	CMDIFrameWndXP::OnUpdateVerNavegador(pCmdUI);
}

void CMainFrame::OnConfiguracionServidores()
{
	if( GetDlgDataBases()==IDOK) {

		if( m_pUsuario) {
			SetErrorEx(NULL, "Debe Reiniciar SVE para realizar los cambios", 0, "conector");
		} else {
			m_pConector.SetMainFrame( this);
			m_pConector.Cargar();
		}
	}
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	if( nIDEvent==TIMER_EVENT) {
		CTime tiempo= CTime::GetCurrentTime();
		CTimeSpan span(0, 0, 1, 0);
		if( tiempo<m_pTLocal || span < tiempo-m_pTLocal) {
			KillTimer(TIMER_EVENT);
			m_pConector.Query("SELECT SYSDATE()", TIMER_EVENT, this);
		} else {
			m_pTLocal= tiempo;
			m_pTServidor= m_pTLocal-m_psDiferencia;
		}
	} else if( nIDEvent==ANIMATION_TIMER) {

		if( !m_nCurrentHilos) {
			if( m_nAnimation!= IDB_BLOADING1) {
				m_nAnimation=IDB_BLOADING1;
				CString temp;
				temp.Format("%d", m_nAnimation);
				m_wndStatusBar.SetBitmap(4, temp, temp);
			}
			return;
		}
		if( m_nAnimation==IDB_BLOADING10) 
			m_nAnimation=IDB_BLOADING1;
		m_nAnimation++;
		CString temp;
		temp.Format("%d", m_nAnimation);
		m_wndStatusBar.SetBitmap(4, temp, temp);
#ifdef ENABLE_LICENSE_CHECK
	} else if(nIDEvent==4) { // licencia

		CMyRegKey key;
		key.MyOpen(HKEY_LOCAL_MACHINE, CApplicacion::GetAppKey() + "\\control");
		CDefender info(key.GetString("licencia","", ""));
		if( info.IsValidSerial()!=&info) {
			CTrialInfo info;
			if( info.GetLeftDays()<=0) {
				PostMessage(WM_CLOSE);
			}
		} else {
			KillTimer(4);
		}
#endif
	} else if( nIDEvent==50000) {
		// inactividad
		KillTimer(nIDEvent);
		return;
		m_nInactivo++;
		if( m_bInactivo || m_nInactivo>=AfxGetApp()->GetProfileInt("control", "timeout", 300)) {
			KillTimer(50000);
			SendMessage(WM_REGISTRARUSUARIO, 0, -3);
			SendMessageBroadCast(WM_INACTIVIDAD, 0,0);
			m_bInactivo=true;
			SAFE_DELETE(m_pFadeWnd);
			if( m_bFocus) {
				if( AfxGetApp()->GetProfileInt("control", "efecto", 1) && IsWindowsNT())
					m_pFadeWnd= new CFadeWnd(this);
				while( Conectar()==false) {
					if( AfxMessageBox( "Esta acción Cerrara SVE.\n¿Desea Cerrarlo?", MB_ICONQUESTION | MB_YESNO)==IDYES) {
						PostMessage(WM_CLOSE);
						return;
					}
				}
				m_bInactivo=false;
				m_nInactivo=0;
			}
		}
	}
	CMDIFrameWndXP::OnTimer(nIDEvent);
}

void CMainFrame::OnUpdateFileNew(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!m_pUsuario || AfxGetApp()->GetProfileInt("control", "timeout", 1));
}

LRESULT CMainFrame::OnInsertODS(WPARAM wParam, LPARAM lParam) {

	if(wParam==10) {
		CString m_szODS= (const char *) lParam;
		m_PropBar.SetODS(m_szODS, wParam);
	} else {
		SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, m_pNavBar.m_Tree.GetItemData(m_pNavBar.FindItemEx(wParam ? "CEntregaView" : "CSVEView")));
		SendMessageBroadCast(WM_INSERTODS, wParam, lParam);
	}
	return 0;
}

void CMainFrame::OnVerPropiedades()
{
	if( !m_PropBar.IsWindowVisible()) 

		if(m_PropBar.IsHide()) {

			m_PropBar.InitShow();
			m_PropBar.Update(true);
			return;
		}
	if(!m_PropBar.IsWindowVisible())
		m_PropBar.Update(true);
	m_PropBar.ShowWindow((m_PropBar.IsWindowVisible()) == 0);
	RecalcLayout();
}

void CMainFrame::OnUpdateVerPropiedades(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
	pCmdUI->SetCheck( m_PropBar.IsWindowVisible());
}

void CMainFrame::OnAyudaIndice()
{
	if( !m_wndHelpBar.IsWindowVisible())  {

		if(m_wndHelpBar.IsHide()) {

			m_wndHelpBar.InitShow();
			return;
		}
	}
	m_wndHelpBar.m_pDlgHelp.m_TabCtrl.ActivateSSLPage(0);
	if( !m_wndHelpBar.IsWindowVisible())
		m_wndHelpBar.ShowWindow((m_wndHelpBar.IsWindowVisible()) == 0);
	RecalcLayout();
}

void CMainFrame::OnAyudaBuscar()
{
	if( !m_wndHelpBar.IsWindowVisible()) {

		if(m_wndHelpBar.IsHide()) {

			m_wndHelpBar.InitShow();
			return;
		}
	}
	m_wndHelpBar.m_pDlgHelp.m_TabCtrl.ActivateSSLPage(1);
	if( !m_wndHelpBar.IsWindowVisible())
		m_wndHelpBar.ShowWindow((m_wndHelpBar.IsWindowVisible()) == 0);
	RecalcLayout();
}

void CMainFrame::OnUpdateAyudaIndice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
}

void CMainFrame::OnUpdateAyudaBuscar(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pUsuario!=NULL);
}

LRESULT CMainFrame::OnQueryNavigate(WPARAM wParam, LPARAM lParam) {

	if( wParam==1) { // ayuda
		SendMessage(WM_TREENAVEGADOR, NM_DBLCLK, (LPARAM) m_wndHelpBar.GetHelpDoc());
	}
	SendMessageBroadCast(WM_HHELP, wParam, lParam);
	return 0;
}

void CMainFrame::OnDestroy()
{
	CMDIFrameWndXP::OnDestroy();
	if(m_pFinger) {
		delete m_pFinger;
		m_pFinger=NULL;
	}
}

LRESULT CMainFrame::OnFingerPrint(WPARAM wParam, LPARAM lParam) {

	SendMessageBroadCast(WM_FINGERPRINT, wParam, lParam);
	return 0;
}

LRESULT CMainFrame::OnQueryFinger(WPARAM wParam, LPARAM lParam) {
	if(lParam) {
		CWnd *handle= (CWnd *)lParam;
		handle->SendMessage(WM_FINGERPRINT, GR_PLUG, (LPARAM) m_pFinger);
	}
	return (LRESULT)m_pFinger;
}

void CMainFrame::OnHerramientasPersonalizar()
{
	COpcionesSheet opciones("Personalizar",this);
	opciones.DoModal();
}

int CMainFrame::GetDevicesCount() const {

	if(m_pEventCallback != NULL)
		return m_pEventCallback->GetNumDevices();
	return 0;
}
