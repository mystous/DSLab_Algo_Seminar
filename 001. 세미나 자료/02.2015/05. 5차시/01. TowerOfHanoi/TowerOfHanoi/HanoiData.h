#pragma once
#include <vector>

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

private:
	std::vector<DISK> m_vectorColumn;
};

