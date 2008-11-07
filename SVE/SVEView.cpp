// SVEView.cpp: implementación de la clase CSVEView
//

#include "stdafx.h"
#include "SVE.h"
#include "SVEView.h"
# include "ImagenStream.h"
# include "MainFrm.h"
# include "DlgProgreso.h"

# include <atlimage.h>
# include "wia/WiaWrap.h"

# include "CutPhoto.h"
# include "BitmapLib.h"

DEFINE_GUID(ImageFormatJPEG, 0xb96b3cae,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);
DEFINE_GUID(ImageFormatBMP, 0xb96b3cab,0x0728,0x11d3,0x9d,0x7b,0x00,0x00,0xf8,0x1e,0xf3,0x2e);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSVEView

IMPLEMENT_DYNCREATE(CSVEView, CFormView)

BEGIN_MESSAGE_MAP(CSVEView, CFormView)
	ON_BN_CLICKED(IDC_BCAPTURAR, OnBnClickedBcapturar)
	ON_BN_CLICKED(IDC_BCAPTURAR2, OnBnClickedBcapturar2)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_EN_KILLFOCUS(IDC_ECODIGO, OnEnKillfocusEcodigo)
	ON_EN_KILLFOCUS(IDC_ECEDULA, OnEnKillfocusEcedula)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_CBN_SELENDOK(IDC_CAMARAS, OnCbnSelendokCamaras)
	ON_CBN_SELENDOK(IDC_CAMARAS2, OnCbnSelendokCamaras2)
	ON_MESSAGE(WM_INSERTODS,OnInsertODS)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_BN_CLICKED(IDC_BCAPTURAR3, OnBnClickedBcapturar3)
	ON_CBN_SELENDOK(IDC_CAMARAS3, OnCbnSelendokCamaras3)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_BN_CLICKED(IDC_BAGREGAR, OnBnClickedBagregar)
	ON_NOTIFY(NM_DBLCLK, IDC_LMULTIPLE, OnNMDblclkLmultiple)
	ON_NOTIFY(NM_RCLICK, IDC_LMULTIPLE, OnNMRclickLmultiple)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LMULTIPLE, OnLvnKeydownLmultiple)
	ON_COMMAND(315,OnEditItem)
	ON_COMMAND(316,OnDeleteItem)
	ON_COMMAND(ID_B_AGREGAR, OnBAgregar)
	ON_MESSAGE(WM_FINGERPRINT,OnFingerPrint)
	ON_COMMAND(ID_FILE_SAVE,OnGuardar)
	ON_COMMAND(ID_FILE_SAVE_AS,OnGuardar)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdatePrint)
	ON_EN_CHANGE(IDC_ECEDULA, &CSVEView::OnEnChangeEcedula)
END_MESSAGE_MAP()

// Construcción o destrucción de CSVEView

CSVEView::CSVEView()
	: CFormView(CSVEView::IDD)
{
	m_VMRCap= m_VMRCap2=m_VMRCap3= NULL;
	m_pPrint=NULL;
	m_pSplash=NULL;
	m_bExiste=false;
	m_nCamaras=0;
	m_nFrameLen[0]=0;
	m_nFrameLen[1]=0;
	m_pCamaras[0]=NULL;
	m_pCamaras[1]=NULL;
	m_bFound=false;

	m_bUsuario= false;
	m_bIngreso=false;
}

CSVEView::~CSVEView()
{
	if( m_pPrint) 
		delete m_pPrint;
	m_nFrameLen[0]=0;
	m_nFrameLen[1]=0;
	m_pTemporal=0;
	SAFE_DELETE(m_pCamaras[0]);
	SAFE_DELETE(m_pCamaras[1]);
}

void CSVEView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ECODIGO, m_ECodigo);
	DDX_Control(pDX, IDC_ECEDULA, m_EDocumento);
	DDX_Control(pDX, IDC_ENOMBRE, m_ENombre);
	DDX_Control(pDX, IDC_EAPELLIDO, m_EApellido);
	DDX_Control(pDX, IDC_EFECHA, m_EFecha);
	DDX_Control(pDX, IDC_CAMARAS, m_CBCamara1);
	DDX_Control(pDX, IDC_CAMARAS2, m_CBCamara2);
	DDX_Control(pDX, IDC_BCAPTURAR, m_BFoto1);
	DDX_Control(pDX, IDC_BCAPTURAR2, m_BFoto2);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_BUTTON3, m_BCancelar);
	DDX_Control(pDX, IDC_SFOTO3, m_SFoto1);
	DDX_Control(pDX, IDC_SFOTO4, m_SFoto2);
	DDX_Control(pDX, IDC_SFOTO6, m_SFoto3);
	DDX_Control(pDX, IDC_SHUELLA, m_SHuella);
	DDX_Control(pDX, IDC_CAMARAS3, m_CBCamara3);
	DDX_Control(pDX, IDC_BCAPTURAR3, m_BCapturar3);
	DDX_Control(pDX, IDC_BAGREGAR, m_BAgregar);
	DDX_Control(pDX, IDC_LMULTIPLE, m_LCMultiple);
	DDX_Control(pDX, IDC_SLOGO, m_SLogo);
	DDX_Control(pDX, IDC_STEMP, m_STemp);
	DDX_Control(pDX, IDC_SMENSAJE, m_SMensaje);
}

