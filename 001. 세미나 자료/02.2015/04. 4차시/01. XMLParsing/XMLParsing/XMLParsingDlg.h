
// XMLParsingDlg.h : header file
//

#pragma once
#include "LBPackageNavigation.h"
#include "LBPackageParser.h"
#include "afxcmn.h"

// CXMLParsingDlg dialog
class CXMLParsingDlg : public CDialogEx
{
// Construction
public:
	CXMLParsingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XMLPARSING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	CLBPackageNavigation	Nav;
	BOOL m_bInitImage;
	CImageList m_ImageTree;

	void InsertNode(XML_POSITION pos, HTREEITEM hParent);
	void FillTag(LPPARSER_RESULT lpResult);
	void InSertXMLData(LPPARSER_RESULT lpResult);
	void InitTreeEnv();
	void FillTree(LPPARSER_RESULT lpResult);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonXmlLoad();
	CTreeCtrl m_treeView;
};
