
// Vezba1View.h : interface of the CVezba1View class
//

#pragma once


class CVezba1View : public CView
{
private:
	int gridSize = 50;
	int lineWidth = 4;
	COLORREF red = RGB(255, 0, 0);
	COLORREF green = RGB(0, 255, 0);
	COLORREF blue = RGB(0, 0, 255);
	COLORREF yellow = RGB(255, 255, 0);
	COLORREF magenta = RGB(255, 0, 255);
	COLORREF cyan = RGB(0, 255, 255);
	COLORREF black = RGB(0, 0, 0);
	COLORREF gray = RGB(200, 200, 200);
	XFORM XformOld;

protected: // create from serialization only
	CVezba1View() noexcept;
	DECLARE_DYNCREATE(CVezba1View)

	// Attributes
public:
	CVezba1Doc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CVezba1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP();

private:
	void DrawGrid(CDC* pDC);
	void DrawPoligon(double x, double y, double numberOfPoints, double length, double angle, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawSquare(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawDiamond(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawHexagon(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawOctagon(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawTrapezoid(double x1, double y1, double x2, double y2, double lenght, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawRectangle(double x1, double y1, double x2, double y2, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	void DrawCircle(double x, double y, double r, int hatch, COLORREF line, COLORREF fill, CDC* pDC);
	CPoint* CVezba1View::DrawHalf(double x, double y, double radius, double numberOfPoints, double angle, CDC* pDC);
	void Rotate(double angle, CDC* pDC);
	void ResetRotation(CDC* pDC);

	void DrawBase(CDC* pDC);
	void DrawLonger(CDC* pDC);
	void DrawShorter(CDC* pDC);
	void DrawArm(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Vezba1View.cpp
inline CVezba1Doc* CVezba1View::GetDocument() const
{
	return reinterpret_cast<CVezba1Doc*>(m_pDocument);
}
#endif

