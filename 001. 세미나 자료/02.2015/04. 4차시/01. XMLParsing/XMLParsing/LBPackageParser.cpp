// LBPackageParser.cpp: implementation of the CLBPackageParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LBPackageParser.h"
#include "LBPackageParserXMLCheck.h"

//#define CR '\015'       // \r
//#define LF '\012'       // \n
#define VTAB '\013'
#define FF '\014'
//#define TAB '\011'

extern "C" FILE* ser_fopen( char* fileName, char *openMode );
extern "C" FILE* ser_wfopen( WCHAR *pfileName, WCHAR *openMode );
extern "C" extern void ser_fclose( FILE *fp );
extern "C" long ser_fopen2mem_with_null( char* fileName, char **ppBuffer );
extern "C" void ser_fclose2mem( char *pMem );
extern "C" FILE*	pPackageFile;

#ifndef DISABLE_MERGED_FILE
	#include "ser_package.h"
#endif


#define MAX_BUFFER_SIZE		8192

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define CHECK_WELLFORMED_N_VALIDATATION			// Wellformed Check�� ���� ���� �Ÿ� �ּ����� �����.

CLBPackageParser::CLBPackageParser()
{
	m_pszRawData = NULL;
	m_nRawPos = 0;
	m_nRawDataSize = 0;
}

CLBPackageParser::~CLBPackageParser()
{
	if(m_pszRawData)
	{
		free(m_pszRawData);
		m_pszRawData = NULL;
	}
}


/*----------------------------------------------------------------------*
 *		Samsung eBook Viewer based on OEB 1.0							*
 *																		*
 *		XML Parser Release 1.0.2	Author By Mystous(Cho, Kyu nam)		*
 *		Date: 2000, 11, 27  Mon		Time: AM: 11:04						*
 *																		*
 *----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*
 *		������ ���ڰ� ��ū���� �ƴ����� �����ϴ� �Լ�					*
 *----------------------------------------------------------------------*/
bool CLBPackageParser::IsToken(char ch, int Current, bool bInQuter)
{
	bool bRtn = false;
	if(Current == ATTR_VALUE){		/* ���� �±��� �Ӽ����� �а� ���� ���� ���� �����ϸ�		*/
		if(bInQuter){				/* �±׸� �д� ���� 2���� ��ΰ� �ִ�. " " �� �ѷ�����		*/ 
			if(ch == QUOT){			/* �κп����� ' " ' ���� ��ū���� �ν��ϰ� �׷��� ����		*/
				bRtn = true;		/* ��쿡�� ' " ', '>', ' ', '?', '/'�� ��ū���� �ν��Ѵ�	*/
				return bRtn;
			}
			return bRtn;
		}
		else{
			if(ch == QUOT || 
				ch == RANGLEBR || 
				ch == SPACE || 
				ch == QUESTION || 
				ch == ENDTAG || 
				ch == EQUAL){
				bRtn = true;
				return bRtn;
			}
			return bRtn;
		}
	}

	if(Current == CONTENTS){		
		if(ch == LANGLEBR || ch == CR || ch == LF || ch == TAB){
			bRtn = true;		/* Contents�� �а� �ִ� ������ '<' ���� ��ū����	*/
			return bRtn;		/* ����ϰ� CR, LF, TAB�� �����Ѵ�. ������ szTemp��	*/
		}						/* �������� �ʱ� ���ؼ� �����Ѵ�.					*/
		return bRtn;
	}

	bRtn = true;
	if(ch == SPACE)				/* ' ' ASC 32	*/
		return bRtn;
	else if(ch == LF)			/* '\n'			*/
		return bRtn;
	else if(ch == CR)			/* '\r'			*/
		return bRtn;
	else if(ch == TAB)			/* '\t' ASC 9	*/
		return bRtn;
	else if(ch == LANGLEBR)		/* '<'			*/
		return bRtn;
	else if(ch == RANGLEBR)		/* '>'			*/
		return bRtn;
	else if(ch == EQUAL)		/* '='			*/
		return bRtn;
	else if(ch == QUESTION)		/* '?'			*/
		return bRtn;
	else if(ch == ENDTAG)		/* '/'			*/
		return bRtn;

	bRtn = false;
	return bRtn;
}
/*----------------------------------------------------------------------*
 *		'=' �� ��ū���� ���Ǿ����� �Ӽ��� �о� �帮�� �־��ٸ�		*
 *		�Ӽ��� ��� �о� �鿴���Ƿ� SetAttribute()�� ����ؼ� ��		*
 *		����� �Ӽ��� �����Ѵ�. ���� �Ӽ����� �о� ���̴� �߰���		*
 *		'='�� ���Ǿ��ٸ� �Ӽ��� ���� �Ǿ� ���� ���� �����Ƿ�			*
 *		assert(0)�� �˷� �ְ� ���α׷��� ���� �Ѵ�.						*
 *																		*
 *		ex) <para attri1 = "test" attri2 = "test" = "test">				*
 *												^^^^^					*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnEqual(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	if(*nCurrent == ATTRIBUTE)
		SetAttribute(nCurrent, lpContents, szBuff, Index);

	if(*nCurrent != ATTR_VALUE)
	{
#ifndef _WINCE
		assert(0);
#endif
		exit(1);
	}
}
/*----------------------------------------------------------------------*
 *		�о� ���� ��ũ�� ���� TAG_CONTENTS ����ü�� szTag �� �����Ѵ�.	*
 *		szTemp�� �ٸ� ���� �����ϱ� ���� Index�� 0���� �ٲپ� �ְ� ��	*
 *		�� ���¸� ATTRIBUT�� �����Ѵ�( *nCurrent <<= 1)					*
 *----------------------------------------------------------------------*/