BOOL CSVEView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CSVEView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	GetDocument()->SetTitle("Entrega");

	CWaitCursor wait;

	m_pConector= (CConector *) AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);
	m_pUsuario= (CUsuario *) AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO, 0, (LPARAM)this);

	m_nAncho= AfxGetApp()->GetProfileInt("bitmap", "ancho", 320);
	m_nAlto= AfxGetApp()->GetProfileInt("bitmap", "alto", 240);

	m_SLogo.SetBitmap(CApplicacion::GetBitmap());

	m_LCMultiple.SetMessageLista("No hay códigos");
	m_LCMultiple.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_LCMultiple.InsertColumn(0, "Código");
	m_LCMultiple.AutoSizeColumns();

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

	m_nCamaras= CVMR_Capture::EnumDevices(m_CBCamara1);
	CVMR_Capture::EnumDevices(m_CBCamara2);
	CVMR_Capture::EnumDevices(m_CBCamara3);
	if(((CMainFrame *) AfxGetMainWnd())->GetDevicesCount()>0)
		m_CBCamara2.SetItemData(m_CBCamara2.AddString("Escáner"), -1);
	// todo: cambiar a uno y a dos
	if( m_nCamaras>2) {
		m_CBCamara1.SetCurSel(AfxGetApp()->GetProfileInt("camaras","uno", 0));
		if(AfxGetApp()->GetProfileInt("camaras","dos", 1)==-1)
			m_CBCamara2.SelectedData=-1;
        else m_CBCamara2.SetCurSel(AfxGetApp()->GetProfileInt("camaras","dos", 1));
		m_CBCamara3.SetCurSel(AfxGetApp()->GetProfileInt("camaras","tres", 2));
		OnCbnSelendokCamaras();
		OnCbnSelendokCamaras2();
		OnCbnSelendokCamaras3();
	} else if(m_nCamaras>1) {
		m_CBCamara1.SetCurSel(AfxGetApp()->GetProfileInt("camaras","uno", 0));
		if(AfxGetApp()->GetProfileInt("camaras","dos", 1)==-1)
			m_CBCamara2.SelectedData=-1;
		else m_CBCamara2.SetCurSel(AfxGetApp()->GetProfileInt("camaras","dos", 1));
		OnCbnSelendokCamaras();
		OnCbnSelendokCamaras2();
	} else if( m_nCamaras>0) {
		m_CBCamara1.SetCurSel(AfxGetApp()->GetProfileInt("camaras","uno", 0));
		OnCbnSelendokCamaras();
		if(AfxGetApp()->GetProfileInt("camaras","dos", 1)==-1) {
			m_CBCamara2.SelectedData=-1;
			OnCbnSelendokCamaras2();
		}
	}
	Clean(true);

	if( m_nCamaras<=0) {
		MessageBox("Por favor conecte al menos una camara.\rY Reinicie SVE.", NULL, MB_ICONERROR);
		m_BAceptar.EnableWindow(false);
	}

	m_SHuella.HideInfo();
	m_SHuella.SetDefault();
	SetTimer(2, 1000, NULL);

	m_ECodigo.SetLimitText(50);
	m_EDocumento.SetLimitText(19);

	m_pSplash->Hide();
	m_pSplash=NULL;
}


// Diagnósticos de CSVEView

#ifdef _DEBUG
void CSVEView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSVEView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

#endif //_DEBUG


// Controladores de mensaje de CSVEView

bool CSVEView::CopyTo(int camara) {

	if(camara>1)
		return false;
	if(m_CBCamara2.SelectedIndex!=-1 && m_CBCamara2.SelectedData==-1) {

		return ScanImage(camara);
	} else {
		CVMR_Capture *info=NULL;
		if( camara==1 && m_VMRCap2) {
			info= m_VMRCap2;
		} else {
			info= m_VMRCap;
		}
		if( info) {
			SAFE_DELETE(m_pCamaras[camara]);
			m_nFrameLen[camara]= info->GrabFrame();
			if(m_nFrameLen[camara]>0)
			{
				m_pCamaras[camara]= new BYTE[m_nFrameLen[camara]];
				BYTE *pImage;
				info->GetFrame(&pImage);
				memcpy(m_pCamaras[camara], pImage, m_nFrameLen[camara]);
				camara==1 ? m_SFoto3.ShowImage(m_pCamaras[camara]) : m_SFoto2.ShowImage(m_pCamaras[camara]);
			}
			return true;
		}
	}
	return false;
}

void CSVEView::OnBnClickedBcapturar()
{
	CWaitCursor wait;
	if(m_VMRCap) 
		Pause(1,!m_VMRCap->IsPaused());
}

void CSVEView::OnBnClickedBcapturar2()
{
	CWaitCursor wait;
	if(m_VMRCap2) {
		Pause(2,!m_VMRCap2->IsPaused());
	} else CopyTo(0);
}

