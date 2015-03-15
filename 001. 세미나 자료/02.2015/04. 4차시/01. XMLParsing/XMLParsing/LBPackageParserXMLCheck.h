// LBPackageParserXMLCheck.h: interface for the CLBPackageParserXMLCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LBPACKAGEPARSERXMLCHECK_H__8254E47A_17AB_4C0E_8792_83A379CD8768__INCLUDED_)
#define AFX_LBPACKAGEPARSERXMLCHECK_H__8254E47A_17AB_4C0E_8792_83A379CD8768__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <malloc.h>
#ifndef _WINCE
	#include <assert.h>
#endif
#include <string.h>
#include <stdlib.h>

#include "LBPackageParser.h"

#define NOTWELLFORMED	1					/* Well Formed도 아니고 Valid하지도 않을때		*/
#define	INVALID			2					/* Validation Check에서 오류가 났을때			*/
#define WELLANDVALID	3					/* Well Formed & Valid할 때						*/
#define WELLFORMED		4					/* Well Formed 일 때							*/
#define VALID			5					/* Valid 할 때									*/

typedef struct _tagTagStack{				/* 현재 어떤 태그 인지를 저장하는 스택의 원소	*/
	struct _tagTagStack			*prev;
	struct _tagTagStack			*next;
	char						szTag[256];
} STACK_ELEMENT, *LPSTACK_ELEMENT;


class CLBPackageParserXMLCheck  
{
public:
	int CheckXMLByStream(char *pszStream);
	int CheckXML(char *szXML);						/* XML Check가 시작되는 함수				*/
	CLBPackageParserXMLCheck();
	virtual ~CLBPackageParserXMLCheck();

private:
	LPSTACK_ELEMENT		lpHead, lpTail;
	//FILE				*XML;
	FILE				*DTD;
	int					gnCount;
	bool				gbQouter;
	void MakeRawData(FILE *file);
	int GetPos(){ return m_nRawPos; };
	void SetPos(int nPos){ m_nRawPos = nPos; };
	void Seek(int nPos, int nStartPoint);
	char GetCh();
	char *m_pszRawData;
	int m_nRawPos;
	int m_nRawDataSize;
	int CheckValidation(char *szXML);
	bool SetQouter(bool bIn);						/* " " 로 둘러 쌓여져 있는지 아닌지를 셋팅 하는 함수		*/
	char MoveToNonSpaceChar(char CHAR);				/* 현재 위치에서 Space가 아닌 다음 문자까지 움직이는 함수	*/
	/*int OnTagStart(char preCh);						/* '<'를 만났을때							*/
	int OnTagStart(bool *bExit);						/* '<'를 만났을때							*/
	int ReadOneTag(char *szBuff,bool *bExit);		/* < > 로 둘러 쌓인 하나의 테그를 처리 할때	*/
	int OnTagEnd(char preCh);						/* '>'를 만났을때							*/
	int OnSlash(char preCh);						/* '/'를 만났을때							*/
	/*int StartFormCheck();							/* WellFormed Check를 위해서 파일 읽기 시작	*/
	//int StartFormCheck(fpos_t *fpos);				/* WellFormed Check를 위해서 파일 읽기 시작	*/
	int StartFormCheck(int *fpos);				/* WellFormed Check를 위해서 파일 읽기 시작	*/
	void InitStack();								/* 스택의 초기화							*/
	void EmptyStack();								/* 스택 비우기								*/
	void PushStack(char *szTag);					/* 스택에 집어 넣기							*/
	void PopStack(char *szTag);						/* 스택에서 꺼내기							*/
	int CheckWellFormed();							/* Well Formed인지를 검사의 시작			*/
	//fpos_t ReadByToken(char *szTemp, char Token);		/* 주어진 Token까지 파일에서 읽어 들인다	*/
	int ReadByToken(char *szTemp, char Token);		/* 주어진 Token까지 파일에서 읽어 들인다	*/
	int ReadXMLInfoPart();							/* <?xml?> , <?xml-stylesheet?> 부분 처리	*/
	int ReadXMLDocTypePart();						/* <!DOCTYPE> 부분 처리						*/
	int ReadXMLComment();							/* 주석 부분 처리							*/

};

#endif // !defined(AFX_LBPACKAGEPARSERXMLCHECK_H__8254E47A_17AB_4C0E_8792_83A379CD8768__INCLUDED_)
