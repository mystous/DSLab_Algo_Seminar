
// XMLParsingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMLParsing.h"
#include "XMLParsingDlg.h"
#include "afxdialogex.h"
#include "LBPackageParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CXMLParsingDlg dialog



CXMLParsingDlg::CXMLParsingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CXMLParsingDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMLParsingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_NAV, m_treeView);
}

BEGIN_MESSAGE_MAP(CXMLParsingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CXMLParsingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_XML_LOAD, &CXMLParsingDlg::OnBnClickedButtonXmlLoad)
END_MESSAGE_MAP()


// CXMLParsingDlg message handlers

BOOL CXMLParsingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_bInitImage = FALSE;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMLParsingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXMLParsingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXMLParsingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CXMLParsingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CXMLParsingDlg::OnBnClickedButtonXmlLoad()
{
	CFileDialog dlgFileOpen(TRUE, L"XML", NULL, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, L"XML Files(*.xml)|*.xml|HTML Files(*.htm, *.html)|*.htm; *.html|All Files(*.*)|*.*||", NULL);

	if (dlgFileOpen.DoModal() == IDOK)
	{
		PARSER_RESULT		Result;
		char				szTemp[256];

		POSITION		pos = dlgFileOpen.GetStartPosition();
		CString strTemp = dlgFileOpen.GetNextPathName(pos);

		


		CLBPackageParser		parser;
		USES_CONVERSION;
		if (parser.StartParsing(OLE2A(strTemp), &Result))
		{
			FillTree(&Result);
			parser.RemoveAllNode(&Result);
		}
		else
			AfxMessageBox(L"무결성 체크를 통과 하지 못했거나 잘못된 XML 문서입니다!", MB_ICONSTOP);

	}
}

void CXMLParsingDlg::FillTree(LPPARSER_RESULT lpResult)
{
	XML_POSITION pos;
	if (Nav.GetXMLRoot(&pos, lpResult) == true)
	{
		InitTreeEnv();
		InSertXMLData(lpResult);
		FillTag(lpResult);
	}
}

void CXMLParsingDlg::InitTreeEnv()
{
	if (!m_bInitImage)
	{
		m_ImageTree.Create(IDB_TREE, 16, 1, RGB(172, 172, 172));
		m_treeView.SetImageList(&m_ImageTree, TVSIL_NORMAL);
		m_bInitImage = !m_bInitImage;
	}

	m_treeView.DeleteAllItems();
}

void CXMLParsingDlg::InSertXMLData(LPPARSER_RESULT lpResult)
{

	TV_INSERTSTRUCT		tvinsert;

	USES_CONVERSION;
	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
	//tvinsert.item.mask = TVIF_IMAGE | TVIF_TEXT ;
	//tvinsert.item.state = 0;
	//tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 256;

	tvinsert.item.pszText = L"XML Version";
	tvinsert.item.iImage = 3;
	tvinsert.item.iSelectedImage = 3;
	HTREEITEM VERSION = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = VERSION;
	tvinsert.item.pszText = A2OLE(lpResult->szXMLVersion);
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = NULL;
	tvinsert.item.pszText = L"DOC TYPE";
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 0;
	HTREEITEM DOC_TYPE = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = DOC_TYPE;
	tvinsert.item.pszText = L"Root TAG";
	tvinsert.item.iImage = 1;
	tvinsert.item.iSelectedImage = 1;
	HTREEITEM ROOT_TAG = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = ROOT_TAG;
	tvinsert.item.pszText = A2OLE(lpResult->szRootTAg);
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = DOC_TYPE;
	tvinsert.item.pszText = L"Document Type";
	tvinsert.item.iImage = 1;
	tvinsert.item.iSelectedImage = 1;
	HTREEITEM DOCUMENT = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = DOCUMENT;
	tvinsert.item.pszText = A2OLE(lpResult->szDocType);
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	m_treeView.InsertItem(&tvinsert);


	tvinsert.hParent = DOC_TYPE;
	tvinsert.item.pszText = L"DTD Name";
	tvinsert.item.iImage = 1;
	tvinsert.item.iSelectedImage = 1;
	HTREEITEM DTD = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = DTD;
	tvinsert.item.pszText = A2OLE(lpResult->szDTDName);
	tvinsert.item.iImage = 4;
	tvinsert.item.iSelectedImage = 4;
	m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = DOC_TYPE;
	tvinsert.item.pszText = L"DTD URL";
	tvinsert.item.iImage = 1;
	tvinsert.item.iSelectedImage = 1;
	HTREEITEM DTD_URL = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = DTD_URL;
	tvinsert.item.pszText = A2OLE(lpResult->szDTDURL);
	tvinsert.item.iImage = 6;
	tvinsert.item.iSelectedImage = 6;
	m_treeView.InsertItem(&tvinsert);


	tvinsert.hParent = NULL;
	tvinsert.item.pszText = L"Style Sheet";
	tvinsert.item.iImage = 5;
	tvinsert.item.iSelectedImage = 5;
	HTREEITEM STYLE_SHEET = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = STYLE_SHEET;
	tvinsert.item.pszText = L"Style Sheet Type";
	tvinsert.item.iImage = 1;
	tvinsert.item.iSelectedImage = 1;
	HTREEITEM STYLE_TYPE = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = STYLE_TYPE;
	tvinsert.item.pszText = A2OLE(lpResult->szStyleSheetType);
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = STYLE_SHEET;
	tvinsert.item.pszText = L"Style Sheet href";
	tvinsert.item.iImage = 6;
	tvinsert.item.iSelectedImage = 6;
	HTREEITEM STYLE_HREF = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = STYLE_HREF;
	tvinsert.item.pszText = A2OLE(lpResult->szStyleHref);
	tvinsert.item.iImage = 2;
	tvinsert.item.iSelectedImage = 2;
	m_treeView.InsertItem(&tvinsert);




}

