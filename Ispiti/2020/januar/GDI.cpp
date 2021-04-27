void DrawCycle(CDC *pDC, int r, int n, int d)
{
    CPen *pen1 = new CPen(0, d);
    pDC->SelectObject(pen1);
    pDC->Ellipse(-r, -r, r, r);
    CPen *pen2 = new CPen(0, 20);
}