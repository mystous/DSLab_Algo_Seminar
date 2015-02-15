#include <stdio.h>
//#include <Windows.h>

int main(void)
{
	int answer[4][3];  //answer[2] -> 100의자리


	int str  = 0;
	int ball = 0;
	int i,j,z;

	int init = 0;

	int ball_to_str=0;

	//정답 배열 인덱스
	int result_count = 0;
	int result_a[2] = {NULL,NULL};
	int result_b[2] = {NULL,NULL};
	int result_c[2] = {NULL,NULL};

	//문제 입력
	printf("문제를 내세요 3자리 : ");	
	for(int i=0;i<3;i++)
		scanf("%d",&answer[0][i]);


	//비교할 배열 초기화 012,345,678
	for(j=1; j<4; j++)
	{
		for(i=0;i<3;i++)
		{
			answer[j][i]=init;
			printf("%d\n",answer[j][i]);
			init++;
		}
	}

	//스트라이크, 볼 검사

	for(int k=1;k<4; k++)
	{
		for(int i=0; i<3; i++)
		{
			if(answer[0][i]==answer[k][i])
			{
				if(result_count==0 && result_a[0]==NULL)
				{result_count+=1,result_a[0]=k, result_a[1]=i;}
				if(result_count==1 && result_a[1]==NULL)
				{result_count+=1,result_b[0]=k, result_b[1]=i;}
				if(result_count==2 && result_a[2]==NULL)
				{result_count+=1,result_c[0]=k, result_c[1]=i;}

				str++;
			}

			for(int j=0; j<3; j++)
			{
				if(i!=j)
				{
					if(answer[0][i]==answer[k][j])
					{
						ball++;



					}
				}
			}
			
		}

		printf("%d Ball, %d Strike\n",ball,str);
		str  = 0;
		ball = 0;
	}
	
	//printf("answer is : %d,%d,%d\n",
	//system("pause");
	
	return 0;


}

