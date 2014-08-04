#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream> 
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_BUFFER		10002

#define CASE_1			1
#define CASE_2			2
#define CASE_3			4
#define CASE_4			5
#define CASE_5			10

#define INF_			987654321

int cache[MAX_BUFFER];

int JudgeCase(char candidate[5], int nWide)
{
	int			i;
	bool		bPass = true;


	for (i = 1; i < nWide; i++)
	{
		if (candidate[i - 1] != candidate[i])
		{
			bPass = false;
			break;
		}
	}

	if (bPass)
		return CASE_1;

	bPass = true;
	for (i = 0; i < nWide - 2; i++)
	{
		if (candidate[i] != candidate[i + 2])
		{
			bPass = false;
			break;
		}
	}
	if (bPass)
		return CASE_3;


	bPass = true;
	int gap = candidate[0] - candidate[1];
	for (i = 2; i < nWide; i++)
	{
		if (gap != candidate[i - 1] - candidate[i])
		{
			bPass = false;
			break;
		}
	}

	if (bPass)
	{
		if (1 == abs(gap))
			return CASE_2;
		else
			return CASE_4;
	}

	return CASE_5;
}

int getMin(int nFirst, int nSecond)
{
	if (nFirst < nSecond)
		return nFirst;

	return nSecond;
}

int getMinValue(char *pszBuffer, int nBufferSize, int nStartPos)
{
	if (nBufferSize == nStartPos)
		return 0;

	if (-1 != cache[nStartPos])
		return cache[nStartPos];


	cache[nStartPos] = INF_;

	for (int i = 3; i <= 5; i++)
	{
		if (nStartPos + i <= nBufferSize)
			cache[nStartPos] = getMin(cache[nStartPos], getMinValue(pszBuffer, nBufferSize, nStartPos + i) + JudgeCase(pszBuffer + nStartPos, i));
	}

	return cache[nStartPos];
}

int main()
{
	int				nTestcases;

#ifdef _WIN32
	freopen("input.txt", "r", stdin);
#endif //_WIN32

	cin >> nTestcases;
	while (nTestcases--)
	{
		char	chNumber;
		int		nLen;
		char	buffer[MAX_BUFFER];

		for (int i = 0; i < MAX_BUFFER; i++)
			cache[i] = -1;


		cin >> buffer;
		nLen = strlen(buffer);

		cout << getMinValue(buffer, nLen, 0) << endl;
	}

	return 0;
}