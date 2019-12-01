
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

#include "wingdi.h"

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
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CVezba1View construction/destruction

CVezba1View::CVezba1View() noexcept
{
	//this->baseAngle = 0;
	//this->bigArmAngle = 0;
	//this->smallArmAngle = 0;
	//this->pincerAngle = 0;

	this->baseAngle = 180;
	this->bigArmAngle = -90;
	this->smallArmAngle = 90;
	this->pincerAngle = 0;
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
	pDC->SetWindowExt(500, 500);
	/*pDC->SetWindowExt(1000, 1000);
	pDC->SetWindowOrg(-500, -500);*/
	pDC->SetViewportExt(rect.right, rect.bottom);
	//CDC* MemDC = pDC;

	CBitmap memBitMap;
	memBitMap.CreateCompatibleBitmap(pDC, 500, 500);


	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);
	CBitmap* old = MemDC->SelectObject(&memBitMap);

	MemDC->Rectangle(0, 0, 505, 505);

	int prevMode = SetGraphicsMode(MemDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM Xform;
	BOOL b = GetWorldTransform(MemDC->m_hDC, &XformOld);

	Xform.eM11 = (FLOAT)1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = SetWorldTransform(MemDC->m_hDC, &Xform);
	dw = GetLastError();

	this->DrawGrid(MemDC);

	Translate(3 * this->gridSize, 3 * this->gridSize, MemDC);
	Rotate(baseAngle, MemDC);
	CPoint origin = DrawBase(CPoint(0, 0), MemDC);

	Translate(origin.x, origin.y, MemDC);
	Rotate(bigArmAngle, MemDC);

	origin = DrawLonger(CPoint(0, 0), MemDC);

	Translate(origin.x, origin.y, MemDC);
	Rotate(smallArmAngle, MemDC);
	origin = DrawShorter(CPoint(0, 0), MemDC);

	Translate(origin.x, origin.y, MemDC);
	Rotate(pincerAngle - 90, MemDC);
	origin = DrawArm(CPoint(0, 0), MemDC);

	Rotate((-pincerAngle * 2) - 180, MemDC);
	MirrorVertical(MemDC);

	origin = DrawArm(CPoint(0, 0), MemDC);

	prevMode = SetGraphicsMode(MemDC->m_hDC, GM_ADVANCED);
	dw = GetLastError();

	b = GetWorldTransform(MemDC->m_hDC, &XformOld);

	Xform.eM11 = (FLOAT)1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = SetWorldTransform(MemDC->m_hDC, &Xform);
	dw = GetLastError();

	pDC->BitBlt(0, 0, 500, 500, MemDC, 0, 0, SRCCOPY);

	MemDC->DeleteDC();
	delete MemDC;
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

void CVezba1View::Resize(double scale, CDC* pDC)
{
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM Xform;
	BOOL b = GetWorldTransform(pDC->m_hDC, &XformOld);

	Xform.eM11 = (FLOAT)scale;
	Xform.eM12 = (FLOAT)0.0;
	Xform.eM21 = (FLOAT)0.0;
	Xform.eM22 = (FLOAT)scale;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = ModifyWorldTransform(pDC->m_hDC, &Xform, MWT_LEFTMULTIPLY);
	dw = GetLastError();
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
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = ModifyWorldTransform(pDC->m_hDC, &Xform, MWT_LEFTMULTIPLY);
	dw = GetLastError();
}

void CVezba1View::Translate(double x, double y, CDC* pDC)
{
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM Xform;
	BOOL b = GetWorldTransform(pDC->m_hDC, &XformOld);

	Xform.eM11 = (FLOAT)1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)x;
	Xform.eDy = (FLOAT)y;

	b = ModifyWorldTransform(pDC->m_hDC, &Xform, MWT_LEFTMULTIPLY);
	dw = GetLastError();
}

void CVezba1View::ResetTranslate(CDC* pDC)
{
	this->Translate(0, 0, pDC);
}

