
// GDIView.h : interface of the CGDIView class
//

#pragma once

#include"CMipMap.h"


class CGDIView : public CView
{
protected: // create from serialization only
	CGDIView() noexcept;
	DECLARE_DYNCREATE(CGDIView)

	CMipMap* mipmap;

// Attributes
public:
	CGDIDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void Translate(CDC*, float, float, bool);
	void Scale(CDC*, float, float, bool);
	void Rotate(CDC*, float, bool);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawWheel(CDC*, double, double, double);
	void DrawWP(CDC*, double, double, double, double, double, double);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GDIView.cpp
inline CGDIDoc* CGDIView::GetDocument() const
   { return reinterpret_cast<CGDIDoc*>(m_pDocument); }
#endif

