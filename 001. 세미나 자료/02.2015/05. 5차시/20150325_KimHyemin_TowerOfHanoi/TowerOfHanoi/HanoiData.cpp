#include "stdafx.h"
#include "HanoiData.h"


CHanoiData::CHanoiData()
{
}


CHanoiData::~CHanoiData()
{
}


DISK CHanoiData::Pop()
{
	DISK		disk;

	disk.brush = RGB(0, 0, 0);
	disk.nSize = 0;

	if (m_vectorColumn.empty())
	{
		AfxMessageBox(L"막대가 비었습니다.");
		return disk;
	}

	disk = m_vectorColumn.at(m_vectorColumn.size() - 1);
	m_vectorColumn.pop_back();
	return disk;
}

void CHanoiData::Push(DISK disk)
{
	if (disk.brush == RGB(0, 0, 0) && disk.nSize == 0)
		return;

	m_vectorColumn.push_back(disk);
}

void CHanoiData::ChangeBar(CHanoiData* pColumLeft, CHanoiData* pColumnCenter, CHanoiData* pColumnRight)
{



	/*
	 //이부분은 동작을 보여주기 위한 셈플임 참고만 하고 지우셈
	static int		nOrder = 0;

	switch (nOrder)
	{
	case 0:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 1:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 2:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 3:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 4:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 5:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 6:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 7:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 8:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	}

	nOrder++;
	*/
}

int CHanoiData::isEmptyStack(int top)
{
	if(top <= -1) 
		return 1;
	else 
		return 0;
}

void CHanoiData::h_push(int* top,int num, int* stack)
{
	if(*top >= 1024)
	{
		AfxMessageBox(_T("[error] stack is full!"));
		exit(1);
	}
	stack[++(*top)]=num;
}

void CHanoiData::h_move(int* moveCount, int n, int from, int to)
{
	(*moveCount)++;
	printf("[%5d]th\tmove number %d from %d to %d\n", *moveCount, n, from, to);
}

int CHanoiData::h_pop(int* top, int* stack)
{
	if(isEmptyStack(*top))
	{
		AfxMessageBox(_T("[error] stack is empty!"));				
		return 0;
	}
	return stack[(*top)--];
}

void CHanoiData::ChangeBar2(int x, int y, CHanoiData* pColumnLeft, CHanoiData* pColumnCenter, CHanoiData* pColumnRight)
{
	int nOrder = 0;

	// left -> center
	if(x==Left&&y==Center)
		pColumnCenter->Push(pColumnLeft->Pop());

	// left -> right
	else if(x==Left&&y==Right)
		pColumnRight->Push(pColumnLeft->Pop());
	
	// center -> left
	else if(x==Center&&y==Left)
		pColumnLeft->Push(pColumnCenter->Pop());

	// center -> right
	else if(x==Center&&y==Right)
		pColumnRight->Push(pColumnCenter->Pop());

	// right -> left
	else if(x==Right&&y==Left)
		pColumnLeft->Push(pColumnRight->Pop());
	
	// right -> center
	else
		pColumnCenter->Push(pColumnRight->Pop());
}