void CVezba1View::MirrorVertical(CDC* pDC)
{
	int prevMode = SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);
	DWORD dw = GetLastError();

	XFORM Xform;
	BOOL b = GetWorldTransform(pDC->m_hDC, &XformOld);

	Xform.eM11 = (FLOAT)-1;
	Xform.eM12 = (FLOAT)0;
	Xform.eM21 = (FLOAT)0;
	Xform.eM22 = (FLOAT)1;
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;

	b = ModifyWorldTransform(pDC->m_hDC, &Xform, MWT_LEFTMULTIPLY);
	dw = GetLastError();
}

void CVezba1View::MakeTransparent(CPoint point, Hatch h, int slika, CDC* pDC) {

	CBitmap bmpImage;
	BOOL suc = bmpImage.LoadBitmap(slika);

	CBitmap bmpMask;
	BITMAP bm;
	bmpImage.GetBitmap(&bm);

	bmpMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	CDC* SrcDC = new CDC();
	SrcDC->CreateCompatibleDC(NULL);
	CDC* DstDC = new CDC();
	DstDC->CreateCompatibleDC(NULL);

	CBitmap* pOldSrcBmp = SrcDC->SelectObject(&bmpImage);
	CBitmap* pOldDstBmp = DstDC->SelectObject(&bmpMask);

	COLORREF clrTopLeft = SrcDC->GetPixel(0, 0);
	COLORREF clrSaveBk = SrcDC->SetBkColor(clrTopLeft);

	DstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, SrcDC, 0, 0, SRCCOPY);

	COLORREF clrSaveDstText = SrcDC->SetTextColor(RGB(255, 255, 255));
	SrcDC->SetBkColor(RGB(0, 0, 0));
	SrcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, DstDC, 0, 0, SRCAND);

	DstDC->SetTextColor(clrSaveDstText);
	SrcDC->SetBkColor(clrSaveBk);
	SrcDC->SelectObject(pOldSrcBmp);
	DstDC->SelectObject(pOldDstBmp);
	SrcDC->DeleteDC();
	delete SrcDC;
	DstDC->DeleteDC();
	delete DstDC;

	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(NULL);
	CBitmap* bmpOldT = MemDC->SelectObject(&bmpMask);
	pDC->BitBlt(-point.x, -point.y, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCAND);

	DWORD dwValue;

	BYTE* bmpBuffer = (BYTE*)GlobalAlloc(GPTR, bm.bmWidthBytes * bm.bmHeight);//allocate memory for image byte buffer

	dwValue = bmpImage.GetBitmapBits(bm.bmWidthBytes * bm.bmHeight, bmpBuffer);//Get the bitmap bits into a structure 

	for (int i = 0; i < bm.bmHeight; i++) {
		for (int j = 0; j < bm.bmWidth; j++)
		{
			switch (h.type)
			{
			case 0: {
				FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
			}break;

			case 1: {
				if (i % 50 < 25) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 2: {
				if (i % 50 > 25) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;

			case 3: {
				if (j % 50 < 25) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 4: {
				if (j % 50 > 25) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;

			case 5: {
				if (i % 50 < 25 && j % 50 < 25 || i % 50 > 25 && j % 50 > 25) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 6: {
				if (i % 50 > 25 && j % 50 < 25 || i % 50 < 25 && j % 50 > 25) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 7: {
				if (25 < (i + bm.bmWidth - j) % 50) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 8: {
				if (25 < (i + bm.bmWidth + j) % 50) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 9: {
				if ((25 < (i + bm.bmWidth - j) % 50 && j <= bm.bmWidth / 2) ||
					(25 > (i + bm.bmWidth + j) % 50 && j > bm.bmWidth / 2)) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 10: {
				if ((25 < (i + bm.bmWidth + j) % 50 && j <= bm.bmWidth / 2) ||
					(25 > (i + bm.bmWidth - j) % 50 && j > bm.bmWidth / 2)) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;

			case 11: {
				if ((25 > (i + bm.bmWidth - j) % 50 && j <= bm.bmWidth / 2) ||
					(25 < (i + bm.bmWidth + j) % 50 && j > bm.bmWidth / 2)) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 12: {
				if ((25 < (i + bm.bmWidth - j) % 50 && j <= bm.bmWidth / 2) ||
					(25 > (i + bm.bmWidth + j) % 50 && j > bm.bmWidth / 2)) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;

			case 13: {
				if ((25 < (i + bm.bmWidth - j) % 50 && j <= bm.bmWidth / 2) ||
					(25 < (i + bm.bmWidth + j) % 50 && j > bm.bmWidth / 2)) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			case 14: {
				if ((25 > (i + bm.bmWidth - j) % 50 && j <= bm.bmWidth / 2) ||
					(25 > (i + bm.bmWidth + j) % 50 && j > bm.bmWidth / 2)) {
					FilterPixel(bmpBuffer, i, j, bm.bmWidthBytes, h);
				}
			}break;
			default:
				break;
			}

		}
	}

	bmpImage.SetBitmapBits(bm.bmWidthBytes * bm.bmHeight, bmpBuffer);

	MemDC->SelectObject(&bmpImage);
	pDC->BitBlt(-point.x, -point.y, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCPAINT);

	MemDC->SelectObject(bmpOldT);
	MemDC->DeleteDC();
	delete MemDC;
}

void CVezba1View::FilterPixel(BYTE* buffer, int x, int y, int width, Hatch h) {
	BYTE* pixel = buffer + (x * width + y * 4);
	if (!h.B)
		pixel[0] = 0;
	if (!h.G)
		pixel[1] = 0;
	if (!h.R)
		pixel[2] = 0;
}

CPoint CVezba1View::DrawBase(CPoint origin, CDC* pDC)
{
	double x = origin.x * 1.0 / this->gridSize;
	double y = origin.y * 1.0 / this->gridSize;

	Hatch h;
	h.B = 1;
	h.G = 1;
	h.R = 1;

	this->MakeTransparent(CPoint(100, 37.5), h, IDB_BITMAP1, pDC);

	return CPoint(x * this->gridSize, y * this->gridSize);
}

CPoint CVezba1View::DrawLonger(CPoint origin, CDC* pDC)
{
	double x = origin.x * 1.0 / this->gridSize;
	double y = origin.y * 1.0 / this->gridSize;

	hatch h;
	h.B = 0;
	h.G = 1;
	h.R = 0;
	h.type = 1;

	this->MakeTransparent(CPoint(62.5, 290), h, IDB_BITMAP2, pDC);

	return CPoint(x * this->gridSize, (y - 10) * this->gridSize);
}

CPoint CVezba1View::DrawShorter(CPoint origin, CDC* pDC)
{
	double x = origin.x * 1.0 / this->gridSize;
	double y = origin.y * 1.0 / this->gridSize;

	Hatch h;
	h.B = 0;
	h.G = 0;
	h.R = 1;
	h.type = 0;

	this->MakeTransparent(CPoint(62.5, 225), h, IDB_BITMAP3, pDC);

	return CPoint(x * this->gridSize, (y - 7.5) * this->gridSize);
}

CPoint CVezba1View::DrawArm(CPoint origin, CDC* pDC)
{
	double x = origin.x * 1.0 / this->gridSize;
	double y = origin.y * 1.0 / this->gridSize;

	Hatch h;
	h.B = 1;
	h.G = 1;
	h.R = 1;

	this->MakeTransparent(CPoint(37.5, 62.5), h, IDB_BITMAP4, pDC);

	return CPoint(x * this->gridSize, (y)*this->gridSize);
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


void CVezba1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int angleIncrement = 5;

	if (nChar == 'Q') {
		this->bigArmAngle -= angleIncrement;
	}
	else if (nChar == 'W') {
		this->bigArmAngle += angleIncrement;
	}
	else if (nChar == 'E') {
		this->smallArmAngle -= angleIncrement;
	}
	else if (nChar == 'R') {
		this->smallArmAngle += angleIncrement;
	}
	else if (nChar == 'T') {
		this->pincerAngle -= angleIncrement;
	}
	else if (nChar == 'Y') {
		this->pincerAngle += angleIncrement;
	}

	Invalidate();

	//CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CVezba1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
