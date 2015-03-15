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

#define CHECK_WELLFORMED_N_VALIDATATION			// Wellformed Check를 하지 않을 거면 주석으로 만든다.

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
 *		현재의 글자가 토큰인지 아닌지를 구별하는 함수					*
 *----------------------------------------------------------------------*/
bool CLBPackageParser::IsToken(char ch, int Current, bool bInQuter)
{
	bool bRtn = false;
	if(Current == ATTR_VALUE){		/* 현재 태그의 속성값을 읽고 있을 경우는 따로 생각하며		*/
		if(bInQuter){				/* 태그를 읽는 동안 2가지 모두가 있다. " " 로 둘러쌓인		*/ 
			if(ch == QUOT){			/* 부분에서는 ' " ' 만을 토큰으로 인식하고 그렇지 않은		*/
				bRtn = true;		/* 경우에는 ' " ', '>', ' ', '?', '/'를 토큰으로 인식한다	*/
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
			bRtn = true;		/* Contents를 읽고 있는 동안은 '<' 만을 토큰으로	*/
			return bRtn;		/* 사용하고 CR, LF, TAB은 무시한다. 하지만 szTemp에	*/
		}						/* 저장하지 않기 위해서 무시한다.					*/
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
 *		'=' 이 토큰으로 사용되었을때 속성을 읽어 드리고 있었다면		*
 *		속성을 모두 읽어 들였으므로 SetAttribute()를 사용해서 읽		*
 *		어들인 속성을 저장한다. 만약 속성값을 읽어 들이는 중간에		*
 *		'='이 사용되었다면 속성이 정의 되어 있지 않은 것으므로			*
 *		assert(0)로 알려 주고 프로그램을 종료 한다.						*
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
 *		읽어 들인 태크를 현재 TAG_CONTENTS 구조체의 szTag 에 저장한다.	*
 *		szTemp에 다른 값을 저장하기 위해 Index를 0으로 바꾸어 주고 현	*
 *		재 상태를 ATTRIBUT를 변경한다( *nCurrent <<= 1)					*
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
 *		읽어 들인 속성을 현재 TAG_CONTENTS 구조체의 szAttr 에 저장한다.	*
 *		szTemp에 다른 값을 저장하기 위해 Index를 0으로 바꾸어 주고 현	*
 *		재 상태를 ATTRI_VALUE를 변경한다( *nCurrent <<= 1)				*
 *		Attribute는 여러개 일 수 있으므로 nAttr로 현재 몇개의 속성이	*
 *		저장되었는지 구별하며 nAttr값은 속성값을 저장하고 증가시킨다.	*
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
 *		CR, LF, SPACE, TAB을 모두 OnWhiteSpace에서 처리 한다.			*
 *		태크 값, 속성, 속성값을 저장하는 토큰으로 사용된다.				*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnWhiteSpace(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, bool *bExit)
{
	char	ch;
#ifdef _HUNMIN
	ch = 0;
#endif

	switch(*nCurrent)
	{
	case TAG_NAME:												/* 태그 이름을 읽고 있을때	*/
		SetTag(nCurrent, lpContents, szBuff, Index);
		break;
	case ATTRIBUTE:												/* 속성을 읽고 있을때		*/
		if(*Index == 0)
			szBuff[*Index++] = MoveToNextChar(' ');				/* 아무겂도 읽지 않은 상태라면 빈칸을 모두 없애 버린다.		*/
		else													/* <tag attr     =    "Test">								*/
			SetAttribute(nCurrent, lpContents, szBuff, Index);	/*           ^^^^ ^^^^										*/
		break;													/* 그렇지 않다면 속성을 저장한다.							*/
	case ATTR_VALUE:											
		if(*Index == 0)											/* 속성값을 읽어 들일때도 마찬가지로 빈칸을 처리 한다.		*/
			MoveToNextChar(' ');
		else
			SetAttrValue(nCurrent, lpContents, szBuff, Index);
		break;
	case CONTENTS:												/* Contents를 읽고 있을 경우에는 빈칸을 모두 저장한다.		*/
		szBuff[*Index++] = ' ';									/* Contents를 읽고 있을 경우에는 CR, LF, TAB은 토큰으로		*/
		break;													/* 취급하지 않는다.											*/
	case OUT_OF_TAG:											
	case START:													/* 태그 밖에 있을 경우는 다음 '<'가 나올때까지 즉 다음 태그	*/
		while( ch != LANGLEBR)									/* 가 시작될때까지 빈칸을 모두 제거 한다.					*/
		{														/* Start는 특별하게 XML 문서를 처음으로 읽어 들일때 사용	*/
			//ch = fgetc(XMLFILE);									/* 되는 함수 이다.											*/
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
 *		Attribute의 값을 Contents 구조체로 저장한다.					*
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
 *		하나의 태그에 대한 파싱이 끝났을때의 처리						*
 *		더 정확하게 말해서는 시작 태그가 끝났을때						*
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
 *		하나의 태그에 대한 파싱이 끝났을때의 처리						*
 *		더 정확하게 말해서는 시작 태그가 끝났을때						*
 *		<tag attr = "test"> ... </tag>									*
 *                        ^												*
 *----------------------------------------------------------------------*/
void CLBPackageParser::OnRAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index)
{
	OneTagParsingComplete(lpResult, nCurrent, lpContents, szBuff, Index);
	*nCurrent = CONTENTS;
}

/*----------------------------------------------------------------------*
 *		현재 태그에 대한 정보 TAG_CONTENTS를 부모 TAG_CONTENTS에		*
 *		NESTED 시킨다.													*
 *----------------------------------------------------------------------*/
void CLBPackageParser::AddToParent(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpContents, bool *bExit)
{
	AddList(lpParent, lpContents);
	lpContents->lpParent->nNested ++;
	*bExit = true;												/* 재귀 함수가 끝났다는 것을 알리기 위한 플래그			*/
}

/*----------------------------------------------------------------------*
 *		'/' 토큰을 만났을때 처리 (패어로된 태그가 끝났을때)				*
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
 *		'<' 를 만났을때 새로운 태크를 시작하거나 NESTED TAG를 시작하는	*
 *		함수 NESTED함수의 경우 OnLAngleBR을 호출한 ReadFile을 다시 호출	*
 *		해서 새로운 태크를 읽어 들인다. <?xml ...?> <?xml-stylesheet?>	*
 *		<!DOCTYPE >의 경우 각각에 맞는 처리 함수를 호출한다.			*
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
		while(ch != RANGLEBR)							/* /EndTag> 가 파일에 남아 있으므로 파일 포인터를 이 뒤로 이동시킨다.	*/
			ch = GetCh();
			//ch = fgetc(XMLFILE);
		*nCurrent = OUT_OF_TAG;							
		AddToParent(lpParent, lpContents, bExit);		/* 부모 태그에 추가 시킨다. ROOT Tag의 경우 더미로 넣어준 XML_ROOT에	*/
		return;											/* 추가 시킨다.															*/
		break;

	case SPACE:											/* Space 이면 태그가 나올때까지 빈칸을 읽어 들인다.						*/
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
				ReadComment();				/* 주석의 경우 모두 무시한다.					*/
		}
		else if( ch == 'D')					/* For <!DOCTYPE> Tag							*/
			GetDOCTYPEData(lpResult);		/* <!DOCTYPE> 태그의 Atrribute를 읽어 들인다.	*/

		return;
		break;
	case QUESTION:							/* For <?xml?> <?xml-stylesheet?> Tag			*/
		GetXMLData(lpResult);				/* <?xml* ?>태그의 Attribute를 읽어 들인다.		*/
		return;
		break;
	}

	ch = MoveToNextChar(ch);				/* 처음 Switch 문의 SPACE가 공백을 모두 읽어	*/
											/* 들이고 있음 여기와 겹치는 것 같음			*/
	*Index = 0;
	lpContents->nAttr = 0;					/* Tag를 szBuff에 저장함						*/
	*nCurrent = TAG_NAME;
	szBuff[(*Index)++] = ch;
}

/*----------------------------------------------------------------------*
 *		파일 포인터를 주석의 다음으로 이동 시킴							*
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
 *		파일 포인터를 Space다음으로	이동 시킴							*
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
 *		인자로 주어진 토큰이 위치한 곳까지 파일에서 읽어 szTemp에		*
 *		저장한다. 토큰은 저장하지 않는다.								*
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
 *		<!DCOTYPE >에 있는 정보를 읽어서 PARSER_RESULT에 저장한다.		*
 *----------------------------------------------------------------------*/
void CLBPackageParser::GetDOCTYPEData(LPPARSER_RESULT lpResult)
{
	char		ch;
	char		szTemp[256];

	while((ch = GetCh()) != SPACE);

	ReadStrByToken(szTemp, SPACE);				/* ROOT Tag의 이름을 저장한다.												*/
#ifdef MALLOC_BY_SIZE
	if(lpResult->szRootTAg != NULL)
		free(lpResult->szRootTAg);
	lpResult->szRootTAg = (char*)malloc(strlen(szTemp) * sizeof(TCHAR)+1);
#endif
	strcpy(lpResult->szRootTAg, szTemp);
	
	ReadStrByToken(szTemp, SPACE);				/* Document Type을 저장한다.												*/
	strcpy(lpResult->szDocType, szTemp);

	if(!strcmp(szTemp, ("PUBLIC")))			/* DTD의 종류가 PUBLIC일 경우 "DTD Name"과 "DTD URL"을 읽어서 저장한다.		*/
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
 *		XML의 ROOT Tag가 나오기 전에 나오는 XML에 대한 데이타를 가져	*
 *		오는 루틴 <?xml version="1.0"?> 과 <?xml-styleshhet ... ?>에	*
 *		있는 내용을 가져 오는 루틴										*
 *----------------------------------------------------------------------*/
void CLBPackageParser::GetXMLData(LPPARSER_RESULT lpResult)
{
	char		ch;
	char		szTemp[256], szTempXML[256];
	
	ReadStrByToken(szTemp, SPACE);
	if( strcmp(szTemp, ("xml")) == 0 )					/* <?xml ...?>의 경우				*/
	{
		while((ch = GetCh()) != QUOT);					/* version을 읽지 않고 건너뜀		*/
		ReadStrByToken(szTemp, QUOT);					/* version의 Atrribute값을 읽음		*/
		strcpy(lpResult->szXMLVersion, szTemp);			/* PARSER_RESULT에 저장				*/

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
	else if( strcmp(szTemp, ("xml-stylesheet")) == 0 )	/* <?xml-stylesheet ...?>의 경우	*/
	{
		while((ch = GetCh()) != QUOT);					/* <?xml-stylesheet "DTD Type" "DTD HREF"?>		*/
		ReadStrByToken(szTemp, QUOT);					/* 에서 Type과 Herf를 읽어서 PARSER_RESULT에	*/
		strcpy(lpResult->szStyleSheetType, szTemp);		/*	저장										*/

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
 *		' " '를 만나면 다음에 나올 구문이 따옴표로 쌓여 있는지 아니면	*
 *		따옴표에서 벗어났는지를 알기 위한 플래그를 셋팅한다.			*
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
 *		실제적인 파싱을 시작하는 부분 NESTED TAG 를 만나면 재귀적으로	*
 *		호출이 된다. 리턴값 fpos_t는 현재 파일 포지션을 넘겨 주며 재귀	*
 *		적으로 호출이 되기 때문에 리턴하고 나면 호출되기 전에 파일 위치	*
 *		로 돌아 가게 된다. 따라서 리턴값을 가지고 재귀 함수가 호출이 되	*
 *		고 나서의 위치로 이동해야 한다. OnLAngleBR()함수에서 이 일을	*
 *		하게 된다.														*
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
 *		ROOT 태그가 나오기 전을 제외하고는 모든 태그가 새로 시작할때	*
 *		함수가 호출 되므로 이곳에서 TAG_CONTENTS를 새로 생성한다 나중에	*
 *		파일에서 한글자씩 읽어 들여서 각각에 맞는 처리를 한다.			*
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
		if(IsToken(ch, nCurrentState, bInQuter))/* 현재의 글자가 Token인지를 검사한다. 글자마다 절대적인 것이 아니라	*/
		{										/* 현재 파싱하고 있는 것이 어떤 것인지를 따져서 판별한다.				*/
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
			szBuff[Index++] = ch;	/* 토큰이 아닐 경우에는 szBuff에 저장한다.	*/
		
		if( bExit == true)			/* 재귀 함수의 할일이 모두 끝났을때(태그를 모두 다 읽었을때)	*/
		{							/* 현재 파일 포인트를 리턴하고 함수를 끝냈다					*/
			pos = GetPos();
			return pos;
		}

	}while(ch != EOF);

	return pos;
}

/*----------------------------------------------------------------------*
 *		외부에서 함수에서 파싱을 위해서 호출하는 함수					*
 *		인자로 LPPARSER_RESULT를 넣어주며 메모리 allocation은 이 함수	*
 *		에서 하므로 포인터만 넣어주면 된다. 파일이름도 함께 넣어준다.	*
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
#endif*/	// Commented by kyu nam - 없는 파일을 생성해야 Assert는 꼭 에러 같다 -_-;
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

	switch(XMLChecker.CheckXMLByStream(m_pszRawData))		/* XML 파일이 Well-Formed 인지 Valid한지를 검사하는 함수, 현재 구현되지 않았음	*/
	{
	case NOTWELLFORMED:
		/* validation, well formed 체크에서 모두 맞지 않을때 에러 핸들러 추가 요	*/
#ifdef DISABLE_MERGED_FILE
		fclose(XMLFILE);
#else //DISABLE_MERGED_FILE
		ser_fclose2mem(m_pszRawData);
#endif //DISABLE_MERGED_FILE
		return bRtn;
		break;
//	case INVALID:
//		/* validation 체크에서 모두 맞지 않을때 에러 핸들러 추가 요	*/
//		fclose(XMLFILE);
//		return bRtn;
//		break;
// Commented by kyu nam 20010530 Validation Check는 아직 구현되지 않았음
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
 *		파싱을 끝내고 나서(ReadFile() 실행후에) XML_ROOT를				*
 *		PARSER_RESULT의 XML_ROOT멤버에 대입 시키는 함수					*
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
 *		TAG_CONTENTS에서 NESTED TAG를 저장하기 위한 리스트관련 함수들	*
 *		별도의 파일로 만들어서 분리 해야 함								*
 *																		*
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*
 *		TAG CONTENTS의 리스트를 없애는 함수 XML_ROOT 스트럭처부터		*
 *		서치하면서 모두 없애는 함수가 필요하다. 현재 이 함수는 호출		*
 *		되지 않고 있다.													*
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
 *		TAG CONTENTS가 새로 만들어 졌을때 리스트를 초기화 해주는 함수	*
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
 *		NESTED TAG를 TAG CONTENTS에 추가 시키는 함수					*
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

	switch(XMLChecker.CheckXMLByStream(m_pszRawData))		/* XML 파일이 Well-Formed 인지 Valid한지를 검사하는 함수, 현재 구현되지 않았음	*/
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
