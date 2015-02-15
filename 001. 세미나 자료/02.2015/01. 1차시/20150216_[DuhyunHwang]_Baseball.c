#define use _CRT_SECURE_NO_WARNINGS

#define GAMENUM 3
#define TOTALNUM 10

#define YOURNUM 0
#define COMNUM 1
#define STRIKE1 2
#define STRIKE2 3

#define NUMTABLE 2
#define OUTWEIGHT 0
#define BALLWEIGHT 2
#define STRIKEWEIGHT 3
#define TURE 1
#define FALSE 0


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "BaseballGame.h"

//전역변수
tableElement *numTable = NULL;
ballCountNode *head = NULL;
ballCount currentBallCount;
int answer[GAMENUM] = {-1, -1, -1};
int detectFlag = TURE;
int insertFlag = TURE;
int cmpCount = 0;
int key = 0; //마지막 점검을 진행할지 결정
int gameRound = 0;

int main(void){

	int yourNum[GAMENUM];
	int comNum[GAMENUM];
	int quotient = TOTALNUM/GAMENUM;



	printScreen();
	inputYourNum(yourNum);
	while(1){
		makeComNum(comNum, &gameRound, &quotient, &key);
		if(detectFlag == TURE){
			printf("answer : %d, %d, %d\n", answer[0], answer[1], answer[2]);
			gameRound++;
			printf("===================== gameRound : %d =====================\n", gameRound);
			printNum(comNum, COMNUM);
			if(detectCount(yourNum, comNum, &currentBallCount) == 0){
				printf("answer : %d, %d, %d\n", answer[0], answer[1], answer[2]);
				printf("Success at Round %d\n", gameRound);
				exit(1);	
			}
			if(insertFlag == TURE){
				printf("BallCount(B, S, O) : %d, %d, %d\n", currentBallCount.ball, currentBallCount.strike, currentBallCount.out);
				insertNode(&head, &currentBallCount, comNum); //list에 ballCount를 wight에 따라 정렬하여 저장한다.
			}else{
				findNum(yourNum, comNum, &currentBallCount);
			}
		}
		if(detectFlag == TURE)	printBallCountList(&head);
		if(detectCount(yourNum, answer, &currentBallCount) == 0){
			printf("answer : %d, %d, %d\n", answer[0], answer[1], answer[2]);
			printf("Success at Round %d\n", gameRound);
			exit(1);	
		}
	}
	return 0;
}

int printScreen(void){
	printf("Let us start Baseball Game\n");
	return 0;
}

void inputYourNum(int * yourNum){
	printf("Input your three number (ex. \"1,2,3\") : ");
	while(1){
		//어떻게 내가 원하는 개수만큼 scan할 수 있는가?//
		if(scanf_s("%d,%d,%d", &yourNum[0], &yourNum[1], &yourNum[2]) == GAMENUM){
			if(checkNum(yourNum) == 0){
				printNum(yourNum, YOURNUM);
				break;
			}
		}
		fflush(stdin);
		printf("pleas input your collect number : ");
	}
}

int checkNum(int* yourNum){
	int i, j;
	for(i = 0; i < GAMENUM; i++){
		//Correct Num?
		if(yourNum[i] < 0 || yourNum[i] > 9){
			return -1;
		}
		//Overlapping?
		for(j = i + 1; j < GAMENUM; j++){
			if(yourNum[i] == yourNum[j]){
				return -1;
			}
		}
	}
	return 0;
}

int makeComNum(int *comNum, int *gameRound, int *quotient, int *key){
	int i, j;
	int restNum = TOTALNUM%GAMENUM;
	int startIndex = (*gameRound)*GAMENUM;

	if(*gameRound < *quotient){//전체를 한번씩 점검 하기
		if((*key) == GAMENUM){ //이미 나왔으면 계속 detecting할 필요 없음
			*quotient = FALSE;
			detectFlag = FALSE;
			return 0;
		}
		for(i = 0; i < GAMENUM; i++){
			comNum[i] = (startIndex+i);
		}
	}else if(*gameRound == *quotient){
		if(restNum == 0 || (*key) == GAMENUM){//이미 나왔으면 계속 detecting할 필요 없음
			*quotient = FALSE;
			detectFlag = FALSE;
			return 0;
		}else{
			detectFlag = TURE;
			for(i = 0; i < restNum; i++){
				comNum[i] = startIndex++;
			}
			j = 0;
			for(i = restNum; i < GAMENUM-restNum; i++){
				comNum[i] = j++;
			}
			return 0;
		}
	}else{ //전체검색 이후, weight에 따라 결정
		//weight가 큰 것 부터 처리, 처리가 완료되면 링크 제거
		detectFlag = TURE;
		insertFlag = FALSE;
		for(i = 0; i < GAMENUM; i++){
			comNum[i] = (head->comNum)[i];
		}
		//exit(1);
	}
	return 0;
}

