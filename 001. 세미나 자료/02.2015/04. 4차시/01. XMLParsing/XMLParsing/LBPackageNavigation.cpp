// LBPackageNavigation.cpp: implementation of the CLBPackageNavigation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LBPackageNavigation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLBPackageNavigation::CLBPackageNavigation()
{

}

CLBPackageNavigation::~CLBPackageNavigation()
{

}

/*----------------------------------------------------------------------*
 *		XML Runtime Structure Navigation 관련 함수						*
 *-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*
 *		XML의 ROOT를 얻어 오는 함수										*
 *		인자로는 XML_POSITION의 &가 넘어 온다.							*
 *																		*
 *		ex) XML_POSTION pos;											*
 *			GetXMLRoot(&pos);											*
 *----------------------------------------------------------------------*/
bool CLBPackageNavigation::GetXMLRoot(LPXML_POSITION pos, LPPARSER_RESULT lpResult)
{
	bool				bRtn = true;

	pos->lpTag = lpResult->XML_ROOT;

	if(pos->lpTag == NULL)
		bRtn = false;

	pos->bLeaf = false;
	pos->bRoot = true;
	pos->nDepth = 1;
	pos->nChild = 0;

	return bRtn;
}

bool CLBPackageNavigation::MoveToParent(LPXML_POSITION pos)
{
	bool			bRtn = true;

	if( pos->bRoot == true )
		bRtn = false;
	else
	{
		pos->lpTag = pos->lpTag->lpParent;
		
		if( pos->bLeaf == true )
			pos->bLeaf = false;
		
		if( pos->lpTag->lpParent == NULL )
			pos->bRoot = true;

		pos->nDepth --;
		pos->nChild = 0;
	}

	return bRtn;
}
/*	사용 안할 함수 임 FindFirst와 겹치는 내용	*/
bool CLBPackageNavigation::MoveToChild(LPXML_POSITION pos, char *szChlid)
{
	bool			bRtn = true;
	LPNESTED_TAG	lpTag;
	
	if( pos->bLeaf == true )
		bRtn = false;
	else
	{
		lpTag = pos->lpTag->lpHead->next;
		while(lpTag != pos->lpTag->lpTail)
		{
			if( strcmp(lpTag->Tag->szTag, szChlid) == 0 )
			{
				pos->lpTag = lpTag->Tag;
				if( pos->bRoot == true )
					pos->bRoot = false;

				if( lpTag->Tag->nNested == 0 )
					pos->bLeaf = true;

				pos->nDepth++;

				break;
			}

			lpTag = lpTag->next;
		}

		/*if( lpTag == pos->lpTag->lpTail )
			pos = NULL;*/					/* 자식 테그를 찾지 못했을때 부모를 리턴할지 NULL을 리턴할지 정해야 함	*/
											/* 이 코드는 NULL을 리턴하는 코드임 현재는 부모를 리턴함				*/
	}

	return bRtn;
}

bool CLBPackageNavigation::GetTag(XML_POSITION const pos, char *szTag)
{
	bool		bRtn = true;

	strcpy(szTag, pos.lpTag->szTag);
	
	return bRtn;
}

bool CLBPackageNavigation::GetAttribute(XML_POSITION const pos, int Index, char *szAttr)
{
	bool		bRtn = false;

	if( Index >= pos.lpTag->nAttr )
		return bRtn;

	strcpy(szAttr, pos.lpTag->szValue[Index]);
	bRtn = true;

	return bRtn;
}

bool CLBPackageNavigation::GetAttributeName(XML_POSITION const pos, int Index, char *szName)
{
	bool		bRtn = true;

	strcpy(szName, pos.lpTag->szAttr[Index]);

	return bRtn;
}

int	CLBPackageNavigation::GetChlidSize(XML_POSITION const pos)
{
	return pos.lpTag->nNested;
}

int CLBPackageNavigation::GetAttrSize(const XML_POSITION pos)
{
	return pos.lpTag->nAttr;
}

