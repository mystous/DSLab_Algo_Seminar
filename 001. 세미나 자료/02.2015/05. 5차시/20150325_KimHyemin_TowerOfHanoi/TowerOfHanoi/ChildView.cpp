
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "TowerOfHanoi.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HANOI_TIMER			10


// CChildView

CChildView::CChildView()
{
	InitDisk();

}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::DrawImage(CDC *pDC)
{
	CRect rect;
	GetClientRect(&rect);

	// 메모리 DC 선언
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // 흰색으로 초기화

	// 메모리 DC에 그리기
	DrawHanoi(&memDC);

	// 메모리 DC를 화면 DC에 고속 복사
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CChildView::DrawHanoi(CDC *pDC)
{
	SetCoordinate();
	AdjustPoint();
	DrawHanoiBoard(pDC);
	DrawDiskSection(pDC);
}

void CChildView::DrawDiskSection(CDC *pDC)
{
	int					i;

	for (i = 0; i < 3; i++)
		DrawColumn(pDC, &m_column[i], m_pointBarBottom[i], m_pointBarBottom[i]);
}

void CChildView::DrawColumn(CDC *pDC, CHanoiData *pHanoiData, CPoint pointTop, CPoint pointBottom)
{
	int						i, nSize = pHanoiData->GetSize();

	for (i = 0; i < nSize; i++)
	{
		LPDISK		lpDisk = pHanoiData->GetDiskInfo(i);
		CPoint		pointDiskBottom = pointBottom;

		pointDiskBottom.Offset(0, -m_nDiskCircleHeight*i - m_nDiskHeight*i - 20);
		DrawDisk(pDC, pointDiskBottom, lpDisk->nSize, lpDisk->brush);

		if (i == nSize - 1)
		{
			pointDiskBottom.Offset(0, -m_nDiskHeight);
			DrawDiskTop(pDC, pointDiskBottom, lpDisk->nSize);
		}
	}
}

void CChildView::DrawDiskTop(CDC *pDC, CPoint pointBottom, int nSizeStep)
{
	CRect				rect[3];
	CPoint				leftPt[2], rightPt[2];
	int					nHeightRatio = 2;
	CBrush				*pDldBrush;
	CBrush				brushWhite(RGB(255, 255, 255));

	leftPt[0].SetPoint(pointBottom.x - m_nBarWidth / 2, pointBottom.y - m_nBarCircleHeight / 2);
	rightPt[0].SetPoint(pointBottom.x + m_nBarWidth / 2, pointBottom.y + m_nBarCircleHeight / 2);
	rect[0].SetRect(leftPt[0].x, leftPt[0].y, rightPt[0].x, rightPt[0].y);

	rect[1] = rect[0];
	rect[1].OffsetRect(0, -m_nBarCircleHeight / 2);
	rect[1].top -= m_nDiskCircleHeight * 8;

	leftPt[0].x = rect[1].left;
	leftPt[0].y = rect[1].top;
	leftPt[1] = leftPt[0];
	leftPt[1].y = rect[1].bottom;

	rightPt[0].x = rect[1].right - 1;
	rightPt[0].y = rect[1].top;
	rightPt[1] = rightPt[0];
	rightPt[1].y = rect[1].bottom;

	pDC->Ellipse(rect[0]);
	pDC->FillSolidRect(rect[1], RGB(255,255,255));

	pDC->MoveTo(leftPt[0]);
	pDC->LineTo(leftPt[1]);

	pDC->MoveTo(rightPt[0]);
	pDC->LineTo(rightPt[1]);


}

void CChildView::DrawDisk(CDC *pDC, CPoint pointBottom, int nSizeStep, COLORREF color)
{
	CRect				rect[3];
	CPoint				leftPt[2], rightPt[2];
	CBrush				brushFill(color);
	int					nHeightRatio = 2;
	CBrush				*pDldBrush;

	leftPt[0].SetPoint(pointBottom.x - m_nDiskRadius * (nSizeStep+1), pointBottom.y - m_nDiskCircleHeight * nSizeStep);
	rightPt[0].SetPoint(pointBottom.x + m_nDiskRadius * (nSizeStep+1), pointBottom.y + m_nDiskCircleHeight * nSizeStep);
	rect[0].SetRect(leftPt[0].x, leftPt[0].y, rightPt[0].x, rightPt[0].y);

	leftPt[1] = leftPt[0];
	rightPt[1] = rightPt[0];

	leftPt[0].y = pointBottom.y;
	leftPt[1].y = pointBottom.y - m_nDiskHeight;

	rightPt[0].y = pointBottom.y;
	rightPt[1].y = pointBottom.y - m_nDiskHeight;

	rect[1] = rect[0];
	rect[1].OffsetRect(0, -m_nDiskHeight);

	rect[2].SetRect(leftPt[0].x, leftPt[0].y, rightPt[1].x, rightPt[1].y);

	pDldBrush = pDC->SelectObject(&brushFill);
	pDC->Ellipse(rect[0]);
	pDC->FillSolidRect(rect[2], color);
	pDC->Ellipse(rect[1]);
	pDC->SelectObject(pDldBrush);

	pDC->MoveTo(leftPt[0]);
	pDC->LineTo(leftPt[1]);

	pDC->MoveTo(rightPt[0]);
	pDC->LineTo(rightPt[1]);
}

void CChildView::DrawHanoiBoard(CDC *pDC)
{
	int					i;
	CRect				rect[3];
	CPoint				leftPt[2], rightPt[2];
	CBrush				brushWhite(RGB(255, 255, 255));



	pDC->MoveTo(m_pointBasePointLeftBottom);
	pDC->LineTo(m_pointBasePointLeftTop);
	pDC->LineTo(m_pointBasePointRightTop);
	pDC->LineTo(m_pointBasePointRightBottom);
	pDC->LineTo(m_pointBasePointLeftBottom);
	pDC->LineTo(m_pointFloorPointLeft);
	pDC->LineTo(m_pointFloorPointRight);
	pDC->LineTo(m_pointBasePointRightBottom);

	for (i = 0; i < 3; i++)
	{
		leftPt[0].SetPoint(m_pointBarBottom[i].x - m_nBarWidth / 2, m_pointBarBottom[i].y - m_nBarCircleHeight / 2 - m_nAdjustValue);
		rightPt[0].SetPoint(m_pointBarBottom[i].x + m_nBarWidth / 2, m_pointBarBottom[i].y + m_nBarCircleHeight / 2 - m_nAdjustValue);
		rect[0].SetRect(leftPt[0].x, leftPt[0].y, rightPt[0].x, rightPt[0].y);

		leftPt[1].SetPoint(m_pointBarTop[i].x - m_nBarWidth / 2, m_pointBarTop[i].y - m_nBarCircleHeight / 2);
		rightPt[1].SetPoint(m_pointBarTop[i].x + m_nBarWidth / 2, m_pointBarTop[i].y + m_nBarCircleHeight / 2);
		rect[1].SetRect(leftPt[1].x, leftPt[1].y, rightPt[1].x, rightPt[1].y);

		leftPt[0].Offset(0, m_nBarCircleHeight / 2);
		rightPt[0].Offset(-1, -m_nBarCircleHeight / 2);

		leftPt[1].Offset(0, m_nBarCircleHeight / 2);
		rightPt[1].Offset(-1, -m_nBarCircleHeight / 2);

		rect[2].SetRect(leftPt[0].x, leftPt[0].y, rightPt[1].x, rightPt[1].y);

		pDC->FillRect(rect[2], &brushWhite);

		pDC->Arc(rect[0], leftPt[0], rightPt[0]);
		pDC->Ellipse(rect[1]);

		pDC->MoveTo(leftPt[0]);
		pDC->LineTo(leftPt[1]);
		pDC->MoveTo(rightPt[0]);
		pDC->LineTo(rightPt[1]);
	}
}

void CChildView::AdjustPoint()
{
	CRect		rectScreen;
	int			i;
	int			nWidth, nHeight;
	double		fXRatio, fYRatio;

	GetClientRect(&rectScreen);

	nWidth = rectScreen.Width();
	nHeight = rectScreen.Height();

	fXRatio = nWidth / 100.;
	fYRatio = nHeight / 100.;

	ConvertPoint(m_pointBasePointLeftBottom, fXRatio, fYRatio);
	ConvertPoint(m_pointBasePointRightBottom, fXRatio, fYRatio);
	ConvertPoint(m_pointBasePointLeftTop, fXRatio, fYRatio);
	ConvertPoint(m_pointBasePointRightTop, fXRatio, fYRatio);
	ConvertPoint(m_pointFloorPointRight, fXRatio, fYRatio);
	ConvertPoint(m_pointFloorPointLeft, fXRatio, fYRatio);

	ConvertScalar(m_nBarWidth, fXRatio);
	ConvertScalar(m_nBarCircleHeight, fYRatio);
	ConvertScalar(m_nDiskHeight, fYRatio);
	ConvertScalar(m_nDiskRadius, fXRatio);
	ConvertScalar(m_nDiskCircleHeight, fYRatio);
	ConvertScalar(m_nAdjustValue, fYRatio);

	for (i = 0; i < 3; i++)
	{
		ConvertPoint(m_pointBarBottom[i], fXRatio, fYRatio);
		ConvertPoint(m_pointBarTop[i], fXRatio, fYRatio);
	}
}

void CChildView::ConvertPoint(CPoint &point, double fXRatio, double fYRatio)
{
	point.SetPoint(point.x * fXRatio, point.y * fYRatio);
}

void CChildView::ConvertScalar(int &nScalar, double fRatio)
{
	nScalar *= fRatio;
}

void CChildView::SetCoordinate()
{
	m_pointBasePointLeftBottom.SetPoint(20, 80);
	m_pointBasePointRightBottom.SetPoint(80, 80);
	m_pointBasePointLeftTop.SetPoint(23, 70);
	m_pointBasePointRightTop.SetPoint(77, 70);
	m_pointFloorPointRight.SetPoint(80, 85);
	m_pointFloorPointLeft.SetPoint(20, 85);

	m_nBarWidth = 2;
	m_nBarCircleHeight = 1;
	m_nBarHeight = 50;
	m_nDiskRadius = 1;
	m_nDiskHeight = 3;
	m_nDiskCircleHeight = 1;
	m_nAdjustValue = 3;

	m_pointBarBottom[0].SetPoint(31, 75);
	m_pointBarBottom[1] = m_pointBarBottom[0];
	m_pointBarBottom[1].Offset(19, 0);
	m_pointBarBottom[2] = m_pointBarBottom[1];
	m_pointBarBottom[2].Offset(19, 0);

	m_pointBarTop[0] = m_pointBarBottom[0];
	m_pointBarTop[1] = m_pointBarBottom[1];
	m_pointBarTop[2] = m_pointBarBottom[2];

	m_pointBarTop[0].Offset(0, -m_nBarHeight);
	m_pointBarTop[1].Offset(0, -m_nBarHeight);
	m_pointBarTop[2].Offset(0, -m_nBarHeight);
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawImage(&dc);
}



void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Invalidate();
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CHanoiData::ChangeBar(&m_column[0], &m_column[1], &m_column[2]);

		//test 초기화 ////////////////////////////////////////////////////////////////////////////////

	SetTimer(HANOI_TIMER, 100, NULL);

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::InitDisk()
{
	int				i, j;

	srand(time(NULL));
	SetCoordinate();
	for (j = 1; j < 2;j++)
		for (i = 8; i > 0 ; i--)
		{
			DISK			disk;

			disk.brush = RGB(rand() / 256, rand() / 256, rand() / 256);
			disk.nSize = i ;
			m_column[j].Push(disk);
		}

	m_hadata.complete=0;
	m_hadata.temp=0;
	m_hadata.moveCount=0;
	m_hadata.funcCallCount=0;

	m_hadata.top=-1;
	m_hadata.bRtn=0;
	//총 접시의 개수
	m_hadata.n=8;
	//ex) 3개의 접시를 from:1 에서 by:2 를 경유하여 to:3 으로 이동 
	m_hadata.from=2;
	m_hadata.by=3;
	m_hadata.to=1;
}

void CChildView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	Invalidate();
}


