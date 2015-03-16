
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "HanoiData.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
protected:
	CPoint		m_pointBasePointLeftBottom;
	CPoint		m_pointBasePointRightBottom;
	CPoint		m_pointBasePointLeftTop;
	CPoint		m_pointBasePointRightTop;
	CPoint		m_pointFloorPointRight;
	CPoint		m_pointFloorPointLeft;
	int			m_nAdjustValue;
	int			m_nBarWidth, m_nBarHeight, m_nBarCircleHeight, m_nDiskRadius, m_nDiskHeight, m_nDiskCircleHeight;
	CPoint		m_pointBarBottom[3];
	CPoint		m_pointBarTop[3];
	CHanoiData	m_column[3];

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	void DrawImage(CDC *pDC);
	void DrawHanoi(CDC *pDC);
	void DrawHanoiBoard(CDC *pDC);
	void DrawDiskSection(CDC *pDC);
	void DrawColumn(CDC *pDC, CHanoiData *pHanoiData, CPoint pointTop, CPoint pointBottom);
	void DrawDisk(CDC *pDC, CPoint pointBottom, int nSizeStep, COLORREF color);
	void DrawDiskTop(CDC *pDC, CPoint pointBottom, int nSizeStep);
	void ConvertPoint(CPoint &point, double fXRatio, double fYRatio);
	void ConvertScalar(int &nScalar, double fRatio);
	void AdjustPoint();
	void SetCoordinate();
	void InitDisk();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

