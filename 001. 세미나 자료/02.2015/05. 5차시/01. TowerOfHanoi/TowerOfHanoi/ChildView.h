
// ChildView.h : interface of the CChildView class
//


#pragma once


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
	int			m_nBarWidth, m_nBarHeight, m_nBarCircleHeight;
	CPoint		m_pointBarBottom[3];
	CPoint		m_pointBarTop[3];

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
	void ConvertPoint(CPoint &point, double fXRatio, double fYRatio);
	void ConvertScalar(int &nScalar, double fRatio);
	void AdjustPoint();
	void SetCoordinate();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