void CChildView::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	Invalidate();
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (!m_hadata.complete)
	{

		if ((m_hadata.bRtn % 2) == 0)
		{
			while (m_hadata.n > 1)
			{
				CHanoiData::h_push(&m_hadata.top, m_hadata.to, m_hadata.stack);
				CHanoiData::h_push(&m_hadata.top, m_hadata.by, m_hadata.stack);
				CHanoiData::h_push(&m_hadata.top, m_hadata.from, m_hadata.stack);
				CHanoiData::h_push(&m_hadata.top, m_hadata.n, m_hadata.stack);
				(m_hadata.n)--;
				swap(m_hadata.to, m_hadata.by, m_hadata.temp);

			}
			CHanoiData::h_move(&(m_hadata.moveCount), m_hadata.n, m_hadata.from, m_hadata.to);

			CHanoiData::ChangeBar2(m_hadata.from, m_hadata.to, &m_column[0], &m_column[1], &m_column[2]);

		}

		if ((m_hadata.bRtn % 2) == 1)
		{
			if (!(CHanoiData::isEmptyStack(m_hadata.top)))
			{
				m_hadata.n = CHanoiData::h_pop(&m_hadata.top, m_hadata.stack);
				m_hadata.from = CHanoiData::h_pop(&m_hadata.top, m_hadata.stack);
				m_hadata.by = CHanoiData::h_pop(&m_hadata.top, m_hadata.stack);
				m_hadata.to = CHanoiData::h_pop(&m_hadata.top, m_hadata.stack);
				CHanoiData::h_move(&(m_hadata.moveCount), m_hadata.n, m_hadata.from, m_hadata.to);

				CHanoiData::ChangeBar2(m_hadata.from, m_hadata.to, &m_column[0], &m_column[1], &m_column[2]);


				(m_hadata.n)--;
				swap(m_hadata.from, m_hadata.by, m_hadata.temp);

			}
			else
			{
				m_hadata.complete = 1;
				KillTimer(HANOI_TIMER);
				AfxMessageBox(_T("Calculation is finished!"));
			}
		}

		m_hadata.bRtn++;
		InvalidateRect(NULL);
	}
		


	CWnd::OnTimer(nIDEvent);
}
