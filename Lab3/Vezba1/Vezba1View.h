
// Vezba1View.h : interface of the CVezba1View class
//

#pragma once

typedef struct hatch {
	char R;
	char G;
	char B;
	char type;
}Hatch;

class CVezba1View : public CView
{
private:
	int gridSize = 25;
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

	double baseAngle = 0;
	double bigArmAngle = 0;
	double smallArmAngle = 0;
	double pincerAngle = 0;

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
	void Resize(double scale,CDC* pDC);
	void Rotate(double angle, CDC* pDC);
	void Translate(double x, double y, CDC* pDC);
	void ResetTranslate(CDC *pDC);
	void MirrorVertical(CDC* pDC);

	void FilterPixel(BYTE* buffer, int x, int y, int width, Hatch h);

	CPoint DrawBase(CPoint origin, CDC* pDC);
	CPoint DrawLonger(CPoint origin, CDC* pDC);
	CPoint DrawShorter(CPoint origin, CDC* pDC);
	CPoint DrawArm(CPoint origin, CDC* pDC);

	void MakeTransparent(CPoint point, Hatch h, int slika, CDC* pDC);
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in Vezba1View.cpp
inline CVezba1Doc* CVezba1View::GetDocument() const
{
	return reinterpret_cast<CVezba1Doc*>(m_pDocument);
}
#endif



