// EntregaView.cpp : implementation file
//

#include "stdafx.h"
#include "SVE.h"
#include "EntregaView.h"
// CEntregaView

IMPLEMENT_DYNCREATE(CEntregaView, CFormView)

CEntregaView::CEntregaView()
	: CFormView(CEntregaView::IDD)
{
	m_pUsuario= NULL;
	m_pSplash=NULL;
	m_bExiste=false;
	m_bFound=false;
	m_pPrint= NULL;
	m_VMRCap= NULL;
	m_nAncho=0;
	m_nAlto=0;
	m_nCamaras=0;
	m_pPrint=NULL;
}

CEntregaView::~CEntregaView()
{
	if( m_pPrint) 
		delete m_pPrint;
}

void CEntregaView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EGUIA, m_EGuia);
	DDX_Control(pDX, IDC_EOFICINA, m_EOficina);
	DDX_Control(pDX, IDC_ECIUDAD, m_ECiudad);
	DDX_Control(pDX, IDC_DFECHA, m_DTFecha);
	DDX_Control(pDX, IDC_CTIPOD, m_CBTipoD);
	DDX_Control(pDX, IDC_ENODOCUMENTO, m_EDocumento);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDC_EDIRECCION, m_EDireccion);
	DDX_Control(pDX, IDC_ETELEFONO, m_ETelefono);
	DDX_Control(pDX, IDC_ECANTIDAD, m_ECantidad);
	DDX_Control(pDX, IDC_EPESO, m_EPeso);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_BCANCELAR, m_BCancelar);
	DDX_Control(pDX, IDC_SLOGO, m_SLogo);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_EDESCRIPCION, m_EDescripcion);
	DDX_Control(pDX, IDC_EOBSERVACIONES, m_EObservaciones);
	DDX_Control(pDX, IDC_CAMARAS, m_CBCamaras);
	DDX_Control(pDX, IDC_BCAPTURAR, m_BFoto);
	DDX_Control(pDX, IDC_SFOTO3, m_SFoto);
	DDX_Control(pDX, IDC_STEMP, m_SCamara);
}

BEGIN_MESSAGE_MAP(CEntregaView, CFormView)
	ON_EN_CHANGE(IDC_EGUIA, &CEntregaView::OnEnChangeEguia)
	ON_EN_CHANGE(IDC_ENODOCUMENTO, &CEntregaView::OnEnChangeEnodocumento)
	ON_BN_CLICKED(IDOK, &CEntregaView::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BCANCELAR, &CEntregaView::OnBnClickedBcancelar)
	ON_MESSAGE(WM_MYSQL,OnMysqlMessage)
	ON_EN_KILLFOCUS(IDC_EGUIA, &CEntregaView::OnEnKillfocusEguia)
	ON_EN_KILLFOCUS(IDC_ENODOCUMENTO, &CEntregaView::OnEnKillfocusEnodocumento)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_FINGERPRINT,OnFingerPrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdatePrint)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_SAVE,OnTemp)
	ON_COMMAND(ID_FILE_SAVE_AS,OnTemp)
	ON_BN_CLICKED(IDC_BCAPTURAR, &CEntregaView::OnBnClickedBcapturar)
	ON_CBN_SELENDOK(IDC_CAMARAS, &CEntregaView::OnCbnSelendokCamaras)
	ON_WM_TIMER()
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
END_MESSAGE_MAP()


// CEntregaView diagnostics

#ifdef _DEBUG
void CEntregaView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEntregaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEntregaView message handlers