void CLBPackageParser::SetTag(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	szBuff[*Index] = '\0';
#ifdef MALLOC_BY_SIZE
	if(lpContents->szTag != NULL)
		free(lpContents->szTag);
	lpContents->szTag = (char*)malloc(strlen(szBuff) * sizeof(TCHAR)+1);
#endif
	strcpy(lpContents->szTag, szBuff);
	*Index = 0;
	*nCurrent <<= 1;
}

/*----------------------------------------------------------------------*
 *		�о� ���� �Ӽ��� ���� TAG_CONTENTS ����ü�� szAttr �� �����Ѵ�.	*
 *		szTemp�� �ٸ� ���� �����ϱ� ���� Index�� 0���� �ٲپ� �ְ� ��	*
 *		�� ���¸� ATTRI_VALUE�� �����Ѵ�( *nCurrent <<= 1)				*
 *		Attribute�� ������ �� �� �����Ƿ� nAttr�� ���� ��� �Ӽ���	*
 *		����Ǿ����� �����ϸ� nAttr���� �Ӽ����� �����ϰ� ������Ų��.	*
 *----------------------------------------------------------------------*/
void CLBPackageParser::SetAttribute(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	szBuff[*Index] = '\0';
#ifdef MALLOC_BY_SIZE
	if(lpContents->szAttr[lpContents->nAttr] != NULL)
		free(lpContents->szAttr[lpContents->nAttr]);
	lpContents->szAttr[lpContents->nAttr] = (char*)malloc(strlen(szBuff) * sizeof(TCHAR)+1);
#endif
	strcpy(lpContents->szAttr[lpContents->nAttr], szBuff);
	*nCurrent <<= 1;
	*Index = 0;
}

/*----------------------------------------------------------------------*
 *		CR, LF, SPACE, TAB�� ��� OnWhiteSpace���� ó�� �Ѵ�.			*
 *		��ũ ��, �Ӽ�, �Ӽ����� �����ϴ� ��ū���� ���ȴ�.				*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnWhiteSpace(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, bool *bExit)
{
	char	ch;
#ifdef _HUNMIN
	ch = 0;
#endif

	switch(*nCurrent)
	{
	case TAG_NAME:												/* �±� �̸��� �а� ������	*/
		SetTag(nCurrent, lpContents, szBuff, Index);
		break;
	case ATTRIBUTE:												/* �Ӽ��� �а� ������		*/
		if(*Index == 0)
			szBuff[*Index++] = MoveToNextChar(' ');				/* �ƹ����� ���� ���� ���¶�� ��ĭ�� ��� ���� ������.		*/
		else													/* <tag attr     =    "Test">								*/
			SetAttribute(nCurrent, lpContents, szBuff, Index);	/*           ^^^^ ^^^^										*/
		break;													/* �׷��� �ʴٸ� �Ӽ��� �����Ѵ�.							*/
	case ATTR_VALUE:											
		if(*Index == 0)											/* �Ӽ����� �о� ���϶��� ���������� ��ĭ�� ó�� �Ѵ�.		*/
			MoveToNextChar(' ');
		else
			SetAttrValue(nCurrent, lpContents, szBuff, Index);
		break;
	case CONTENTS:												/* Contents�� �а� ���� ��쿡�� ��ĭ�� ��� �����Ѵ�.		*/
		szBuff[*Index++] = ' ';									/* Contents�� �а� ���� ��쿡�� CR, LF, TAB�� ��ū����		*/
		break;													/* ������� �ʴ´�.											*/
	case OUT_OF_TAG:											
	case START:													/* �±� �ۿ� ���� ���� ���� '<'�� ���ö����� �� ���� �±�	*/
		while( ch != LANGLEBR)									/* �� ���۵ɶ����� ��ĭ�� ��� ���� �Ѵ�.					*/
		{														/* Start�� Ư���ϰ� XML ������ ó������ �о� ���϶� ���	*/
			//ch = fgetc(XMLFILE);									/* �Ǵ� �Լ� �̴�.											*/
			ch = GetCh();
			if(ch == EOF)
			{
#ifndef _WINCE
				assert(0);
#endif
				exit(1);
			}
		}
		OnLAngleBR(lpResult, nCurrent, lpContents, szBuff, Index, NULL, bExit);
		break;
	}
}
/*----------------------------------------------------------------------*
 *		Attribute�� ���� Contents ����ü�� �����Ѵ�.					*
 *----------------------------------------------------------------------*/
void CLBPackageParser::SetAttrValue(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	szBuff[*Index] = '\0';
	*Index = 0;
	*nCurrent >>= 1;
#ifdef MALLOC_BY_SIZE
	if(lpContents->szValue[lpContents->nAttr] != NULL)
		free(lpContents->szValue[lpContents->nAttr]);
	lpContents->szValue[lpContents->nAttr] = (char*)malloc(strlen(szBuff) * sizeof(TCHAR)+1);
#endif
	strcpy(lpContents->szValue[lpContents->nAttr++], szBuff);
}

