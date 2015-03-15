// LBPackageWritting.cpp: implementation of the CLBPackageWritting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LBPackageWritting.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define HAVECHILD		1
//#define EMPTYTAG		2
//#define HAVECONTENTS	3

#define GETTAGBYPARSER

extern "C" unsigned int ksc5601toUnicodeN(unsigned short *des, char *str, unsigned int len);


CLBPackageWritting::CLBPackageWritting()
{
	m_nTabOrder = 0;
	m_file = NULL;
}

CLBPackageWritting::~CLBPackageWritting()
{

}


void CLBPackageWritting::DecreaseOrder()
{
	if(m_nTabOrder != 0)
		m_nTabOrder--;
}

bool CLBPackageWritting::WritePackage(LPPARSER_RESULT lpResult, char *szFileName)
{
	bool	bRtn = FALSE;

#ifdef _WINCE
	TCHAR	szTemp[256];
	ksc5601toUnicodeN(szTemp, szFileName, 256);
	if((m_file = _tfopen(szTemp, TEXT("wt"))) == NULL)
#else
	if((m_file = fopen(szFileName, "wt")) == NULL)
#endif
		return bRtn;
	else
		bRtn = TRUE;

	m_nTabOrder = 0;
	StartWritting(lpResult);
	fclose(m_file);

	return bRtn;
}

void CLBPackageWritting::StartWritting(LPPARSER_RESULT lpResult)
{
	WriteHead(lpResult);
	WriteBody(lpResult);
}

void CLBPackageWritting::WriteHead(LPPARSER_RESULT lpResult)
{
	char		szXmlVersion[2][256];
	char		szDocType[2][256];
	char		szDocName[256];
	char		szDocURL[256];
	char		szStandAlone[256];
	char		szEncoding[256];

	strcpy(szXmlVersion[0],  ("<?xml version=\"") );
	strcpy(szXmlVersion[1], ("?>"));

	strcpy(szDocType[0],  ("<!DOCTYPE ") );
	strcpy(szDocType[1], (">"));

	fputs(szXmlVersion[0], m_file);
	fputs(lpResult->szXMLVersion, m_file);
	fputs("\"", m_file);
	if(strcmp(lpResult->szEncoding, ""))
	{
		WriteSpace();
		sprintf(szEncoding, "encoding=\"%s\"", lpResult->szEncoding);
		fputs(szEncoding, m_file);
	}
	if(strcmp(lpResult->szStandAlone, ""))
	{
		WriteSpace();
		sprintf(szStandAlone, "standalone=\"%s\"", lpResult->szStandAlone);
		fputs(szStandAlone, m_file);
	}
	
	fputs(szXmlVersion[1], m_file);
	WriteCR();
	
	fputs(szDocType[0], m_file);
	fputs(lpResult->szRootTAg, m_file);
	WriteSpace();
	fputs(lpResult->szDocType, m_file);
	WriteSpace();
	if(!strcmp(lpResult->szDocType, "PUBLIC"))
	{
		sprintf(szDocName, "\"%s\"", lpResult->szDTDName);
		fputs(szDocName, m_file);
		WriteSpace();
	}
	sprintf(szDocURL, "\"%s\"", lpResult->szDTDURL);
	fputs(szDocURL, m_file);
	//fputs(("\"library.dtd\""), m_file);
	//fputc('"', m_file);
	//fwrite(lpResult->szDTDURL, strlen(lpResult->szDTDURL)*sizeof(char), 1, m_file);
	//fputc('"', m_file);
	fputs(szDocType[1], m_file);
	WriteCR();
}

void CLBPackageWritting::WriteBody(LPPARSER_RESULT lpResult)
{
	XML_POSITION		pos;

	NAV.GetXMLRoot(&pos, lpResult);
	WriteTag(&pos);
	
}

void CLBPackageWritting::WriteCR()
{
	fputs(("\r\n"), m_file);
}

void CLBPackageWritting::WriteSpace()
{
	fputc(32, m_file);
}

