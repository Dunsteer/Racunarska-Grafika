
// 2015View.cpp : implementation of the CMy2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2015.h"
#endif

#include "2015Doc.h"
#include "2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2015View

IMPLEMENT_DYNCREATE(CMy2015View, CView)

BEGIN_MESSAGE_MAP(CMy2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMy2015View construction/destruction

CMy2015View::CMy2015View() noexcept
{
	// TODO: add construction code here
	background.Load(CString(".\\res\\blue.png"));
}

CMy2015View::~CMy2015View()
{
}

BOOL CMy2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2015View drawing

void CMy2015View::OnDraw(CDC* pDC)
{
	CMy2015Doc* pDoc = GetDocument();
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

	memDC->Rectangle(0, 0, width, height);

	//DRAW

	background.Draw(memDC, CRect(0, 0, background.Width(), background.Height()), CRect(0, 0, width, height));

	CRect pacmanRect(400 + this->pacmanXOffset, 400 + this->pacmanYOffset, 500 + this->pacmanXOffset, 500 + this->pacmanYOffset);

	DrawPacman(memDC, pacmanRect, 30);

	CRect ghostRect(100, 120, 196, 216);

	DrawGhost(memDC, ghostRect);

	CPoint pacmanCenter = pacmanRect.CenterPoint();
	CPoint ghostCenter = ghostRect.CenterPoint();

	//The end
	if (pow(pacmanCenter.x - ghostCenter.x, 2) + pow(pacmanCenter.y - ghostCenter.y, 2) < pow(pacmanRect.right - pacmanCenter.x + ghostRect.right - ghostCenter.x, 2)) {
		CFont font;
		font.CreateFont(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));

		memDC->SelectObject(&font);

		SetBkMode(memDC->m_hDC, TRANSPARENT);
		SetTextColor(memDC->m_hDC, RGB(255, 255, 255));
		memDC->DrawText(CString("The end"), CRect(0, height / 2 - 72 / 2, width, height), DT_CENTER);
	}

	LoadIdentity(memDC);

	pDC->BitBlt(0, 0, width, height, memDC, 0, 0, SRCCOPY);

	memDC->DeleteDC();
	delete memDC;
}


// CMy2015View printing

BOOL CMy2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMy2015View::LoadIdentity(CDC* pDC)
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

void CMy2015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	CBrush yellow(RGB(255, 255, 0));
	CBrush* old = pDC->SelectObject(&yellow);

	int r = (rect.left - rect.right) / 2;

	angle = angle * 3.14159 / 180;

	float angleOffset = 3.14159 / 2;

	angleOffset = (angleOffset * (this->pacmanDirection - 1));

	CPoint center(rect.left + abs((rect.left - rect.right) / 2), rect.top + abs((rect.top - rect.bottom) / 2));
	CPoint top(center.x + r * cos(-angle + angleOffset), center.y + r * sin(-angle + angleOffset));
	CPoint bottom(center.x + r * cos(angle + angleOffset), center.y + r * sin(angle + angleOffset));

	if (pacmanMouthState) {
		pDC->Pie(rect, top, bottom);
	}
	else {
		pDC->Ellipse(rect.left, rect.top, rect.right, rect.bottom);
	}

	CPoint eye;

	switch (pacmanDirection)
	{
	case 1: {
		eye.y = bottom.y;
		eye.x = bottom.x + 20;
	}break;
	case 2: {
		eye.x = top.x;
		eye.y = top.y + 20;
	}break;
	case 3: {
		eye.x = top.x - 20;
		eye.y = top.y - 5;
	}break;
	case 4: {
		eye.x = top.x - 5;
		eye.y = top.y - 20;
	}break;
	default:
		break;
	}
	if (pacmanDirection == 1) {

	}

	CBrush black(RGB(0, 0, 0));

	pDC->SelectObject(&black);

	pDC->Ellipse(eye.x, eye.y, eye.x + 10, eye.y + 10);


	pDC->SelectObject(old);
}