void CEntregaView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);
	m_pUsuario= (CUsuario *) AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO, 0, (LPARAM)this);

	GetDocument()->SetTitle("Internacional");

	m_nAncho= AfxGetApp()->GetProfileInt("bitmap", "ancho", 320);
	m_nAlto= AfxGetApp()->GetProfileInt("bitmap", "alto", 240);

	m_SLogo.SetBitmap(IDB_BLOGO);

	m_pSplash=new CSplashScreenEx();

	m_pSplash->Create(this,"SVE splash",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	m_pSplash->SetBitmap(IDB_ACERCADE,255,0,255);
	m_pSplash->SetTextFont("Arial",100,CSS_TEXT_NORMAL);
	m_pSplash->SetTextRect(CRect(100,300,600,350));
	m_pSplash->SetTextColor(RGB(0,0,0));
	m_pSplash->SetTextFormat(DT_RIGHT | DT_VCENTER);
	m_pSplash->Show();
	CString szTemp;
	szTemp.Format("SVE Versión %s\r\n",SVE_VERSION);
	m_pSplash->SetText(szTemp);

	m_nCamaras= CVMR_Capture::EnumDevices(m_CBCamaras);
	if(m_nCamaras) {
		m_CBCamaras.SetCurSel(AfxGetApp()->GetProfileInt("camaras","cuatro", 0));
		OnCbnSelendokCamaras();
	}

	if( m_nCamaras<=0) {
		MessageBox("Por favor conecte al menos una camara.\rY Reinicie SVE.", NULL, MB_ICONERROR);
		m_BAceptar.EnableWindow(false);
	}

	if(m_pSplash) {
		CString szTemp;
		szTemp.Format("SVE Versión %s\r\nIniciando Dispositivo de Huellas",SVE_VERSION);
		m_pSplash->SetText(szTemp);
	}

	m_SHuella.HideInfo();
	// m_pFinger.SetParent(this);
	// m_pFinger.Init(&m_SHuella);

	m_pConector->Query("SELECT id,nombre FROM shared WHERE nReservado=3 AND (szReservado!=-1 OR szReservado IS NULL)", 100, this);

	m_pSplash->Hide();
	m_pSplash=NULL;
}

void CEntregaView::OnEnChangeEguia()
{
	
}

void CEntregaView::OnEnChangeEnodocumento()
{
	m_bExiste=false;
}

