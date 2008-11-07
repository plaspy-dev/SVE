// InformesView.cpp: archivo de implementación
//

#include "stdafx.h"
#include "SVE.h"
#include "InformesView.h"

// CInformesView

IMPLEMENT_DYNCREATE(CInformesView, CFormView)

CInformesView::CInformesView()
	: CFormView(CInformesView::IDD)
{
	m_pListPrint= NULL;
}

CInformesView::~CInformesView()
{
	if( m_pListPrint) 
		delete m_pListPrint;
}

void CInformesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BAceptar);
	DDX_Control(pDX, IDC_LRESULTADOS, m_LCResultados);
	DDX_Control(pDX, IDC_DDESDE, m_DTDesde);
	DDX_Control(pDX, IDC_DHASTA, m_DTHasta);
	DDX_Control(pDX, IDC_EDOCUMENTO, m_EDocumento);
}

BEGIN_MESSAGE_MAP(CInformesView, CFormView)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_MYSQL, OnMysqlMessage)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LRESULTADOS, OnNMDblclkLresultados)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdatePrint)
	ON_COMMAND(ID_FILE_SAVE, OnArchivoExportar)
	ON_COMMAND(ID_FILE_SAVE_AS, OnArchivoExportar)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// Diagnósticos de CInformesView

#ifdef _DEBUG
void CInformesView::AssertValid() const
{
	CFormView::AssertValid();
}

void CInformesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

void CInformesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	GetDocument()->SetTitle("Informes");

	m_pConector= (CConector *)AfxGetMainWnd()->SendMessage(WM_QUERYCONECTOR, 0, (LPARAM)this);
	m_pUsuario= (CUsuario *)AfxGetMainWnd()->SendMessage(WM_QUERYUSUARIO, 0, (LPARAM)this);

	/*m_CBTipo.SetItemData(m_CBTipo.AddString("Entrega"), 0);
	m_CBTipo.SetItemData(m_CBTipo.AddString("Internacional"),1);
	m_nModo= AfxGetApp()->GetProfileInt("informes", "tipo", 0);
	m_CBTipo.SetSelData(m_nModo);*/

	m_LCResultados.SetOrdenable();

	m_LCResultados.AddColumn("Código");
	m_LCResultados.AddColumn("Documento", CMyListCtrl::tipocol_numero);
	m_LCResultados.AddColumn("Nombre");
	m_LCResultados.AddColumn("Apellido");
	m_LCResultados.AddColumn("Fecha", CMyListCtrl::tipocol_fechahora);

	m_LCResultados.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_LCResultados.SetCanShowMenu();

	m_LCResultados.AutoSizeColumns();
	OnBnClickedOk();
}

void CInformesView::OnBnClickedOk()
{
	if( !m_pConector)
		return;

	m_LCResultados.SetMessageLista("Realizando consulta");
	m_LCResultados.DeleteAllItems();

	CString desde, hasta;
	CMyTime tiempo;
	m_DTDesde.GetTime(tiempo);
	desde= tiempo.GetFormatoMysql();
	m_DTHasta.GetTime(tiempo);
	hasta= tiempo.GetFormatoMysql();
	CString usuario;
	m_EDocumento.GetWindowText(usuario);

	CString query;

	query.Format("SELECT entrega.codigo,entrega.documento,usuario.nombre,usuario.apellido,entrega.fecha FROM entrega LEFT JOIN usuario ON (usuario.documento=entrega.documento) WHERE entrega.fecha>=\"%s 00:00:01\" AND entrega.fecha<=\"%s 23:59:59\"", desde, hasta);
	if( !usuario.IsEmpty())
		query+=" AND entrega.documento='" + usuario + "'";

	m_pConector->Query(query,1, this);
}

LRESULT CInformesView::OnMysqlMessage(WPARAM wParam, LPARAM lParam) {

	MySQLMessage *mess=(MySQLMessage *)lParam;
	CThreadMySQL *hilo= (CThreadMySQL *) lParam;

	switch( wParam) {
		case QUERY_EMPTY:
			break;
		case QUERY_RESULT:
			{
				CMyTime tiempo(mess->GetAt(4));
				int pos= m_LCResultados.AddItem(mess->GetAt(0));
				m_LCResultados.SetItemText(pos, 1, mess->GetAt(1));
				m_LCResultados.SetItemText(pos, 2, mess->GetAt(2));
				m_LCResultados.SetItemText(pos, 3, mess->GetAt(3));
				m_LCResultados.SetItemText(pos, 4, tiempo.GetFechaHora());
			}
			break;
		case THREAD_END:
			m_LCResultados.SetMessageLista("No se encontraron resultados");
			m_LCResultados.AutoSizeColumns();
			break;
	}
	return 0;
}
void CInformesView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if( ::IsWindow(m_LCResultados)) {
		m_LCResultados.MoveWindow(5, 70, cx-10, cy-80);
	}
}

void CInformesView::OnNMDblclkLresultados(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if( pNMListView->iItem == -1 || pNMListView->iSubItem == -1) 
		return;
	CString texto= m_LCResultados.GetItemText(pNMListView->iItem, 0);
	AfxGetMainWnd()->SendMessage(WM_INSERTODS, 0, (LPARAM) (const char *)texto);
	*pResult = 0;
}

BOOL CInformesView::OnPreparePrinting(CPrintInfo* pInfo)
{

	if( m_pListPrint)
		delete m_pListPrint;
	m_pListPrint= new CListDemoViewPrint();

	m_pListPrint->SetListCtrl(&m_LCResultados);
	m_pListPrint->SetAppName("SVE");
	m_pListPrint->SetDocTitle(GetDocument()->GetTitle());
	m_pListPrint->SetFooterIzq(m_pUsuario->GetNombre());

	CMyTime tiempo(true);
	m_pListPrint->SetFooter(tiempo.GetFechaHora(AfxGetApp()->GetProfileInt("fechas", "larga", 0)));

	m_pListPrint->OnPreparePrinting(pInfo);

	m_pListPrint->SetMarginBottomPix(AfxGetApp()->GetProfileInt("printer", "cy", 20));
	m_pListPrint->SetMarginLeftPix(AfxGetApp()->GetProfileInt("printer", "x", 20));
	m_pListPrint->SetMarginRightPix(AfxGetApp()->GetProfileInt("printer", "cx", 20));
	m_pListPrint->SetMarginTopPix(AfxGetApp()->GetProfileInt("printer", "y", 20));

	return DoPreparePrinting(pInfo);
}

void CInformesView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnBeginPrinting(pDC, pInfo);
}

void CInformesView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnPrint(pDC, pInfo);
}

void CInformesView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_pListPrint->OnEndPrinting(pDC, pInfo);
	delete m_pListPrint;
	m_pListPrint= NULL;
}

void CInformesView::OnUpdatePrint(CCmdUI *pCmdUI) {

	if(m_pUsuario->Pertenece(1) || m_pUsuario->Pertenece(4))
		pCmdUI->Enable(m_LCResultados.GetItemCount()>0);
	else pCmdUI->Enable(false);
}

void CInformesView::OnArchivoExportar()
{
	Exportar( &m_LCResultados, GetDocument()->GetTitle(), true);
}

BOOL CInformesView::PreTranslateMessage(MSG* pMsg)
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

void CInformesView::OnDestroy()
{
	CFormView::OnDestroy();
}
