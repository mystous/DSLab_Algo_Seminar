#include <iostream>
#define FOR(A,B) for(A=0; A<B; A++)
using namespace std;

const int INF = 987654321;
int n;
int A[101], pSum[101], pSqSum[101];

void sort(int* A) {
  for(int i=0; i<n; ++i) {
	int j = i;
	while(j>0 && A[j-1]>A[j]) {
	  int temp = A[j];
	  A[j]=A[j-1];
	  A[j-1]=temp;
	  --j;
	}
  }
}

void precalc(){
  sort(A);
  pSum[0]=A[0];
  pSqSum[0]=A[0]*A[0];
  for(int i=1; i<n; ++i) {
    pSum[i]=pSum[i-1]+A[i];
	pSqSum[i]=pSqSum[i-1]+A[i]*A[i];
  }
}

int minError(int lo, int hi) {
  int sum=pSum[hi]-(lo==0? 0:pSum[lo-1]);
  int sqSum=pSqSum[hi]-(lo==0? 0:pSqSum[lo-1]);
  int m=int(0.5+(double)sum/(hi-lo+1));
  int ret=sqSum-2*m*sum+m*m*(hi-lo+1);
  return ret;
}

int cache[101][11];
int quantize(int from, int parts) {
  if(from==n) return 0;
  if(parts==0) return INF;
  int& ret = cache[from][parts];
  if(ret!=-1) return ret;
 
  ret=INF;
  for(int partSize=1; from+partSize<=n; ++partSize)
	  ret = min(ret, minError(from,from+partSize-1)+
					 quantize(from+partSize,parts-1));

  if(cache[from][parts]==-1) cache[from][parts]=ret;
  return ret;
}

int main()
{
freopen("test.txt", "r", stdin);

int testcase;	
cin>>testcase;                           //테스트 케이스 입력
if(testcase>50) return 0;				  //테스트 케이스 체크(<=50)
int i,j,c;
int parts;

FOR(c,testcase){						 
	cin>>n;
	cin>>parts;
	FOR(i,n)
	  cin>>A[i];

	precalc();
	
	FOR(i,101)
	  FOR(j,11)
		cache[i][j]=-1;

	cout<<quantize(0,parts)<<endl;
	}
return 0;
}
