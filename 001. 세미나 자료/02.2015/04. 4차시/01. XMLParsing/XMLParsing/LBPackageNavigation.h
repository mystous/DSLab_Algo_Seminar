// LBPackageNavigation.h: interface for the CLBPackageNavigation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LBPACKAGENAVIGATION_H__59704A66_8BE1_4705_9B41_6099FBAAAB66__INCLUDED_)
#define AFX_LBPACKAGENAVIGATION_H__59704A66_8BE1_4705_9B41_6099FBAAAB66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define APPEND		-1				// AddToParent 함수의 디폴트 인자

#include "LBPackageParser.h"

class CLBPackageNavigation  
{
public:

	bool SetAttributeName(XML_POSITION const pos, int Index, char *szName);
	void SetContents(XML_POSITION pos, char *szContents);
	void SetAttributeValue(const XML_POSITION pos, int nIndex, char *pszAttrValue);
	void SetAttribute(const XML_POSITION pos, char *szAttr, char *szAttrValue);
	
	int GetAttrSize(const XML_POSITION pos);
	char* GetContents(XML_POSITION const pos);
	int GetTagType(const XML_POSITION pos);
	bool GetXMLRoot(LPXML_POSITION pos, LPPARSER_RESULT lpResult);
	bool GetTag(XML_POSITION const pos, char *szTag);
	bool GetAttribute(XML_POSITION const pos, int Index, char *szAttr);
	bool GetAttributeName(XML_POSITION const pos, int Index, char *szName);
	int	GetChlidSize(XML_POSITION const pos);
	bool GetContents(XML_POSITION const pos, char *szContents);

	bool FindFirst(LPXML_POSITION pos, char *szSearch);
	bool FindNext(LPXML_POSITION pos, char *szSearch);
	BOOL FindAttr(const XML_POSITION pos, const char *pszAttrName, char *pszAttrValue);

	bool MoveToParent(LPXML_POSITION pos);
	bool MoveToChild(LPXML_POSITION pos, char *szChlid);
	bool MoveToFirstChlid(LPXML_POSITION pos);
	bool MoveToNext(LPXML_POSITION pos);
	bool MoveToLastChlid(LPXML_POSITION pos);

	void ChangeContents(XML_POSITION pos, char *szContents);
	bool RemoveTag(LPXML_POSITION pos);
	void AddToParent(LPXML_POSITION posParent, char *szTag, int tagType = HAVECONTENTS, int Index = APPEND);
	bool IsHaveChild(XML_POSITION pos);

	CLBPackageNavigation();
	virtual ~CLBPackageNavigation();

};

#endif // !defined(AFX_LBPACKAGENAVIGATION_H__59704A66_8BE1_4705_9B41_6099FBAAAB66__INCLUDED_)