void CEntregaView::OnBnClickedOk()
{
	CStringArray valores, campos;
	CStringArray valoresu, camposu;
	CStringArray valorest, campost;
	CString temp;
	CString usuario;
	CString szGuia;
	
	m_EGuia.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_EGuia, "Por favor ingrese un número de guia",2);
		return;
	}
	szGuia=temp;

	if(m_szCurrentId.IsEmpty()) {
		valores.Add(temp);
		campos.Add("codigo");
	}
	m_EOficina.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_EOficina, "Por favor ingrese una oficina",2);
		return;
	}
	valores.Add(temp);
	campos.Add("oficina");
	
	m_ECiudad.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_ECiudad, "Por favor ingrese una ciudad",2);
		return;
	}
	valores.Add(temp);
	campos.Add("ciudad");

	CMyTime tiempo;
	m_DTFecha.GetTime(tiempo);
	valores.Add(tiempo.GetFormatoMysql());
	campos.Add("fecha");

	int pos= m_CBTipoD.GetCurSelEx();
	if( pos==-1) {
		SetError(&m_CBTipoD, "Por favor seleccione un tipo de documento", 2);
		return;
	}
	temp.Format("%d", m_CBTipoD.GetItemData(pos));
	valorest.Add(temp);
	campost.Add("tipodocumento");
	

	m_EDocumento.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_EDocumento, "Por favor ingrese un número de documento",2);
		return;
	}
	usuario= temp;
	valores.Add(temp);
	campos.Add("usuario");
	valorest.Add(temp);
	campost.Add("usuario");
	valoresu.Add(temp);
	camposu.Add("documento");

	m_ENombre.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_ENombre, "Por favor ingrese un nombre",2);
		return;
	}
	valoresu.Add(temp);
	camposu.Add("nombre");

	m_EApellido.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_EApellido, "Por favor ingrese un apellido",2);
		return;
	}
	valoresu.Add(temp);
	camposu.Add("apellido");

	m_EDireccion.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_EDireccion, "Por favor ingrese una dirección",2);
		return;
	}
	valores.Add(temp);
	campos.Add("direccion");

	m_ETelefono.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetError(&m_ETelefono, "Por favor ingrese un teléfono",2);
		return;
	}
	valores.Add(temp);
	campos.Add("telefono");

	m_ECantidad.GetWindowText(temp);
	if( temp.IsEmpty() || atoi(temp)==0) {
		SetError(&m_ECantidad, "Por favor ingrese una cantidad",2);
		return;
	}
	valores.Add(temp);
	campos.Add("cantidad");


	m_EPeso.GetWindowText(temp);
	if( temp.IsEmpty() || atoi(temp)==0) {
		SetError(&m_EPeso, "Por favor ingrese un peso",2);
		return;
	}
	valores.Add(temp);
	campos.Add("peso");

	m_EDescripcion.GetWindowText(temp);
	if(temp.IsEmpty()) {
		SetError(&m_EDescripcion, "Por favor ingrese una descripción", 2);
		return;
	}
	valores.Add(temp);
	campos.Add("descripcion");

	m_EObservaciones.GetWindowText(temp);
	valores.Add(temp);
	campos.Add("observaciones");

	if( !m_SHuella.HasHuella()) {
		SetError(NULL, "Por favor coloque la huella", 2);
		return;
	}

	if(!m_bFound) {
		/*if(m_pDlgConfirmar)
			delete m_pDlgConfirmar;
		m_pDlgConfirmar= new CDlgConfirmar();
		m_pDlgConfirmar->m_pFinger= &m_pFinger;
		int retorno= m_pDlgConfirmar->DoModal();
		delete m_pDlgConfirmar;
		m_pDlgConfirmar=NULL;*/
		// m_pFinger.SetBitmap(&m_SHuella);
		/*if(retorno!=IDOK)
			return;*/
	}

	m_pConector->Query(m_szCurrentId.IsEmpty() ? CQuery::CreateInsert("entregaguia", campos, valores) : CQuery::CreateUpdate("entregaguia", campos, valores, "codigo="+ m_szCurrentId), 0, this);
	
	// usuario
	temp= CQuery::CreateInsert("usuario", camposu, valoresu);
	temp.Replace("INSERT INTO", "INSERT IGNORE INTO");
	m_pConector->Query(temp, 0, this);
	m_pConector->Query(CQuery::CreateUpdate("usuario", camposu, valoresu, "documento="+ usuario), 0, this);


	// tipo de usuario
	temp= CQuery::CreateInsert("datosusuario", campost, valorest);
	temp.Replace("INSERT INTO", "INSERT IGNORE INTO");
	m_pConector->Query(temp, 0, this);
	m_pConector->Query(CQuery::CreateUpdate("datosusuario", campost, valorest, "usuario="+ usuario), 0, this);

	CString app;
	BYTE *buffer;

	if(m_SHuella.HasHuella()) {

		valores.RemoveAll();
		if( !m_bExiste)
			valores.Add(usuario);
		// ancho
		//temp.Format("%d", m_SHuella.m_nAncho);
		//valores.Add(temp);
		//// alto
		//temp.Format("%d", m_SHuella.m_nAlto);
		//valores.Add(temp);
		//// resolucion
		//temp.Format("%d", m_SHuella.m_nResolucion);
		//valores.Add(temp);
		// huella
		/*valores.Add("%0%");
		buffer= m_SHuella.m_cRaw;
		for( int i=0; i<m_SHuella.GetSize(); i++) {
			switch( (buffer)[i]) {
				case 0:
					app+="\\0";
					break;
				case '\\':
					app+="\\\\";
					break;
				case '\"':
					app+="\\\"";
					break;
				default:
					app+= (buffer)[i];
					break;
			}
		}*/
		if( m_bExiste) {
			campos.RemoveAll();
			campos.Add("ancho");
			campos.Add("alto");
			campos.Add("resolucion");
			campos.Add("huella");
			temp= CQuery::CreateUpdate("huella", campos, valores,"documento=" + usuario); 
		} else temp= CQuery::CreateInsert("huella", valores);
		if(AfxGetApp()->GetProfileInt("control", "compress", 1))
			temp.Replace("\"%0%\"", "COMPRESS(\"%0%\")");
		temp.Replace("%0%", app);
		m_pConector->Query(temp, 0 , this);
	}


	// vectores de la huella
	valores.RemoveAll();
	campos.RemoveAll();

	campos.Add("documento");
	valores.Add(usuario);
	// huella
	campos.Add("huella");
	valores.Add("%0%");
	app.Empty();
	/*buffer= m_SHuella.m_cTemplate;
	for( int i=0; i<m_SHuella.m_nTemplate; i++) {
		switch( (buffer)[i]) {
			case 0:
				app+="\\0";
				break;
			case '\\':
				app+="\\\\";
				break;
			case '\"':
				app+="\\\"";
				break;
			default:
				app+= (buffer)[i];
				break;
		}
	}*/
	temp= CQuery::CreateInsert("template", campos, valores);
	temp.Replace("INSERT INTO", "INSERT IGNORE INTO");
	temp.Replace("%0%", app);
	m_pConector->Query(temp, 0 , this);
	temp.Empty();
	
	temp= CQuery::CreateUpdate("template", campos, valores, "documento=" + usuario);
	temp.Replace("%0%", app);
	m_pConector->Query(temp, 0 , this);




	// inicio con las camaras
	Pause(true);
	int len=0;

	unsigned char *imagen=NULL;

	if(m_VMRCap && (len=m_VMRCap->GrabFrame())) {
		valores.RemoveAll();

		valores.Add(usuario);
		valores.Add("%0%");
		CString app;
		// imagen= m_SFoto.GetCompress(len, AfxGetApp()->GetProfileInt("local", "calidad", 75));
		ASSERT(imagen);
		ASSERT(len);
		for( int i=0; i<len; i++) {
			switch( imagen[i]) {
				case 0:
					app+="\\0";
					break;
				case '\\':
					app+="\\\\";
					break;
				case '\"':
					app+="\\\"";
					break;
				default:
					app+= imagen[i];
					break;
			}
		}
		valores.SetAt(0, szGuia);
		CString query= CQuery::CreateInsert("fotoentrega", valores);
		query.Replace("%0%", app);
		m_pConector->Query(query, 0 , this);
	}

	if(imagen) {
		delete []imagen;
		imagen=NULL;
	}
	OnBnClickedBcancelar();
}

