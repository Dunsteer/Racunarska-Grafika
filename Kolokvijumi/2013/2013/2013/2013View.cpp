
// 2013View.cpp : implementation of the CMy2013View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2013.h"
#endif

#include "2013Doc.h"
#include "2013View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2013View

IMPLEMENT_DYNCREATE(CMy2013View, CView)

BEGIN_MESSAGE_MAP(CMy2013View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMy2013View construction/destruction

CMy2013View::CMy2013View() noexcept
{
	// TODO: add construction code here

}

CMy2013View::~CMy2013View()
{
}

BOOL CMy2013View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2013View drawing

void CMy2013View::OnDraw(CDC* /*pDC*/)
{
	CMy2013Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMy2013View printing

BOOL CMy2013View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2013View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2013View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMy2013View diagnostics

#ifdef _DEBUG
void CMy2013View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2013View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2013Doc* CMy2013View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2013Doc)));
	return (CMy2013Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2013View message handlers
