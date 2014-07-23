#include <iostream>
#define FOR(A,B) for(A=0; A<B; A++)
using namespace std;

int depth,cache[100][100],input[100][100];
int trianglepath(int n, int r){
if(cache[n][r]!=-1)
	return cache[n][r];
if(n>=depth) return 0;
cache[n][r]=input[n][r];
return cache[n][r]+=trianglepath(n+1,r)>trianglepath(n+1,r+1)?trianglepath(n+1,r):trianglepath(n+1,r+1);
}

int main()
{
int n;	
cin>>n;                           //테스트 케이스 입력
if(n>50) return 0;				  //테스트 케이스 체크(<=50)
int i,j,c;
//memset(cache, -1, sizeof(cache));
FOR(i,100)
	FOR(j,100)
		cache[i][j]=-1;
FOR(c,n){						 
	cin>>depth;
	FOR(i,depth)
		FOR(j,i+1)
		    cin>>input[i][j];
	cout<<trianglepath(0, 0)<<endl;
	FOR(i,depth)
		FOR(j,i+1)
			cache[i][j]=-1;
	}
return 0;
}
