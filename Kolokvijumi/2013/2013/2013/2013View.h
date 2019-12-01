
// 2013View.h : interface of the CMy2013View class
//

#pragma once


class CMy2013View : public CView
{
protected: // create from serialization only
	CMy2013View() noexcept;
	DECLARE_DYNCREATE(CMy2013View)

// Attributes
public:
	CMy2013Doc* GetDocument() const;

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
	void DrawBlade(CDC* pDC, int size);

// Implementation
public:
	virtual ~CMy2013View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in 2013View.cpp
inline CMy2013Doc* CMy2013View::GetDocument() const
   { return reinterpret_cast<CMy2013Doc*>(m_pDocument); }
#endif

