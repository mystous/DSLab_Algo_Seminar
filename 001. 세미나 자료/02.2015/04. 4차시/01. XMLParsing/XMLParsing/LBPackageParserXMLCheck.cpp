// LBPackageParserXMLCheck.cpp: implementation of the CLBPackageParserXMLCheck class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LBPackageParserXMLCheck.h"

extern "C" FILE* ser_fopen( char* fileName, char *openMode );
extern "C" FILE* ser_wfopen( WCHAR *pfileName, WCHAR *openMode );
extern "C" extern void ser_fclose( FILE *fp );

#ifndef DISABLE_MERGED_FILE
	#include "ser_package.h"
#endif

#ifdef _WINCE
	extern "C" unsigned int	ksc5601toUnicode(unsigned short *des, char *str, unsigned int len);
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLBPackageParserXMLCheck::CLBPackageParserXMLCheck()
{
	m_pszRawData = NULL;
	m_nRawPos = 0;
	m_nRawDataSize = 0;
}

CLBPackageParserXMLCheck::~CLBPackageParserXMLCheck()
{

}


void CLBPackageParserXMLCheck::PushStack(char *szTag)
{
	LPSTACK_ELEMENT		NewData = NULL;

	NewData = (LPSTACK_ELEMENT)malloc(sizeof(STACK_ELEMENT));

	if(NewData)
	{
		strcpy(NewData->szTag, szTag);

		NewData->prev = lpHead;
		NewData->next = lpHead->next;

		lpHead->next = NewData;
		lpHead->next->prev = NewData;

		gnCount++;
	}
}

void CLBPackageParserXMLCheck::PopStack(char *szTag)
{
	LPSTACK_ELEMENT		lpData;

	if( gnCount == 0 )
	{
		szTag = NULL;
		return;
	}

	lpData = lpHead->next;

	lpHead->next = lpData->next;
	lpData->next->prev = lpHead;

	strcpy(szTag, lpData->szTag);
	free(lpData);

	gnCount--;
}

void CLBPackageParserXMLCheck::InitStack()
{
	lpHead = (LPSTACK_ELEMENT)malloc(sizeof(STACK_ELEMENT));
	lpTail = (LPSTACK_ELEMENT)malloc(sizeof(STACK_ELEMENT));

	lpHead->szTag[0] = '\0';
	lpTail->szTag[0] = '\0';

	lpHead->prev = NULL;
	lpHead->next = lpTail;

	lpTail->prev = lpHead;
	lpTail->next = NULL;

	gnCount = 0;
	gbQouter = false;
}	

void CLBPackageParserXMLCheck::EmptyStack()
{
	LPSTACK_ELEMENT		lpRemove, lpData;

	lpData = lpHead->next;

	while(lpData != lpTail)
	{
		lpRemove = lpData;
		lpData = lpData->next;
		
		free(lpRemove);
	}

	free(lpHead);
	free(lpTail);

	lpHead = NULL;
	lpTail = NULL;

	gnCount = 0;
}

bool CLBPackageParserXMLCheck::SetQouter(bool bIn)
{
	bool	bRtn;

	if(bIn == true)
		bRtn = false;
	else
		bRtn = true;

	return bRtn;
}

int CLBPackageParserXMLCheck::ReadXMLInfoPart()
{
	int		nRtn = WELLFORMED;
	char	ch;
	char	chBefore;
	bool	bInQouter = false;

	do{
		ch = GetCh();

		if( ch == QUOT )
			bInQouter = SetQouter(bInQouter);

		if(bInQouter == false)
		{
			switch(ch)
			{
			case LANGLEBR:
			case RANGLEBR:
			case ENDTAG:
				nRtn = NOTWELLFORMED;
				break;
			}
		}

		if(ch == EOF)
		{
			nRtn = NOTWELLFORMED;
			break;
		}
		
	}while(ch != '?');

	if( nRtn == WELLFORMED)
	{
		chBefore = ch;
		ch = MoveToNonSpaceChar(ch);

		if(chBefore == ch)
			ch = GetCh();

		if(ch != RANGLEBR)
			nRtn = NOTWELLFORMED;
	}

	return nRtn;
}

