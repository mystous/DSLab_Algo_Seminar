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

	disk = m_vectorColumn.at(m_vectorColumn.size() - 1);
	return disk;
}

void CHanoiData::Push(DISK disk)
{
	m_vectorColumn.push_back(disk);
}