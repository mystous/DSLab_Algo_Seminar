#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

bool isFinished(const vector<string>& board, char turn);

int bijection(const vector<string>& board) {
	int ret =0;
	for(int y=0; y<3; ++y)
		for(int x=0; x<3; ++x) {
			ret = ret * 3;
			if(board[y][x] == 'o') ++ret;
			else if(board[y][x] == 'x') ret += 2;
		}
	return ret;
}

int cache[19683];

int canWin(vector<string>& board, char turn) {
	if(isFinished(board, 'o'+'x'-turn)) return -1;
	int& ret = cache[bijection(board)];
	if(ret!=-2) return ret;
	int minValue = 2;
	for(int y=0; y<3; ++y)
		for(int x=0; x<3; ++x) 
			if(board[y][x] == '.') {
				board[y][x] = turn;
				minValue = min(minValue, canWin(board, 'o'+'x'-turn));
				board[y][x] = '.';
		}
	if(minValue == 2 || minValue == 0) return ret = 0;
	return ret = -minValue;
}


bool isFinished(const vector<string>& board, char turn) {
	int a[4];
	for(int y=0; y<3; ++y) {
		for(int i=0; i<4; ++i)
			a[i] = 0;
		for(int x=0; x<3; ++x) {
			if(board[y][x] == turn) ++a[0];  
			if(board[x][y] == turn) ++a[1];
			if(board[x][x] == turn) ++a[2];
			if(board[2-x][x] == turn) ++a[3];
		}
		if(a[0] == 3 || a[1] == 3 || a[2] == 3 || a[3] == 3) return true;
	}

	return false;
}

int main()
{
//freopen("test.txt", "r", stdin);

int testcase;
cin>>testcase;                 //테스트 케이스 입력

for(int i=0; i<19683; i++) {           
	cache[i]=-2;
	}

while(testcase--){ 

	vector<string> board; 
	string a;
	
 	for(int y=0; y<3; ++y) 
		 {
			cin>>a;
			board.push_back(a);
		}

	int count=0;
	for(int y=0; y<3; ++y) 
		for(int x=0; x<3; ++x) 
			if(board[y][x] == '.') 
				count++;

	char first;
	if(count%2==0) first='o';
	else first='x';

	int win=canWin(board, first);

	if(win == 1) cout<<first<<endl;
	else if(win == -1) cout<<(char)('o'+'x'-first)<<endl;
	else cout<<"TIE"<<endl;
  }
return 0;     
} 