int CLBPackageParserXMLCheck::ReadXMLDocTypePart()
{
	int		nRtn = WELLFORMED;
	char	ch;
	bool	bInQouter = false;

	do{
		ch = GetCh();

		if( ch == QUOT )
		bInQouter = SetQouter(bInQouter);

		if(bInQouter == false)
		{
			switch(ch)
			{
			case LANGLEBR:
			case ENDTAG:
				nRtn = NOTWELLFORMED;
				break;
			}
		}
		
	}while(ch != RANGLEBR);


	return nRtn;
}

int CLBPackageParserXMLCheck::ReadXMLComment()
{
	int		nRtn = WELLFORMED;
	char	ch;

	while(ch = GetCh())
	{
		if(ch == '-'){
			if((ch = GetCh()) == '-'){
					if((ch = GetCh()) == RANGLEBR)
						break;
			}
		}
	}
	
	return nRtn;
}

int CLBPackageParserXMLCheck::ReadOneTag(char *szBuff,bool *bExit)
{
	int			nRtn = WELLFORMED;
	char		ch = EOS, before = EOS;
	int			pos;
	char		szPop[256];
	char		szCurrent[256];
	bool		bBreak = false;

	do{
		if( ch == EOF )
		{
			nRtn = NOTWELLFORMED;
			return nRtn;
		}

		if( ch != SPACE )
			before = ch;
		ch = GetCh();
	}while( ch != RANGLEBR );

	if( before != ENDTAG )
	{
			PushStack(szBuff);
			while(1)
			{
				bBreak = false;
				while((ch = GetCh()) != LANGLEBR){
					if(ch == EOF)
					{
						bBreak = true;
						break;
					}
				}

				if(bBreak == true)
				{
					nRtn = NOTWELLFORMED;
					break;
				}
				
				ch = GetCh();
				if(ch != ENDTAG)
				{
					Seek(-2, _SEEK_CUR);
					nRtn = StartFormCheck(&pos);
					if( nRtn == NOTWELLFORMED )
						break;
					SetPos(pos);
				}
				else
				{
					ReadByToken(szCurrent, RANGLEBR);
					PopStack(szPop);
					if(strcmp(szPop, szCurrent) != 0)
						nRtn = NOTWELLFORMED;
					break;
				}
			}
	}

	
	
	*bExit = true;
	return nRtn;
}

int CLBPackageParserXMLCheck::OnTagStart(bool *bExit)
{
	int			Index = 0;
	char		szBuff[256];
	int			nRtn = WELLFORMED;
	bool		bInQouter = false;
	int			pos;

	pos = ReadByToken(szBuff, SPACE);
	SetPos(pos);

	if(strcmp(szBuff, "?xml") == 0)
		nRtn = ReadXMLInfoPart();
	else if(strcmp(szBuff, "?xml-stylesheet") == 0)
		nRtn = ReadXMLInfoPart();
	else if(strcmp(szBuff, "!DOCTYPE") == 0)
		nRtn = ReadXMLDocTypePart();
	else if(strncmp(szBuff, "!--", 3) == 0)
	{
		nRtn = ReadXMLComment();
		if( gnCount > 0 )
			*bExit = true;
	}
	else
		nRtn = ReadOneTag(szBuff, bExit);
	
	return nRtn;
}

/*----------------------------------------------------------------------*
 *		인자로 주어진 토큰이 위치한 곳까지 파일에서 읽어 szTemp에		*
 *		저장한다. 토큰은 저장하지 않는다.								*
 *----------------------------------------------------------------------*/
int CLBPackageParserXMLCheck::ReadByToken(char *szTemp, char Token)
{
	int		Index = 0;
	char	ch;
	int		pos;
	
	while((ch = GetCh()) != Token)
	{
		if(ch == RANGLEBR)
		{
			Seek(-1, _SEEK_CUR);
			break;
		}
		if(ch == EOF)
			break;

		szTemp[Index++] = ch;
	}


	szTemp[Index] = '\0';
	pos = GetPos();
	SetPos(pos);

	return pos;
}

int CLBPackageParserXMLCheck::OnTagEnd(char preCh)
{
	int			Index = 0;
	char		ch = SPACE;
	char		szTemp[256];
	char		szBuff[256];
	int			nRtn = WELLFORMED;
	
	while(1)
	{
		while(ch != LANGLEBR)
			ch = GetCh();

		if( ch == LANGLEBR )
		{
			if((ch = GetCh()) == ENDTAG)
			{
				ReadByToken(szBuff, RANGLEBR);
				PopStack(szTemp);

				if(strcmp(szBuff, szTemp) != 0 )
					nRtn = NOTWELLFORMED;

				break;
			}
			else
			{
				Seek(-1, _SEEK_CUR);
				/*OnTagStart(chBefore);*/	/*StartForm에서 호출할때는 필요함*/
			}
		}
	}

	return nRtn;
}

