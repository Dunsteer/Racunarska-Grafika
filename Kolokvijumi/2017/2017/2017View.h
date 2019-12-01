
// 2017View.h : interface of the CMy2017View class
//

#pragma once
#include "DImage.h"


class CMy2017View : public CView
{
protected: // create from serialization only
	CMy2017View() noexcept;
	DECLARE_DYNCREATE(CMy2017View)

	DImage* body = nullptr;
	DImage* leg1 = nullptr;
	DImage* leg2 = nullptr;
	DImage* leg3 = nullptr;
	DImage* back2 = nullptr;

	int width = 1500;
	int height = 1000;

	int backgroundXOffset = 0;
	int backgroundYOffset = 0;

	int transporterOffset = 0;
	float transporterScale = 1;

	int leg1Angle = 20;
	int leg1Direction = -1;

	int leg2Angle = -20;
	int leg2Direction = 1;

	int leg3Angle = 0;
	int leg3Direction = -1;

	int leg4Angle = 0;
	int leg4Direction = 1;

	// Attributes
public:
	CMy2017Doc* GetDocument() const;

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
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);

	// Implementation
public:
	virtual ~CMy2017View();
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

#ifndef _DEBUG  // debug version in 2017View.cpp
inline CMy2017Doc* CMy2017View::GetDocument() const
{
	return reinterpret_cast<CMy2017Doc*>(m_pDocument);
}
#endif