/*----------------------------------------------------------------------*
 *		�ϳ��� �±׿� ���� �Ľ��� ���������� ó��						*
 *		�� ��Ȯ�ϰ� ���ؼ��� ���� �±װ� ��������						*
 *		<tag attr = "test"> ... </tag>									*
 *                        ^												*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OneTagParsingComplete(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	switch(*nCurrent)
	{
	case ATTR_VALUE:
		SetAttrValue(nCurrent, lpContents, szBuff, Index);
		break;
	case TAG_NAME:
		SetTag(nCurrent, lpContents, szBuff, Index);
		break;
	}
	
	*Index = 0;
}

/*----------------------------------------------------------------------*
 *		�ϳ��� �±׿� ���� �Ľ��� ���������� ó��						*
 *		�� ��Ȯ�ϰ� ���ؼ��� ���� �±װ� ��������						*
 *		<tag attr = "test"> ... </tag>									*
 *                        ^												*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnRAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	OneTagParsingComplete(lpResult, nCurrent, lpContents, szBuff, Index);
	*nCurrent = CONTENTS;
}

/*----------------------------------------------------------------------*
 *		���� �±׿� ���� ���� TAG_CONTENTS�� �θ� TAG_CONTENTS��		*
 *		NESTED ��Ų��.													*
 *----------------------------------------------------------------------*/
void CLBPackageParser::AddToParent(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpContents, bool *bExit)
{
	AddList(lpParent, lpContents);
	lpContents->lpParent->nNested ++;
	*bExit = true;												/* ��� �Լ��� �����ٴ� ���� �˸��� ���� �÷���			*/
}

/*----------------------------------------------------------------------*
 *		'/' ��ū�� �������� ó�� (�о�ε� �±װ� ��������)				*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnEndTag(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, LPTAG_CONTENTS lpParent, bool *bExit)
{
	lpContents->nType = EMPTYTAG;
#ifndef MALLOC_BY_SIZE
	strcpy(lpContents->szContents, "");
#else	
	lpContents->szContents = NULL;
#endif
	OneTagParsingComplete(lpResult, nCurrent, lpContents, szBuff, Index);
	*nCurrent = OUT_OF_TAG;
	AddToParent(lpParent, lpContents, bExit);
}

/*----------------------------------------------------------------------*
 *		'<' �� �������� ���ο� ��ũ�� �����ϰų� NESTED TAG�� �����ϴ�	*
 *		�Լ� NESTED�Լ��� ��� OnLAngleBR�� ȣ���� ReadFile�� �ٽ� ȣ��	*
 *		�ؼ� ���ο� ��ũ�� �о� ���δ�. <?xml ...?> <?xml-stylesheet?>	*
 *		<!DOCTYPE >�� ��� ������ �´� ó�� �Լ��� ȣ���Ѵ�.			*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnLAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, LPTAG_CONTENTS lpParent, bool *bExit)
{

	char		ch;
	int			pos;
	

	ch = GetCh();

	switch(ch)
	{
	case ENDTAG:
		szBuff[*Index] = '\0';
#ifdef MALLOC_BY_SIZE
		if(lpContents->szContents != NULL)
			free(lpContents->szContents);
		lpContents->szContents = (char*)malloc(strlen(szBuff) * sizeof(TCHAR)+1);
#endif
		strcpy(lpContents->szContents, szBuff);
		*Index = 0;
		while(ch != RANGLEBR)							/* /EndTag> �� ���Ͽ� ���� �����Ƿ� ���� �����͸� �� �ڷ� �̵���Ų��.	*/
			ch = GetCh();
			//ch = fgetc(XMLFILE);
		*nCurrent = OUT_OF_TAG;							
		AddToParent(lpParent, lpContents, bExit);		/* �θ� �±׿� �߰� ��Ų��. ROOT Tag�� ��� ���̷� �־��� XML_ROOT��	*/
		return;											/* �߰� ��Ų��.															*/
		break;

	case SPACE:											/* Space �̸� �±װ� ���ö����� ��ĭ�� �о� ���δ�.						*/
		ch = MoveToNextChar(ch);
		break;
	}

	if(*nCurrent == CONTENTS)
	{
		Seek(-2, _SEEK_CUR);
		lpContents->nType |= HAVECHILD;
		pos = ReadFile(lpResult, lpContents);
		SetPos(pos);
		return;
	}

	switch(ch)
	{
	case EMOTION:
		ch = GetCh();
		if(ch == '-')
		{
			if((ch = GetCh()) == '-')
				ReadComment();				/* �ּ��� ��� ��� �����Ѵ�.					*/
		}
		else if( ch == 'D')					/* For <!DOCTYPE> Tag							*/
			GetDOCTYPEData(lpResult);		/* <!DOCTYPE> �±��� Atrribute�� �о� ���δ�.	*/

		return;
		break;
	case QUESTION:							/* For <?xml?> <?xml-stylesheet?> Tag			*/
		GetXMLData(lpResult);				/* <?xml* ?>�±��� Attribute�� �о� ���δ�.		*/
		return;
		break;
	}

	ch = MoveToNextChar(ch);				/* ó�� Switch ���� SPACE�� ������ ��� �о�	*/
											/* ���̰� ���� ����� ��ġ�� �� ����			*/
	*Index = 0;
	lpContents->nAttr = 0;					/* Tag�� szBuff�� ������						*/
	*nCurrent = TAG_NAME;
	szBuff[(*Index)++] = ch;
}

