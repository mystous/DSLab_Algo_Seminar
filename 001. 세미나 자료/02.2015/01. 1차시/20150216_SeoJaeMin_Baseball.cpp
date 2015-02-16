#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void make_num(int *num);
int compare(int* input, int* num);
void result(int* num, int strike);
int restart_check();

int main(){

	int num[3] = {0};
	int num2[3] = {0};
	int input[3] = {0};
	int i=0, j=0;
	int cnt=1;
	int strike=0;
	
	int strike_array[3] = {0};

	while(1){

		//cnt=1;

		//make_num(num);

		if (scanf("%d %d %d", &input[0], &input[1], &input[2]) != 3){
			printf("숫자만쓰라고 \n");
			fflush(stdin);
			continue;		
		}

		if( (input[0] == input[1] || input[0] == input[2] || input[1] == input[2]) || (input[0] < 1 || input[1] < 1 || input[2] < 1 || input[0] > 10 ||  input[1] > 10 ||  input[2] > 10) ){
			printf("다시입력해\n");
			continue;
		}

		while (1){

			make_num(num);
			
			/*while(num2[0]==num[0]&&num2[1]==num[1]&&num2[2]==num[2])
			{
				make_num(num);
			}*/
			printf("computer_num : %d%d%d \n", num[0], num[1], num[2]);

			strike = compare(num, input);

			result(num, strike);

			if(strike > 0){
				srand(time(NULL));
				for(i=0; i<3; i++){
					if(input[i] == num[i])
						strike_array[i] = num[i];
					else if(input[i] != num[i])
						num[i] = rand()%9+1;						
				}
				
			}
			for(i=0; i<3; i++)
			{
				num2[i] = num[i];
			}
			if(strike == 3){
				break;
			}
			else{
				continue;
			}
			cnt++;
			
		}

		if( strike == 3){
			system("cls");
			continue;
		}
		else
			break;

	}

	return 0;
}

void make_num(int *num){
	
	int i=0;
	int j=0;
	time_t time2=1000;
	srand(time(&time2));
	for(i=2; i>=0; i--){
		num[i] = rand() % 9 + 1;
		for(j=0; j<3; j++)
		{
			if(i!=j)
			{
				if(num[i]==num[j])
				{
					num[i] = rand() % 9 + 1;
				}
			}
		}
		/*while(num[i] == num[i-1] || num[i] == num[i-2])
			num[i] = rand() % 9 + 1;*/
	}

}

int compare(int* num, int* input){
	
	int i,j;
	//int s[3] ={0};
	int strike=0, ball=0;

	for(i=0, j=0; i<3; i++){
		for(j=0; j<3; j++){
			if(num[i] == input[i]){
				strike++;
				//s[i]=i;
				break;
			}
			else if(num[i] == input[j]){
				ball++;
			}
		}
	}


	printf("%d strike, %d ball \n", strike, ball);

	return strike;
}

void result(int*num, int strike){
	
	if(strike == 3){
		printf("올ㅋ\n");
	}
}