void CEntregaView::Clean() {
	
	m_bExiste=false;
	m_szOldCedula.Empty();
	m_szCurrentId.Empty();

	m_EOficina.SetWindowText("");
	m_ECiudad.SetWindowText("");
	m_CBTipoD.SetCurSel(-1);
	m_EDocumento.SetWindowText("");
	m_ENombre.SetWindowText("");
	m_EApellido.SetWindowText("");
	m_EDireccion.SetWindowText("");
	m_ETelefono.SetWindowText("");
	m_ECantidad.SetWindowText("");
	m_EPeso.SetWindowText("");
	m_BAceptar.EnableWindow(true);
	CMyTime tiempo(true);
	m_DTFecha.SetTime(tiempo);
	m_EObservaciones.SetWindowText("");
	m_EDescripcion.SetWindowText("");
	
	
	m_SFoto.SetWindowText("");
	// m_SFoto.SetTitulo();
	// m_SFoto.ShowImage(NULL);
	Pause(false);
	m_BFoto.EnableWindow();

	m_SHuella.Clean();
	m_SHuella.SetBitmap((HBITMAP) NULL);
	m_SHuella.SetWindowText("");
	m_SHuella.Invalidate();

	m_EOficina.SetReadOnly(false);
	m_ECiudad.SetReadOnly(false);
	m_DTFecha.EnableWindow();
	m_EDocumento.SetReadOnly(false);
	m_EDireccion.SetReadOnly(false);
	m_ETelefono.SetReadOnly(false);
	m_ECantidad.SetReadOnly(false);
	m_EPeso.SetReadOnly(false);
	m_CBTipoD.EnableWindow();
	m_ENombre.SetReadOnly(false);
	m_EApellido.SetReadOnly(false);
	m_EDescripcion.SetReadOnly(false);
	m_EObservaciones.SetReadOnly(false);

	// m_pFinger.Start();
}

void CEntregaView::OnBnClickedBcancelar()
{
	Clean();
	m_szOldGuia.Empty();
	m_EGuia.SetWindowText("");
	SetError(&m_EGuia, "Listo");
}

LRESULT CEntregaView::OnFingerPrint(WPARAM wParam, LPARAM lParam) {

	if( lParam==(LPARAM)&m_SHuella) {

		m_bFound=false;
		m_EDocumento.SetReadOnly();
		m_EGuia.SetReadOnly();
		// VERIFY(m_pFinger.Prepare());
		m_pConector->QueryBinary("SELECT huella,documento FROM template", 100, this);
	} /*else if(m_pDlgConfirmar) {
		return m_pDlgConfirmar->SendMessage(WM_FINGERPRINT, wParam, lParam);
	}*/
	return 0;
}

