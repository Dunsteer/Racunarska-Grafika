
// 2014View.h : interface of the CMy2014View class
//

#pragma once
#include "DImage.h"


class CMy2014View : public CView
{
private:
	DImage wheelImage;
	HENHMETAFILE carImage;
	float groundAngle;
	int carMoved;
	int width;
	int height;
protected: // create from serialization only
	CMy2014View() noexcept;
	DECLARE_DYNCREATE(CMy2014View)

	// Attributes
public:
	CMy2014Doc* GetDocument() const;

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
	void DrawGround(CDC* pDC, float angle);
	XFORM Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawCar(CDC* pDC, int x, int y, int w, int h);
	void DrawWheel(CDC* pDC, int x, int y, int r, float angle);

	// Implementation
public:
	virtual ~CMy2014View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in 2014View.cpp
inline CMy2014Doc* CMy2014View::GetDocument() const
{
	return reinterpret_cast<CMy2014Doc*>(m_pDocument);
}
#endif

