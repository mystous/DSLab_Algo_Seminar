#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
const int MAX = 1000000000 + 1;
int length[51]; //length[i] = X나 Y를 i번 치환시킨 길이
const string EXPAND_X = "X+YF";
const string EXPAND_Y = "FX-Y";
void precalc();
char expand(const string& dragonCurve, int generations, int skip);
int main()
{
	int testcase;
	scanf("%d", &testcase);
	for (int i = 0; i < testcase; i++)
	{ 
		int n, p, l; //n은 세대 p번째 글자부터  l글자 출력
		scanf("%d %d  %d", &n, &p, &l);
		precalc();

		for (int i = p; i < p+l; i++)
		{
			char a = expand("FX", n, i-1);
			printf("%c", a);
		}

		printf("\n");

	}
}

void precalc()
{
	length[0] = 1;
	for (int i = 1; i <= 50; i++)
		length[i] = min(MAX, length[i - 1] * 2 + 2);
}

char expand(const string& dragonCurve, int generations, int skip) //dragonCurve를 generations 진화시킨 결과에서 skip번째 문자 반환
{
	if (generations== 0)
	{
		return dragonCurve[skip];
	}

	for (int i = 0; i < dragonCurve.size(); i++)
	{
		if (dragonCurve[i] == 'X' || dragonCurve[i] == 'Y') //문자열이 확장되는 경우
		{
			if (skip >= length[generations])
				skip -= length[generations];

			else if (dragonCurve[i] == 'X')
				return expand(EXPAND_X, generations - 1, skip);
			else if (dragonCurve[i] == 'Y')
				return expand(EXPAND_Y, generations - 1, skip);
		}

		else if (skip > 0)
			skip--;
		else
			return dragonCurve[i];
	}
	return '#';
}