// ya9_KDH.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>

#define BUFSIZE 3

void intro();
void inputData();
void gameStart();
int checkStrike(void);
int checkStrike(int &tmpStrike, int &tmpBall);
void processing(void);
//void checkNSaveData(int* );

struct comSelectNum
{
	int pos; //1, 2, 3
	int isStrike; // 0 strike 1 ball 2 out
} comNum[10];

int ball=0;
int strike=0;
int out=0;

int buf[3]={-1,-1,-1};
int fix[BUFSIZE]={-1,-1,-1};
int testNumber[10]={0,1,2,3,4,5,6,7,8,9};
int limitIndex=10;
int num[3];

int _tmain(int argc, _TCHAR* argv[])
{
	
	intro();
	inputData();
	while(1)
	{
		gameStart();
		break;

	}

	return 0;
}

void gameStart()
{
	printf("GameStart ! \n\n\n");

	
	/////////////////////////// ���� ���� ///////////////////////////
	while(1)
	{
		

		processing();
		
		break;
	}
	
	for(int i=0 ; i<10 ; i++)
	{
		printf("%3d", testNumber[i]);
	}

	printf("���� ����");
}

void processing(void)
{
	///////////////////////////// 1
	printf("step1\n");
	int i;
	int SNB=0;
	for(i=0 ; i<3 ; i++){
		if(SNB==3){
			for(int k=3*i ; k<10 ; k++){
				testNumber[k]=-1;
			}
			break;
		}
		num[0] = testNumber[3*i];
		num[1] = testNumber[3*i+1];
		num[2] = testNumber[3*i+2];
		int checkN = checkStrike();
		if(checkN==0){
			testNumber[3*i]=-1;
			testNumber[3*i+1]=-1;
			testNumber[3*i+2]=-1;
		}
		SNB+= checkN;
		printf("strike&ball %d \n\n",SNB);
	}
	if(SNB==3)
	{
		testNumber[9] = -1;
	}
	
	num[0]=-1;
	num[1]=-1;
	num[2]=-1;
	
	
	///////////////////////////// 2
	printf("step2\n");
	int outNum, outNumIndex;
	for(int i=0 ; i<10 ; i++){
		if(testNumber[i]==-1){
			outNum = i;
			break;
		}
	}

	outNumIndex=2;// ó�� num[2]�� �־ Ȯ��


	
	int compareIndex=0;

	while(strike<1){
		for(int i=0 ; i<10 ; i++){
			
			if(testNumber[i] != -1){
				num[compareIndex]=testNumber[i];
				compareIndex++;			
				if(compareIndex==2){ 
					num[outNumIndex] = outNum;
					break;
				}
			}
			
		}
		
		int tmpStrike=0, tmpBall=0;
		checkStrike(tmpStrike, tmpBall);
		if(tmpStrike==0 && tmpBall==0){
			int tmp;
			tmp=num[0]; testNumber[tmp]=-1;
			tmp=num[1]; testNumber[tmp]=-1;
		}
		else if(tmpStrike==2){
			int tmp;
			tmp=num[0]; testNumber[tmp]=-1; fix[0]=tmp;
			tmp=num[1]; testNumber[tmp]=-1; fix[1]=tmp;
			strike=2;
		}
		else if(tmpStrike==1 && tmpBall==0){
			int tmp;
			int _tmpStrike=0, _tmpBall=0;
			tmp=num[1]; num[1]= num[outNumIndex/*2*/]; num[outNumIndex]=tmp;
			checkStrike(_tmpStrike, _tmpBall);
			if(_tmpStrike==0 && _tmpBall==1){
				fix[1] = tmp;
				tmp=num[0]; testNumber[tmp]=-1; 
				tmp=num[1]; testNumber[tmp]=-1; 
				tmp=num[2]; testNumber[tmp]=-1; 
			}else{
				fix[0] = num[0];
				tmp=num[0]; testNumber[tmp]=-1; 
				tmp=num[1]; testNumber[tmp]=-1; 
				tmp=num[2]; testNumber[tmp]=-1;
			}
			strike=1;

		}else if(tmpStrike==1 && tmpBall==1){		
			int tmp;
			int _tmpStrike=0, _tmpBall=0; 
			
			tmp=num[1]; num[1]= num[2/*2*/]; num[2]=tmp;
			checkStrike(_tmpStrike, _tmpBall);
			
			if(_tmpStrike==2){
				tmp=num[0]; testNumber[tmp]=-1; fix[0]=tmp;
				tmp=num[2]; testNumber[tmp]=-1; fix[2]=tmp;
			}else{
				fix[1] = tmp;
				fix[2] = num[0];
				tmp=num[0]; testNumber[tmp]=-1; 
				tmp=num[1]; testNumber[tmp]=-1; 
				tmp=num[2]; testNumber[tmp]=-1;
			}
			strike=2;

		}else if(tmpStrike==0 && tmpBall==1){
			int tmp;
			int _tmpStrike=0, _tmpBall=0;
			
			tmp=num[0]; num[0]=num[1]; num[1] = tmp;
			checkStrike(_tmpStrike, _tmpBall);
			if(_tmpStrike==1)
			{
				tmp=0; 
				_tmpStrike=0, _tmpBall=0;
				
				tmp=num[1]; num[1]= num[outNumIndex/*2*/]; num[outNumIndex]=tmp;
				checkStrike(_tmpStrike, _tmpBall);
				if(_tmpStrike==0 && _tmpBall==1){
					fix[1] = tmp;
					tmp=num[0]; testNumber[tmp]=-1; 
					tmp=num[1]; testNumber[tmp]=-1; 
					tmp=num[2]; testNumber[tmp]=-1; 
				}else{
					fix[0] = num[0];
					tmp=num[0]; testNumber[tmp]=-1; 
					tmp=num[1]; testNumber[tmp]=-1; 
					tmp=num[2]; testNumber[tmp]=-1;
				}

			}else{
				_tmpStrike=0, _tmpBall;
				tmp = num[1]; num[2]=num[1]; num[2]=tmp;
				checkStrike(_tmpStrike,_tmpBall);
				if(_tmpStrike==1){
					fix[2]=num[2];
					tmp=num[0]; testNumber[tmp]=-1; 
					tmp=num[1]; testNumber[tmp]=-1; 
					tmp=num[2]; testNumber[tmp]=-1;
					strike=1;

				}else{
					fix[2]=num[0];
					tmp=num[0]; testNumber[tmp]=-1; 
					tmp=num[1]; testNumber[tmp]=-1; 
					tmp=num[2]; testNumber[tmp]=-1;
				}
			}
			strike=1;


		}else if(tmpStrike==0 && tmpBall==2){
			int tmp;
			int _tmpStrike=0, _tmpBall=0;
			tmp = num[0]; num[0]=num[1]; num[1]=tmp;

			tmp = num[1]; num[1]=num[2]; num[2]=tmp;
			checkStrike(_tmpStrike, _tmpBall);
		
			if(_tmpStrike==2){
				tmp=num[0]; testNumber[tmp]=-1; fix[0]=tmp;
				tmp=num[2]; testNumber[tmp]=-1; fix[2]=tmp;
			}else{
				fix[1] = tmp;
				fix[0] = num[0];
				tmp=num[0]; testNumber[tmp]=-1; 
				tmp=num[1]; testNumber[tmp]=-1; 
				tmp=num[2]; testNumber[tmp]=-1;
			}
			strike=2;
		}
	}
	for(int i=0 ; i<10 ; i++)
	{
		printf("%3d", testNumber[i]);
	}

	printf("\n\n fix[0] : %d, fix[1] : %d, fix[2] : %d \n\n", fix[0], fix[1], fix[2]);

	/////////////////////////////////////// 3
	
	for(int i=0; i<3; i++)
	{
		num[i] = fix[i];
	}
	if(strike==1)
	{
		printf("step3\n");
		int knownInd;
		int _tmpStrike=0, _tmpBall=0;
		for(int i=0; i<3; i++)
		{
			if(num[i] != -1){
				knownInd=i;
				break;
			}
		}
		
		int restInd1, restInd2;
		if(knownInd==0)			{restInd1=1; restInd2=2;}
		else if(knownInd==1)		{restInd1=0; restInd2=2;}
		else					{restInd1=0; restInd2=1;}

		int pick=0;
		while(strike<2){
			for(int i=0 ; i<10 ; i++){   //num�� ������ ä���ֱ�
				if(testNumber[i] != -1){
					if(pick==knownInd){	
						pick++;
					}
					num[pick++]=i;
					if(pick==3) {
						pick=0;
						break;
					}
				}
			}
			
			checkStrike(_tmpStrike,_tmpBall); // �˻�
			if(_tmpStrike==1 && _tmpBall==0){ // ä������ ���� ��� ����ġ
				for(int i=0 ; i<3; i++)
				{
					if(i==knownInd)i++;
					else{
						testNumber[i]=-1;
					}
				}
			}else if(_tmpStrike==3){ //��� ��Ʈ����ũ
				int tmp;
				fix[restInd1] = num[restInd1]; tmp=fix[restInd1]; testNumber[tmp]=-1;
				fix[restInd2] = num[restInd2]; tmp=fix[restInd2]; testNumber[tmp]=-1;
				strike=3;

			}else if(_tmpStrike==1 && _tmpBall==2){ //1s 2b
				int tmp;
				fix[restInd1] = num[restInd2]; tmp=fix[restInd1]; testNumber[tmp]=-1;
				fix[restInd2] = num[restInd1]; tmp=fix[restInd2]; testNumber[tmp]=-1;
				strike=3;

			}else if(_tmpStrike==1 && _tmpBall==1){
				int tmp;
				int _tmpStrike=0, _tmpBall=0;
				num[restInd1] = outNum; 
				tmp = num[restInd1];
				testNumber[tmp]=-1; 
				testNumber[restInd2]=-1;

				checkStrike(_tmpStrike, _tmpBall);
				
				if(_tmpStrike==1 && _tmpBall==1){
					fix[restInd1] = num[restInd2]; 
					strike=2;
				}else if(_tmpStrike==1 && _tmpBall==0){
					fix[restInd2]=num[restInd1];
					strike=2;
				}
			}
		}

	}
	
	for(int i=0 ; i<10 ; i++)
	{
		printf("%3d", testNumber[i]);
	}
	printf("\n\n fix[0] : %d, fix[1] : %d, fix[2] : %d \n\n", fix[0], fix[1], fix[2]);



	/////////////////////////////////////// 4

	for(int i=0; i<3 ; i++)
	{
		num[i] = fix[i];
	}
	if(strike==2)
	{
		printf("\nstep4\n");
		int unknownInd;
		for(int i=0 ; i<3 ; i++) //-1�� ����ִ� ��ġ ( �𸣴� ��ȣ ��ġ )
		{
			if(num[i] == -1){
				unknownInd=i;
				break;
			}
		}
		for(int i=0 ; i<10 ; i++){
			if(testNumber[i] != -1){
				int _tmpStrike=0, _tmpBall=0;
				num[unknownInd]=i;
				checkStrike(_tmpStrike, _tmpBall);
				if(_tmpStrike==3){
					fix[unknownInd]=i;
					break;
				}else{
					testNumber[i]=-1;
				}
			}
		}
	}
	printf("\n\n fix[0] : %d, fix[1] : %d, fix[2] : %d \n\n", fix[0], fix[1], fix[2]);


}


