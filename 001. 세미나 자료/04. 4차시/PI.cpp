#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

//이 때, 각 조각들의 난이도는 다음과 같이 정해집니다 :
//
//모든 숫자가 같을 때(예 : 333, 5555) 난이도 : 1
//숫자가 1씩 단조 증가하거나 단조 감소할 때(예 : 23456, 3210) 난이도 : 2
//두 개의 숫자가 번갈아 가며 출현할 때(예 : 323, 54545) 난이도 : 4
//숫자가 등차 수열을 이룰 때(예 : 147, 8642) 난이도 : 5
//그 외의 경우 난이도 : 10

#define MAX_BUFFER		10002

#define CASE_1			0
#define CASE_2			1
#define CASE_3			2
#define CASE_4			3
#define CASE_5			4

int g_Score[5] = { 1, 2, 4, 5, 10 };

int JudgeCase(char candidate[5])
{
	int gap1 = candidate[0] - candidate[1];
	int gap2 = candidate[1] - candidate[2];

	if (candidate[0] == candidate[1] && candidate[1] == candidate[2])
		return CASE_1;

	if (1 == abs(gap1) && gap1 == gap2)
		return CASE_2;

	if (candidate[0] == candidate[2])
		return CASE_3;

	if (gap1 == gap2)
		return CASE_4;

	return CASE_5;
}

bool isExtensionOfCase(char candidate[5], char *pchCandidate, int nCase, int nCadIndex)
{
	bool		bRtn = true;

	switch (nCase)
	{
	case CASE_1:
		if (candidate[nCadIndex - 1] == pchCandidate[0])
			return bRtn;
		break;
	case CASE_2:
		if (1 == abs(candidate[nCadIndex - 1] - pchCandidate[0]))
		{
			if (candidate[nCadIndex - 1] - pchCandidate[0] == candidate[nCadIndex - 2] - candidate[nCadIndex - 1])
				return bRtn;
		}
		break;
	case CASE_3:
		if (candidate[nCadIndex - 2] == pchCandidate[0] && candidate[nCadIndex - 3] == candidate[nCadIndex - 1])
			return bRtn;
		break;
	case CASE_4:
		if (candidate[nCadIndex - 1] - pchCandidate[0] == candidate[nCadIndex - 2] - candidate[nCadIndex - 1])
			return bRtn;
		break;
	case CASE_5:
		break;
	default:
		break;
	}

	bRtn = false;
	return bRtn;
}

void IncreaseScore(int &nMinScore, int &nCadIndex, int nCase)
{
	nMinScore += g_Score[nCase];
	nCadIndex = 0;
}

int main()
{
	int				nTestcases;
	int				i, j;

	freopen("input.txt", "r", stdin);

	cin >> nTestcases;
	while (nTestcases--)
	{
		int		nNumber;
		int		nLen;
		char	buffer[MAX_BUFFER];
		char	candidate[5];
		int		nCandiIndex = 0;
		int		i;
		int		nMinScore = 0;

		cin >> nNumber;

		sprintf(buffer, "%d", nNumber);
		nLen = strlen(buffer);

		for (i = 0; i < nLen; i++)
		{
			if (nCandiIndex < 3)
				candidate[nCandiIndex++] = buffer[i];

			if (3 == nCandiIndex)
			{
				int nCase = JudgeCase(candidate);
				
				if (i == nLen -1)
				{
					IncreaseScore(nMinScore, nCandiIndex, nCase);
					break;
				}
				
				if (isExtensionOfCase(candidate, buffer+ i + 1, nCase, nCandiIndex))
				{
					candidate[nCandiIndex++] = buffer[1+i++];
					
					if (i == nLen - 1)
					{
						IncreaseScore(nMinScore, nCandiIndex, nCase);
						break;
					}

					if (isExtensionOfCase(candidate, buffer + i + 1, nCase, nCandiIndex))
						candidate[nCandiIndex++] = buffer[1+i++];

					IncreaseScore(nMinScore, nCandiIndex, nCase);
				}
				else
				{
					IncreaseScore(nMinScore, nCandiIndex, nCase);
				}
			}
		}
		
		cout << nMinScore << endl;
	}

	return 0;
}
