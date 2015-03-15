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

private:
	std::vector<DISK> m_vectorColumn;
};

