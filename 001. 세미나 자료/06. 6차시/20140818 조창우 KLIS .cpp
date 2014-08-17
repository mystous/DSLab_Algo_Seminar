#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const int MAX=2000000000+1;
using namespace std;

int width;
int chack[501], cache[501], S[500];

int lis3(int start) {
int& ret = cache[start+1];
if(ret != -1) return ret;
ret = 1;
for(int next = start+1; next < width; ++next) 
  if(start==-1 || S[start] < S[next])
    ret = max(ret, lis3(next)+1);
return ret;
}

int ck(int start) {
if(lis3(start)==1) return 1;
int& ret = chack[start+1];
if(ret != -1) return ret;
ret = 0;
for(int next = start+1; next < width; ++next) 
  if((start==-1 || S[start] < S[next]) && lis3(start)==lis3(next)+1)
    ret = min<long long>(MAX, (long long)ret+ck(next));
return ret;
}

void reconstruct(int start, long int skip, vector<int>& lis)
{
  if(start != -1) lis.push_back(S[start]);
  vector<pair<int,int>> followers;
  for(int next = start+1; next<width; ++next)
	  if((start==-1 || S[start]<S[next]) && lis3(start)==lis3(next)+1)
		  followers.push_back(make_pair(S[next],next));
  sort(followers.begin(), followers.end());
  for(int i=0; i<followers.size(); ++i) {
	int idx=followers[i].second;
	int cnt=ck(idx);
	if(cnt<=skip)
		skip-=cnt;
	else {
		reconstruct(idx,skip,lis);
		break;
	}
  }
  return; 
}

int main()
{
//freopen("test.txt", "r", stdin);

long int k;
int testcase;
cin>>testcase;                 //테스트 케이스 입력
if(testcase>50) return 0;      //테스트 케이스 체크(<=50)
  //cache초기화
 for(int i=0; i<501; i++) {           
    cache[i]=-1;
    chack[i]=-1;
   }

while(testcase--){ 
 cin>>width; 
 cin>>k;

  //수열 입력
 for(int i=0; i<width; i++)            
    cin>>S[i];

 int maxnum=0;
 for(int i=-1; i<width; i++)
    maxnum=max(maxnum,lis3(i));
 
ck(-1);

vector<int> lis;
cout<<maxnum-1<<endl;
reconstruct(-1,k-1,lis);
for(int i=0; i<lis.size(); i++)
  cout<<lis[i]<<' ';

cout<<endl;

 for(int i=0; i<width+1; i++) {           
    cache[i]=-1;
    chack[i]=-1;
   }
  }
   
return 0; 
} 
