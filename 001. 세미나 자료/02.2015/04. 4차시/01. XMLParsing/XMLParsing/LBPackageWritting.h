// LBPackageWritting.h: interface for the CLBPackageWritting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LBPACKAGEWRITTING_H__1A3EA82F_9ED1_4AB0_8160_F6E7992EA93A__INCLUDED_)
#define AFX_LBPACKAGEWRITTING_H__1A3EA82F_9ED1_4AB0_8160_F6E7992EA93A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LBPackageParser.h"
#include "LBPackageNavigation.h"

class CLBPackageWritting  
{
public:
	bool WritePackage(LPPARSER_RESULT lpResult, char *szFileName);
	CLBPackageWritting();
	virtual ~CLBPackageWritting();

private:
	void WriteHaveContents(LPXML_POSITION pos);
	void WriteEmptyTag(LPXML_POSITION pos);
	void WriteHaveChild(LPXML_POSITION pos);
	void WriteAttr(XML_POSITION pos);
	void WriteStr(char *str, int size);
	void WriteCh(int ch);
	void WriteTag(LPXML_POSITION pos);
	void WriteTab();
	void WriteSpace();
	void WriteCR();
	void WriteBody(LPPARSER_RESULT lpResult);
	void WriteHead(LPPARSER_RESULT lpResult);
	
	void IncreaseOrder() { m_nTabOrder++; }
	void DecreaseOrder();

	int GetTagType(XML_POSITION pos);

	void StartWritting(LPPARSER_RESULT lpResult);

	int						m_nTabOrder;
	
	FILE					*m_file;
	CLBPackageParser		PARSER;
	CLBPackageNavigation	NAV;
};

#endif // !defined(AFX_LBPACKAGEWRITTING_H__1A3EA82F_9ED1_4AB0_8160_F6E7992EA93A__INCLUDED_)
