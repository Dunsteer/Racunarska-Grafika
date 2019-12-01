
// 2017View.cpp : implementation of the CMy2017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2017.h"
#endif

#include "2017Doc.h"
#include "2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2017View

IMPLEMENT_DYNCREATE(CMy2017View, CView)

BEGIN_MESSAGE_MAP(CMy2017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMy2017View construction/destruction

CMy2017View::CMy2017View() noexcept
{
	// TODO: add construction code here
		//Body.png, Leg1.png, Leg2.png, Leg3.png i Back2.jpg
	body = new DImage();
	body->Load(CString(".\\res\\Body.png"));

	leg1 = new DImage();
	leg1->Load(CString(".\\res\\Leg1.png"));
	leg2 = new DImage();
	leg2->Load(CString(".\\res\\Leg2.png"));
	leg3 = new DImage();
	leg3->Load(CString(".\\res\\Leg3.png"));

	back2 = new DImage();
	back2->Load(CString(".\\res\\Back2.jpg"));
}

CMy2017View::~CMy2017View()
{
	delete body;
	delete leg1;
	delete leg2;
	delete leg3;
	delete back2;
}

BOOL CMy2017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2017View drawing

void CMy2017View::OnDraw(CDC* pDC)
{
	CMy2017Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;

	GetClientRect(&rect);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(width, height);
	pDC->SetViewportExt(rect.right, rect.bottom);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, width, height);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	memDC->SelectObject(bitmap);

	memDC->SetGraphicsMode(GM_ADVANCED);

	LoadIdentity(memDC);

	//DRAW

	DrawScene(memDC, CRect(0 + backgroundXOffset, 0 + backgroundYOffset, width + backgroundXOffset, height + backgroundYOffset));

	LoadIdentity(memDC);

	pDC->BitBlt(0, 0, width, height, memDC, 0, 0, SRCCOPY);
}


// CMy2017View printing

BOOL CMy2017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMy2017View::LoadIdentity(CDC* pDC)
{
	XFORM form;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;
	form.eDx = 0;
	form.eDy = 0;

	pDC->SetWorldTransform(&form);
}

