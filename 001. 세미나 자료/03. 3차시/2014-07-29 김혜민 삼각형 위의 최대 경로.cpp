
#include <iostream>
#include <math.h>
#include <string.h>


using namespace std;

int n;
int triangle[100][100];
int cash[100][100];

int main()
{
	int test_count;
	int test_line;
	
	
	
	int i,j;
	int k=0;

	memset(triangle,0,100*100*sizeof(int));
	
	cout<<"plese enter test case :";
	cin>>test_count;

	

	while(test_count--){

	cout<<"plese enter test line :";
	cin>>test_line;

	
	if(test_line<=1)
	{
		cout<<"2이상의 정수를 입력하세요";
		puts(" ");
		return -1;

	}
	

	//삼각형 입력
	for(i=0;i<test_line;i++)
		for(j=0;j<i+1;j++)
			cin>>triangle[i][j];

	int max = 0;
	cash[0][0] = triangle[0][0];

	for(i=1;i<test_line;i++)
	{
		for(j=0;j<test_line;j++)
		{
			if(j>i) continue;
			int d = (j==0)? 0 : triangle[i][j] + cash[i-1][j-1];    //오른쪽 하단과의 합
			int t = triangle[i][j] + cash[i-1][j];                  //하단과의 합


			//경로 크기비교
			cash[i][j] = d > t ? d:t;
			
			if(i == test_line-1) 
				if(cash[i][j] > max) 
				max = cash[i][j];
		
		}
	
	}
	

	cout<<"최대값은 :"<<max<<endl;

	}
		
	return 0;

}