void CSVEView::OnBnClickedOk()
{
	CWaitCursor wait;

	CStringArray valores, campos;
	CString temp,archivo;
	CStringArray entregas;
	CString codigo;
	CString documento;
	
	m_ECodigo.GetWindowText(codigo);
	if( codigo.IsEmpty() && !m_LCMultiple.GetItemCount()) {
		SetErrorEx(&m_ECodigo, "Debe Ingresar una guia", 2, "sveview");
		return;
	}
	// codigo
	valores.Add(codigo);

	m_EDocumento.GetWindowText(documento);
	if( documento.IsEmpty()) {
		SetErrorEx(&m_EDocumento, "Debe ingresar un documento", 2, "sveview");
		return;
	}
	// documento
	valores.Add(documento);
	valores.Add("SYSDATE()");
	if(!codigo.IsEmpty())
		entregas.Add(CQuery::CreateInsert("entrega", valores));
	for( int i=0; i<m_LCMultiple.GetItemCount(); i++) {
		valores.SetAt(0, m_LCMultiple.GetItemText(i, 0));
        entregas.Add(CQuery::CreateInsert("entrega", valores));
	}
	valores.RemoveAll();
	// fin entrega
	archivo=documento;

	// documento
	if(!m_bExiste)
		valores.Add(documento);

	m_ENombre.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_ENombre, "Debe ingresar un nombre", 2, "sveview");
		return;
	}
	valores.Add(temp);

	m_EApellido.GetWindowText(temp);
	if( temp.IsEmpty()) {
		SetErrorEx(&m_EApellido, "Debe ingresar un apellido", 2, "sveview");
		return;
	}
	valores.Add(temp);

	if(m_nCamaras==1) {
		if( m_pCamaras[0]==NULL) {
			CopyTo(0);
			SetError(NULL, "Esperando otra foto.");
			return;
		} else if( m_pCamaras[1]==NULL) {
			CopyTo(1);
			SetError(NULL, "Esperando otra foto.");
			return;
		}
	} else if( m_nCamaras==2) {
		if( m_pCamaras[1]==NULL) {
			CopyTo(1);
			SetError(NULL, "Esperando otra foto.");
			return;
		}
	}

	if( !m_SHuella.HasHuella()) {
		SetErrorEx(NULL, "Por favor coloque la huella", 2, "sveview");
		return;
	}

	// entrega
	for( int i=0; i<entregas.GetCount(); i++) {
		m_pConector->Query(entregas.GetAt(i), 0, this);
	}
	// usuario
	if(m_bExiste) {
		campos.Add("nombre");
		campos.Add("apellido");
		m_pConector->Query(CQuery::CreateUpdate("usuario",campos, valores, "documento=" + documento), 0, this);
	} else m_pConector->Query(CQuery::CreateInsert("usuario", valores), 0, this);
	// propiedades
	valores.RemoveAll();
	valores.Add(codigo);
	CString usuario;
	usuario.Format("%d", m_pUsuario->GetID());
	valores.Add(usuario);
	valores.Add("SYSDATE()");

	if(!codigo.IsEmpty())
		m_pConector->Query(CQuery::CreateInsert("propiedades", valores), 0, this);

	for( int i=0; i<m_LCMultiple.GetItemCount(); i++) {
		valores.SetAt(0, m_LCMultiple.GetItemText(i, 0));
		m_pConector->Query(CQuery::CreateInsert("propiedades", valores), 0, this);
	}

	// inicio con las camaras
	Pause(1, true);
	
	CString query;
	int len=0;

	unsigned char *imagen=NULL;

	if(m_VMRCap && (len=m_VMRCap->GrabFrame())) {
		valores.RemoveAll();

		valores.Add(documento);
		valores.Add("%0%");
		CString app;
		imagen= m_SFoto1.GetCompress(len, AfxGetApp()->GetProfileInt("local", "calidad", 75));
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
		if(!codigo.IsEmpty()) {
			valores.SetAt(0, codigo);
			query= CQuery::CreateInsert("fotoe1", valores);
			query.Replace("%0%", app);
			m_pConector->Query(query, 0 , this);
		}

		for( int i=0; i<m_LCMultiple.GetItemCount(); i++) {
			
			valores.SetAt(0, m_LCMultiple.GetItemText(i, 0));
			query= CQuery::CreateInsert("fotoe1", valores);
			query.Replace("%0%", app);
			m_pConector->Query(query, 0 , this);
		}
	}

	if(imagen) {
		delete []imagen;
		imagen=NULL;
	}

	Pause(2, true);

	if((m_VMRCap2 && (len=m_VMRCap2->GrabFrame())) || (len=m_nFrameLen[0])) {
		
		valores.RemoveAll();
		valores.Add(documento);
		valores.Add("%0%");
		CString app;
		if(m_CBCamara2.SelectedIndex!=-1 && m_CBCamara2.SelectedData==-1) 
			imagen= m_pCamaras[0];
		else imagen=  m_SFoto2.GetCompress(len, AfxGetApp()->GetProfileInt("local", "calidad", 75));
		
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

		if(!codigo.IsEmpty()) {
			valores.SetAt(0, codigo);
			query= CQuery::CreateInsert("fotoe2", valores);
			query.Replace("%0%", app);
			m_pConector->Query(query, 0 , this);
		}

		for( int i=0; i<m_LCMultiple.GetItemCount(); i++) {

			valores.SetAt(0, m_LCMultiple.GetItemText(i, 0));
			query= CQuery::CreateInsert("fotoe2", valores);
			query.Replace("%0%", app);
			m_pConector->Query(query, 0 , this);
		}
		if(m_CBCamara2.SelectedIndex!=-1 && m_CBCamara2.SelectedData==-1) 
			imagen= NULL;
	}

	Pause(3, true);

	if(imagen) {
		delete []imagen;
		imagen=NULL;
	}

	if(m_VMRCap3 && (len=m_VMRCap3->GrabFrame()) || (len=m_nFrameLen[1])) {
		valores.RemoveAll();
		valores.Add(documento);
		valores.Add("%0%");
		CString app;
		if(m_CBCamara2.SelectedIndex!=-1 && m_CBCamara2.SelectedData==-1) 
			imagen= m_pCamaras[1];
		else imagen=  m_SFoto3.GetCompress(len, AfxGetApp()->GetProfileInt("local", "calidad", 75));

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
		if(!codigo.IsEmpty()) {
			valores.SetAt(0, codigo);
			query= CQuery::CreateInsert("fotoe3", valores);
			query.Replace("%0%", app);
			m_pConector->Query(query, 0 , this);
		}

		for( int i=0; i<m_LCMultiple.GetItemCount(); i++) {

			valores.SetAt(0, m_LCMultiple.GetItemText(i, 0));
			query= CQuery::CreateInsert("fotoe3", valores);
			query.Replace("%0%", app);
			m_pConector->Query(query, 0 , this);
		}
		if(m_CBCamara2.SelectedIndex!=-1 && m_CBCamara2.SelectedData==-1) 
			imagen= NULL;
	}
	if(imagen) {
		delete []imagen;
		imagen=NULL;
	}

	if(m_SHuella.HasHuella()) {

		valores.RemoveAll();
		if( !m_bExiste)
			valores.Add(documento);
		// ancho
		CFingerMessage *huella= m_SHuella.GetHuella();
		ASSERT(huella);

		temp.Format("%d",  huella->GetAncho());
		valores.Add(temp);
		// alto
		temp.Format("%d", huella->GetAlto());
		valores.Add(temp);
		// resolucion
		temp.Format("%d", huella->GetResolucion());
		valores.Add(temp);
		// huella
		valores.Add("%0%");
		CString app;
		BYTE *buffer= huella->GetRaw();
		for( int i=0; i<huella->GetSize(); i++) {
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
		}
		if( m_bExiste) {
			campos.RemoveAll();
			campos.Add("ancho");
			campos.Add("alto");
			campos.Add("resolucion");
			campos.Add("huella");
            query= CQuery::CreateUpdate("huella", campos, valores,"documento=" + documento); 
		} else query= CQuery::CreateInsert("huella", valores);
		if(AfxGetApp()->GetProfileInt("control", "compress", 1))
			query.Replace("\"%0%\"", "COMPRESS(\"%0%\")");
		query.Replace("%0%", app);
		m_pConector->Query(query, 0 , this);

		// vectores de la huella
		valores.RemoveAll();
		campos.RemoveAll();

		campos.Add("documento");
		valores.Add(documento);
		// huella
		campos.Add("huella");
		valores.Add("%0%");
		app.Empty();
		buffer= huella->GetTemplate();
		for( int i=0; i<huella->GetTamanoTemplate(); i++) {
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
		}
		temp= CQuery::CreateInsert("template", campos, valores);
		temp.Replace("INSERT INTO", "INSERT IGNORE INTO");
		temp.Replace("%0%", app);
		m_pConector->Query(temp, 0 , this);
		temp.Empty();

		temp= CQuery::CreateUpdate("template", campos, valores, "documento=" + documento);
		temp.Replace("%0%", app);
		m_pConector->Query(temp, 0 , this);
	}

	m_szOld="";
	m_szOldCodigo="";
	m_ECodigo.SetWindowText("");
	m_LCMultiple.DeleteAllItems();
	OnBnClickedButton3();
}