/****************************************************************/
/*		이 함수 굉장히 더티 함~ -_-; 파서에서 처리 해주어야		*/
/*		할 것 같음. 우선은 이 함수로 처리함						*/
/****************************************************************/
/****************************************************************
		우선은 Empty Tag는 구별 할 수 있게 해 두었음
		Nested Tag와 Contents가 같이 있는 것은 DTD로 
		구별해야 함 근데... 어려움 -_-;
*****************************************************************/
int CLBPackageWritting::GetTagType(XML_POSITION pos)
{
	int			nRtn;
	int			nMask;
	int			nType, nOri;
	
	nMask = EMPTYTAG + HAVECHILD;

	nType = NAV.GetTagType(pos);

	nOri = nType;
	nType &= nMask;

	if( nType != 0 )
		nRtn = nType;
	else
		nRtn = HAVECONTENTS;

	return nRtn;

}

void CLBPackageWritting::WriteTab()
{
	for(int i = 0; i < m_nTabOrder ; i++ )
		fputc('\t', m_file);
}

void CLBPackageWritting::WriteTag(LPXML_POSITION pos)
{
//	int		n = GetTagType(*pos);
	switch(GetTagType(*pos))
	{
	case HAVECHILD:
		WriteHaveChild(pos);		
		break;
	case EMPTYTAG:
		WriteEmptyTag(pos);
		break;
	case HAVECONTENTS:
		WriteHaveContents(pos);
		break;
	}
}

void CLBPackageWritting::WriteCh(int ch)
{
	fputc(ch, m_file);
}

void CLBPackageWritting::WriteStr(char *str, int size)
{
	fputs(str, m_file);	
}

void CLBPackageWritting::WriteAttr(XML_POSITION pos)
{
	int		nCount = NAV.GetAttrSize(pos);
	int		i;
	char	cEqual = '=';
	char	cQutor = '"';
	char	szTemp[256];

	for( i = 0 ; i < nCount ; i ++ )
	{
		WriteSpace();
		NAV.GetAttributeName(pos, i, szTemp);
		WriteStr(szTemp, strlen(szTemp));
		WriteCh(cEqual);
		NAV.GetAttribute(pos, i, szTemp);
		WriteCh(cQutor);
		WriteStr(szTemp, strlen(szTemp));
		WriteCh(cQutor);
	}
}

void CLBPackageWritting::WriteHaveChild(LPXML_POSITION pos)
{
	char			szTemp[256];
	XML_POSITION	SubPos;
	WriteTab();
	IncreaseOrder();
	WriteCh('<');
	NAV.GetTag(*pos, szTemp);
	WriteStr(szTemp, strlen(szTemp));	
	WriteAttr(*pos);
	WriteCh('>');
	WriteCR();

	if(NAV.MoveToFirstChlid(pos)){
		do
		{
			memcpy(&SubPos, pos, sizeof(XML_POSITION));
			//WriteTag(pos);
			WriteTag(&SubPos);
		}while(NAV.MoveToNext(pos));
	}

	DecreaseOrder();
	WriteTab();
	WriteStr(("</"), 2);
	WriteStr(szTemp, strlen(szTemp));	
	WriteCh('>');
	WriteCR();
}

void CLBPackageWritting::WriteEmptyTag(LPXML_POSITION pos)
{
	char		szTemp[256];
	WriteTab();
	WriteCh('<');
	NAV.GetTag(*pos, szTemp);
	WriteStr(szTemp, strlen(szTemp));	
	WriteAttr(*pos);
	WriteStr((" />"), 3);
	WriteCR();
}

void CLBPackageWritting::WriteHaveContents(LPXML_POSITION pos)
{
	char		szTag[256];
	char		szContents[16384];
	WriteTab();
	WriteCh('<');
	NAV.GetTag(*pos, szTag);
	WriteStr(szTag, strlen(szTag));	
	WriteAttr(*pos);
	WriteCh('>');
	NAV.GetContents(*pos, szContents);
	WriteStr(szContents, strlen(szContents));	
	WriteStr(("</"), 2);
	WriteStr(szTag, strlen(szTag));	
	WriteCh('>');
	WriteCR();
}
