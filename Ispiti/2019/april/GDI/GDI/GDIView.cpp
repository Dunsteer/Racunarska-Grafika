
// GDIView.cpp : implementation of the CGDIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI.h"
#endif

#include "GDIDoc.h"
#include "GDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGDIView

IMPLEMENT_DYNCREATE(CGDIView, CView)

BEGIN_MESSAGE_MAP(CGDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGDIView construction/destruction

CGDIView::CGDIView() noexcept
{
	// TODO: add construction code here
}

CGDIView::~CGDIView()
{
}

BOOL CGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDIView drawing

void CGDIView::OnDraw(CDC* pDC)
{
	CGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	SetGraphicsMode(*pDC, GM_ADVANCED);

	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = 1.0;

	pDC->SetWorldTransform(lpxform);

	this->DrawWP(pDC, 200, 150, 50, 800, 20, 200);
	//this->mipmap = new CMipMap(new CString(""), pDC);
	//this->mipmap->DrawMipmap(pDC, 0, 0, 300);

	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CGDIView::Translate(CDC* pDC, float dx, float dy, bool right)
{
	XFORM* lpxform = new XFORM();
	lpxform->eDx = dx;
	lpxform->eDy = dy;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = 1.0;
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDIView::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = sx;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = sy;
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDIView::Rotate(CDC* pDC, float alpha, bool right)
{
	XFORM* lpxform = new XFORM();
	float rad = alpha * (3.14 / 180);
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = cos(rad);
	lpxform->eM12 = sin(rad);
	lpxform->eM21 = -sin(rad);
	lpxform->eM22 = cos(rad);
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDIView::DrawWheel(CDC* pDC, double r1, double r2, double w)
{
	double cx = 0;
	double cy = 0;
	double rTemp = (r1 + r2) / 2;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(100, 100, 100);

	LOGBRUSH logBrush2;
	logBrush2.lbStyle = BS_SOLID;
	logBrush2.lbColor = RGB(0, 0, 0);

	CPen pen1(PS_GEOMETRIC | PS_ENDCAP_FLAT, w, &logBrush2);
	CPen pen2(PS_GEOMETRIC | PS_ENDCAP_FLAT, w - 20, &logBrush);

	for (int i = 0; i < 3; i++) {
		if (i > 0) {
			this->Rotate(pDC, -60, false);
		}

		pDC->MoveTo(cx, cy - rTemp);

		pDC->SelectObject(&pen1);
		pDC->LineTo(cx, cy + rTemp);

		pDC->MoveTo(cx, cy - rTemp);

		pDC->SelectObject(&pen2);
		pDC->LineTo(cx, cy + rTemp);
	}

	double a1 = cx - rTemp;
	double b1 = cy - rTemp;
	double a2 = cx + rTemp;
	double b2 = cy + rTemp;

	pDC->SelectObject(&pen1);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Ellipse(a1, b1, a2, b2);

	pDC->SelectObject(&pen2);
	pDC->Ellipse(a1, b1, a2, b2);
}

void CGDIView::DrawWP(CDC* pDC, double r1, double r2, double w, double L, double alpha, double d)
{
	double rad = 3.14 * alpha / 180;
	double h = tan(rad) * L;
	double circumference = 6.28 * r1;
	double angle = d / circumference * 360;
	double naspramna = sin(rad) * d;
	double nalegla = cos(rad) * d;

	CPen pen1(0, 2.5, RGB(0, 0, 0));
	pDC->SelectObject(&pen1);
	CBrush brush1(RGB(100, 100, 100));
	pDC->SelectObject(&brush1);

	pDC->BeginPath();
	pDC->MoveTo(r1, r1 * 2 );
	pDC->LineTo(r1, r1 * 2 + h);
	pDC->LineTo(r1+ L, r1 * 2 + h);
	pDC->LineTo(r1, r1 * 2);
	pDC->EndPath();
	pDC->StrokeAndFillPath();

	this->Translate(pDC, r1 / cos(rad) + nalegla, r1 / cos(rad) + naspramna, true);
	this->Rotate(pDC, angle, false);

	this->DrawWheel(pDC, r1, r2, w);


}

// CGDIView printing

BOOL CGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDIView diagnostics

#ifdef _DEBUG
void CGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDIDoc* CGDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIDoc)));
	return (CGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CGDIView message handlers