bool CLBPackageNavigation::GetContents(XML_POSITION const pos, char *szContents)
{
	bool		bRtn = true;

	if(pos.lpTag->szContents)
		strcpy(szContents, pos.lpTag->szContents);
	else
	{
		strcpy(szContents, "");
		bRtn = false;
	}

	return bRtn;
}

bool CLBPackageNavigation::FindFirst(LPXML_POSITION pos, char *szSearch)
{
	bool			bRtn = false;
	LPNESTED_TAG	lpTag;

	if( pos->lpTag->nNested != 0)
	{
		lpTag = pos->lpTag->lpHead->next;
		pos->nChild = 1;
		while(lpTag != pos->lpTag->lpTail)
		{
			if( strcmp(lpTag->Tag->szTag, szSearch) == 0 )
			{
				pos->lpTag = lpTag->Tag;
				if( pos->bRoot == true )
					pos->bRoot = false;

				if( lpTag->Tag->nNested == 0 )
					pos->bLeaf = true;

				pos->nDepth++;
	
				bRtn = true;
				break;
			}

			lpTag = lpTag->next;
			pos->nChild++;
		}
	}

	return bRtn;
}

bool CLBPackageNavigation::FindNext(LPXML_POSITION pos, char *szSearch)
{
	bool			bRtn = false;
	LPNESTED_TAG	lpTag;
	int				i;

	if( pos->nChild == pos->lpTag->lpParent->nNested )
		return bRtn;

	lpTag = pos->lpTag->lpParent->lpHead->next;
	//for( i = 1 ; i < pos->nChild ; i++)
	for( i = 0 ; i < pos->nChild ; i++)
		lpTag = lpTag->next;

	while( lpTag != pos->lpTag->lpParent->lpTail )
	{
		if( strcmp(lpTag->Tag->szTag, szSearch) == 0 )
		{
			pos->lpTag = lpTag->Tag;
			bRtn = true;
			pos->nChild++;
			break;
		}
		lpTag = lpTag->next;
		pos->nChild++;
	}

	return bRtn;
}

bool CLBPackageNavigation::MoveToFirstChlid(LPXML_POSITION pos)
{
	bool			bRtn = true;
	LPNESTED_TAG	lpTag;

	if( pos->lpTag->nNested == 0 )
		bRtn = false;
	else
	{
		lpTag = pos->lpTag->lpHead->next;

		pos->nChild = 1;
		pos->lpTag = lpTag->Tag;
	}

	return bRtn;
}

bool CLBPackageNavigation::MoveToNext(LPXML_POSITION pos)
{
	bool			bRtn = true;
	LPNESTED_TAG	lpTag;
	int				i;

	if( pos->nChild == pos->lpTag->lpParent->nNested )
	{
		bRtn = false;
		return bRtn;
	}

	lpTag = pos->lpTag->lpParent->lpHead->next;
	for( i = 0 ; i < pos->nChild ; i++)
		lpTag = lpTag->next;
		
	pos->lpTag = lpTag->Tag;
	pos->nChild++;

	return bRtn;
}

bool CLBPackageNavigation::MoveToLastChlid(LPXML_POSITION pos)
{
	bool			bRtn = true;
	LPNESTED_TAG	lpTag;

	if( pos->lpTag->nNested == 0 )
		bRtn = false;
	else
	{
		lpTag = pos->lpTag->lpHead->next;
		pos->nChild = 1;

		while(lpTag!= pos->lpTag->lpTail->prev)		
		{
			lpTag = lpTag->next;
			pos->nChild++;
		}

		pos->lpTag = lpTag->Tag;
		pos->bRoot = false;
		pos->nDepth++;
	}

	return bRtn;
}

bool CLBPackageNavigation::IsHaveChild(XML_POSITION pos)
{
	bool			bRtn = true;

	if( pos.lpTag->nNested == 0 )
		bRtn = false;

	return bRtn;
}

