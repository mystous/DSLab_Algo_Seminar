#include <stdio.h>

int N,M,X,D,R,C[55][55],S[55];
double P[111][55];

int main()
{
	//freopen("input.txt", "r", stdin);


	int test_num,i,j,d;

	scanf ("%d",&test_num); 
	while (test_num--)
	{
		//N : 마을 수
		//D : 지난 일 수
		//R : 교도소가 있는 마을의 번호
		scanf ("%d %d %d",&N,&D,&R);
		
		for (i=0;i<N;i++) S[i] = 0;
		
		for (i=0;i<N;i++) 
			for (j=0;j<N;j++) {
				
				// C[] 갈수있는 곳 배열 입력
				scanf ("%d",&C[i][j]);

				// S[] 각 마을에서의 확률
				S[i] += C[i][j];
			}

		
		//각 마을 초기화 = 0 , 교도소 =1
		
			for (i=0;i<N;i++) 
				P[0][i] = 0;
			P[0][R] = 1;
		
		// for 지난일수, for 마을 수
		for (d=1;d<=D;d++){
			for (i=0;i<N;i++) 
				P[d][i] = 0;
			
			for (i=0;i<N;i++)
				for (j=0;j<N;j++)
					// 마을이 연결되있는 확인후 확률을 더함
					if (C[i][j])
						P[d][j] += P[d-1][i] / S[i];
		}

		// M 계산할 마을 수
		scanf ("%d",&M);

		// 계산할 마을번호 입력 및 출력
		for (i=0;i<M;i++){
			scanf ("%d",&X);
			printf ("%.9lf ",P[D][X]);
		}
		printf ("\n");
	}

	return 0;
}