void CXMLParsingDlg::FillTag(LPPARSER_RESULT lpResult)
{

	XML_POSITION		pos;

	Nav.GetXMLRoot(&pos, lpResult);
	InsertNode(pos, NULL);
}

void CXMLParsingDlg::InsertNode(XML_POSITION pos, HTREEITEM hParent)
{

	TV_INSERTSTRUCT		tvinsert;
	char				szTemp[256];

	USES_CONVERSION;
	tvinsert.hParent = NULL;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
	tvinsert.item.cchTextMax = 256;

	Nav.GetTag(pos, szTemp);

	tvinsert.hParent = hParent;
	tvinsert.item.pszText = A2OLE(szTemp);
	tvinsert.item.iImage = 0;
	tvinsert.item.iSelectedImage = 0;
	HTREEITEM NODE = m_treeView.InsertItem(&tvinsert);


	int nCount = Nav.GetAttrSize(pos);
	for (int i = 0; i < nCount; i++)
	{
		tvinsert.hParent = NODE;
		Nav.GetAttributeName(pos, i, szTemp);
		tvinsert.item.pszText = A2OLE(szTemp);
		tvinsert.item.iImage = 1;
		tvinsert.item.iSelectedImage = 1;
		HTREEITEM ATTR = m_treeView.InsertItem(&tvinsert);

		tvinsert.hParent = ATTR;
		Nav.GetAttribute(pos, i, szTemp);
		tvinsert.item.pszText = A2OLE(szTemp);
		tvinsert.item.iImage = 4;
		tvinsert.item.iSelectedImage = 4;
		m_treeView.InsertItem(&tvinsert);
	}

	tvinsert.hParent = NODE;
	tvinsert.item.pszText = L"Contents";
	tvinsert.item.iImage = 1;
	tvinsert.item.iSelectedImage = 1;
	HTREEITEM NODE_CONTENTS = m_treeView.InsertItem(&tvinsert);

	tvinsert.hParent = NODE_CONTENTS;
	Nav.GetContents(pos, szTemp);
	tvinsert.item.pszText = A2OLE(szTemp);
	tvinsert.item.iImage = 4;
	tvinsert.item.iSelectedImage = 4;
	m_treeView.InsertItem(&tvinsert);


	if (Nav.IsHaveChild(pos))
	{
		if (Nav.MoveToFirstChlid(&pos))
		{
			InsertNode(pos, NODE);
			while (Nav.MoveToNext(&pos))
				InsertNode(pos, NODE);
		}

	}

}