int CLBPackageParserXMLCheck::OnSlash(char preCh)
{
	int			Index = 0;
	char		ch;
	char		szTemp[256];
	int			nRtn = WELLFORMED;

	ch = GetCh();
	if(ch != RANGLEBR)
		nRtn = NOTWELLFORMED;
	else
		PopStack(szTemp);

	return nRtn;
}

int CLBPackageParserXMLCheck::StartFormCheck(int *fpos)
{
	char			ch;
	int				Index = 0;
	int				nRtn = NOTWELLFORMED;
	bool			bExit = false;
	
	do
	{
		ch = GetCh();
		
		if(ch == LANGLEBR)
			nRtn = OnTagStart(&bExit);
		
		if( nRtn == NOTWELLFORMED )
			break;

		if(bExit == true)
		{
			if(fpos != NULL)
				*fpos = GetPos();
			return nRtn;
		}

	}while( ch != EOF );

	return nRtn;
}


int CLBPackageParserXMLCheck::CheckWellFormed()
{
	int			nRtn;

	InitStack();
	nRtn = StartFormCheck(NULL);
	EmptyStack();
	
	return nRtn;
}

int CLBPackageParserXMLCheck::CheckValidation(char *szXML)
{
	int nRtn = NOTWELLFORMED;


	nRtn = VALID;
	return nRtn;
}

int CLBPackageParserXMLCheck::CheckXML(char *szXML)
{
	int		nRtn = NOTWELLFORMED;
	FILE	*XML;

#ifdef _WINCE
	TCHAR	tcsXML[256];
	ksc5601toUnicode(tcsXML, szXML, 0);
	if((XML = _tfopen(tcsXML, TEXT("rt"))) == NULL)
#else
	USES_CONVERSION;
	if((XML = _tfopen(A2OLE(szXML), TEXT("rt"))) == NULL)
#endif
	{
#ifndef _WINCE
		assert(0);
#endif
		exit(1);
	}

	MakeRawData(XML);

	if(CheckWellFormed() == WELLFORMED)
	{
		if(CheckValidation(szXML) == VALID)
			nRtn = WELLANDVALID;
		else
			nRtn = INVALID;
	}

	if(m_pszRawData)
	{
		free(m_pszRawData);
		m_pszRawData = NULL;
		m_nRawPos = 0;
		m_nRawDataSize = 0;
	}

	fclose(XML);
	
	return nRtn;
} 

/*----------------------------------------------------------------------*
 *		파일 포인터를 Space다음으로	이동 시킴							*
 *----------------------------------------------------------------------*/
char CLBPackageParserXMLCheck::MoveToNonSpaceChar(char CHAR)
{
	char ch;

	ch = CHAR;
	if( ch == SPACE )
			while(ch != SPACE)
				ch = GetCh();

	return ch;
}

char CLBPackageParserXMLCheck::GetCh()
{
	char		chRtn = '\0';

	if(m_nRawPos >= m_nRawDataSize )
		return EOF;

	if(m_pszRawData)
		chRtn = m_pszRawData[m_nRawPos++];

	return chRtn;
}

void CLBPackageParserXMLCheck::Seek(int nPos, int nStartPoint)
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


void CLBPackageParserXMLCheck::MakeRawData(FILE *file)
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

int CLBPackageParserXMLCheck::CheckXMLByStream(char *pszStream)
{
	int		nRtn = NOTWELLFORMED;

	if(pszStream)
	{
		m_pszRawData = pszStream;
		m_nRawPos = 0;
		m_nRawDataSize = strlen(pszStream);
	}
	else
		return nRtn;


	if(CheckWellFormed() == WELLFORMED)
	{
		if(CheckValidation(NULL) == VALID)
			nRtn = WELLANDVALID;
		else
			nRtn = INVALID;
	}

	if(m_pszRawData)
	{
		m_pszRawData = NULL;
		m_nRawPos = 0;
		m_nRawDataSize = 0;
	}

	return nRtn;
}
