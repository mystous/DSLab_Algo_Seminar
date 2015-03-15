// LBPackageParser.h: interface for the CLBPackageParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LBPACKAGEPARSER_H__449471D4_DCAE_4C94_8035_1244E5C3AC01__INCLUDED_)
#define AFX_LBPACKAGEPARSER_H__449471D4_DCAE_4C94_8035_1244E5C3AC01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WINCE
	#include <assert.h>
#endif

//#include "DataStruct.h"									/* 파서의 데이타 구조를 정의 한 헤더파일 XML문서의		*/
//#include "XMLCheck.h"									/* 무결성과 유효성을 검사하는 루틴						*/

#define START			1								/* ReadFile() 함수 안에서 현재 어떤 상태이지를 나타내는 */
#define NEW_TAG			2								/* 매크로 정의 쉬프트를 통해서 이동할 수 있다.			*/
#define TAG_NAME		4								/* START = 초기 상태, NEW_TAG = 새 태크 시작			*/
#define ATTRIBUTE		8								/* TAG_NAME = 태크 이름 검색, ATTRIBUTE = 태그의 속성	*/
#define ATTR_VALUE		16								/* 검색, ATTR_VALUE = 속성 값 검색 CONTENTS = 컨텐츠값	*/
#define CONTENTS		32								/* END_TAG = 태그의 끝									*/
#define END_TAG			64
#define OUT_OF_TAG		128

#define QUOT		'"'
#define SPACE		' '
#define LANGLEBR	'<'
#define RANGLEBR	'>'
#define EQUAL		'='
#define TAB			'\t'							// Commented by kyu nam 20010224 redefine in nsCRT.h
#define LF			'\n'
#define CR			'\r'

#define COMMENT		"!--"
#define ENDCOMMENT	"--"

#define ENDTAG		'/'
#define EMOTION		'!'
#define QUESTION	'?'

#define EOS			'\0'

// Seek에서 StartPoint로 사용될 매크로
#define _SEEK_SET		0
#define _SEEK_CUR		1
#define _SEEK_END		2



/*-----------------------------------------------------------*
 *     각 테그가 갔는 자식 태그를 리스트로 만들때 리스트의   *
 *     원소가 되는 구조체									 *
 *-----------------------------------------------------------*/
typedef struct _tagNest{
	struct _tagNest		*next;
	struct _tagNest		*prev;
	struct _tagTag		*Tag;
} NESTED_TAG, *LPNESTED_TAG;


#define HAVECHILD		1
#define EMPTYTAG		2
#define HAVECONTENTS	4

/*-----------------------------------------------------------*
 *     파서가 하나의 태그에 대한 정보를 저장하는 구조체      *
 *-----------------------------------------------------------*/
typedef struct _tagTag{
	struct _tagTag		*lpParent;
#ifndef MALLOC_BY_SIZE
	char				szTag[256];
	char				szAttr[10][256];
	char				szValue[10][256];
	char				szContents[256];
#else
	char				*szTag;
	char				*szAttr[10];
	char				*szValue[10];
	char				*szContents;
#endif
	int					nType;
	int					nAttr;
	int					nNested;
	struct _tagNest		*lpHead;
	struct _tagNest		*lpTail;
} TAG_CONTENTS, *LPTAG_CONTENTS;

/*-----------------------------------------------------------*
 *     다른 함수에서 인자로 들어온 PARSER_RESULT 구조체에    *
 *     파싱 결과를 모두 집어 넣어서 보내준다.                *
 *-----------------------------------------------------------*/
typedef struct _tagParser{
	char				szXMLVersion[10];
	char				szEncoding[50];
	char				szStandAlone[4];
	char				szStyleSheetType[10];
	char				szDocType[10];
#ifndef MALLOC_BY_SIZE
	char				szStyleHref[256];
	char				szDTDName[256];
	char				szDTDURL[256];
	char				szRootTAg[256];
#else
	char				*szStyleHref;
	char				*szDTDName;
	char				*szDTDURL;
	char				*szRootTAg;
#endif
	struct _tagTag		*XML_ROOT;
} PARSER_RESULT, *LPPARSER_RESULT;

typedef struct _tagPos{
	struct _tagTag		*lpTag;
	int					nDepth;
	bool				bRoot;
	bool				bLeaf;
	int					nChild;
} XML_POSITION, *LPXML_POSITION;