// on button cancelar.
void CSVEView::OnBnClickedButton3()
{
	m_EDocumento.SetWindowText("");
	m_ECodigo.SetWindowText("");
	
	Clean();

	m_szOld="";
	m_szOldCodigo="";
	SetError(&m_ECodigo, "Listo");
}

void CSVEView::OnEnKillfocusEcodigo()
{
	CString temp;
	m_ECodigo.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szOldCodigo)
		return;
	m_szOldCodigo=temp;
	temp.Format("SELECT * FROM entrega WHERE codigo='%s'", m_szOldCodigo);
	if(m_bIngreso)
		Clean();
	m_szOld="";
	m_pConector->Query(temp,CSVEView::findentrega, this);
}

void CSVEView::OnEnKillfocusEcedula()
{
	CString temp;
	m_EDocumento.GetWindowText(temp);
	if( temp.IsEmpty() || temp==m_szOld)
		return;
	if( !m_BAceptar.IsWindowEnabled())
		return;
	m_szOld=temp;
	m_bUsuario=false;
	temp.Format("SELECT nombre,apellido FROM usuario WHERE documento='%s'", m_szOld);
	m_EDocumento.SetWindowText(m_szOld);
	m_pConector->Query(temp, CSVEView::findcedula, this);
}

void CSVEView::Pause(int camara, bool pausa) {

	CVMR_Capture *info=NULL;
	CShowpic *foto=NULL;
	CButtonXP *boton=NULL;
	KillTimer(1);

	switch( camara) {
		case 1:
			info= m_VMRCap;
			foto= &m_SFoto1;
			boton= &m_BFoto1;
			break;
		case 2:
			info= m_VMRCap2;
			foto= &m_SFoto2;
			boton= &m_BFoto2;
			break;
		case 3:
			info= m_VMRCap3;
			foto= &m_SFoto3;
			boton= &m_BCapturar3;
			break;
		default:
			return;
	}
	if(info) {
		if( pausa) {
			KillTimer(4+camara);
			if( !info->IsPaused()) {
				info->SetPause(true);
				DWORD dwSize;
				dwSize=info->GrabFrame();
				if(dwSize>0)
				{
					BYTE *pImage;
					info->GetFrame(&pImage);
					foto->ShowImage(pImage);
				}
			}
		} else if(info->IsPaused()) {
			if(AfxGetApp()->GetProfileInt("local", "reiniciar", 0)) {
				switch(camara) {
					case 1:
						OnCbnSelendokCamaras();
						break;
					case 2:
						OnCbnSelendokCamaras2();
						break;
					case 3:
						OnCbnSelendokCamaras3();
						break;
				}
			} else info->SetPause(false);
			SetTimer(4+camara, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		}
		CString texto= info->IsPaused() ? "Reanudar" : "Capturar";
		texto.Insert(camara-1, "&");
		boton->SetWindowText(texto);
	}
}

void CSVEView::Clean(bool search) {
	

	m_EDocumento.SetWindowText("");
	m_ENombre.SetWindowText("");
	m_EApellido.SetWindowText("");


	m_bIngreso=false;

	m_bExiste=false;

	m_nFrameLen[0]=0;
	m_nFrameLen[1]=0;
	SAFE_DELETE(m_pCamaras[0]);
	SAFE_DELETE(m_pCamaras[1]);

	m_SFoto1.SetWindowText("");
	m_SFoto2.SetWindowText("");
	m_SFoto3.SetWindowText("");

	m_SFoto1.SetTitulo();
	m_SFoto2.SetTitulo();
	m_SFoto3.SetTitulo();

	if(!search) {

		m_SFoto1.ShowImage(NULL);
		m_SFoto2.ShowImage(NULL);
		m_SFoto3.ShowImage(NULL);

		Pause(1, false);
		Pause(2, false);
		Pause(3, false);

	}

	m_BFoto1.EnableWindow();
	m_BFoto2.EnableWindow();
	m_BCapturar3.EnableWindow();

	m_SHuella.Clean();
	m_SHuella.SetWindowText(m_EDocumento.Text.IsEmpty() ? "Buscar" : "");
	m_SHuella.Invalidate();

	m_SMensaje.SetWindowText("");
	m_BAgregar.EnableWindow();

	KillTimer(2);
	SetTimer(2, 1000,NULL);
	m_BAceptar.EnableWindow(m_nCamaras>0);
	m_EApellido.SetReadOnly(false);
	m_ENombre.SetReadOnly(false);
	m_EDocumento.SetReadOnly(false);
}

void CSVEView::Refresh(int camara) {

	CVMR_Capture *info=NULL;
	CShowpic *foto= NULL;
	switch(camara) {
		case 0:
			info= m_VMRCap;
			foto= &m_SFoto1;
			break;
		case 1:
			info= m_VMRCap2;
			foto= &m_SFoto2;
			break;
		case 2:
			info= m_VMRCap3;
			foto= &m_SFoto3;
			break;
	}
	if( info && info->GrabFrame()) {
		BYTE *pImage;
		info->GetFrame(&pImage);
		foto->ShowImage(pImage);
	}
}

void CSVEView::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==2) {
		CTime *tiempo= (CTime *)AfxGetMainWnd()->SendMessage(WM_QUERYTIME);
		CMyTime tiempof(tiempo);
		DWORD sel= m_EFecha.GetSel();
		m_EFecha.SetWindowText(tiempof.GetFechaHora());
		m_EFecha.SetSel(sel);
	} else if( nIDEvent==5 || nIDEvent==6 || nIDEvent==7) {
		Refresh(nIDEvent-5);
	}
	CFormView::OnTimer(nIDEvent);
}