/*----------------------------------------------------------------------*
 *		���� �����͸� �ּ��� �������� �̵� ��Ŵ							*
 *----------------------------------------------------------------------*/
void CLBPackageParser::ReadComment()
{
	char ch;

	while(1){
		ch = GetCh();
		if( ch == '-')
			if((ch = GetCh()) == '-')
				if((ch = GetCh()) == '>')
					break;
	}

}

/*----------------------------------------------------------------------*
 *		���� �����͸� Space��������	�̵� ��Ŵ							*
 *----------------------------------------------------------------------*/
char CLBPackageParser::MoveToNextChar(char CHAR)
{
	char ch;

	ch = CHAR;
	if( ch == SPACE )
			while(ch != SPACE)
				ch = GetCh();

	return ch;
}

/*----------------------------------------------------------------------*
 *		���ڷ� �־��� ��ū�� ��ġ�� ������ ���Ͽ��� �о� szTemp��		*
 *		�����Ѵ�. ��ū�� �������� �ʴ´�.								*
 *----------------------------------------------------------------------*/
void CLBPackageParser::ReadStrByToken(char *szTemp, char Token)
{
	int		Index = 0;
	char	ch;
	
	while((ch = GetCh()) != Token)
		szTemp[Index++] = ch;
	szTemp[Index] = '\0';
}

/*----------------------------------------------------------------------*
 *		<!DCOTYPE >�� �ִ� ������ �о PARSER_RESULT�� �����Ѵ�.		*
 *----------------------------------------------------------------------*/
void CLBPackageParser::GetDOCTYPEData(LPPARSER_RESULT lpResult)
{
	char		ch;
	char		szTemp[256];

	while((ch = GetCh()) != SPACE);

	ReadStrByToken(szTemp, SPACE);				/* ROOT Tag�� �̸��� �����Ѵ�.												*/
#ifdef MALLOC_BY_SIZE
	if(lpResult->szRootTAg != NULL)
		free(lpResult->szRootTAg);
	lpResult->szRootTAg = (char*)malloc(strlen(szTemp) * sizeof(TCHAR)+1);
#endif
	strcpy(lpResult->szRootTAg, szTemp);
	
	ReadStrByToken(szTemp, SPACE);				/* Document Type�� �����Ѵ�.												*/
	strcpy(lpResult->szDocType, szTemp);

	if(!strcmp(szTemp, ("PUBLIC")))			/* DTD�� ������ PUBLIC�� ��� "DTD Name"�� "DTD URL"�� �о �����Ѵ�.		*/
	{
		while((ch = GetCh()) != QUOT);
		ReadStrByToken(szTemp, QUOT);
#ifdef MALLOC_BY_SIZE
		if(lpResult->szDTDName != NULL)
			free(lpResult->szDTDName);
		lpResult->szDTDName = (char*)malloc(strlen(szTemp) * sizeof(TCHAR)+1);
#endif
		strcpy(lpResult->szDTDName, szTemp);

		while((ch = GetCh()) != QUOT);
		ReadStrByToken(szTemp, QUOT);
#ifdef MALLOC_BY_SIZE
		if(lpResult->szDTDURL != NULL)
			free(lpResult->szDTDURL);
		lpResult->szDTDURL= (char*)malloc(strlen(szTemp) * sizeof(TCHAR)+1);
#endif
		strcpy(lpResult->szDTDURL, szTemp);
	}
	else if(!strcmp(szTemp, "SYSTEM"))
	{
		while((ch = GetCh()) != QUOT);
		ReadStrByToken(szTemp, QUOT);
#ifdef MALLOC_BY_SIZE
		if(lpResult->szDTDURL != NULL)
			free(lpResult->szDTDURL);
		lpResult->szDTDURL= (char*)malloc(strlen(szTemp) * sizeof(TCHAR)+1);
#endif
		strcpy(lpResult->szDTDURL, szTemp);
	}

	//while((ch = fgetc(XMLFILE)) != RANGLEBR);
	while((ch = GetCh()) != RANGLEBR);
}

/*----------------------------------------------------------------------*
 *		XML�� ROOT Tag�� ������ ���� ������ XML�� ���� ����Ÿ�� ����	*
 *		���� ��ƾ <?xml version="1.0"?> �� <?xml-styleshhet ... ?>��	*
 *		�ִ� ������ ���� ���� ��ƾ										*
 *----------------------------------------------------------------------*/
