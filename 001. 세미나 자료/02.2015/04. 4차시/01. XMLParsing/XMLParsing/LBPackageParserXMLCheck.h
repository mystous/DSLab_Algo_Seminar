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

#define NOTWELLFORMED	1					/* Well Formed�� �ƴϰ� Valid������ ������		*/
#define	INVALID			2					/* Validation Check���� ������ ������			*/
#define WELLANDVALID	3					/* Well Formed & Valid�� ��						*/
#define WELLFORMED		4					/* Well Formed �� ��							*/
#define VALID			5					/* Valid �� ��									*/

typedef struct _tagTagStack{				/* ���� � �±� ������ �����ϴ� ������ ����	*/
	struct _tagTagStack			*prev;
	struct _tagTagStack			*next;
	char						szTag[256];
} STACK_ELEMENT, *LPSTACK_ELEMENT;


class CLBPackageParserXMLCheck  
{
public:
	int CheckXMLByStream(char *pszStream);
	int CheckXML(char *szXML);						/* XML Check�� ���۵Ǵ� �Լ�				*/
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
	bool SetQouter(bool bIn);						/* " " �� �ѷ� �׿��� �ִ��� �ƴ����� ���� �ϴ� �Լ�		*/
	char MoveToNonSpaceChar(char CHAR);				/* ���� ��ġ���� Space�� �ƴ� ���� ���ڱ��� �����̴� �Լ�	*/
	/*int OnTagStart(char preCh);						/* '<'�� ��������							*/
	int OnTagStart(bool *bExit);						/* '<'�� ��������							*/
	int ReadOneTag(char *szBuff,bool *bExit);		/* < > �� �ѷ� ���� �ϳ��� �ױ׸� ó�� �Ҷ�	*/
	int OnTagEnd(char preCh);						/* '>'�� ��������							*/
	int OnSlash(char preCh);						/* '/'�� ��������							*/
	/*int StartFormCheck();							/* WellFormed Check�� ���ؼ� ���� �б� ����	*/
	//int StartFormCheck(fpos_t *fpos);				/* WellFormed Check�� ���ؼ� ���� �б� ����	*/
	int StartFormCheck(int *fpos);				/* WellFormed Check�� ���ؼ� ���� �б� ����	*/
	void InitStack();								/* ������ �ʱ�ȭ							*/
	void EmptyStack();								/* ���� ����								*/
	void PushStack(char *szTag);					/* ���ÿ� ���� �ֱ�							*/
	void PopStack(char *szTag);						/* ���ÿ��� ������							*/
	int CheckWellFormed();							/* Well Formed������ �˻��� ����			*/
	//fpos_t ReadByToken(char *szTemp, char Token);		/* �־��� Token���� ���Ͽ��� �о� ���δ�	*/
	int ReadByToken(char *szTemp, char Token);		/* �־��� Token���� ���Ͽ��� �о� ���δ�	*/
	int ReadXMLInfoPart();							/* <?xml?> , <?xml-stylesheet?> �κ� ó��	*/
	int ReadXMLDocTypePart();						/* <!DOCTYPE> �κ� ó��						*/
	int ReadXMLComment();							/* �ּ� �κ� ó��							*/

};

#endif // !defined(AFX_LBPACKAGEPARSERXMLCHECK_H__8254E47A_17AB_4C0E_8792_83A379CD8768__INCLUDED_)
