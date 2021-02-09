
// OpenGLView.cpp : implementation of the COpenGLView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif

#include "OpenGLDoc.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

BEGIN_MESSAGE_MAP(COpenGLView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// COpenGLView construction/destruction

COpenGLView::COpenGLView() noexcept
{
	// TODO: add construction code here

}

COpenGLView::~COpenGLView()
{
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenGLView drawing

void COpenGLView::OnDraw(CDC* pDC)
{
	COpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// COpenGLView printing

BOOL COpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDoc* COpenGLView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
	return (COpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLView message handlers

#pragma region WindowsMessages
int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}

BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
{
	return true;

}

void COpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}

void COpenGLView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}

void COpenGLView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}

#pragma endregion




void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		m_glRenderer.x += 3;
	}
	else if (nChar == VK_LEFT) {
		m_glRenderer.x -= 3;
	}
	else if (nChar == VK_UP) {
		m_glRenderer.y += 3;
	}
	else if (nChar == VK_DOWN) {
		m_glRenderer.y -= 3;
	}
	else if (nChar == 'Z') {
		m_glRenderer.z += 3;
	}
	else if (nChar == 'R') {
		m_glRenderer.z -= 3;
	}
	else if (nChar == 'Q') {
		m_glRenderer.alfaX += 3;
	}
	else if (nChar == 'W') {
		m_glRenderer.alfaY += 3;
	}
	else if (nChar == 'E') {
		m_glRenderer.alfaZ += 3;
	}
	else if (nChar == 'K') {
		m_glRenderer.kvadar_x += 5;
	}
	else if (nChar == 'L') {
		m_glRenderer.kvadar_x -= 5;
	}
	else if (nChar == 'V') {
		m_glRenderer.kupa_z += 5;
	}
	else if (nChar == 'B') {
		m_glRenderer.kupa_z -= 5;
	}
	else if (nChar == 'M') {
		m_glRenderer.kupa_y += 5;
	}
	else if (nChar == 'N') {
		m_glRenderer.kupa_y -= 5;
	}
	else if (nChar == 'P') {
		m_glRenderer.valjak2_z += 5;
	}
	else if (nChar == 'O') {
		m_glRenderer.valjak2_z -= 5;
	}
	this->Invalidate();
}


void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_glRenderer.mouseStart = true;
	m_glRenderer.global_x = point.x;
	m_glRenderer.global_y = point.y;
}


void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_glRenderer.mouseStart = false;
}


void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_glRenderer.mouseStart) {
		m_glRenderer.alfaY = point.x - m_glRenderer.global_x;
		m_glRenderer.alfaX = point.y - m_glRenderer.global_y;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}
