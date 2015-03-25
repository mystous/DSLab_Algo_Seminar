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

void CHanoiData::MoveTower(int n, CHanoiData* pColumnCenter, CHanoiData* pColumLeft, CHanoiData* pColumnRight)
{
	if(n == 0){}
	else{
		MoveTower(n-1, pColumnCenter, pColumnRight, pColumLeft);
		pColumLeft->Push(pColumnCenter->Pop());
		MoveTower(n-1, pColumnRight, pColumLeft, pColumnCenter);
	}
}


void CHanoiData::ChangeBar(CHanoiData* pColumLeft, CHanoiData* pColumnCenter, CHanoiData* pColumnRight)
{
	do
	{
		if(pColumnCenter->GetSize() <= 0)
		{
			break;
		}
		MoveTower(pColumnCenter->GetSize(), pColumnCenter, pColumLeft, pColumnRight);
	}while(pColumnCenter->GetSize() == 0);



	/*pColumnCenter->GetSize();	
	pColumnCenter->Pop().nSize;
	*/


	/*switch (nOrder)
	{
	case 0:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 1:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 2:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 3:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 4:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 5:
		pColumnRight->Push(pColumLeft->Pop());
		break;
	case 6:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 7:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 8:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 9:
		pColumnCenter->Push(pColumnRight->Pop());
		break;
	case 10:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 11:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 12:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 13:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 14:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 15:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 16:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 17:
		pColumnRight->Push(pColumLeft->Pop());
		break;
	case 18:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 19:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 20:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 21:
		pColumnCenter->Push(pColumnRight->Pop());
		break;
	case 22:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 23:
		pColumnRight->Push(pColumLeft->Pop());
		break;
	case 24:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 25:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 26:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 27:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 28:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 29:
		pColumnRight->Push(pColumLeft->Pop());
		break;
	case 30:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 31:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 32:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 33:
		pColumnCenter->Push(pColumnRight->Pop());
		break;
	case 34:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 35:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 36:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 37:
		pColumLeft->Push(pColumnCenter->Pop());
		break;
	case 38:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 39:
		pColumnCenter->Push(pColumnRight->Pop());
		break;
	case 40:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 41:
		pColumnRight->Push(pColumLeft->Pop());
		break;
	case 42:
		pColumnRight->Push(pColumnCenter->Pop());
		break;
	case 43:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 44:
		pColumLeft->Push(pColumnRight->Pop());
		break;
	case 45:
		pColumnCenter->Push(pColumnRight->Pop());
		break;
	case 46:
		pColumnCenter->Push(pColumLeft->Pop());
		break;
	case 47:
		pColumLeft->Push(pColumnRight->Pop());
		break;

	}
	nOrder++;*/

		

}