void CLBPackageParser::GetXMLData(LPPARSER_RESULT lpResult)
{
	char		ch;
	char		szTemp[256], szTempXML[256];
	
	ReadStrByToken(szTemp, SPACE);
	if( strcmp(szTemp, ("xml")) == 0 )					/* <?xml ...?>�� ���				*/
	{
		while((ch = GetCh()) != QUOT);					/* version�� ���� �ʰ� �ǳʶ�		*/
		ReadStrByToken(szTemp, QUOT);					/* version�� Atrribute���� ����		*/
		strcpy(lpResult->szXMLVersion, szTemp);			/* PARSER_RESULT�� ����				*/

		while((ch = GetCh()) != RANGLEBR)
		{
			if(ch != SPACE && ch != QUESTION)
			{
				szTempXML[0] = ch;
				szTempXML[1] = '\0';
				ReadStrByToken(szTemp, EQUAL);
				strcat(szTempXML, szTemp);

				while((ch = GetCh()) != QUOT);
				ReadStrByToken(szTemp, QUOT);
				
				if(!strcmp(szTempXML, "standalone"))
					strcpy(lpResult->szStandAlone, szTemp);
				else if(!strcmp(szTempXML, "encoding"))
					strcpy(lpResult->szEncoding, szTemp);
			}
		}

	}
	else if( strcmp(szTemp, ("xml-stylesheet")) == 0 )	/* <?xml-stylesheet ...?>�� ���	*/
	{
		while((ch = GetCh()) != QUOT);					/* <?xml-stylesheet "DTD Type" "DTD HREF"?>		*/
		ReadStrByToken(szTemp, QUOT);					/* ���� Type�� Herf�� �о PARSER_RESULT��	*/
		strcpy(lpResult->szStyleSheetType, szTemp);		/*	����										*/

		while((ch = GetCh()) != QUOT);
		ReadStrByToken(szTemp, QUOT);
#ifdef MALLOC_BY_SIZE
		if(lpResult->szStyleHref != NULL)
			free(lpResult->szStyleHref);
		lpResult->szStyleHref = (char*)malloc(strlen(szTemp) * sizeof(TCHAR)+1);
#endif
		strcpy(lpResult->szStyleHref, szTemp);

		while((ch = GetCh()) != RANGLEBR);
	}

	//while((ch = fgetc(XMLFILE)) != RANGLEBR);
}
/*----------------------------------------------------------------------*
 *		' " '�� ������ ������ ���� ������ ����ǥ�� �׿� �ִ��� �ƴϸ�	*
 *		����ǥ���� ��������� �˱� ���� �÷��׸� �����Ѵ�.			*
 *----------------------------------------------------------------------*/
bool CLBPackageParser::SetInQuter(bool bInQuter)
{
	bool bRtn;

	if(bInQuter == true)
		bRtn = false;
	else
		bRtn = true;
	return bRtn;
}

/*----------------------------------------------------------------------*
 *		�������� �Ľ��� �����ϴ� �κ� NESTED TAG �� ������ ���������	*
 *		ȣ���� �ȴ�. ���ϰ� fpos_t�� ���� ���� �������� �Ѱ� �ָ� ���	*
 *		������ ȣ���� �Ǳ� ������ �����ϰ� ���� ȣ��Ǳ� ���� ���� ��ġ	*
 *		�� ���� ���� �ȴ�. ���� ���ϰ��� ������ ��� �Լ��� ȣ���� ��	*
 *		�� ������ ��ġ�� �̵��ؾ� �Ѵ�. OnLAngleBR()�Լ����� �� ����	*
 *		�ϰ� �ȴ�.														*
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
 *		ROOT �±װ� ������ ���� �����ϰ�� ��� �±װ� ���� �����Ҷ�	*
 *		�Լ��� ȣ�� �ǹǷ� �̰����� TAG_CONTENTS�� ���� �����Ѵ� ���߿�	*
 *		���Ͽ��� �ѱ��ھ� �о� �鿩�� ������ �´� ó���� �Ѵ�.			*
 *----------------------------------------------------------------------*/
int CLBPackageParser::ReadFile(LPPARSER_RESULT lpResult, LPTAG_CONTENTS lpParent)
{
	char				ch;
	char				szBuff[MAX_BUFFER_SIZE];
	int					Index;
	LPTAG_CONTENTS		lpContents;
	int					nCurrentState = START;
	bool				bInQuter = false;
	bool				bExit = false;

	int					pos;
	pos = GetPos();

	lpContents = (LPTAG_CONTENTS)malloc(sizeof(TAG_CONTENTS));
	MakeNewList(lpContents);
	InitTagContents(lpParent, lpContents);
	
	Index = 0;
	do{
		ch = GetCh();
		if(IsToken(ch, nCurrentState, bInQuter))/* ������ ���ڰ� Token������ �˻��Ѵ�. ���ڸ��� �������� ���� �ƴ϶�	*/
		{										/* ���� �Ľ��ϰ� �ִ� ���� � �������� ������ �Ǻ��Ѵ�.				*/
			switch(ch)
			{
			case LANGLEBR:		/* '<' */
				OnLAngleBR(lpResult, &nCurrentState, lpContents, szBuff, &Index, lpContents->lpParent, &bExit);
				break;
			case RANGLEBR:		/* '>' */
				OnRAngleBR(lpResult, &nCurrentState, lpContents, szBuff, &Index);
				break;
			case QUOT:			/* ' " ' */
				bInQuter = SetInQuter(bInQuter);
				break;
			case LF:			/* '\r' */
			case CR:			/* '\n' */
			case TAB:			/* '\t' */
				break;
			case SPACE:			/* ' ' */
				OnWhiteSpace(lpResult, &nCurrentState, lpContents, szBuff, &Index, &bExit);
				break;
			case EQUAL:			/* '=' */
				OnEqual(&nCurrentState, lpContents, szBuff, &Index);
				break;
			case QUESTION:		/* '?' */
			case ENDTAG:		/* '/' */
				OnEndTag(lpResult, &nCurrentState, lpContents, szBuff, &Index, lpContents->lpParent, &bExit);
				break;
			}
		}
		else
			szBuff[Index++] = ch;	/* ��ū�� �ƴ� ��쿡�� szBuff�� �����Ѵ�.	*/
		
		if( bExit == true)			/* ��� �Լ��� ������ ��� ��������(�±׸� ��� �� �о�����)	*/
		{							/* ���� ���� ����Ʈ�� �����ϰ� �Լ��� ���´�					*/
			pos = GetPos();
			return pos;
		}

	}while(ch != EOF);

	return pos;
}