void CMy2015View::DrawGhost(CDC* pDC, CRect rect)
{
	CBrush red(RGB(255, 0, 0));

	float oneTwelft = rect.Width() / 12;

	CPen black(0, 2, RGB(0, 0, 0));

	pDC->SelectObject(&black);
	pDC->SelectObject(&red);

	pDC->BeginPath();

	pDC->MoveTo(rect.left, rect.bottom);
	pDC->ArcTo(CRect(rect.left - oneTwelft, rect.bottom - oneTwelft * 2, rect.left + oneTwelft, rect.bottom),
		CPoint(rect.left, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft));

	//pDC->MoveTo(CPoint(rect.left + oneTwelft * 2, rect.bottom - oneTwelft));
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(CRect(rect.left + oneTwelft, rect.bottom - oneTwelft * 2, rect.left + oneTwelft * 3, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft),
		CPoint(rect.left + oneTwelft * 3, rect.bottom - oneTwelft)
	);

	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(CRect(rect.left + oneTwelft * 3, rect.bottom - oneTwelft * 2, rect.left + oneTwelft * 5, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft),
		CPoint(rect.left + oneTwelft * 5, rect.bottom - oneTwelft)
	);

	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(CRect(rect.left + oneTwelft * 5, rect.bottom - oneTwelft * 2, rect.left + oneTwelft * 7, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft),
		CPoint(rect.left + oneTwelft * 7, rect.bottom - oneTwelft)
	);

	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(CRect(rect.left + oneTwelft * 7, rect.bottom - oneTwelft * 2, rect.left + oneTwelft * 9, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft),
		CPoint(rect.left + oneTwelft * 9, rect.bottom - oneTwelft)
	);

	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(CRect(rect.left + oneTwelft * 9, rect.bottom - oneTwelft * 2, rect.left + oneTwelft * 11, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft),
		CPoint(rect.left + oneTwelft * 11, rect.bottom - oneTwelft)
	);

	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(CRect(rect.left + oneTwelft * 11, rect.bottom - oneTwelft * 2, rect.left + oneTwelft * 13, rect.bottom),
		CPoint(rect.left + oneTwelft, rect.bottom - oneTwelft),
		CPoint(rect.left + oneTwelft * 12, rect.bottom)
	);

	pDC->LineTo(rect.right, rect.top + 25);

	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(CRect(rect.left, rect.top - 25, rect.right, rect.bottom - 25),
		CPoint(rect.right, rect.top + rect.Height() / 2 - 25),
		CPoint(rect.left, rect.top + rect.Height() / 2 - 25)
	);

	pDC->LineTo(rect.left, rect.bottom);

	pDC->EndPath();

	pDC->StrokeAndFillPath();


	CBrush white(RGB(255, 255, 255));

	pDC->SelectObject(white);

	CPoint leftEye = rect.CenterPoint();

	leftEye.x -= 10;
	leftEye.y -= 30;

	int eyeDistance = 30;

	pDC->Ellipse(leftEye.x, leftEye.y, leftEye.x + 20, leftEye.y + 20);

	pDC->Ellipse(leftEye.x + eyeDistance, leftEye.y, leftEye.x + eyeDistance + 20, leftEye.y + 20);

	CBrush blue(RGB(0, 0, 255));

	pDC->SelectObject(blue);

	pDC->Ellipse(leftEye.x + 7, leftEye.y + 5, leftEye.x + 10 + 7, leftEye.y + 5 + 10);

	pDC->Ellipse(leftEye.x + eyeDistance + 7, leftEye.y + 5, leftEye.x + eyeDistance + 10 + 7, leftEye.y + 10 + 5);
}


// CMy2015View diagnostics

#ifdef _DEBUG
void CMy2015View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2015Doc* CMy2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2015Doc)));
	return (CMy2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2015View message handlers


void CMy2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	const int arrowleft = 37;
	const int arrowUp = 38;
	const int arrowRight = 39;
	const int arrowDown = 40;

	switch (nChar)
	{
	case arrowleft: {
		this->pacmanXOffset -= 10;
		this->pacmanDirection = 1;
	}break;
	case arrowUp: {
		this->pacmanYOffset -= 10;
		this->pacmanDirection = 2;
	}break;
	case arrowRight: {
		this->pacmanXOffset += 10;
		this->pacmanDirection = 3;
	}break;
	case arrowDown: {
		this->pacmanYOffset += 10;
		this->pacmanDirection = 4;
	}break;
	default:
		break;
	}

	this->pacmanMouthState = this->pacmanMouthState ? 0 : 1;

	Invalidate();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CMy2015View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
