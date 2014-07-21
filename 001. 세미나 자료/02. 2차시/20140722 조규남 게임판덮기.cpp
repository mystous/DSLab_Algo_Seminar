#include<iostream>
#include<string>
using namespace std;

#include <fstream> 

#define EMPTY_BLOCK		' '
#define WHITE_BLOCK		'.'
#define BLACK_BLOCK		'#'
#define SET_BLOCK		'!'

#define MAX_HEIGHT		100
#define MAX_WIDTH		20

#define DIRECTION		4
#define SET_COUNT		3

int g_XSetDirection[DIRECTION][SET_COUNT] = { { 0, 1, 0 }, { 0, 1, 1 }, { 0, 0, 1 }, { 0, 0, -1 } };
int g_YSetDirection[DIRECTION][SET_COUNT] = { { 0, 0, 1 }, { 0, 0, 1 }, { 0, 1, 1 }, { 0, 1,  1 } };

typedef struct _tagInputData{
	char map[MAX_HEIGHT][MAX_WIDTH];
	int nHeight, nWidth;
	int nWhiteBlockCount;
}MAP_DATA, *LPMAP_DATA;

void roundTrip(int nX, int nY, MAP_DATA Map, int &nWayCount);

///< Only for Debugging
void ShowMap(MAP_DATA Map)
{
	return;
	int			i, j;

	for (i = 0; i < Map.nHeight; i++)
	{
		for (j = 0; j < Map.nWidth; j++)
		{
			printf("%c", Map.map[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

bool findNextSetCoordnation(int nX, int nY, int &nNextX, int &nNextY, MAP_DATA Map)
{
	bool			bRtnFind = false;
	int				i, j;
	int				nStartX = nX;

	for (i = nY; i < Map.nHeight; i++)
	{
		for (j = nStartX; j < Map.nWidth; j++)
		{
			if (WHITE_BLOCK != Map.map[i][j])
				continue;

			bRtnFind = true;
			nNextX = j;
			nNextY = i;
			return bRtnFind;
		}
		nStartX = 0;
	}

	return bRtnFind;
}

void initMap(LPMAP_DATA lpMap)
{
	lpMap->nHeight = 0;
	lpMap->nWidth = 0;
	lpMap->nWhiteBlockCount = 0;
	for (int i = 0; i < MAX_HEIGHT; i++)
	{
		for (int j = 0; j < MAX_WIDTH; j++)
		{
			lpMap->map[i][j] = EMPTY_BLOCK;
		}
	}
}

void setThanRound(int nX, int nY, MAP_DATA Map, int direction, int &nWayCount)
{
	int				i;
	bool			bSet = true;

	for (i = 0; i < SET_COUNT; i++)
	{
		Map.map[nY + g_YSetDirection[direction][i]][nX + g_XSetDirection[direction][i]] = SET_BLOCK;
		Map.nWhiteBlockCount--;
	}

	ShowMap(Map);

	if (0 == Map.nWhiteBlockCount)
		nWayCount++;
	else
	{
		int			nNextX, nNextY;
		if (true == findNextSetCoordnation(nX, nY, nNextX, nNextY, Map))
		{
			roundTrip(nNextX, nNextY, Map, nWayCount);
		}
	}
}

void roundTrip(int nX, int nY, MAP_DATA Map, int &nWayCount)
{
	int			i, j;
	bool		bSet = false;


	///< 총 4가지 Type별로 블럭을 놓은 후 다음 블럭을 넣는 것은 재귀적으로 Map data를 복사해서 사용한다.
	for (i = 0; i < DIRECTION; i++)
	{
		ShowMap(Map);

		bSet = true;
		for (j = 0; j < SET_COUNT; j++)
		{
			if (nX + g_XSetDirection[i][j] < 0 || nX + g_XSetDirection[i][j] >= Map.nWidth)
			{
				bSet = false;
				break;
			}

			if (nY + g_YSetDirection[i][j] < 0 || nY + g_YSetDirection[i][j] >= Map.nHeight)
			{
				bSet = false;
				break;
			}

			if (WHITE_BLOCK != Map.map[nY + g_YSetDirection[i][j]][nX + g_XSetDirection[i][j]])
			{
				bSet = false;
				break;
			}
		}

		if (false == bSet) ///< Block을 배치할 수 없으면 다음 type으로 넘어 간다.
			continue;

		setThanRound(nX, nY, Map, i, nWayCount);
	}
}

int main() {
	int			nTestcases;
	int			i, j;

	//freopen("input.txt", "r", stdin);

	cin >> nTestcases;
	while (nTestcases--) 
	{
		int			nWhiteBlockCount = 0;
		int			nWayCount = 0;
		MAP_DATA	Map;

		initMap(&Map);

		cin >> Map.nHeight;
		cin >> Map.nWidth;
		for (i = 0; i < Map.nHeight; i++)
		{
			for (j = 0; j < Map.nWidth; j++)
			{
				cin >> Map.map[i][j];
				if (WHITE_BLOCK == Map.map[i][j])
					Map.nWhiteBlockCount++;
			}
		}

		if (0 == (Map.nWhiteBlockCount % 3))
		{
			int		nX = 0, nY = 0;
			int		nNextX = 0, nNextY = 0;
			if (true == findNextSetCoordnation(nX, nY, nNextX, nNextY, Map))
			{
				roundTrip(nNextX, nNextY, Map, nWayCount);
			}
		}

		///< Print result
		cout << nWayCount << endl;
	}
}