/*----------------------------------------------------------------------*
 *		�ܺο��� �Լ����� �Ľ��� ���ؼ� ȣ���ϴ� �Լ�					*
 *		���ڷ� LPPARSER_RESULT�� �־��ָ� �޸� allocation�� �� �Լ�	*
 *		���� �ϹǷ� �����͸� �־��ָ� �ȴ�. �����̸��� �Բ� �־��ش�.	*
 *----------------------------------------------------------------------*/
bool CLBPackageParser::StartParsing(char *szFileName, LPPARSER_RESULT lpResult)
{
	bool			bRtn = false;
#ifdef DISABLE_MERGED_FILE
	FILE			*XMLFILE;
#endif //DISABLE_MERGED_FILE

#ifdef DISABLE_MERGED_FILE
	if((XMLFILE = fopen(szFileName, ("rt"))) == NULL)
#else //DISABLE_MERGED_FILE
	if(!ser_fopen2mem_with_null(szFileName, &m_pszRawData))
#endif //DISABLE_MERGED_FILE
	{
		/*
#ifndef _WINCE
		assert(0);
#endif*/	// Commented by kyu nam - ���� ������ �����ؾ� Assert�� �� ���� ���� -_-;
		return bRtn;
	}

#ifdef DISABLE_MERGED_FILE
	MakeRawData(XMLFILE);
#else //DISABLE_MERGED_FILE
	m_nRawPos = 0;
	m_nRawDataSize = strlen(m_pszRawData);
#endif //DISABLE_MERGED_FILE


	if(lpResult == NULL)
	{
#ifndef _WINCE
		assert(0);
#endif //_WINCE
		return bRtn;
	}

#ifdef CHECK_WELLFORMED_N_VALIDATATION

	CLBPackageParserXMLCheck		XMLChecker;

	switch(XMLChecker.CheckXMLByStream(m_pszRawData))		/* XML ������ Well-Formed ���� Valid������ �˻��ϴ� �Լ�, ���� �������� �ʾ���	*/
	{
	case NOTWELLFORMED:
		/* validation, well formed üũ���� ��� ���� ������ ���� �ڵ鷯 �߰� ��	*/
#ifdef DISABLE_MERGED_FILE
		fclose(XMLFILE);
#else //DISABLE_MERGED_FILE
		ser_fclose2mem(m_pszRawData);
#endif //DISABLE_MERGED_FILE
		return bRtn;
		break;
//	case INVALID:
//		/* validation üũ���� ��� ���� ������ ���� �ڵ鷯 �߰� ��	*/
//		fclose(XMLFILE);
//		return bRtn;
//		break;
// Commented by kyu nam 20010530 Validation Check�� ���� �������� �ʾ���
	case WELLANDVALID:
		break;
	}

#endif //CHECK_WELLFORMED_N_VALIDATATION


	bRtn = StartReading(lpResult);

	if(m_pszRawData)
	{
#ifdef DISABLE_MERGED_FILE
		free(m_pszRawData);
		m_pszRawData = NULL;
#endif //DISABLE_MERGED_FILE
		m_nRawPos = 0;
		m_nRawDataSize = 0;
	}

		
#ifdef DISABLE_MERGED_FILE
	fclose(XMLFILE);
#else //DISABLE_MERGED_FILE
	ser_fclose2mem(m_pszRawData);
	m_pszRawData = NULL;
#endif //DISABLE_MERGED_FILE

	return bRtn;
}

/*----------------------------------------------------------------------*
 *		�Ľ��� ������ ����(ReadFile() �����Ŀ�) XML_ROOT��				*
 *		PARSER_RESULT�� XML_ROOT����� ���� ��Ű�� �Լ�					*
 *----------------------------------------------------------------------*/
bool CLBPackageParser::StartReading(LPPARSER_RESULT lpResult)
{
	TAG_CONTENTS	XML_ROOT;
	bool			bRtn = false;
	
	XML_ROOT.nAttr = 0;
	XML_ROOT.nNested = 0;
#ifndef MALLOC_BY_SIZE
	strcpy(XML_ROOT.szContents, "");
	strcpy(XML_ROOT.szTag, "XML ROOT");
#else
	XML_ROOT.szContents = NULL;
	XML_ROOT.szTag = NULL;
#endif
	XML_ROOT.lpParent = NULL;

	strcpy(lpResult->szStyleSheetType, (""));
	strcpy(lpResult->szDocType, (""));
	strcpy(lpResult->szXMLVersion, (""));
	strcpy(lpResult->szEncoding, (""));
	strcpy(lpResult->szStandAlone, (""));

#ifndef MALLOC_BY_SIZE
	strcpy(lpResult->szDTDName, "");
	strcpy(lpResult->szDTDURL, "");
	strcpy(lpResult->szRootTAg, "");
	strcpy(lpResult->szStyleHref, "");
#else
	lpResult->szDTDName = NULL;
	lpResult->szDTDURL = NULL;
	lpResult->szRootTAg = NULL;
	lpResult->szStyleHref = NULL;
#endif
	
	MakeNewList(&XML_ROOT);
	ReadFile(lpResult, &XML_ROOT);

	lpResult->XML_ROOT = XML_ROOT.lpHead->next->Tag;

#ifdef MALLOC_BY_SIZE
	free(XML_ROOT.szContents);
	free(XML_ROOT.szTag);
#endif

	if(XML_ROOT.nNested)
	{
		free(XML_ROOT.lpHead->next);
		bRtn = true;
	}
	free(XML_ROOT.lpHead);
	free(XML_ROOT.lpTail);

	return bRtn;
}

