typedef struct ballCount{
	int ball;
	int strike;
	int out;
}ballCount;

typedef union tableElement{
	int n;
	char c;
}tableElement;

struct ballCountNode{
	int comNum[GAMENUM];
	ballCount ballCount;
	int weight;
	struct ballCountNode *link;
};

typedef struct ballCountNode ballCountNode;


int printScreen(void);
void inputYourNum(int *yourNum);
int checkNum(int *yourNum);
int makeComNum(int *comNum, int *gameRound, int *quotient, int *key);
int detectCount(int *yourNum, int *comNum, ballCount *currentBallCount);
int initBallCount(ballCount *ballCount);
int printNum(int *data, int numType);
int insertNode(ballCountNode **head, ballCount *currentBallCount, int* comNum);
int removeNode();
int makeNode(ballCountNode **newNode, ballCount *currentBallCount, int* comNum);
int calcWeight(ballCount *currentBallCount);
int printBallCountList(ballCountNode **head);
int findNum(int *yourNum, int *comNum, ballCount *currentBallCount);