void CMy2017View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false)
{
	XFORM form;
	form.eM11 = 1;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = 1;
	form.eDx = dX;
	form.eDy = dY;

	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy2017View::Rotate(CDC* pDC, float angle, bool rightMultiply = false)
{
	angle = 3.14159 / 180 * angle;

	XFORM form;
	form.eM11 = cos(angle);
	form.eM12 = sin(angle);
	form.eM21 = -sin(angle);
	form.eM22 = cos(angle);
	form.eDx = 0;
	form.eDy = 0;

	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy2017View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false)
{
	XFORM form;
	form.eM11 = sX;
	form.eM12 = 0;
	form.eM21 = 0;
	form.eM22 = sY;
	form.eDx = 0;
	form.eDy = 0;

	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy2017View::DrawScene(CDC* pDC, CRect rect)
{
	back2->Draw(pDC, rect, CRect(0, 0, width, height));

	Scale(pDC, this->transporterScale, this->transporterScale);
	body->Draw(pDC, CRect(0, 0, body->Width(), body->Height()), CRect(100 + transporterOffset, 100, body->Width() + 100 + transporterOffset, body->Height() + 100));
	DrawLeg(pDC, leg1Angle, 100 + transporterOffset + 298, 100 + 196);

	Scale(pDC, this->transporterScale, this->transporterScale);
	DrawLeg(pDC, leg2Angle, 100 + transporterOffset + 298, 100 + 196);

	Scale(pDC, this->transporterScale, this->transporterScale);
	DrawLeg(pDC, leg3Angle, 100 + transporterOffset + 80, 100 + 196);

	Scale(pDC, this->transporterScale, this->transporterScale);
	DrawLeg(pDC, leg4Angle, 100 + transporterOffset + 80, 100 + 196);
}

void CMy2017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	Translate(pDC, dx, dy);
	Rotate(pDC, alpha);
	leg1->Draw(pDC, CRect(0, 0, leg1->Width(), leg1->Height()), CRect(-leg1->Width() / 2, -28, leg1->Width() / 2, leg1->Height() - 28));

	Translate(pDC, 0, leg1->Height() - 29-28);
	Rotate(pDC, -alpha);
	leg2->Draw(pDC, CRect(0, 0, leg2->Width(), leg2->Height()), CRect(-leg2->Width() / 2, -29, leg2->Width() / 2, leg2->Height() - 29));

	int leg3Offset = 25;
	Translate(pDC, 0, leg2->Height() - leg3Offset);
	leg3->Draw(pDC, CRect(0, 0, leg3->Width(), leg3->Height()), CRect(-leg3->Width() / 2, -leg3Offset, leg3->Width() / 2, leg3->Height() - leg3Offset));

	LoadIdentity(pDC);
}


// CMy2017View diagnostics

#ifdef _DEBUG
void CMy2017View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2017Doc* CMy2017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2017Doc)));
	return (CMy2017Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2017View message handlers


void CMy2017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	const int arrowleft = 37;
	const int arrowUp = 38;
	const int arrowRight = 39;
	const int arrowDown = 40;
	const int w = 'W';
	const int s = 'S';
	const int a = 'A';
	const int d = 'D';

	switch (nChar)
	{
	case arrowleft: {
		this->transporterOffset -= 10;

		if ((this->leg1Direction * 10) + this->leg1Angle > 20 || (this->leg1Direction * 10) + this->leg1Angle < -20) {
			this->leg1Direction *= -1;
		}

		if ((this->leg2Direction * 10) + this->leg2Angle > 20 || (this->leg2Direction * 10) + this->leg2Angle < -20) {
			this->leg2Direction *= -1;
		}

		if ((this->leg3Direction * 10) + this->leg3Angle > 20 || (this->leg1Direction * 10) + this->leg3Angle < -20) {
			this->leg3Direction *= -1;
		}

		if ((this->leg4Direction * 10) + this->leg4Angle > 20 || (this->leg4Direction * 10) + this->leg4Angle < -20) {
			this->leg4Direction *= -1;
		}

		this->leg1Angle += this->leg1Direction * 10;
		this->leg2Angle += this->leg2Direction * 10;
		this->leg3Angle += this->leg3Direction * 10;
		this->leg4Angle += this->leg4Direction * 10;

	}break;
	case arrowUp: {
		this->transporterScale += 0.1;
	}break;
	case arrowRight: {
		this->transporterOffset += 10;

		if ((this->leg1Direction * 10) + this->leg1Angle > 20 || (this->leg1Direction * 10) + this->leg1Angle < -20) {
			this->leg1Direction *= -1;
		}

		if ((this->leg2Direction * 10) + this->leg2Angle > 20 || (this->leg2Direction * 10) + this->leg2Angle < -20) {
			this->leg2Direction *= -1;
		}

		if ((this->leg3Direction * 10) + this->leg3Angle > 20 || (this->leg1Direction * 10) + this->leg3Angle < -20) {
			this->leg3Direction *= -1;
		}

		if ((this->leg4Direction * 10) + this->leg4Angle > 20 || (this->leg4Direction * 10) + this->leg4Angle < -20) {
			this->leg4Direction *= -1;
		}

		this->leg1Angle += this->leg1Direction * 10;
		this->leg2Angle += this->leg2Direction * 10;
		this->leg3Angle += this->leg3Direction * 10;
		this->leg4Angle += this->leg4Direction * 10;
	}break;
	case arrowDown: {
		this->transporterScale -= 0.1;

	}break;

	case w: {
		if (this->backgroundYOffset - 200 < 0) {
			backgroundYOffset = 0;
		}
		else {
			this->backgroundYOffset -= 200;
		}
	}break;
	case s: {
		if (this->backgroundYOffset + 200 > back2->Height() - height) {
			backgroundYOffset = back2->Height() - height;
		}
		else {
			this->backgroundYOffset += 200;
		}
	}break;
	case a: {
		if (this->backgroundXOffset - 200 < 0) {
			backgroundXOffset = 0;
		}
		else {
			this->backgroundXOffset -= 200;
		}
	}break;
	case d: {
		if (this->backgroundXOffset + 200 > back2->Width() - width) {
			backgroundXOffset = back2->Width() - width;
		}
		else {
			this->backgroundXOffset += 200;
		}
	}break;
	default:
		break;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CMy2017View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