void CSVEView::OnDestroy()
{
	OnBnClickedButton3();
	KillTimer(5);
	KillTimer(6);
	KillTimer(7);
	SAFE_DELETE(m_VMRCap);
	SAFE_DELETE(m_VMRCap2);
	SAFE_DELETE(m_VMRCap3);
	CFormView::OnDestroy();
}

LRESULT CSVEView::OnFingerPrint(WPARAM wParam, LPARAM lParam) {

	if(wParam==GR_FINGER_UP) {
		if( m_BAceptar.IsWindowEnabled()) {
			if(m_EDocumento.Text.IsEmpty()) {
				// buscar
				m_EDocumento.SetWindowText("Buscando");
				m_EDocumento.SetReadOnly(true);
				CFingerMessage *mess=(CFingerMessage *)lParam;
				mess->Prepare();
				m_pTemporal= GetTickCount();
				m_pConector->QueryBinary("SELECT huella,documento FROM template", query_huellasearch, this);
			} 
			m_SHuella.SetHuella(lParam);
		}
	}
	return 0;
}

LRESULT CSVEView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;

	switch( wParam) {
		case THREAD_END:
			if(mess->GetID()==query_foto1) {
				m_SFoto1.SetWindowText(m_SFoto1.HasImage() ? "" : "No hay imagen");
				m_ECodigo.Step();
			} else if(mess->GetID()==query_foto2) {
				m_SFoto2.SetWindowText(m_SFoto2.HasImage() ? "" : "No hay imagen");
				m_ECodigo.Step();
			} else if( mess->GetID()==query_foto3) {
				m_SFoto3.SetWindowText(m_SFoto3.HasImage() ? "" : "No hay imagen");
				m_ECodigo.Step();
			} else if( mess->GetID()==query_huella) {
				m_SHuella.SetWindowText(m_SHuella.HasHuella() ? "" : "No hay huella");
				m_SHuella.Invalidate();
				m_ECodigo.Step();
				m_ECodigo.HideProgress();
			} else if(mess->GetID()==query_huellasearch) {
				m_EDocumento.SetReadOnly(false);
				if(!m_bExiste) {
					CString resultado;
					resultado.Format("Huella no encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
					m_SMensaje.SetWindowText(resultado);
					SetError(&m_EDocumento, resultado);
					m_EDocumento.Text="";
				}
				m_EDocumento.SetPos(m_EDocumento.GetMaximo());
			}
			break;
		case QUERY_COUNT2:
			if(mess->GetID()==query_huellasearch) {
				m_EDocumento.SetRange(0, mess->GetCount());
			}
			break;
		case QUERY_EMPTY:
			if( mess->GetID()==CSVEView::findentrega) {

				m_bIngreso=false;
				m_szOld="";
			} else if( mess->GetID()==CSVEView::findcedula) {

				m_bUsuario=false;
				m_bExiste=false;
				m_ENombre.SetWindowText("");
				m_EApellido.SetWindowText("");
				m_ENombre.SetReadOnly(false);
				m_EApellido.SetReadOnly(false);
			}
			break;
		case QUERY_RESULT:
			if( mess->GetID()==query_foto1) {
				m_SFoto1.SetBitmap(*mess->GetBinaryData());
			} else if( mess->GetID()==query_foto2) {
				m_SFoto2.SetBitmap(*mess->GetBinaryData());
			} else if( mess->GetID()==query_foto3) {
				m_SFoto3.SetBitmap(*mess->GetBinaryData());
			} else if( mess->GetID()==query_huella) {
				m_SHuella.SetHuella(mess->GetBinaryData());
			}else if( mess->GetID()==CSVEView::findcedula) {
				
				CWaitCursor wait;
				m_ENombre.SetWindowText(mess->GetAt(0));
				m_EApellido.SetWindowText(mess->GetAt(1));
				m_ENombre.SetReadOnly();
				m_EApellido.SetReadOnly();
				m_bExiste=true;
				m_bUsuario=true;

			} else if(mess->GetID()==CSVEView::findentrega) {
				
				if(m_LCMultiple.GetItemCount()) {
					if( MessageBox("¿Esta guia ya fue entregado desea ver el registro?", "SVE" , MB_ICONQUESTION | MB_YESNO)==IDNO) {
						m_ECodigo.SetWindowText("");
						m_szOldCodigo.Empty();
						m_ECodigo.SetFocus();
						return 0;
					}
				}
				m_ECodigo.SetRange(0, 5);
				m_ECodigo.Step();

				CWaitCursor wait;

				m_bIngreso=true;

				AfxGetMainWnd()->SendMessage(WM_INSERTODS, 10, (LPARAM)(const char *)mess->GetAt(0));
				m_EDocumento.SetWindowText(mess->GetAt(1));
				OnEnKillfocusEcedula();

				Pause(1, true);
				Pause(2, true);
				Pause(3, true);

				KillTimer(5);
				KillTimer(6);
				KillTimer(7);

				m_EApellido.SetReadOnly();
				m_ENombre.SetReadOnly();
				m_EDocumento.SetReadOnly();
				m_BAceptar.EnableWindow(false);
				m_BAgregar.EnableWindow(false);
				m_LCMultiple.DeleteAllItems();

				m_BFoto1.EnableWindow(false);
				m_BFoto2.EnableWindow(false);
				m_BCapturar3.EnableWindow(false);

				m_SFoto1.SetBitmap(NULL);
				m_SFoto2.SetBitmap(NULL);
				m_SFoto3.SetBitmap(NULL);
				m_SHuella.Clean();
				m_SHuella.SetBitmap((HBITMAP)NULL);

				m_SFoto1.SetWindowText("Cargando");
				m_SFoto2.SetWindowText("Cargando");
				m_SFoto3.SetWindowText("Cargando");
				m_SHuella.SetWindowText("Cargando");

				m_SFoto1.SetTitulo("Entrega" + mess->GetAt(0)+ "-Foto1");
				m_SFoto2.SetTitulo("Entrega" + mess->GetAt(0)+ "-Foto2");
				m_SFoto3.SetTitulo("Entrega" + mess->GetAt(0)+ "-Foto3");

				CMyTime tiempo(mess->GetAt(2));

				KillTimer(2);
				m_EFecha.SetWindowText(tiempo.GetFechaHora());
				CString texto;
				for( int i=1; i<4; i++) {
					// solo por compatibilidad con la version 1.3 e inferiores
					texto.Format("SELECT foto FROM foto%d WHERE documento='%s'", i, mess->GetAt(1));
					m_pConector->QueryBinary(texto, query_foto1+(i-1), this);
					// nuevo en la version 1.4
					texto.Format("SELECT foto FROM fotoe%d WHERE codigo='%s'", i, mess->GetAt(0));
					m_pConector->QueryBinary(texto, query_foto1+(i-1), this);
				}
				m_pConector->Query("SELECT ancho,alto,resolucion FROM huella WHERE documento=" + mess->GetAt(1), datos_huella, this);
				if(AfxGetApp()->GetProfileInt("control", "compress", 1))
					m_pConector->QueryBinary("SELECT UNCOMPRESS(huella) FROM huella WHERE documento=" + mess->GetAt(1), query_huella, this);
				else m_pConector->QueryBinary("SELECT huella FROM huella WHERE documento=" + mess->GetAt(1), query_huella, this);
			} else if( mess->GetID()==datos_huella) {
				m_SHuella.SetParametros(atoi(mess->GetAt(1)),
					atoi(mess->GetAt(0)),
					atoi(mess->GetAt(2)));
			} else if(mess->GetID()==query_huellasearch) {
				m_EDocumento.Step();
				CFingerMessage *huella= m_SHuella.GetHuella();
				if(huella) {
					int score=0;
					if(huella->Compare((const char *)mess->GetBinaryData()->GetData(), &score)) {

						CString resultado;
						resultado.Format("Huella encontrada en %g segundos",(double)(GetTickCount()-m_pTemporal)/1000);
						if(m_pConector)
							m_pConector->KillQuery(query_huellasearch, this);
						SetError(&m_EDocumento, resultado);
						m_SMensaje.SetWindowText(resultado);
						CString texto= mess->GetAt(0);
						m_EDocumento.SetWindowText(texto);
						m_EDocumento.SetReadOnly(false);
						m_EDocumento.SetPos(m_EDocumento.GetMaximo());
						OnEnKillfocusEcedula();
						m_bExiste=true;
					}
				}
			}
			break;
	}
	return 0;
}

