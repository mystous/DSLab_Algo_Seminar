#include <iostream>

using namespace std;

int cache[101];

const int MOD = 1000000007;

int tiling(int width)
{

	if (width <= 1) return 1;

	if (cache[width] != 0) return cache[width];
	int &ret = cache[width];

	ret = (tiling(width - 2) + tiling(width - 1)) % MOD;
	return ret;

}

int asymtiling(int width)
{
	if (1 == width % 2)
		return (tiling(width) - tiling(width / 2) + MOD) % MOD;

	int ret = tiling(width);
	ret = (ret - tiling(width / 2) + MOD) % MOD;
	ret = (ret - tiling(width / 2 - 1) + MOD) % MOD;
	return ret;
}

int main()
{
	int				nTestcases;

#ifdef _WIN32
	freopen("input.txt", "r", stdin);
#endif //_WIN32

	int			width;
	cin >> nTestcases;
	while (nTestcases--)
	{
		cin >> width;

		cout << asymtiling(width) << endl;
	}

	return 0;
}