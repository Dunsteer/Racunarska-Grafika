
// BiciklaView.cpp : implementation of the CBiciklaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Bicikla.h"
#endif

#include "BiciklaDoc.h"
#include "BiciklaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ToRad 3.14/180;

// CBiciklaView

IMPLEMENT_DYNCREATE(CBiciklaView, CView)

BEGIN_MESSAGE_MAP(CBiciklaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CBiciklaView construction/destruction

CBiciklaView::CBiciklaView() noexcept
{
	// TODO: add construction code here

}

CBiciklaView::~CBiciklaView()
{
}

BOOL CBiciklaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBiciklaView drawing

void CBiciklaView::OnDraw(CDC* pDC)
{
	CBiciklaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	SetGraphicsMode(*pDC, GM_ADVANCED);

	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = 1.0;

	pDC->SetWorldTransform(lpxform);

	this->Translate(pDC, 500, 500);

	DrawBicycle(pDC, 150, 20, 5, 10, 20);
	Translate(pDC, 2.5 * 150, 150);
	Rotate(pDC, -3.14 / 2);
	DrawTexty(pDC, 150, CString("penny-farthing"), RGB(69, 23, 169));

	//DrawTendril(pDC, 100, 6, 5);
}

void CBiciklaView::Translate(CDC* pDC, double x, double y)
{
	XFORM* form = new XFORM();
	form->eDx = x;
	form->eDy = y;
	form->eM11 = 1.0;
	form->eM12 = 0.0;
	form->eM21 = 0.0;
	form->eM22 = 1.0;
	pDC->ModifyWorldTransform(form, MWT_LEFTMULTIPLY);
}

void CBiciklaView::Rotate(CDC* pDC, double angle, bool right)
{
	XFORM* form = new XFORM();
	form->eDx = 0;
	form->eDy = 0;
	form->eM11 = cos(angle);
	form->eM12 = sin(angle);
	form->eM21 = -sin(angle);
	form->eM22 = cos(angle);
	pDC->ModifyWorldTransform(form, right?MWT_RIGHTMULTIPLY: MWT_LEFTMULTIPLY);
}

void CBiciklaView::DrawCycle(CDC* pDC, int r, int n, int d)
{
	double angle = 360.0 / n;
	double rads = angle * 3.14 / 180;

	CPen* pen1 = new CPen(0, d, RGB(68, 123, 169));
	pDC->SelectObject(pen1);
	pDC->Ellipse(-r, -r, r, r);
	CPen* pen2 = new CPen(0, 1, RGB(68, 123, 169));
	CBrush* brush = new CBrush(RGB(68, 123, 169));
	pDC->SelectObject(pen2);
	brush = pDC->SelectObject(brush);
	pDC->Ellipse(-2 * d, -2 * d, 2 * d, 2 * d);

	CPen* pen3 = new CPen(0, d / 3, RGB(68, 123, 169));
	pDC->SelectObject(pen3);
	for (int i = 0; i < n; i++)
	{
		double x = r * cos(rads * i);
		double y = r * sin(rads * i);
		pDC->MoveTo(0, 0);
		pDC->LineTo(x, y);
	}

	pDC->SelectObject(brush);
}

void CBiciklaView::DrawBicycle(CDC* pDC, int r, int n, int d, int alpha, int beta)
{
	double maliTocak = r / 4.0;
	double tr = 1.95 * r;

	this->DrawCycle(pDC, r, n, d);
	this->Translate(pDC, tr, r - r / 4);
	this->DrawCycle(pDC, maliTocak, n, d);
	this->Translate(pDC, -tr, -(r - r / 4));
	CPen* pen1 = new CPen(0, d, RGB(68, 123, 169));
	pDC->SelectObject(pen1);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, -1.4 * r);
	pDC->LineTo(0.2 * r, -1.4 * r);
	pDC->MoveTo(0, 0);
	this->Rotate(pDC, -alpha);
	pDC->MoveTo(0, 0.4 * r);
	pDC->LineTo(0, -0.4 * r);
	pDC->MoveTo(0, 0);

	Translate(pDC, 0, -0.4 * r);
	this->Rotate(pDC, alpha);

	pDC->LineTo(0.1 * r, 0);
	pDC->LineTo(-0.1 * r, 0);

	this->Rotate(pDC, -alpha);

	Translate(pDC, 0, 0.8 * r);

	this->Rotate(pDC, alpha);
	pDC->LineTo(0.1 * r, 0);
	pDC->LineTo(-0.1 * r, 0);

	this->Rotate(pDC, -alpha);

	Translate(pDC, 0, -0.4 * r);

	this->Rotate(pDC, alpha);

	pDC->MoveTo(0, 0);
	pDC->Arc(CRect(-1.2 * r, -1.2 * r, 1.2 * r, 1.2 * r), CPoint(1.2 * r, 0), CPoint(0, -1.2 * r));
	pDC->MoveTo(1.2 * r, 0);
	pDC->LineTo(1.4 * r, 0);

	maliTocak += 0.2 * r;

	Translate(pDC, 2.4 * r - maliTocak, 0);
	pDC->MoveTo(0, 0);
	//pDC->LineTo(0, 1.2 * r - maliTocak);

	pDC->Arc(CRect(-1.2 * r + maliTocak, -1.2 * r + maliTocak, 1.2 * r - maliTocak, 1.2 * r - maliTocak), CPoint(-1.2 * r + maliTocak, 0), CPoint(0, 1.2 * r - maliTocak));

	Translate(pDC, -(2.4 * r - maliTocak), 0);

}