void CLBPackageParser::RemoveNode(LPTAG_CONTENTS lpNode)
{
	int					nCount;
	LPNESTED_TAG		lpChild, lpRemove;
	LPTAG_CONTENTS		lpChildContents;

	nCount = lpNode->nNested;
	if( nCount > 0)
	{
		lpChild = lpNode->lpHead->next;
		
		while(lpChild != lpNode->lpTail)
		{
			lpChildContents = lpChild->Tag;
			RemoveNode(lpChildContents);

			lpRemove = lpChild;
			lpChild = lpChild->next;

			free(lpRemove);
		}
	}

	free(lpNode->lpHead);
	free(lpNode->lpTail);
	
	lpNode->lpParent = NULL;
	lpNode->lpHead = NULL;
	lpNode->lpTail = NULL;

				
#ifdef MALLOC_BY_SIZE
	for( int i = 0 ; i < 10 ; i ++ )
	{
		if(lpNode->szAttr[i])
			free(lpNode->szAttr[i]);
		if(lpNode->szValue[i])
			free(lpNode->szValue[i]);
	}
	if(lpNode->szContents)
		free(lpNode->szContents);
	if(lpNode->szTag)
		free(lpNode->szTag);
#endif

	free(lpNode);
	lpNode = NULL;
}

void CLBPackageParser::RemoveAllNode(LPPARSER_RESULT lpResult)
{
	LPTAG_CONTENTS		lpRoot;

	lpRoot = lpResult->XML_ROOT;
	RemoveNode(lpRoot);
#ifdef MALLOC_BY_SIZE
	free(lpResult->szStyleHref);
	free(lpResult->szDTDName);
	free(lpResult->szDTDURL);
	free(lpResult->szRootTAg);
#endif
}

/*----------------------------------------------------------------------*
 *		TAG_CONTENTS���� NESTED TAG�� �����ϱ� ���� ����Ʈ���� �Լ���	*
 *		������ ���Ϸ� ���� �и� �ؾ� ��								*
 *																		*
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*
 *		TAG CONTENTS�� ����Ʈ�� ���ִ� �Լ� XML_ROOT ��Ʈ��ó����		*
 *		��ġ�ϸ鼭 ��� ���ִ� �Լ��� �ʿ��ϴ�. ���� �� �Լ��� ȣ��		*
 *		���� �ʰ� �ִ�.													*
 *----------------------------------------------------------------------*/
void CLBPackageParser::RelaseAll(LPTAG_CONTENTS lpContents)
{
	LPNESTED_TAG	lpRemove, lpData;
	LPNESTED_TAG	lpHead, lpTail;
	
	lpHead = lpContents->lpHead;
	lpTail = lpContents->lpTail;

	lpData = lpHead->next;

	while(lpData != lpTail)
	{
		lpRemove = lpData;
		lpData = lpData->next;

		free(lpRemove);
		lpRemove = NULL;
	}

	free(lpHead);
	lpHead = NULL;

	free(lpTail);
	lpTail = NULL;
}

/*----------------------------------------------------------------------*
 *		TAG CONTENTS�� ���� ����� ������ ����Ʈ�� �ʱ�ȭ ���ִ� �Լ�	*
 *----------------------------------------------------------------------*/
void CLBPackageParser::MakeNewList(LPTAG_CONTENTS lpContents)
{
	LPNESTED_TAG	lpHead = NULL;
	LPNESTED_TAG	lpTail = NULL;

	lpHead = (LPNESTED_TAG)malloc(sizeof(NESTED_TAG));
	lpTail = (LPNESTED_TAG)malloc(sizeof(NESTED_TAG));

	if(lpHead == NULL)
	{
#ifndef _WINCE
		assert(0);
#endif
		exit(1);
	}

	if(lpTail == NULL)
	{
#ifndef _WINCE
		assert(0);
#endif
		exit(1);
	}

	lpHead->next = lpTail;
	lpHead->prev = NULL;
	lpHead->Tag = NULL;

	lpTail->next = NULL;
	lpTail->prev = lpHead;
	lpTail->Tag = NULL;

	lpContents->lpHead = lpHead;
	lpContents->lpTail = lpTail;
#ifdef MALLOC_BY_SIZE
	for( int i = 0 ; i < 10 ; i ++ )
	{
		lpContents->szAttr[i] = NULL;
		lpContents->szValue[i] = NULL;
	}
	lpContents->szContents = NULL;
	lpContents->szTag = NULL;
#endif
}

/*----------------------------------------------------------------------*
 *		NESTED TAG�� TAG CONTENTS�� �߰� ��Ű�� �Լ�					*
 *----------------------------------------------------------------------*/