void CSVEView::OnCbnSelendokCamaras()
{
	CWaitCursor wait;
	if(m_pSplash) {
		CString szTemp;
		szTemp.Format("SVE Versión %s\r\nIniciando cámara 1",SVE_VERSION);
		m_pSplash->SetText(szTemp);
	}
	SetError(NULL, "Iniciando cámara 1");
	KillTimer(5);
	SAFE_DELETE(m_VMRCap);
	if( m_CBCamara1.GetCurSel()!=-1) {
		m_VMRCap= new CVMR_Capture();
		if(SUCCEEDED(m_VMRCap->Init(m_CBCamara1.GetCurSel(),m_STemp, m_nAncho, m_nAlto))) {
			AfxGetApp()->WriteProfileInt("camaras","uno", m_CBCamara1.GetCurSel());
			SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		} else {
			delete m_VMRCap;
			m_VMRCap=NULL;
			SetErrorEx(&m_CBCamara1, "No se puede iniciar cámara 1", 3,"Camaras");
		}
	}
}

void CSVEView::OnCbnSelendokCamaras2()
{
	CWaitCursor wait;
	if(m_pSplash && m_CBCamara2.SelectedIndex!=-1) {
		CString szTemp;
		szTemp.Format("SVE Versión %s\r\nIniciando %s",SVE_VERSION, m_CBCamara2.SelectedData==-1 ? "escáner" : "cámara 2");
		m_pSplash->SetText(szTemp);
	}
	SetError(NULL, m_CBCamara2.SelectedData==-1 ? "Iniciando escáner" : "Iniciando cámara 2");
	SAFE_DELETE(m_VMRCap2);
	if(m_CBCamara2.SelectedIndex==-1)
		return;
	if( m_CBCamara2.SelectedData!=-1) {
		m_VMRCap2= new CVMR_Capture();
		if(SUCCEEDED(m_VMRCap2->Init(m_CBCamara2.GetCurSel(),m_STemp, m_nAncho, m_nAlto))) {
			AfxGetApp()->WriteProfileInt("camaras","dos", m_CBCamara2.GetCurSel());
			SetTimer(6, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		} else {
			AfxGetApp()->WriteProfileInt("camaras","dos", m_CBCamara2.GetCurSel());
			delete m_VMRCap2;
			m_VMRCap2=NULL;
			SetErrorEx(&m_CBCamara2, "No se puede iniciar cámara 2", 3, "Camaras");
		}
		m_CBCamara3.EnableWindow(true);
	} else {
		m_CBCamara3.EnableWindow(false);
		/*if(!m_pSplash) {
			CString texto;
			m_pItemRoot= WiaWrap::SelectDevice(*this, &texto
#ifdef _DEBUG
				,StiDeviceTypeDefault
#endif
				);
		}*/
		AfxGetApp()->WriteProfileInt("camaras","dos", -1);
	}
}

LRESULT CSVEView::OnInsertODS(WPARAM wParam, LPARAM lParam) {
	
	if(wParam==0) {
		CString texto;
		texto= (const char *)lParam;
		m_szOld="";
		m_szOldCodigo="";
		m_ECodigo.SetWindowText(texto);
		OnEnKillfocusEcodigo();
	}
	return 0;
}

HBITMAP CSVEView::GetBitmap(HBITMAP bmp) {

	CEnBitmap enbmp;
	VERIFY(enbmp.CopyImage(bmp));
	enbmp.ResizeImage((int)AfxGetApp()->GetProfileInt("fotos", "cx", 320), (int)AfxGetApp()->GetProfileInt("fotos", "cy", 240));
	return (HBITMAP)enbmp.Detach();
}

bool CSVEView::InitRTF() {

	if(m_szOldCodigo.IsEmpty())
		return false;

	CRect rect;
	GetClientRect(rect);
	
	if(!::IsWindow(m_pRTF)) {
		DWORD dwStyle = WS_CHILD|WS_BORDER|WS_VSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN| ES_READONLY;
		VERIFY(m_pRTF.Create(dwStyle, rect, this, 0x1245 ));
	}

	CString temp;

	m_ECodigo.GetWindowText(temp);
	m_pRTF.SetVariable("{$guia}", temp);
	m_EDocumento.GetWindowText(temp);
	m_pRTF.SetVariable("{$documento}", temp);
	m_ENombre.GetWindowText(temp);
	m_pRTF.SetVariable("{$nombre}", temp);
	m_EApellido.GetWindowText(temp);
	m_pRTF.SetVariable("{$apellido}", temp);
	m_EFecha.GetWindowText(temp);
	m_pRTF.SetVariable("{$fecha}", temp);
	m_pRTF.SetVariable("{$usuario}", m_pUsuario->GetNombre());
	CMyTime tiempo((CTime *)AfxGetMainWnd()->SendMessage(WM_QUERYTIME));
	m_pRTF.SetVariable("{$system.date}", tiempo.GetFechaHora());

	m_pRTF.SetBitmap("{$foto1}", GetBitmap(m_SFoto1.m_hBitmap));
	m_pRTF.SetBitmap("{$foto2}", GetBitmap(m_SFoto2.m_hBitmap));
	m_pRTF.SetBitmap("{$foto3}", GetBitmap(m_SFoto3.m_hBitmap));
	m_pRTF.SetBitmap("{$huella}", GetBitmap(m_SHuella.m_hBitmap));

	VERIFY(m_pRTF.LoadFromFile(CApplicacion::GetAppPath( "extras\\entrega.rtf")));

	return true;
}

void CSVEView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnBeginPrinting(pDC, pInfo);
}