LRESULT CEntregaView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {
	
	MySQLMessage *mess=(MySQLMessage *)lParam;
	CThreadMySQL *hilo=(CThreadMySQL *)lParam;
	MySQLMessageB *messb= (MySQLMessageB *)lParam;

	switch(wParam)
	{
	case QUERY_RESULT:
		if(mess->GetID()==100) {
			m_CBTipoD.SetItemData(m_CBTipoD.AddString(mess->GetAt(1)), atoi(mess->GetAt(0)));
		} else if (mess->GetID()==CEntregaView::findguia)
		{
			m_bExiste=true;
			m_BAceptar.EnableWindow(false);
			///*entregaguia.codigo
			m_szCurrentId= mess->GetAt(0);
			m_EGuia.SetWindowText(mess->GetAt(0));
			//,entregaguia.oficina
			m_EOficina.SetWindowText(mess->GetAt(1));
			//,entregaguia.ciudad
			m_ECiudad.SetWindowText(mess->GetAt(2));
			//,entregaguia.fecha
			CMyTime tiempo(mess->GetAt(3));
			m_DTFecha.SetTime(tiempo);
			//,entregaguia.usuario
			m_szOldCedula= mess->GetAt(4);
			m_EDocumento.SetWindowText(m_szOldCedula);
			//entregaguia.direccion
			m_EDireccion.SetWindowText(mess->GetAt(5));
			//,entregaguia.telefono
			m_ETelefono.SetWindowText(mess->GetAt(6));
			//,entregaguia.cantidad
			m_ECantidad.SetWindowText(mess->GetAt(7));
			//,entregaguia.peso
			m_EPeso.SetWindowText(mess->GetAt(8));
			//,datosusuario.tipodocumento
			m_CBTipoD.SetSelData(atoi(mess->GetAt(9)));
			//,usuario.nombre
			m_ENombre.SetWindowText(mess->GetAt(10));
			//,usuario.apellido
			m_EApellido.SetWindowText(mess->GetAt(11));
			// entregaguia.descripcion
			m_EDescripcion.SetWindowText(mess->GetAt(12));
			// entregaguia.observaciones
			m_EObservaciones.SetWindowText(mess->GetAt(13));

			m_EOficina.SetReadOnly();
			m_ECiudad.SetReadOnly();
			m_DTFecha.EnableWindow(false);
			m_EDocumento.SetReadOnly();
			m_EDireccion.SetReadOnly();
			m_ETelefono.SetReadOnly();
			m_ECantidad.SetReadOnly();
			m_EPeso.SetReadOnly();
			m_CBTipoD.EnableWindow(false);
			m_ENombre.SetReadOnly();
			m_EApellido.SetReadOnly();
			m_EObservaciones.SetReadOnly();
			m_EDescripcion.SetReadOnly();
			

			Pause(true);
			KillTimer(5);
			m_BFoto.EnableWindow(false);
			m_SFoto.SetBitmap(NULL);
			m_SFoto.SetWindowText("Cargando");
			// m_SFoto.SetTitulo("Internacional" + mess->GetAt(0));

			m_SHuella.Clean();
			m_SHuella.SetBitmap((HBITMAP)NULL);
			m_SHuella.SetWindowText("Cargando");
			// m_pFinger.Stop();

			CString texto;
			texto.Format("SELECT foto FROM fotoentrega WHERE codigo=%s", mess->GetAt(0));
			m_pConector->QueryBinary(texto, CEntregaView::findfoto, this);


			m_pConector->Query("SELECT ancho,alto,resolucion FROM huella WHERE documento=" + m_szOldCedula, 7, this);
			if(AfxGetApp()->GetProfileInt("control", "compress", 1))
				m_pConector->QueryBinary("SELECT UNCOMPRESS(huella) FROM huella WHERE documento=" + m_szOldCedula, 8, this);
			else m_pConector->QueryBinary("SELECT huella FROM huella WHERE documento=" + m_szOldCedula, 8, this);

		} else if(mess->GetID()==CEntregaView::finddocumento) {
			// usuario.nombre,
			m_ENombre.SetWindowText(mess->GetAt(0));
			// usuario.apellido,
			m_EApellido.SetWindowText(mess->GetAt(1));
			// datosusuario.tipodocumento
			m_CBTipoD.SetSelData(atoi(mess->GetAt(2)));
			m_ENombre.SetReadOnly();
			m_EApellido.SetReadOnly();
			m_CBTipoD.EnableWindow(mess->GetAt(2).IsEmpty());

			if(!m_bExiste) {
				m_SHuella.Clean();
				m_SHuella.SetBitmap((HBITMAP)NULL);
				m_SHuella.SetWindowText("Cargando");
				m_pConector->Query("SELECT ancho,alto,resolucion FROM huella WHERE documento=" + m_szOldCedula, 7, this);
				if(AfxGetApp()->GetProfileInt("control", "compress", 1))
					m_pConector->QueryBinary("SELECT UNCOMPRESS(huella) FROM huella WHERE documento=" + m_szOldCedula, 8, this);
				else m_pConector->QueryBinary("SELECT huella FROM huella WHERE documento=" + m_szOldCedula, 8, this);
			}
			m_bExiste=true;
		} else if( mess->GetID()==7) {
			m_SHuella.SetParametros(atoi(mess->GetAt(1)),
				atoi(mess->GetAt(0)),
				atoi(mess->GetAt(2)));
		} else if( messb->GetID()==8) {
			// m_pFinger.SetHuella(&messb->pMessage);
		} else if(messb->GetID()==100) {
			int score=0;
			//if(m_pFinger.Compare((const char *)messb->pMessage.GetData(), &score)) {
			//	// CString resultado;
			//	// resultado.Format("Huella encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
			//	m_pConector->KillQuery(100, this);
			//	// m_SMensaje.SetWindowText(resultado);
			//	m_EDocumento.SetWindowText(messb->pStrings.GetAt(0));
			//	// m_szOld.Empty();
			//	m_bExiste=true;
			//	m_bFound=true;
			//	OnEnKillfocusEnodocumento();
			//}
		} else if(messb->GetID()==CEntregaView::findfoto) {
			m_SFoto.SetBitmap(*messb->GetBinaryData());
		}
		break;
	case QUERY_EMPTY:
		if(mess->GetID()==CEntregaView::finddocumento) {
			m_bExiste=false;
			m_ENombre.SetWindowText("");
			m_EApellido.SetWindowText("");
			m_ENombre.SetReadOnly(false);
			m_EApellido.SetReadOnly(false);
			m_CBTipoD.EnableWindow(true);
		} else if (mess->GetID()==CEntregaView::findguia)
		{
			if(!m_szCurrentId.IsEmpty()) {
				Clean();
			}
			m_szCurrentId.Empty();
		}
		break;
	case THREAD_END:
		if( mess->GetID()==8) {
			m_SHuella.SetWindowText(m_SHuella.HasHuella() ? "" : "No hay huella");
			m_SHuella.Invalidate();
		} else if(mess->GetID()==100) {
			m_EDocumento.SetReadOnly(false);
			m_EGuia.SetReadOnly(false);
			if(m_bExiste && !m_bFound) {

				m_bExiste=false;
				m_EDocumento.SetWindowText("");
				m_szOldCedula.Empty();
				m_ENombre.SetWindowText("");
				m_EApellido.SetWindowText("");
				m_ENombre.SetReadOnly(false);
				m_EApellido.SetReadOnly(false);
				m_CBTipoD.EnableWindow(true);
				/*m_SHuella.Clean();
				CString resultado;
				resultado.Format("Huella no encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
				m_SMensaje.SetWindowText(resultado);*/
			}
		} else if(mess->GetID()==CEntregaView::findfoto) {
			m_SFoto.SetWindowText(m_SFoto.HasImage() ? "" : "No hay imagen");
		}
	    break;
	}
	return 0;
}

