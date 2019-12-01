
// 2014View.cpp : implementation of the CMy2014View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2014.h"
#endif

#include "2014Doc.h"
#include "2014View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2014View

IMPLEMENT_DYNCREATE(CMy2014View, CView)

BEGIN_MESSAGE_MAP(CMy2014View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMy2014View construction/destruction

CMy2014View::CMy2014View() noexcept
{
	// TODO: add construction code here
	//wheelImage = new DImage();
	wheelImage.Load(CString(".\\res\\Wheel.png"));
	carImage = GetEnhMetaFile(CString(".\\res\\clio.emf"));
	groundAngle = 10;
	carMoved = 0;

	width = 1000;
	height = 1000;
}

CMy2014View::~CMy2014View()
{
}

BOOL CMy2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2014View drawing

void CMy2014View::OnDraw(CDC* pDC)
{
	CMy2014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;


	CRect rect;

	GetClientRect(&rect);

	width = rect.right;
	height = rect.bottom;

	pDC->SetMapMode(MM_ISOTROPIC);
	pDC->SetWindowExt(width, height);
	/*pDC->SetWindowExt(1000, 1000);
	pDC->SetWindowOrg(-500, -500);*/
	pDC->SetViewportExt(width, height);

	CBitmap memBitMap;
	memBitMap.CreateCompatibleBitmap(pDC, width, height);


	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap* old = memDC->SelectObject(&memBitMap);

	CBrush brush(RGB(0, 255, 255));
	CBrush* oldBrush = memDC->SelectObject(&brush);
	memDC->Rectangle(0, 0, width, height);
	memDC->SelectObject(oldBrush);

	int prevMode = SetGraphicsMode(memDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM XformOld;
	XFORM Xform;
	BOOL b = GetWorldTransform(memDC->m_hDC, &XformOld);

	Xform.eM11 = (FLOAT)1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = SetWorldTransform(memDC->m_hDC, &Xform);
	dw = GetLastError();

	this->DrawGround(memDC, groundAngle);

	int widthCar = 450;

	this->DrawCar(memDC, 300 + carMoved, -450 / 2 - 50, widthCar, widthCar);

	CPen redPen(0, 2, RGB(255, 0, 0));

	CPen* oldPen = pDC->SelectObject(&redPen);

	//wheelImage.Draw(memDC, CRect(0, 0, wheelImage.Width(), wheelImage.Height()), CRect(0, 0, 200, 200));


	Xform.eM11 = (FLOAT)1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = SetWorldTransform(memDC->m_hDC, &Xform);
	dw = GetLastError();

	pDC->BitBlt(0, 0, width, height, memDC, 0, 0, SRCCOPY);
}


// CMy2014View printing

BOOL CMy2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMy2014View::DrawGround(CDC* pDC, float angle)
{
	angle = 3.14159 / 180 * (-angle);
	this->Translate(pDC, 0, height, false);
	this->Rotate(pDC, angle, false);


	CBrush hatch(2, RGB(0, 255, 0));
	CBrush* oldBrush = pDC->SelectObject(&hatch);

	pDC->Rectangle(0, 0, this->width * sqrt(2), this->width);

	pDC->SelectObject(oldBrush);



}

XFORM CMy2014View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM Xform, XfromOld;
	GetWorldTransform(pDC->m_hDC, &XfromOld);

	Xform.eM11 = (FLOAT)1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)dX;
	Xform.eDy = (FLOAT)dY;

	ModifyWorldTransform(pDC->m_hDC, &Xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

	return XfromOld;
}

void CMy2014View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM Xform;

	Xform.eM11 = (FLOAT)cos(angle);
	Xform.eM12 = (FLOAT)sin(angle);
	Xform.eM21 = (FLOAT)-sin(angle);
	Xform.eM22 = (FLOAT)cos(angle);
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	ModifyWorldTransform(pDC->m_hDC, &Xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy2014View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM Xform;

	Xform.eM11 = (FLOAT)sX;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)sY;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	ModifyWorldTransform(pDC->m_hDC, &Xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMy2014View::DrawCar(CDC* pDC, int x, int y, int w, int h)
{
	DrawWheel(pDC, x - 155, y  + 70 +170, 38, carMoved);

	DrawWheel(pDC, x + 135, y  + 70+ 170, 38, carMoved);

	Scale(pDC, 1, 1 / 2.5, false);
	PlayEnhMetaFile(pDC->m_hDC, this->carImage, new CRect(x + w / 2, y - h / 2, x - w / 2, y + h / 2));
}

void CMy2014View::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
{
	angle = 3.14159 / 180 * angle;

	XFORM xForm = Translate(pDC, x, y, false);
	Rotate(pDC, angle, false);


	wheelImage.Draw(pDC, CRect(52, 15, wheelImage.Width() - 60, wheelImage.Height() - 90), CRect(0 - r, 0 - r, 0 + r, 0 + r));
	SetWorldTransform(pDC->m_hDC, &xForm);
}


// CMy2014View diagnostics

#ifdef _DEBUG
void CMy2014View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2014Doc* CMy2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2014Doc)));
	return (CMy2014Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2014View message handlers


BOOL CMy2014View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CMy2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	const int arrowleft = 37;
	const int arrowUp = 38;
	const int arrowRight = 39;
	const int arrowDown = 40;

	switch (nChar)
	{
	case arrowleft: {
		this->carMoved -= 10;
	}break;
	case arrowUp: {
		if (this->groundAngle < 80)
			this->groundAngle += 10;
	}break;
	case arrowRight: {
		this->carMoved += 10;
	}break;
	case arrowDown: {
		if (this->groundAngle > -10)
			this->groundAngle -= 10;
	}break;
	default:
		break;
	}

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
