#include "pch.h"
#include "CMipMap.h"
#include "resource.h"
#include <iostream>
using namespace std;

CMipMap::CMipMap() {

}

CMipMap::CMipMap(CString* filename, CDC* pDC)
{
	BITMAP bmp;
	CBitmap cbmp;
	cbmp.LoadBitmap(IDB_BITMAP1);//treba na osnovu filename, later
	CDC* pomdc = new CDC();
	cbmp.GetBitmap(&bmp);
	BYTE* originalBits = (BYTE*)GlobalAlloc(GPTR, bmp.bmWidthBytes * bmp.bmHeight);
	cbmp.GetBitmapBits(bmp.bmWidthBytes * bmp.bmHeight, originalBits);
	int dcCount = log2(bmp.bmWidth);
	this->n = dcCount;
	this->DCniz = new CDC  [dcCount];
	for (int i = n-1; i >=0; i--) {
		int mul = pow(2, i);
		CBitmap pomCbmp;
		BYTE* pomBits = (BYTE*)GlobalAlloc(GPTR, bmp.bmWidthBytes * bmp.bmHeight / mul);
		for (int j = 0; j < bmp.bmWidthBytes * bmp.bmHeight; j += 4 * mul) {
			pomBits[j/mul] = originalBits[j];
			pomBits[j / mul + 1] = originalBits[j + 1];
			pomBits[j / mul + 2] = originalBits[j + 2];
			pomBits[j / mul + 3] = originalBits[j + 3];
		}
		BOOL ye = pomCbmp.CreateBitmap(bmp.bmWidth / mul, bmp.bmHeight / mul, 1, 32, pomBits);
		//pomCbmp.SetBitmapBits(bmp.bmWidthBytes * bmp.bmHeight / mul, pomBits);
		this->DCniz[i].CreateCompatibleDC(pDC);
		this->DCniz[i].SelectObject(&pomCbmp);
		GlobalFree((HGLOBAL)pomBits);
	}
	GlobalFree((HGLOBAL)originalBits);
}

void CMipMap::DrawMipmap(CDC* pDC, int x, int y, int size)
{
	int actualSize = 1;

	while (actualSize < size) {
		actualSize = actualSize * 2;
	}

	actualSize = abs(actualSize - size) > abs(actualSize / 2 - size) ? actualSize / 2 : actualSize;
	int i = n-log2(actualSize);

 	pDC->BitBlt(0, 0, actualSize, actualSize, &(this->DCniz[i]), 0, 0, SRCCOPY);
}