void CEntregaView::OnEnKillfocusEguia()
{
	CString temp;
	m_EGuia.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szOldGuia) {
		return;
	}
	m_szOldGuia= temp;
	temp.Format("SELECT entregaguia.codigo,entregaguia.oficina,entregaguia.ciudad,entregaguia.fecha,entregaguia.usuario,\
				entregaguia.direccion,entregaguia.telefono,entregaguia.cantidad,entregaguia.peso\
				,datosusuario.tipodocumento,usuario.nombre,usuario.apellido\
				,entregaguia.descripcion,entregaguia.observaciones\
				FROM entregaguia \
				LEFT JOIN datosusuario ON datosusuario.usuario=entregaguia.usuario \
				LEFT JOIN usuario ON usuario.documento=entregaguia.usuario \
				WHERE entregaguia.codigo='%s'"
				,m_szOldGuia);
	m_pConector->Query(temp, CEntregaView::findguia, this);
}

void CEntregaView::OnEnKillfocusEnodocumento()
{
	CString temp;
	m_EDocumento.GetWindowText(temp);
	if( temp.IsEmpty() || m_szOldCedula==temp) {
		return;
	}
	m_szOldCedula= temp;
	temp.Format("SELECT usuario.nombre,usuario.apellido,datosusuario.tipodocumento\
				FROM usuario LEFT JOIN datosusuario ON datosusuario.usuario=usuario.documento\
				WHERE usuario.documento='%s'", m_szOldCedula);
	m_pConector->Query(temp, CEntregaView::finddocumento, this);
}

void CEntregaView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CEntregaView::OnDestroy()
{
	m_EGuia.SetWindowText("");
	m_EDocumento.SetWindowText("");
	SAFE_DELETE(m_VMRCap);
	// m_pFinger.OnDestroy();
	CFormView::OnDestroy();
}

void CEntregaView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnBeginPrinting(pDC, pInfo);
}

void CEntregaView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnPrint(pDC, pInfo);
	// m_pRTF.Print(pDC, pInfo);
	// CFormView::OnPrint(pDC, pInfo);
}

