
// Vezba1View.cpp : implementation of the CVezba1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Vezba1.h"
#endif

#include "Vezba1Doc.h"
#include "Vezba1View.h"

#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159


// CVezba1View

IMPLEMENT_DYNCREATE(CVezba1View, CView)

BEGIN_MESSAGE_MAP(CVezba1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CVezba1View construction/destruction

CVezba1View::CVezba1View() noexcept
{
	// TODO: add construction code here

}

CVezba1View::~CVezba1View()
{
}

BOOL CVezba1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CVezba1View drawing

void CVezba1View::OnDraw(CDC* pDC)
{
	CVezba1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;

	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(2000, 2000);
	pDC->SetWindowOrg(-1000, -1000);
	pDC->SetViewportExt(rect.right, rect.bottom);

	this->DrawGrid(pDC);

	Rotate(90,pDC);

	DrawBase(pDC);

	

	DrawLonger(pDC);
	DrawShorter(pDC);
	
	DrawArm(pDC);
}


// CVezba1View printing

BOOL CVezba1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVezba1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVezba1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CVezba1View::DrawGrid(CDC* pDC) {
	CPen* oldPen;
	CPen* gridPen = new CPen(0, 1, RGB(200, 200, 200));
	oldPen = pDC->SelectObject(gridPen);

	pDC->MoveTo(0, 0);

	for (int i = 0; i < 1000 / this->gridSize; i++) {
		pDC->MoveTo(i * this->gridSize, 0);
		pDC->LineTo(i * this->gridSize, 1000);

		pDC->MoveTo(0, i * this->gridSize);
		pDC->LineTo(1000, i * this->gridSize);
	}
}

void CVezba1View::DrawPoligon(double x, double y, double numberOfPoints, double length, double angle, int hatch, COLORREF line, COLORREF fill, CDC* pDC) {
	int firstX;
	int firstY;

	angle = angle * 3.14159 / 180;

	double innerAngle = ((360 / numberOfPoints) * 3.14159 / 180);

	CPoint* points = new CPoint[numberOfPoints];

	for (int i = 0; i <= numberOfPoints; i++) {
		firstX = x + length * cos((double)(angle + innerAngle * i));
		firstY = y + length * sin((double)(angle + innerAngle * i));
		points[i] = CPoint(firstX, firstY);
	}

	CPen* oldPen;
	CPen* gridPen = new CPen(0, lineWidth, line);
	oldPen = pDC->SelectObject(gridPen);

	CBrush* oldBrush;
	CBrush* brush = new CBrush();
	
	if (hatch != -1)
		brush->CreateHatchBrush(hatch, fill);
	else
		brush = new CBrush(fill);

	oldBrush = pDC->SelectObject(brush);

	pDC->Polygon(points, numberOfPoints);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

void CVezba1View::DrawSquare(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	this->DrawPoligon(x, y, 4, length, 45, hatch, line, fill, pDC);
}

void CVezba1View::DrawDiamond(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	this->DrawPoligon(x, y, 4, length, 0, hatch, line, fill, pDC);
}

void CVezba1View::DrawHexagon(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	this->DrawPoligon(x, y, 6, length, 0, hatch, line, fill, pDC);
}

void CVezba1View::DrawOctagon(double x, double y, double length, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	this->DrawPoligon(x, y, 8, length, 0, hatch, line, fill, pDC);
}

void CVezba1View::DrawTrapezoid(double x1, double y1, double x2, double y2, double lenght, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	if (x1 > x2&& y2 > y1 || x1 == x2) {
		CPoint* points = new CPoint[4];

		points[0] = CPoint(x1, y1);
		points[3] = CPoint(x2, y2);
		points[1] = CPoint(x1 + lenght, y1);
		points[2] = CPoint(x2 + lenght + 2 * (abs(x2 - x1)), y2);

		CPen* oldPen;
		CPen* gridPen = new CPen(0, lineWidth, line);
		oldPen = pDC->SelectObject(gridPen);

		CBrush* oldBrush;
		CBrush* brush = new CBrush();

		if (hatch != -1)
			brush->CreateHatchBrush(hatch, fill);
		else
			brush = new CBrush(fill);

		oldBrush = pDC->SelectObject(brush);

		pDC->Polygon(points, 4);

		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);

		delete gridPen;
		delete brush;
		delete[] points;
	}
	else {
		CPoint* points = new CPoint[4];

		points[0] = CPoint(x1, y1);
		points[3] = CPoint(x2, y2);
		points[1] = CPoint(x1, y1 + lenght);
		points[2] = CPoint(x2, y2 + lenght + 2 * (abs(y2 - y1)));

		CPen* oldPen;
		CPen* gridPen = new CPen(0, lineWidth, line);
		oldPen = pDC->SelectObject(gridPen);

		CBrush* oldBrush;
		CBrush* brush = new CBrush(fill);

		oldBrush = pDC->SelectObject(brush);

		pDC->Polygon(points, 4);

		pDC->SelectObject(oldPen);
		pDC->SelectObject(oldBrush);

		delete gridPen;
		delete brush;
		delete[] points;
	}
}

void CVezba1View::DrawRectangle(double x1, double y1, double x2, double y2, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	this->DrawTrapezoid(x1, y1, x1, y2, abs(x2 - x1), hatch, line, fill, pDC);
}

void CVezba1View::DrawCircle(double x, double y, double r, int hatch, COLORREF line, COLORREF fill, CDC* pDC)
{
	CPen* oldPen;
	CPen* gridPen = new CPen(0, lineWidth, line);
	oldPen = pDC->SelectObject(gridPen);

	CBrush* oldBrush;
	CBrush* brush = new CBrush(fill);

	oldBrush = pDC->SelectObject(brush);

	pDC->Ellipse(x - r, y - r, x + r, y + r);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	delete gridPen;
	delete brush;
}

CPoint* CVezba1View::DrawHalf(double x, double y, double radius, double numberOfPoints, double angle, CDC* pDC)
{
	int firstX;
	int firstY;
	double start = angle * 3.14159 / 180;

	double innerAngle = ((360.0 / (numberOfPoints * 2.0)) * 3.14159 / 180);

	CPoint* points = new CPoint[numberOfPoints];

	for (int i = 0; i <= numberOfPoints; i++) {
		firstX = x + radius * cos((double)(start + innerAngle * i));
		firstY = y + radius * sin((double)(start + innerAngle * i));
		points[i] = CPoint(firstX, firstY);
	}

	return points;
}

void CVezba1View::Rotate(double angle, CDC* pDC)
{
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM Xform;
	BOOL b = GetWorldTransform(pDC->m_hDC, &XformOld);
	angle = angle * (PI / 180);

	Xform.eM11 = (FLOAT)cos(angle);
	Xform.eM12 = (FLOAT)sin(angle);
	Xform.eM21 = (FLOAT)-sin(angle);
	Xform.eM22 = (FLOAT)cos(angle);
	Xform.eDx = (FLOAT)1.0;
	Xform.eDy = (FLOAT)1.0;

	b = ModifyWorldTransform(pDC->m_hDC, &Xform, MWT_LEFTMULTIPLY);
	dw = GetLastError();

}

void CVezba1View::DrawBase(CDC* pDC)
{
	this->DrawOctagon(16 * this->gridSize, 16 * gridSize, this->gridSize, -1, green, blue, pDC);
	this->DrawRectangle(13 * this->gridSize, 18 * this->gridSize, 19 * this->gridSize, 19 * this->gridSize, HS_CROSS, green, blue, pDC);
	this->DrawTrapezoid(15 * this->gridSize, 17 * this->gridSize, 14 * this->gridSize, 18 * this->gridSize, this->gridSize * 2, -1, green, blue, pDC);
	this->DrawRectangle(15 * this->gridSize, 16 * this->gridSize, 17 * this->gridSize, 17 * this->gridSize, -1, green, blue, pDC);

	CPen* oldPen;
	CPen* gridPen = new CPen(0, lineWidth, blue);
	oldPen = pDC->SelectObject(gridPen);

	pDC->MoveTo(15 * this->gridSize + lineWidth, 16 * this->gridSize);
	pDC->LineTo(17 * this->gridSize - lineWidth, 16 * this->gridSize);
	pDC->MoveTo(15 * this->gridSize + lineWidth, 17 * this->gridSize);
	pDC->LineTo(17 * this->gridSize - lineWidth, 17 * this->gridSize);
}

void CVezba1View::DrawLonger(CDC* pDC)
{
	this->DrawTrapezoid(3 * this->gridSize, 7 * this->gridSize, 2 * this->gridSize, 17 * this->gridSize, this->gridSize, -1, yellow, blue, pDC);
	this->DrawDiamond(3.5 * this->gridSize, 7 * this->gridSize, this->gridSize * 1, HS_CROSS, yellow, blue, pDC);
	this->DrawDiamond(3.5 * this->gridSize, 7 * this->gridSize, this->gridSize / 2, HS_CROSS, yellow, blue, pDC);
	this->DrawDiamond(3.5 * this->gridSize, 17 * this->gridSize, this->gridSize * 2, HS_CROSS, yellow, blue, pDC);
	this->DrawDiamond(3.5 * this->gridSize, 17 * this->gridSize, this->gridSize / 2, HS_CROSS, yellow, blue, pDC);
}

void CVezba1View::DrawShorter(CDC* pDC)
{
	this->DrawTrapezoid(10.5 * this->gridSize, 3.5 * this->gridSize, 3 * this->gridSize, 3 * this->gridSize, this->gridSize, -1, yellow, cyan, pDC);

	this->DrawDiamond(10.5 * this->gridSize, 4 * this->gridSize, this->gridSize * 1, -1, yellow, red, pDC);
	this->DrawDiamond(10.5 * this->gridSize, 4 * this->gridSize, this->gridSize / 2, -1, yellow, red, pDC);

	this->DrawDiamond(3 * this->gridSize, 4 * this->gridSize, this->gridSize * 1.5, -1, yellow, red, pDC);
	this->DrawDiamond(3 * this->gridSize, 4 * this->gridSize, this->gridSize / 2, -1, yellow, red, pDC);
}

void CVezba1View::DrawArm(CDC* pDC)
{
	int n = 3;

	CPen* oldPen;
	CPen* gridPen = new CPen(PS_JOIN_MITER, 8, magenta);
	oldPen = pDC->SelectObject(gridPen);

	CBrush* oldBrush;
	CBrush* brush = new CBrush(cyan);

	oldBrush = pDC->SelectObject(brush);

	CPoint* first = this->DrawHalf(16 * this->gridSize, 9 * this->gridSize, this->gridSize, n, -90, pDC);

	CPoint* second = this->DrawHalf(16 * this->gridSize, 9 * this->gridSize, this->gridSize + 20, n, -90, pDC);

	CPoint* whole = new CPoint[(n + 1) * 2];

	for (int i = 0; i < n + 1; i++) {
		whole[i] = first[i];
		whole[(n + 1) * 2 - 1 - i] = second[i];
	}

	pDC->Polygon(whole, (n + 1) * 2);

	this->DrawCircle(16 * this->gridSize, 10 * this->gridSize, this->gridSize / 2, 0, magenta, cyan, pDC);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);

	delete gridPen;
	delete brush;
}


// CVezba1View diagnostics

#ifdef _DEBUG
void CVezba1View::AssertValid() const
{
	CView::AssertValid();
}

void CVezba1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVezba1Doc* CVezba1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVezba1Doc)));
	return (CVezba1Doc*)m_pDocument;
}
#endif //_DEBUG


// CVezba1View message handlers
