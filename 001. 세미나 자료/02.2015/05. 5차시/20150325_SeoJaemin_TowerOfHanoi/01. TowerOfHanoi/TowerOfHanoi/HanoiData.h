#pragma once
#include <vector>

#define swap(a, b, temp) {(temp)=(a);(a)=(b);(b)=(temp);}

typedef struct{
	int				nSize;
	COLORREF		brush;
} DISK, *LPDISK;

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

	static void CHanoiData::MoveTower(int n, CHanoiData* pColumnCenter, CHanoiData* pColumLeft, CHanoiData* pColumnRight);

private:
	std::vector<DISK> m_vectorColumn;
};