BOOL CEntregaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	InitRTF();
	if( m_pPrint)
		delete m_pPrint;
	m_pPrint= new CPrintRTF();

	m_pPrint->SetRTF(&m_pRTF);
	m_pPrint->SetAppName("SVE");
	m_pPrint->SetDocTitle(GetDocument()->GetTitle());

	m_pPrint->OnPreparePrinting(pInfo);

	m_pPrint->SetMarginBottomPix(AfxGetApp()->GetProfileInt("printer", "cy", 20));
	m_pPrint->SetMarginLeftPix(AfxGetApp()->GetProfileInt("printer", "x", 20));
	m_pPrint->SetMarginRightPix(AfxGetApp()->GetProfileInt("printer", "cx", 20));
	m_pPrint->SetMarginTopPix(AfxGetApp()->GetProfileInt("printer", "y", 20));

	return DoPreparePrinting(pInfo);
}

void CEntregaView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnEndPrinting(pDC, pInfo);
	delete m_pPrint;
	m_pPrint= NULL;
}

void CEntregaView::OnUpdatePrint(CCmdUI *pCmdUI) {

	pCmdUI->Enable(!m_szCurrentId.IsEmpty());
	/*if(m_pUsuario->Pertenece(1) || m_pUsuario->Pertenece(4))
		pCmdUI->Enable( !m_BAceptar.IsWindowEnabled());
	else pCmdUI->Enable(false);*/
}

bool CEntregaView::InitRTF(bool foto/*=false*/) {
	
	if(m_szCurrentId.IsEmpty())
		return false;

	foto= m_pUsuario->Pertenece(GRUPO_ADMINISTRADOR);


	CRect rect;
	GetClientRect(rect);
	CString bitmap=CImport::GetTemporalPath("huella.bmp");
	// m_pFinger.SaveToFile(bitmap);

	if(!::IsWindow(m_pRTF)) {
		DWORD dwStyle = WS_CHILD|WS_BORDER|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN| ES_READONLY;
		VERIFY(m_pRTF.Create(dwStyle, rect, this, 0x1245 ));
		/*m_pRTF.m_saVariables.RemoveAll();
		m_pRTF.m_saVariables.Add("{$ciudad}");
		m_pRTF.m_saVariables.Add("{$fecha}");
		m_pRTF.m_saVariables.Add("{$oficina}");
		m_pRTF.m_saVariables.Add("{$guia}");
		m_pRTF.m_saVariables.Add("{$cantidad}");
		m_pRTF.m_saVariables.Add("{$peso}");
		m_pRTF.m_saVariables.Add("{$nombre}");
		m_pRTF.m_saVariables.Add("{$apellido}");
		m_pRTF.m_saVariables.Add("{$tipodoc}");
		m_pRTF.m_saVariables.Add("{$documento}");
		m_pRTF.m_saVariables.Add("{$direccion}");
		m_pRTF.m_saVariables.Add("{$telefono}");
		m_pRTF.m_saVariables.Add("{$descripcion}");
		m_pRTF.m_saVariables.Add("{$observaciones}");
		m_pRTF.m_saVariables.Add("{$usuario}");*/
	}

	//CString temp;
	//m_pRTF.m_saValores.RemoveAll();

	//m_ECiudad.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//CMyTime tiempo;
	//m_DTFecha.GetTime(tiempo.m_pTiempo);
	//tiempo.SetTiempo(tiempo.m_pTiempo);
	//m_pRTF.m_saValores.Add(tiempo.GetFecha(true));
	//m_EOficina.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EGuia.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_ECantidad.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EPeso.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_ENombre.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EApellido.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_CBTipoD.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EDocumento.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EDireccion.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_ETelefono.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EDescripcion.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_EObservaciones.GetWindowText(temp);
	//m_pRTF.m_saValores.Add(temp);
	//m_pRTF.m_saValores.Add(m_pUsuario->GetNombre());


	//VERIFY(m_pRTF.LoadFromFile(CApplicacion::GetAppPath( foto ? "extras\\plantillaa.rtf" : "extras\\plantilla.rtf")));

	//if( m_pRTF.SelectText("{$foto}")!=-1) {
	//	CBitmap bitmap;
	//	/*m_SFoto.GetBitmap(&bitmap,
	//		foto ? AfxGetApp()->GetProfileInt("huella", "cxfoto", 320) : AfxGetApp()->GetProfileInt("huella", "cx", 120),
	//		foto ? AfxGetApp()->GetProfileInt("huella", "cyfoto", 240) : AfxGetApp()->GetProfileInt("huella", "cy", 120));*/
	//	CImageDataObject::InsertBitmap(m_pRTF.GetIRichEditOle(), bitmap);
	//}
	//if(m_pRTF.SelectText("{$huella}")!=-1) {
	//	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), 
	//		bitmap,
	//		IMAGE_BITMAP, 
	//		foto ? AfxGetApp()->GetProfileInt("huella", "cxfoto", 320) : AfxGetApp()->GetProfileInt("huella", "cx", 120),
	//		foto ? AfxGetApp()->GetProfileInt("huella", "cyfoto", 240) : AfxGetApp()->GetProfileInt("huella", "cy", 120), 
	//		LR_LOADFROMFILE);
	//	CImageDataObject::InsertBitmap(m_pRTF.GetIRichEditOle(), hBmp);
	//}
	//DeleteFile(bitmap);
	
	return true;
}

