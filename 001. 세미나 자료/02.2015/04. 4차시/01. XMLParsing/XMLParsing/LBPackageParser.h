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

//#include "DataStruct.h"									/* �ļ��� ����Ÿ ������ ���� �� ������� XML������		*/
//#include "XMLCheck.h"									/* ���Ἲ�� ��ȿ���� �˻��ϴ� ��ƾ						*/

#define START			1								/* ReadFile() �Լ� �ȿ��� ���� � ���������� ��Ÿ���� */
#define NEW_TAG			2								/* ��ũ�� ���� ����Ʈ�� ���ؼ� �̵��� �� �ִ�.			*/
#define TAG_NAME		4								/* START = �ʱ� ����, NEW_TAG = �� ��ũ ����			*/
#define ATTRIBUTE		8								/* TAG_NAME = ��ũ �̸� �˻�, ATTRIBUTE = �±��� �Ӽ�	*/
#define ATTR_VALUE		16								/* �˻�, ATTR_VALUE = �Ӽ� �� �˻� CONTENTS = ��������	*/
#define CONTENTS		32								/* END_TAG = �±��� ��									*/
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

// Seek���� StartPoint�� ���� ��ũ��
#define _SEEK_SET		0
#define _SEEK_CUR		1
#define _SEEK_END		2



/*-----------------------------------------------------------*
 *     �� �ױװ� ���� �ڽ� �±׸� ����Ʈ�� ���鶧 ����Ʈ��   *
 *     ���Ұ� �Ǵ� ����ü									 *
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
 *     �ļ��� �ϳ��� �±׿� ���� ������ �����ϴ� ����ü      *
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
 *     �ٸ� �Լ����� ���ڷ� ���� PARSER_RESULT ����ü��    *
 *     �Ľ� ����� ��� ���� �־ �����ش�.                *
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
	bool StartParsing(char *szFileName, LPPARSER_RESULT lpResult);						/* �Ľ��� �����ϴ� �Լ� �ܺο��� �ݵǸ� �ٸ� ��⿡�� ȣ���ϰ� �ȴ�.	*/
	void RemoveAllNode(LPPARSER_RESULT lpResult);										/* �޸𸮿� ������� XML ������ ��� �����Ѵ�							*/
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
	//fpos_t ReadFile(LPPARSER_RESULT lpResult, LPTAG_CONTENTS lpParent);					/* �Ľ��� ���� �غ� �۾��� ��ģ�� ������ �б� �����ϴ� �Լ�				*/
	int ReadFile(LPPARSER_RESULT lpResult, LPTAG_CONTENTS lpParent);					/* �Ľ��� ���� �غ� �۾��� ��ģ�� ������ �б� �����ϴ� �Լ�				*/
	bool IsToken(char ch, int Current, bool bInQuter);									/* ���� �о� ���� ���ڰ� ��ū���� �ƴ����� �˻��ϴ� �Լ�				*/
	bool SetInQuter(bool bInQuter);														/* ���� ����ǥ ������ �������� ������ �ִ� �Լ�							*/
	void GetDOCTYPEData(LPPARSER_RESULT lpResult);										/* XML ������ DOCTYPE������ ��� �ͼ� LPPARSER_RESULT�� �����Ѵ�.		*/
	void GetXMLData(LPPARSER_RESULT lpResult);											/* XML ������ ����Ÿ Version���� ������ �����ͼ� LPPARSER_RESULT�� ����	*/
	void OnWhiteSpace(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, bool *bExit);
																						/* ��ū�� ��ĭ�� ��� ó�� �Լ�											*/
	void OnLAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, LPTAG_CONTENTS lpParent, bool *bExit);
																						/* ��ū�� '<' �� ��� ó�� �Լ�											*/
	void OnEqual(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);	/* ��ū�� '=' �� ��� ó�� �Լ�											*/
	void OnRAngleBR(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);
																						/* ��ū�� '>' �� ��� ó�� �Լ�											*/
	void OnEndTag(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index, LPTAG_CONTENTS lpParent, bool *bExit);
																						/* ��ū�� '/' �� ��� ó�� �Լ�											*/

	void SetAttrValue(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);	/* Attribute Value�� �����ϴ� �Լ�									*/
	void SetAttribute(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);	/* Attribute�� �����ϴ� �Լ�										*/
	void SetTag(int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);		/* �о� ���� �±� ������ TAG_CONTENTS�� �����Ѵ�.					*/
	void ReadStrByToken(char *szTemp, char Token);										/* ���ڷ� �־��� ���ڰ� �ִ� ������ �о szTemp�� �����Ѵ�.			*/
	void OneTagParsingComplete(LPPARSER_RESULT lpResult, int *nCurrent, LPTAG_CONTENTS lpContents, char *szBuff, int *Index);
																						/* �ϳ��� �ױ׿� ���� �Ľ��� ��������									*/
	void AddToParent(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpContents, bool *bExit);	/* ���� �±� ������ �θ��� ����Ʈ�� ���� ��Ų��.						*/


	void MakeNewList(LPTAG_CONTENTS lpContents);				/* ���ο� ����Ʈ�� �����ϴ� �Լ�							*/
	void AddList(LPTAG_CONTENTS lpParent, LPTAG_CONTENTS lpTag);/* ����Ʈ�� ���ο� �ڽ� �ױ׸� �߰� �Ҷ�					*/
	void RelaseAll(LPTAG_CONTENTS lpContents);					/* ����Ʈ���� ��� ���� ����								*/
	bool StartReading(LPPARSER_RESULT lpResult);				/* ��� �Լ� ȣ�� ������ 1�ܰ� ��������						*/
																/* ���ϵǴ� ���� ���� ���ؼ� ȣ��Ǵ� �Լ�					*/
	char MoveToNextChar(char CHAR);								/* ���� ��ġ���� Space�� �ƴ� ���� ���ڱ��� �����̴� �Լ�	*/
	void ReadComment();											/* ���� �����͸� �ڸ�Ʈ�� ������ �����̴� �Լ�				*/
	void RemoveNode(LPTAG_CONTENTS lpNode);						/* �ϳ��� ��带 ���� �ϴ� �Լ� �ڽ��� ���� ��쿡�� �����	*/
																/* ���� ȣ�� �ȴ�.											*/
};

#endif // !defined(AFX_LBPACKAGEPARSER_H__449471D4_DCAE_4C94_8035_1244E5C3AC01__INCLUDED_)
