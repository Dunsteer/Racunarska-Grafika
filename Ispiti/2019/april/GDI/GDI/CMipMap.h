#pragma once
class CMipMap
{
private:
public:
	CMipMap();
	CMipMap(CString*, CDC*);
	CDC* DCniz;
	int n;
	void DrawMipmap(CDC* pDC, int x, int y, int size);


};