int detectCount(int* yourNum, int* comNum, ballCount *currentBallCount){
	int i, j;
	initBallCount(currentBallCount);
	for(i = 0; i < GAMENUM; i++){
		for(j = 0; j < GAMENUM; j++){
			if(yourNum[i] == comNum[j]){
				if(i == j){
					//strike
					currentBallCount->strike++;
					if(key <= TOTALNUM) key++;
				}else{
					//ball
					currentBallCount->ball++;
					if(key <= TOTALNUM) key++;
				}
			}
		}
	}

	currentBallCount->out = GAMENUM - ( currentBallCount->strike + currentBallCount->ball );
	//printf("BallCount(B, S, O) : %d, %d, %d\n", currentBallcount->ball, currentBallcount->strike, currentBallcount->out);
	if(currentBallCount->strike == GAMENUM){
		//printf("Success at Round %d\n");
		//exit(1);	
		return 0;	
	}else return -1;
}

int initBallCount(ballCount *ballCount){
	ballCount->ball = 0;
	ballCount->strike = 0;
	ballCount->out = 0;
	return 0;
}

int printNum(int *data, int type){
	int i;
	if(type == YOURNUM){
		printf("yourNum : ");
	}else if(type ==COMNUM){
		printf("comNum : ");
	}else if(type ==STRIKE1){
		printf("strike_01 : ");
	}else if(type ==STRIKE2){
		printf("strike_02 : ");
	}
	/*
	if(type == YOURNUM || type == COMNUM){
	for(i = 0; i < GAMENUM; i++){
	printf("%d ", data[i]);
	}
	printf("\n");
	}*/
	for(i = 0; i < GAMENUM; i++){
		printf("%d ", data[i]);
	}
	printf("\n");
	return 0;
}

int printTable(tableElement **numTable){
	int i;
	printf("numTable : ");
	for(i = 0; i < TOTALNUM ; i++){
		if((*numTable)[i].c == 'x'){
			printf("%c ", (*numTable)[i].c);
		}else{
			printf("%d ", (*numTable)[i].n);
		}
	}
	printf("\n");
	return 0;
}

int insertNode(ballCountNode **head, ballCount *currentBallCount, int* comNum){
	ballCountNode *newNode, *temp, *preTemp = NULL;
	temp = *head;
	preTemp = temp;
	makeNode(&newNode, currentBallCount, comNum);
	if(*head == NULL){//head만 있을 때
		*head = newNode;
	}else{
		while(temp != NULL){
			if(temp->weight > newNode->weight){//뒤로이동 또는 마지막에 삽입
				if(temp->link == NULL){
					temp->link = newNode;
				}else{
					preTemp = temp;
					temp = temp->link;
				}
			}else{//앞에 삽입
				//node가 하나만 있는경우
				if(preTemp == temp){
					*head = newNode;
					newNode->link = temp;
				}else{//node가 여러개 있을 경어
					preTemp->link = newNode;
					newNode->link = temp;
				}
				return 0;
			}
		}
	}

}

int makeNode(ballCountNode **newNode, ballCount *currentBallCount,int* comNum){
	int i;
	*newNode = (ballCountNode*)malloc(sizeof(ballCountNode));
	for(i = 0; i < GAMENUM; i++){
		((*newNode)->comNum)[i] = comNum[i];
	}
	(*newNode)->ballCount.ball = currentBallCount->ball;
	(*newNode)->ballCount.strike = currentBallCount->strike;
	(*newNode)->ballCount.out = currentBallCount->out;
	(*newNode)->weight = calcWeight(currentBallCount);
	(*newNode)->link = NULL;
	return 0;
}