void CSVEView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnPrint(pDC, pInfo);
}

BOOL CSVEView::OnPreparePrinting(CPrintInfo* pInfo)
{
	InitRTF();
	if( m_pPrint)
		delete m_pPrint;
	m_pPrint= new CPrintRTF();

	m_pPrint->SetRTF(&m_pRTF);
	m_pPrint->SetAppName("SVE");
	m_pPrint->SetDocTitle(GetDocument()->GetTitle());

	m_pPrint->SetFooterIzq(m_pUsuario->GetNombre());

	CMyTime tiempo(true);

	m_pPrint->SetFooter(tiempo.GetFechaHora(AfxGetApp()->GetProfileInt("fechas", "larga", 0)));

	m_pPrint->OnPreparePrinting(pInfo);

	m_pPrint->SetMarginBottomPix(AfxGetApp()->GetProfileInt("printer", "cy", 20));
	m_pPrint->SetMarginLeftPix(AfxGetApp()->GetProfileInt("printer", "x", 20));
	m_pPrint->SetMarginRightPix(AfxGetApp()->GetProfileInt("printer", "cx", 20));
	m_pPrint->SetMarginTopPix(AfxGetApp()->GetProfileInt("printer", "y", 20));

	return DoPreparePrinting(pInfo);
}

void CSVEView::OnBnClickedBcapturar3()
{
	CWaitCursor wait;
	if(m_VMRCap3) 
		Pause(3,!m_VMRCap3->IsPaused());
	else CopyTo(1);
}

void CSVEView::OnCbnSelendokCamaras3()
{
	CWaitCursor wait;
	if(m_pSplash) {
		CString szTemp;
		szTemp.Format("SVE Versión %s\r\nIniciando cámara 3",SVE_VERSION);
		m_pSplash->SetText(szTemp);
	}
	SetError(NULL, "Iniciando cámara 3");
	SAFE_DELETE(m_VMRCap3);
	if( m_CBCamara3.GetCurSel()!=-1) {
		m_VMRCap3= new CVMR_Capture();
		if(SUCCEEDED(m_VMRCap3->Init(m_CBCamara3.GetCurSel(),m_STemp, m_nAncho, m_nAlto))) {
			AfxGetApp()->WriteProfileInt("camaras","tres", m_CBCamara3.GetCurSel());
			SetTimer(7, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		} else {
			delete m_VMRCap3;
			m_VMRCap3=NULL;
			SetErrorEx(&m_CBCamara3, "No se puede iniciar cámara 3", 3, "Camaras");
		}		
	}
}

void CSVEView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pPrint->OnEndPrinting(pDC, pInfo);
	delete m_pPrint;
	m_pPrint= NULL;
}

void CSVEView::OnUpdatePrint(CCmdUI *pCmdUI) {

	if(m_pUsuario->Pertenece(1) || m_pUsuario->Pertenece(4))
		pCmdUI->Enable( !m_BAceptar.IsWindowEnabled());
	else pCmdUI->Enable(false);
}

void CSVEView::OnBnClickedBagregar()
{
	if(!m_BAgregar.IsWindowEnabled())
		return;
	CString temp;
	m_ECodigo.GetWindowText(temp);
	if( !temp.IsEmpty()) {
		for( int i=0; i<m_LCMultiple.GetItemCount(); i++) {
			if( m_LCMultiple.GetItemText(i,0)==temp) {
				SetErrorEx(&m_ECodigo, "Esta guia ya existe.", 2, "existente");
				return;
			}
		}
		m_LCMultiple.InsertItem(m_LCMultiple.GetItemCount(), temp);
	}
	m_ECodigo.SetWindowText("");
	m_ECodigo.SetFocus();
}

void CSVEView::OnNMDblclkLmultiple(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	OnEditItem();
	*pResult = 0;
}

void CSVEView::OnNMRclickLmultiple(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	CNewMenu menu;

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 315, "&Editar");
	menu.AppendMenu(MF_STRING, 316, "E&liminar");
	
	POINT point;
    GetCursorPos( &point );
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	*pResult = 0;
}

void CSVEView::OnLvnKeydownLmultiple(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	if( pLVKeyDow->wVKey==VK_DELETE) {	
		OnDeleteItem();
	}
	*pResult = 0;
}

