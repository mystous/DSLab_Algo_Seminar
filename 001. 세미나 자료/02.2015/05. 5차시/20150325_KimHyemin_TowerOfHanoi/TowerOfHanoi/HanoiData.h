#pragma once
#include <vector>

#define swap(a, b, temp) {(temp)=(a);(a)=(b);(b)=(temp);}

#define Left	1
#define Center	2
#define Right	3

typedef struct{
	int				nSize;
	COLORREF		brush;
} DISK, *LPDISK;

typedef struct{
		int complete;
		int temp;
		int moveCount;
		int funcCallCount;
		int stack[1024];
		int top;
		int bRtn;
		int n;

		int from;
		int by;
		int to;

} hanoi_data;

class CHanoiData
{
public:

	CHanoiData();
	~CHanoiData();

	DISK Pop();
	void Push(DISK disk);
	int GetSize(){ return m_vectorColumn.size(); }
	LPDISK GetDiskInfo(int nIndex) { return &m_vectorColumn[nIndex]; }

	static void ChangeBar(CHanoiData* pColumLeft, CHanoiData* pColumnCenter, CHanoiData* pColumnRight);

	static int isEmptyStack(int top);
	static int h_pop(int* top, int* stack);
	static void h_push(int* top,int num, int* stack);
	static void h_move(int* moveCount, int n, int from, int to);
	
	static void ChangeBar2(int x, int y, CHanoiData* pColumnLeft, CHanoiData* pColumnCenter, CHanoiData* pColumnRight);
	
private:
	std::vector<DISK> m_vectorColumn;
};