int calcWeight(ballCount *currentBallCount){
	int weight = currentBallCount->strike*STRIKEWEIGHT + currentBallCount->ball*BALLWEIGHT + currentBallCount->out*OUTWEIGHT;
	return weight;
}

int printBallCountList(ballCountNode **head){
	ballCountNode *temp;
	int i;
	temp = *head;
	while(temp != NULL){
		printf("N[");
		for(i = 0; i < GAMENUM; i++){
			printf("%d",(temp->comNum)[i]);
			if(i+1 == GAMENUM) break;
			printf(" ");
		}
		printf("]	");
		temp = temp->link;
	}
	printf("\n");
	temp = *head;
	while(temp != NULL){
		printf("C(%d, %d, %d)",temp->ballCount.ball, temp->ballCount.strike, temp->ballCount.out);
		temp = temp->link;
		if(temp != NULL) printf(" ===> ");
	}
	printf("\n");
	temp = *head;
	while(temp != NULL){
		printf("weight %d	",temp->weight);
		temp = temp->link;
	}
	printf("\n");
	return 0;
}

int removeNode(){
	head = head->link;
	return 0;
}

int findNum(int *yourNum, int *comNum, ballCount *currentBallCount){
	int i, j;
	int index = 0;
	int count = 0;
	int temp[GAMENUM];
	int saveStrike1[2][GAMENUM] = {-1, -1, -1, -1, -1, -1};
	int saveStrike2[GAMENUM] = {-1, -1, -1};

	for(i = 0; i < 6; i++){
		gameRound++;
		if(i == 0){
			temp[0] = comNum[0];
			temp[1] = comNum[1];
			temp[2] = comNum[2];
		}else if(i == 1){
			temp[0] = comNum[0];
			temp[1] = comNum[2];
			temp[2] = comNum[1];
		}else if(i == 2){
			temp[0] = comNum[1];
			temp[1] = comNum[0];
			temp[2] = comNum[2];
		}else if(i == 3){
			temp[0] = comNum[1];
			temp[1] = comNum[2];
			temp[2] = comNum[0];
		}else if(i == 4){
			temp[0] = comNum[2];
			temp[1] = comNum[0];
			temp[2] = comNum[1];
		}else if(i == 5){
			temp[0] = comNum[2];
			temp[1] = comNum[1];
			temp[2] = comNum[0];
		}
		detectCount(yourNum, temp, currentBallCount);
		if(head->weight == 4 || head->weight == 6){ //2개 찾기
			if(currentBallCount->strike == 1){
				index++;
				count++;
				for(j = 0; j < GAMENUM; j++){
					if(index == 1){
						saveStrike1[0][j] = temp[j];
					}else{
						saveStrike1[1][j] = temp[j];
					}

				}
			}else if(currentBallCount->strike == 2){
				count++;
				for(j = 0; j < GAMENUM; j++){
					saveStrike2[j] = temp[j];
				}
			}
			if(count == 3)	break;
			
		}else if(head->weight == 3 || head->weight == 2){//1개 찾기
			if(currentBallCount->strike == 1){
				count++;
				index++;
				for(j = 0; j < GAMENUM; j++){
					if(index == 1){
						saveStrike1[0][j] = temp[j];
					}else{
						saveStrike1[1][j] = temp[j];
					}
				}
			}
			if(count == 3)	break;
		}
		
	}
	if(head->weight == 4 || head->weight == 6){ //2개 찾기{
		printNum(saveStrike1[0], STRIKE1);
		printNum(saveStrike1[1], STRIKE1);
		printNum(saveStrike2, STRIKE2);
		for(i = 0; i < GAMENUM; i++){
			if(saveStrike1[0][i] ==  saveStrike2[i]){
				answer[i] = saveStrike2[i];
			}else if(saveStrike1[1][i] ==  saveStrike2[i]){
				answer[i] = saveStrike2[i];
			}
		}
	}else if(head->weight == 3 || head->weight == 2){//1개 찾기
		printNum(saveStrike1[0], STRIKE1);
		printNum(saveStrike1[1], STRIKE1);
		for(i = 0; i < GAMENUM; i++){
			if(saveStrike1[0][i] ==  saveStrike1[1][i]){
				answer[i] = saveStrike1[1][i];
			}
		}
	}
	removeNode();
	return 0;
}