void CLBPackageNavigation::AddToParent(LPXML_POSITION posParent, char *szTag, int tagType, int Index)
{
	LPTAG_CONTENTS		lpTag = (LPTAG_CONTENTS)malloc(sizeof(TAG_CONTENTS));
	LPTAG_CONTENTS		lpParent = posParent->lpTag;
	LPNESTED_TAG		lpNestedHead = (LPNESTED_TAG)malloc(sizeof(NESTED_TAG));
	LPNESTED_TAG		lpNestedTail = (LPNESTED_TAG)malloc(sizeof(NESTED_TAG));

	LPNESTED_TAG		lpTail = lpParent->lpTail;
	LPNESTED_TAG		lpData = (LPNESTED_TAG)malloc(sizeof(NESTED_TAG));

	if(lpTag == NULL)
	{
#ifndef _WINCE
		assert(0);
#endif
		exit(1);
	}
		
	lpData->next = lpTail;
	lpData->prev = lpTail->prev;

	lpTail->prev->next = lpData;
	lpTail->prev = lpData;

	lpData->Tag = lpTag;
	lpTag->nAttr = 0;
	lpTag->nNested = 0;
#ifndef MALLOC_BY_SIZE
	strcpy(lpTag->szTag, szTag);
	strcpy(lpTag->szContents, L"");
#else
	lpTag->szTag = (char*)malloc(strlen(szTag) * sizeof(TCHAR)+1);
	strcpy(lpTag->szTag, szTag);
	lpTag->szContents = NULL;
#endif
	for( int i = 0 ; i < 10 ; i ++ )
	{
#ifndef MALLOC_BY_SIZE
		strcpy(lpTag->szAttr[i], L"");
		strcpy(lpTag->szValue[i], L"");
#else
		lpTag->szAttr[i] = NULL;
		lpTag->szValue[i] = NULL;
#endif
	}
	lpTag->nType = tagType;

	lpTag->lpParent = posParent->lpTag;

	lpTag->lpHead = lpNestedHead;
	lpTag->lpTail = lpNestedTail;

	lpTag->lpHead->next = lpTag->lpTail;
	lpTag->lpHead->prev = NULL;
	lpTag->lpHead->Tag = NULL;

	lpTag->lpTail->prev = lpTag->lpHead;
	lpTag->lpTail->next = NULL;
	lpTag->lpTail->Tag = NULL;

	lpParent->nNested++;
	lpParent->nType |= HAVECHILD;

	MoveToLastChlid(posParent);
}

bool CLBPackageNavigation::RemoveTag(LPXML_POSITION pos)
{
	LPTAG_CONTENTS		lpTag = pos->lpTag;
	LPTAG_CONTENTS		lpParent = lpTag->lpParent;

	LPNESTED_TAG		lpData = NULL;
#ifndef _WINCE
	if(GetChlidSize(*pos))
		assert(0);
#endif
	lpData = lpParent->lpHead->next;
	
	while(lpData->Tag != lpTag)
	{
		lpData = lpData->next;

		if(lpData == lpParent->lpTail)
			return FALSE;
	}

	lpData->next->prev = lpData->prev;
	lpData->prev->next = lpData->next;

	if(MoveToParent(pos))
	{
#ifdef MALLOC_BY_SIZE
		for( int i = 0 ; i < 10 ; i ++ )
		{
			free(lpTag->szValue[i]);
			free(lpTag->szAttr[i]);
		}
		free(lpTag->szContents);
		free(lpTag->szTag);
#endif
		free(lpData);
		free(lpTag);
	}

	return TRUE;
}

