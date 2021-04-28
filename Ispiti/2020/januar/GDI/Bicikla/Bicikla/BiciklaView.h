
// BiciklaView.h : interface of the CBiciklaView class
//

#pragma once


class CBiciklaView : public CView
{
protected: // create from serialization only
	CBiciklaView() noexcept;
	DECLARE_DYNCREATE(CBiciklaView)

// Attributes
public:
	CBiciklaDoc* GetDocument() const;

// Operations
public:
	void Translate(CDC* pDC, double x, double y);
	void Rotate(CDC* pDC, double angle, bool right = false);
	void DrawCycle(CDC* pDC, int r, int n, int d);
	void DrawBicycle(CDC* pDC, int r, int n, int d, int alpha, int beta);
	void DrawTexty(CDC* pDC, int size, CString strText, COLORREF clrText);
	CBitmap* SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp);

	void DrawTendril(CDC* pDC, int r, int n, int d);
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
	virtual ~CBiciklaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BiciklaView.cpp
inline CBiciklaDoc* CBiciklaView::GetDocument() const
   { return reinterpret_cast<CBiciklaDoc*>(m_pDocument); }
#endif

