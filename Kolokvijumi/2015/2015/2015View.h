
// 2015View.h : interface of the CMy2015View class
//

#pragma once
#include "DImage.h"

class CMy2015View : public CView
{
protected: // create from serialization only
	CMy2015View() noexcept;
	DECLARE_DYNCREATE(CMy2015View)
	DImage background;

	int width = 1000;
	int height = 1000;

	int pacmanXOffset = 0;
	int pacmanYOffset = 0;
	int pacmanDirection = 3;
	int pacmanMouthState = 1;

	// Attributes
public:
	CMy2015Doc* GetDocument() const;

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

	void LoadIdentity(CDC* pDC);

	void DrawPacman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);
	// Implementation
public:
	virtual ~CMy2015View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in 2015View.cpp
inline CMy2015Doc* CMy2015View::GetDocument() const
{
	return reinterpret_cast<CMy2015Doc*>(m_pDocument);
}
#endif