void CLBPackageNavigation::SetAttribute(const XML_POSITION pos, char *szAttr, char *szAttrValue)
{
	LPTAG_CONTENTS		lpTag = pos.lpTag;
	int					nCount;

	nCount = GetAttrSize(pos);
#ifndef MALLOC_BY_SIZE
	strcpy(lpTag->szAttr[nCount], szAttr);
	strcpy(lpTag->szValue[nCount], szAttrValue);
#else
	if(lpTag->szAttr[nCount] != NULL)
		free(lpTag->szAttr[nCount]);
	lpTag->szAttr[nCount] = (char*)malloc(strlen(szAttr) * sizeof(TCHAR)+1);
	
	if(lpTag->szValue[nCount] != NULL)
		free(lpTag->szValue[nCount]);
	lpTag->szValue[nCount] = (char*)malloc(strlen(szAttrValue) * sizeof(TCHAR)+1);
	
	strcpy(lpTag->szAttr[nCount], szAttr);
	strcpy(lpTag->szValue[nCount], szAttrValue);
#endif
	lpTag->nAttr++;
}

void CLBPackageNavigation::SetContents(XML_POSITION pos, char *szContents)
{
	if(!szContents)
		return;
#ifdef MALLOC_BY_SIZE
	if(pos.lpTag->szContents != NULL)
		free(pos.lpTag->szContents);
	pos.lpTag->szContents = (char*)malloc(strlen(szContents) * sizeof(TCHAR)+1);
#endif
	strcpy(pos.lpTag->szContents, szContents);
}

bool CLBPackageNavigation::SetAttributeName(const XML_POSITION pos, int Index, char *szName)
{
	bool		bRtn = true;

#ifdef MALLOC_BY_SIZE
	if(pos.lpTag->szAttr[Index] != NULL)
		free(pos.lpTag->szAttr[Index]);
	pos.lpTag->szAttr[Index] = (char*)malloc(strlen(szName) * sizeof(TCHAR)+1);
#endif
	
	strcpy(pos.lpTag->szAttr[Index], szName);

	return bRtn;
}

int CLBPackageNavigation::GetTagType(const XML_POSITION pos)
{
	return pos.lpTag->nType;
}

void CLBPackageNavigation::SetAttributeValue(const XML_POSITION pos, int nIndex, char *pszAttrValue)
{

#ifdef MALLOC_BY_SIZE
	/*
	if(pos.lpTag->szValue[nIndex] != NULL)
		pos.lpTag->szValue[nIndex] = (char*)realloc(pos.lpTag->szValue[nIndex], strlen(pszAttrValue)+1);
	else
		pos.lpTag->szValue[nIndex] = (char*)malloc(strlen(pszAttrValue)+1);
	*/
	if(pos.lpTag->szValue[nIndex] != NULL)
	{
		free(pos.lpTag->szValue[nIndex]);
		pos.lpTag->szValue[nIndex] = NULL;
	}

	pos.lpTag->szValue[nIndex] = (char*)malloc(strlen(pszAttrValue) * sizeof(TCHAR)+1);
#endif

	strcpy(pos.lpTag->szValue[nIndex], pszAttrValue);
}

BOOL CLBPackageNavigation::FindAttr(const XML_POSITION pos, const char *pszAttrName, char *pszAttrValue)
{
	BOOL		bRtn = FALSE;
	int			nCount, i;
	char		szAttr[256];

	nCount = GetAttrSize(pos);
	for( i = 0 ; i < nCount ; i ++ )
	{
		GetAttributeName(pos, i, szAttr);
		if(!strcmp(szAttr, pszAttrName))
		{
			GetAttribute(pos, i, pszAttrValue);
			bRtn = TRUE;
			break;
		}
	}

	return bRtn;
}

char* CLBPackageNavigation::GetContents(const XML_POSITION pos)
{
	char			*pszRtn = NULL;

	pszRtn = (char*)malloc(strlen(pos.lpTag->szContents) * sizeof(TCHAR)+1);
	strcpy(pszRtn, pos.lpTag->szContents);

	return pszRtn;
}

void CLBPackageNavigation::ChangeContents(XML_POSITION pos, char *szContents)
{
#ifdef MALLOC_BY_SIZE
	pos.lpTag->szContents = (char*)realloc(pos.lpTag->szContents, strlen(szContents));
#endif

	strcpy(pos.lpTag->szContents, szContents);
}