class CLBPackageParser  
{
public:
	bool StartParsingByStream(char *pszStream, LPPARSER_RESULT lpResult);
	BOOL CheckFileState(FILE *XML);
	CLBPackageParser();
	virtual ~CLBPackageParser();
	bool StartParsing(char *szFileName, LPPARSER_RESULT lpResult);						/* 파싱을 시작하는 함수 외부에서 콜되며 다른 모듈에서 호출하게 된다.	*/
	void RemoveAllNode(LPPARSER_RESULT lpResult);										/* 메모리에 만들어진 XML 구조를 모두 제거한다							*/
	BOOL RemoveTag(const XML_POSITION pos);

private:
	void MakeRawData(FILE *file);
	int GetPos(){ return m_nRawPos; };
	void SetPos(int nPos){ m_nRawPos = nPos; };
	void Seek(int nPos, int nStartPoint);
	char GetCh();
	char *m_pszRawData;
	int m_nRawPos;
	int m_nRawDataSize;
	void InitTagContents(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpContents);
	//FILE *XMLFILE;
	//fpos_t ReadFile(LPPARSER_RESULT lpResult, LPTAG_CONTENTS lpParent);					/* 파싱을 위한 준비 작업을 마친후 파일을 읽기 시작하는 함수				*/
	int ReadFile(LPPARSER_RESULT lpResult, LPTAG_CONTENTS lpParent);					/* 파싱을 위한 준비 작업을 마친후 파일을 읽기 시작하는 함수				*/
	bool IsToken(char ch, int Current, bool bInQuter);									/* 지금 읽어 들인 문자가 토큰인지 아닌지를 검사하는 함수				*/
	bool SetInQuter(bool bInQuter);														/* 현재 따옴표 안인지 밖인지를 구별해 주는 함수							*/
	void GetDOCTYPEData(LPPARSER_RESULT lpResult);										/* XML 문서의 DOCTYPE정보를 얻어 와서 LPPARSER_RESULT에 저장한다.		*/
	void GetXMLData(LPPARSER_RESULT lpResult);											/* XML 문서의 데이타 Version등의 정보를 가져와서 LPPARSER_RESULT에 저장	*/
	void OnWhiteSpace(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, bool *bExit);
																						/* 토큰이 빈칸인 경우 처리 함수											*/
	void OnLAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, LPTAG_CONTENTS lpParent, bool *bExit);
																						/* 토큰이 '<' 인 경우 처리 함수											*/
	void OnEqual(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);	/* 토큰이 '=' 인 경우 처리 함수											*/
	void OnRAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);
																						/* 토큰이 '>' 인 경우 처리 함수											*/
	void OnEndTag(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, LPTAG_CONTENTS lpParent, bool *bExit);
																						/* 토큰이 '/' 인 경우 처리 함수											*/

	void SetAttrValue(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);	/* Attribute Value를 저장하는 함수									*/
	void SetAttribute(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);	/* Attribute를 저장하는 함수										*/
	void SetTag(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);		/* 읽어 들인 태그 정보를 TAG_CONTENTS에 저장한다.					*/
	void ReadStrByToken(char *szTemp, char Token);										/* 인자로 주어진 글자가 있는 곳까지 읽어서 szTemp에 저장한다.			*/
	void OneTagParsingComplete(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);
																						/* 하나의 테그에 대한 파싱이 끝났을때									*/
	void AddToParent(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpContents, bool *bExit);	/* 현재 태그 정보를 부모의 리스트에 포함 시킨다.						*/


	void MakeNewList(LPTAG_CONTENTS lpContents);				/* 새로운 리스트를 생성하는 함수							*/
	void AddList(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpTag);/* 리스트에 새로운 자식 테그를 추가 할때					*/
	void RelaseAll(LPTAG_CONTENTS lpContents);					/* 리스트에서 모든 원소 제거								*/
	bool StartReading(LPPARSER_RESULT lpResult);				/* 재귀 함수 호출 때문에 1단계 레벨에서						*/
																/* 리턴되는 것을 막기 위해서 호출되는 함수					*/
	char MoveToNextChar(char CHAR);								/* 현재 위치에서 Space가 아닌 다음 문자까지 움직이는 함수	*/
	void ReadComment();											/* 파일 포인터를 코멘트의 끝까지 움직이는 함수				*/
	void RemoveNode(LPTAG_CONTENTS lpNode);						/* 하나의 노드를 제거 하는 함수 자식이 있을 경우에는 재귀적	*/
																/* 으로 호출 된다.											*/
};

#endif // !defined(AFX_LBPACKAGEPARSER_H__449471D4_DCAE_4C94_8035_1244E5C3AC01__INCLUDED_)
