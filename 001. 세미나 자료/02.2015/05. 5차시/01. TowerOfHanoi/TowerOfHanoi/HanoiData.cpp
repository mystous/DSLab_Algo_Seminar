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
		AfxMessageBox(L"���밡 ������ϴ�.");
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
	/* �̺κ��� ������ �����ֱ� ���� ������ ���� �ϰ� �����*/
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
}