void CLBPackageParser::AddList(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpTag)
{

	LPNESTED_TAG	lpTail = lpParent->lpTail;
	LPNESTED_TAG	lpData = (LPNESTED_TAG)malloc(sizeof(NESTED_TAG));

	if(lpTag == NULL)
	{
#ifndef _WINCE
		assert(0);
#endif
		exit(1);
	}
	lpData->Tag = lpTag;
	
	lpData->next = lpTail;
	lpData->prev = lpTail->prev;

	lpTail->prev->next = lpData;
	lpTail->prev = lpData;
}


void CLBPackageParser::InitTagContents(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpContents)
{
	int				i;

#ifndef MALLOC_BY_SIZE
	strcpy(lpContents->szTag, "");
	strcpy(lpContents->szContents, "");

	for( i = 0 ; i < 10 ; i ++ )
	{
		strcpy(lpContents->szAttr[i], "");
		strcpy(lpContents->szValue[i], "");
	}
#else
	lpContents->szTag = NULL;
	lpContents->szContents = NULL;

	for( i = 0 ; i < 10 ; i ++ )
	{
		lpContents->szAttr[i] = NULL;
		lpContents->szValue[i] = NULL;
	}
#endif
	lpContents->lpParent = lpParent;
	lpContents->nType = HAVECONTENTS;

	lpContents->nAttr = 0;
	lpContents->nNested = 0;
	

}

BOOL CLBPackageParser::RemoveTag(const XML_POSITION pos)
{
	LPTAG_CONTENTS		lpParent = pos.lpTag->lpParent;
	LPNESTED_TAG		lpNested = NULL;
	int					i;


	lpNested = lpParent->lpHead->next;
	for( i = 0 ; i < lpParent->nNested ; i ++ )
	{
		if(pos.lpTag == lpNested->Tag)
		{
			lpNested->prev->next = lpNested->next;
			lpNested->next->prev = lpNested->prev;

			RemoveNode(lpNested->Tag);

			lpNested->Tag = NULL;

			free(lpNested);
			lpNested= NULL;

			

			lpParent->nNested--;
			break;
		}
		lpNested = lpNested->next;
	}

	return TRUE;
}



BOOL CLBPackageParser::CheckFileState(FILE *XML)
{
	long				nSize = 0;
	BOOL				bRtn = FALSE;
	char				szTemp[256];
	char				chTemp;
	int					nLen, i;
	char				szXMLHeader[] = ("<?xml version=\"1.0\"");
	
	Seek(0, _SEEK_END);
	
	nSize = ftell(XML);
	if(nSize < 100 )
		return bRtn;

	Seek(0, _SEEK_SET);
	fgets(szTemp, sizeof(szTemp), XML);

	nLen = strlen(szXMLHeader);

	for( i = 0 ; i < nLen - 1 ; i ++ )
	{
		chTemp = szTemp[i];
		if(chTemp != szXMLHeader[i])
			return bRtn;
	}

	Seek(0, _SEEK_SET);
	bRtn = TRUE;

	return bRtn;
}

char CLBPackageParser::GetCh()
{
	char		chRtn = '\0';

	if(m_nRawPos >= m_nRawDataSize)
		return EOF;

	if(m_pszRawData)
		chRtn = m_pszRawData[m_nRawPos++];

	return chRtn;
}

void CLBPackageParser::Seek(int nPos, int nStartPoint)
{
	switch(nStartPoint)
	{
	case _SEEK_SET:
		m_nRawPos = 0;
		break;
	case _SEEK_END:
		m_nRawPos = m_nRawDataSize;
		break;
	}

	m_nRawPos += nPos;
}


void CLBPackageParser::MakeRawData(FILE *file)
{
	int			nSize = 0;
	fpos_t		pos;
	int			i;

	if(m_pszRawData)
	{
		free(m_pszRawData);
		m_pszRawData = NULL;
	}

	fseek(file, 0L, SEEK_END);
	fgetpos(file, &pos);

	nSize = (int)pos;
	m_nRawDataSize = nSize;

	m_pszRawData = (char*)malloc(nSize * sizeof(TCHAR) + 1);

	fseek(file, 0L, SEEK_SET);
	for( i = 0 ; i < nSize ; i ++ )
		m_pszRawData[i] = fgetc(file);

	fseek(file, 0L, SEEK_SET);

	m_pszRawData[i] = '\0';
	m_nRawPos = 0;
}

bool CLBPackageParser::StartParsingByStream(char *pszStream, LPPARSER_RESULT lpResult)
{
	bool			bRtn = false;
	
	if(pszStream == NULL)
		return bRtn;
	else
	{
		m_pszRawData = pszStream;
		m_nRawPos = 0;
		m_nRawDataSize = strlen(pszStream);
	}


	if(lpResult == NULL)
		return bRtn;

#ifdef CHECK_WELLFORMED_N_VALIDATATION

	CLBPackageParserXMLCheck		XMLChecker;

	switch(XMLChecker.CheckXMLByStream(m_pszRawData))		/* XML ������ Well-Formed ���� Valid������ �˻��ϴ� �Լ�, ���� �������� �ʾ���	*/
	{
	case NOTWELLFORMED:
		return bRtn;
		break;
	case WELLANDVALID:
		break;
	}

#endif //CHECK_WELLFORMED_N_VALIDATATION

	bRtn = StartReading(lpResult);

	if(pszStream)
	{
		m_pszRawData = NULL;
		m_nRawPos = 0;
		m_nRawDataSize = 0;
	}

	return bRtn;
}
