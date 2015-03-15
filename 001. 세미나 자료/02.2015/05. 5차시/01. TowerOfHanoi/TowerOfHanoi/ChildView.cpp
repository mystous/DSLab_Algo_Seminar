
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "TowerOfHanoi.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	SetCoordinate();
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
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
	AdjustPoint();
	DrawHanoiBoard(pDC);
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
		leftPt[0].SetPoint(m_pointBarBottom[i].x - m_nBarWidth / 2, m_pointBarBottom[i].y - m_nBarCircleHeight / 2);
		rightPt[0].SetPoint(m_pointBarBottom[i].x + m_nBarWidth / 2, m_pointBarBottom[i].y + m_nBarCircleHeight / 2);
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
	m_nBarCircleHeight = 3;
	m_nBarHeight = 50;

	m_pointBarBottom[0].SetPoint(33, 75);
	m_pointBarBottom[1] = m_pointBarBottom[0];
	m_pointBarBottom[1].Offset(17, 0);
	m_pointBarBottom[2] = m_pointBarBottom[1];
	m_pointBarBottom[2].Offset(17, 0);

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

	SetCoordinate();
	Invalidate();
}