void CBiciklaView::DrawTexty(CDC* pDC, int size, CString strText, COLORREF clrText)
{
	CFont* font = new CFont();
	font->CreatePointFont(size, CString("Arial"));
	pDC->SelectObject(font);
	pDC->DrawText(strText, CRect(0, 0, size * strText.GetLength(), size), 0);
}

void CBiciklaView::DrawTendril(CDC* pDC, int r, int n, int d)
{
	float angle = 360 / n * ToRad;
	float stranica = sin(angle / 2) * r;
	float visina = cos(angle / 2) * r;
	pDC->MoveTo(0, 0);
	CPen *tanak=new CPen(0, d, RGB(255, 255, 255));
	CPen* debeo = new CPen(0, d * 3, RGB(255, 255, 255));

	CPen *obj = pDC->SelectObject(debeo);

	pDC->Rectangle(-stranica, 8 * r, stranica, 0);

	pDC->SelectObject(tanak);

	Translate(pDC, 0, 8 * r + visina);

	for (int i = 0; i < n - 1; i++) {
		int c = cos(angle * i);
		int s = sin(angle * i);

		pDC->MoveTo(c * r, s * r);
		pDC->LineTo(c * r * 2, s * r * 2);

		c = cos(angle * (i + 1));
		s = sin(angle * (i + 1));

		pDC->LineTo(c * r, s * r);
	}

	pDC->SelectObject(obj);

	delete tanak;
	delete debeo;
}

CBitmap* CBiciklaView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp)
{
	CDC* pDCmem = new CDC();

	CBitmap* bmp = new CBitmap();
	bmp->CreateBitmap(rcBmp.Width(), rcBmp.Height(), 4, 8, NULL);

	pDCmem->SelectObject(bmp);
	pDCmem->BitBlt(rcBmp.left, rcBmp.top, rcBmp.right, rcBmp.bottom, pDC, rcDC.top, rcDC.left, SRCCOPY);

	return bmp;
}


// CBiciklaView printing

BOOL CBiciklaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBiciklaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBiciklaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBiciklaView diagnostics

#ifdef _DEBUG
void CBiciklaView::AssertValid() const
{
	CView::AssertValid();
}

void CBiciklaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBiciklaDoc* CBiciklaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBiciklaDoc)));
	return (CBiciklaDoc*)m_pDocument;
}
#endif //_DEBUG


// CBiciklaView message handlers