void CEntregaView::OnTemp() {

	CString temp;
	m_EGuia.GetWindowText(temp);
	CFileDialog dlg( false, "rtf", "Recepción " + temp, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, "Archivo RTF(*.rtf)|*.rtf| Todos los archivos (*.*)|*.*||");
	if(dlg.DoModal()==IDOK) {
		InitRTF();
		if(!m_pRTF.SaveFile(dlg.GetPathName())) {
			AfxMessageBox("No se puede obtener acceso a \"" + dlg.GetPathName() + "\".\nCompruebe que el archivo no este siendo utilizado por otro proceso.", MB_OK | MB_ICONERROR);
			return;
		}
		if( MyMessageBox("¿Desea ver el archivo?", MB_ICONQUESTION | MB_YESNOCANCEL, "showfile")==IDYES) 
			ShellExecute(NULL, "open", dlg.GetPathName(), NULL,NULL,SW_SHOWNORMAL);
	}
}

BOOL CEntregaView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		if(AfxGetApp()->GetProfileInt("componentes","enter", 0) && pMsg->wParam== VK_RETURN) {

			CWnd *foco= GetFocus();
			if( foco)
				if( !foco->IsKindOf( RUNTIME_CLASS(CButtonXP))) 

					pMsg->wParam= VK_TAB;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}

void CEntregaView::Pause(bool pause) {

	if(m_VMRCap) {
		if( pause) {
			KillTimer(5);
			if( !m_VMRCap->IsPaused()) {
				m_VMRCap->SetPause(true);
				DWORD dwSize;
				dwSize=m_VMRCap->GrabFrame();
				if(dwSize>0)
				{
					BYTE *pImage;
					m_VMRCap->GetFrame(&pImage);
					m_SFoto.ShowImage(pImage);
				}
			}
		} else if(m_VMRCap->IsPaused()) {
			if(AfxGetApp()->GetProfileInt("local", "reiniciar", 0)) {
				OnCbnSelendokCamaras();
			} else m_VMRCap->SetPause(false);
			SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		}
		CString texto= m_VMRCap->IsPaused() ? "Reanudar" : "Tomar Foto";
		texto.Insert(0, "&");
		m_BFoto.SetWindowText(texto);
	}
}

void CEntregaView::OnBnClickedBcapturar()
{
	CWaitCursor wait;
	if(m_VMRCap) 
		Pause(!m_VMRCap->IsPaused());
}

void CEntregaView::OnCbnSelendokCamaras()
{
	CWaitCursor wait;
	if(m_pSplash) {
		CString szTemp;
		szTemp.Format("SVE Versión %s\r\nIniciando Camara",SVE_VERSION);
		m_pSplash->SetText(szTemp);
	}
	SetError(NULL, "Iniciando camara");
	KillTimer(5);
	SAFE_DELETE(m_VMRCap);
	if( m_CBCamaras.GetCurSel()!=-1) {
		m_VMRCap= new CVMR_Capture();
		m_VMRCap->Init(m_CBCamaras.GetCurSel(),m_SCamara, m_nAncho, m_nAlto);
		AfxGetApp()->WriteProfileInt("camaras","cuatro", m_CBCamaras.GetCurSel());
		SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
	}
}

void CEntregaView::Refresh() {

	if( m_VMRCap && m_VMRCap->GrabFrame()) {
		BYTE *pImage;
		m_VMRCap->GetFrame(&pImage);
		m_SFoto.ShowImage(pImage);
	}
}

void CEntregaView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==5) {
		Refresh();
	}
	CFormView::OnTimer(nIDEvent);
}

LRESULT CEntregaView::OnInsertODS(WPARAM wParam, LPARAM lParam) {
	
	if(wParam==1) {
		CString texto;
		texto.Format("%d", lParam);
		m_szOldGuia="";
		m_szCurrentId="";
		m_EGuia.SetWindowText(texto);
		OnEnKillfocusEguia();
	}
	return 0;
}