void CSVEView::OnEditItem() {

	int index=m_LCMultiple.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( index!=-1) {
		m_ECodigo.SetWindowText(m_LCMultiple.GetItemText(index, 0));
		m_LCMultiple.DeleteItem(index);
		SetError(&m_ECodigo, "Listo");
	}
}

void CSVEView::OnDeleteItem() {
	int index= -1;
	while( (index= m_LCMultiple.GetNextItem(index, LVNI_ALL | LVNI_SELECTED))!=-1) {
		m_LCMultiple.DeleteItem(index);
		index=-1;
	}
}

void CSVEView::OnBAgregar()
{
	OnBnClickedBagregar();
}

void CSVEView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CSVEView::PreTranslateMessage(MSG* pMsg)
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

void CSVEView::OnGuardar() {

	CString temp;
	m_ECodigo.GetWindowText(temp);
	CFileDialog dlg( false, "rtf", GetDocument()->GetTitle() + " " + temp, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, "Archivo RTF(*.rtf)|*.rtf| Todos los archivos (*.*)|*.*||");
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

bool CSVEView::ScanImage(int camara) {

	bool settimer=false;
	if(m_VMRCap && !m_VMRCap->IsPaused()) {
		settimer= true;
		KillTimer(5);
	}
	AfxOleGetMessageFilter()->EnableNotRespondingDialog(FALSE);

	WiaWrap::CComPtrArray<IStream> ppStream;
	HRESULT hr = WiaWrap::WiaGetImage(
		*this,
		StiDeviceTypeScanner,
		0,
		( AfxGetApp()->GetProfileInt("escaner", "calidad", 0) ? 0 : WIA_INTENT_MINIMIZE_SIZE) | (AfxGetApp()->GetProfileInt("escaner", "color", 1) ? WIA_INTENT_IMAGE_TYPE_COLOR : WIA_INTENT_IMAGE_TYPE_GRAYSCALE),
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		&ppStream.Count(),
		&ppStream,
		AfxGetApp()->GetProfileInt("escaner", "showUI", false)
		);

	AfxOleGetMessageFilter()->EnableNotRespondingDialog();

	CDlgProgreso *dlg= CDlgProgreso::GetDlgActual();
	if (FAILED(hr)) 
	{
		if(dlg) 
			dlg->Cerrar();
		MessageBox("No se puede obtener una imagen del dispositivo.\r\nVerifique que el dispositivo este bien conectado.", CApplicacion::GetAppName(), MB_ICONHAND | MB_OK);
		if(settimer) 
			SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		return false;
	}
	int pos=0;
	if(ppStream.Count()>0)
	{
		if(dlg) {
			dlg->m_BCancelar.EnableWindow(false);
			dlg->SetWindowText("Procesando imagen");
			dlg->m_SMensaje.SetWindowText("Cargando Imagen");
			dlg->m_PCProgreso.SetPos(pos++);
		}
		CImage imagen;
		if(dlg) 
			dlg->m_PCProgreso.SetPos(pos++);
		if(FAILED(imagen.Load(ppStream[0]))) {
			if(dlg) 
				dlg->Cerrar();
			if(settimer) 
				SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
			return false;
		}
		CEnBitmap bmp;
		if(dlg) 
			dlg->m_PCProgreso.SetPos(pos++);		

		VERIFY(bmp.CopyImage(imagen));

		if(AfxGetApp()->GetProfileInt("escaner", "cortar", 1)) {
			
			if(dlg) 
				dlg->m_PCProgreso.SetPos(pos++);
			bmp.ProcessImage(&CCutPhoto());
			if(dlg) 
				dlg->m_PCProgreso.SetPos(pos++);
			bmp.ProcessImage(&CCutPhotoV());
		}
		if(AfxGetApp()->GetProfileInt("escaner", "orientacion", 1)) {
			if(dlg) 
				dlg->m_PCProgreso.SetPos(pos++);
			bmp.RotateImage(90);
		}
		if(dlg) 
			dlg->m_PCProgreso.SetPos(pos++);
		bmp.FlipImage(true, false);
		if(dlg) 
			dlg->m_PCProgreso.SetPos(pos++);
		imagen.Detach();
		imagen.Attach(bmp);

		if(dlg) {
			dlg->m_PCProgreso.SetPos(pos++);
			dlg->m_SMensaje.SetWindowText("Convirtiendo imagen");
		}
		CImagenStream pStream; 
		if(FAILED(imagen.Save(&pStream, ImageFormatJPEG))) {
			imagen.Detach();
			if(dlg) 
				dlg->Cerrar();
			if(settimer) 
				SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
			return false;
		}

		imagen.Detach();
		pos= 96;
		if(dlg) 
			dlg->m_PCProgreso.SetPos(pos++);
		if(dlg)
			dlg->m_SMensaje.SetWindowText("Copiando Imagen");
		SAFE_DELETE(m_pCamaras[camara]);
		if(dlg)
			dlg->m_PCProgreso.SetPos(pos++);
		m_nFrameLen[camara]= pStream.GetSize();
		if(dlg)
			dlg->m_PCProgreso.SetPos(pos++);
		m_pCamaras[camara]= pStream.Copy();
		if(dlg)
			dlg->m_PCProgreso.SetPos(pos++);
		if(camara) {
			m_SFoto3.SetBitmap(*pStream.GetArray());
			m_SFoto3.Invalidate();
#ifdef _DEBUG
			m_SFoto2.SetTitulo("algo1");
#endif
		} else {
			m_SFoto2.SetBitmap(*pStream.GetArray());
			m_SFoto2.Invalidate();
#ifdef _DEBUG
			m_SFoto2.SetTitulo("algo");
#endif
		}
		if(dlg)
			dlg->m_PCProgreso.SetPos(pos++);
		if(dlg) 
			dlg->Cerrar();
		if(settimer) 
			SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
		return true;
	}
	if(dlg) 
		dlg->Cerrar();
	if(settimer) 
		SetTimer(5, AfxGetApp()->GetProfileInt("camaras","refresh", 100), NULL);
	return false;
}

void CSVEView::OnEnChangeEcedula()
{
	m_SHuella.SetWindowText(m_EDocumento.Text.IsEmpty() ? "Buscar" : "");
	m_SHuella.Invalidate();
}