int checkStrike(void)
{
	int i,j;
	int tmpStrike=0, tmpBall=0;
	for(i=0 ; i<BUFSIZE ;i++){
		for(j=0 ; j<BUFSIZE ; j++)
		{
			if( (i==j) && (num[i] == buf[j] ))
			{
				tmpStrike++;
			}
			else if( (i!=j) && (num[i] == buf[j]) )
			{
				tmpBall++;
			}
		}
	}
	if(tmpStrike==3) strike=3;
	printf("Computer Choose : %d %d %d \n", num[0], num[1], num[2]); // �� ����
	printf("Strike %d Ball %d \n", tmpStrike, tmpBall);
	return tmpStrike+tmpBall;

}

int checkStrike(int &tmpStrike, int &tmpBall)
{
	int i,j;
	tmpStrike=0;
	tmpBall=0;
	for(i=0 ; i<BUFSIZE ;i++){
		for(j=0 ; j<BUFSIZE ; j++)
		{
			if( (i==j) && (num[i] == buf[j] ))
			{
				tmpStrike++;
			}
			else if( (i!=j) && (num[i] == buf[j]) )
			{
				tmpBall++;
			}
		}
	}
	if(tmpStrike==3) strike=3;
	printf("Computer Choose : %d %d %d \n", num[0], num[1], num[2]); // �� ����
	printf("Strike %d Ball %d \n", tmpStrike, tmpBall);
	return tmpStrike+tmpBall;

}


void inputData()
{
	int checkNum=0;

	do{
		scanf_s("%1d%1d%1d", &buf[0], &buf[1], &buf[2]);

		checkNum=0;
		for(int i=0 ; i<BUFSIZE ; i++) // ���� Ȯ�� 
		{
			if( 0 <= buf[i] && buf[i] <= 9){
				
			}else
			{
				puts("�ٽ� �Է� \n");
				checkNum=1;
				break;
			}
		}

		if(buf[0] == buf[1] || buf[0]==buf[2] || buf[1]==buf[2]) //���� �� Ȯ�� 
		{
			puts("�ٽ� �Է� \n");
			checkNum=1;
		}

	}while(checkNum);
	
	printf("���� Ȯ�� %d, %d, %d \n\n", buf[0], buf[1], buf[2]);	//test
}

void intro(){
	puts("�߱����� �� :");
	puts("3�ڸ� ���� �Է� : ");
}

