#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

#define MAX_SIZE		100

bool CheckMove(int x, int y, unsigned int maze[MAX_SIZE][MAX_SIZE])
{
	bool			bRtn = true;
	int				nX = x, nY = y;
	unsigned int	nMoveCount;

	if (0 == x && 0 == y)
		return bRtn;

	if (x < 0 || y < 0)
	{
		bRtn = false;
		return bRtn;

	}

	nMoveCount = 0;
	while (nX > 0)
	{
		nMoveCount++;
		nX--;

		if (nMoveCount == maze[y][nX])
		{
			if (CheckMove(nX, y, maze))
				return bRtn;
		}
	}

	nMoveCount = 0;
	while (nY > 0)
	{
		nMoveCount++;
		nY--;

		if (nMoveCount == maze[nY][x])
		{
			if (CheckMove(x, nY, maze))
				return bRtn;
		}
	}

	bRtn = false;
	return bRtn;
}

int main()
{
	int				nTestcases;
	int				i, j;

	//freopen("input.txt", "r", stdin);

	cin >> nTestcases;
	while (nTestcases--)
	{
		int				nMazeSize;
		unsigned int	maze[MAX_SIZE][MAX_SIZE];
		bool			bSuccess = false;

		cin >> nMazeSize;
		for (i = 0; i < nMazeSize; i++)
		{
			for (j = 0; j < nMazeSize; j++)
			{
				cin >> maze[i][j];
			}
		}

		bSuccess = CheckMove(nMazeSize - 1, nMazeSize - 1, maze);

		if (bSuccess)
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}
}