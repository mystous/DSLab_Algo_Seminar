#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
#include <string.h>

using namespace std;

const int M = 1000000000 + 100;
int bino[201][201];
void	MorseDict() 
{
	// 생성자에서 미리 이항계수를 계산한다
	memset(bino, 0, sizeof(bino));
	for (int i = 0; i <= 200; ++i) {
		bino[i][0] = bino[i][i] = 1;
			for (int j = 1; j < i; ++j)
				bino[i][j] = min(M, bino[i - 1][j - 1] + bino[i - 1][j]);
	}
}

using namespace std;

int g_nKEnd = 0;

int factoria(int n)
{
	if (0 == n)
		return 1;
	else if (1 == n)
		return 1;
	else
		return n * factoria(n - 1);
}

void generate_(int n, int m, string s)
{
	if (g_nKEnd < 0) return;

	if (0 == n && 0 == m)
	{
		if (0 == g_nKEnd--)
			cout << s << endl;
		return;
	}

	if (n > 0) generate_(n - 1, m, s + LONG_MOS);
	if (m > 0) generate_(n, m - 1, s + SHORT_MOS);
}


void generate(int n, int m, int k)
{
	int			j, i = 0;
	double			nCurrent = 1, nLastCurrent;
	string		s;

	if (1 == k || 1 == m )
	{
		g_nKEnd = k - 1;
		generate_(n, m, s);
	}
	else
	{
		while (nCurrent < k)
		{
			nLastCurrent = nCurrent;
			double a, b, c;

			/*a = factoria(m + i);
			b = factoria(i + 1);
			c = factoria(m - 1);*/
			//nCurrent +=  (a/ (b*c));
			nCurrent += bino[m + i][m - 1];
			i++;
		}
		i -= 1;

		if (0 == i)
		{
			g_nKEnd = k - 1;
			generate_(n, m, s);
		}
		else
		{
			g_nKEnd = k - nLastCurrent - 1;
			for (j = 1; j < n - i; j++)
				cout << LONG_MOS;
			cout << SHORT_MOS;
			generate_(i + 1, m - i, s);
		}
	}
}

string getKthSignal(int n, int m, int k) {
	if (n == 0 && m == 0) return "";
	// - 로 시작하는 신호의 수
	int dashes = bino[n + m - 1][n - 1];
	if (n > 0 && k < dashes)
		return "-" + getKthSignal(n - 1, m, k);
	return "o" + getKthSignal(n, m - 1, k - dashes);
}

int main()
{
	int				nTestcases;

#ifdef _WIN32
	freopen("input.txt", "r", stdin);
#endif //_WIN32
	MorseDict();
	int			n, m, k;
	cin >> nTestcases;
	while (nTestcases--)
	{
		cin >> n;
		cin >> m;
		cin >> k;

		//generate(n, m, k);
		cout << getKthSignal(n, m, k-1) << endl;
	}

	return 0;
}