void DrawTransparent(CDC *pDC, DImage *pImage, COLORREF color)
{
    CBitmap bmpMask;
    bmpMask.CreateBitmap(pImage->Width(), pImage->Height(), 1, 1, NULL);

    CDC *SrcDC = new CDC();
    SrcDC->CreateCompatibleDC(NULL);
    CDC *DstDC = new CDC();
    DstDC->CreateCompatibleDC(NULL);

    pImage->Draw(SrcDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()));
    DstDC->SelectObject(&bmpMask);

    SrcDC->SetBkColor(color);

    DstDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), SrcDC, 0, 0, SRCCOPY);

    COLORREF clrSaveDstText = SrcDC->SetTextColor(RGB(255, 255, 255));
    SrcDC->SetBkColor(RGB(0, 0, 0));
    SrcDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), DstDC, 0, 0, SRCAND);

    DstDC->SetTextColor(clrSaveDstText);

    SrcDC->DeleteDC();
    delete SrcDC;

    DstDC->DeleteDC();
    delete DstDC;

    CDC *MemDC = new CDC();
    MemDC->CreateCompatibleDC(NULL);
    CBitmap *bmpOldT = MemDC->SelectObject(&bmpMask);
    pDC->BitBlt(-point.x, -point.y, pImage->Width(), pImage->Height(), MemDC, 0, 0, SRCAND);
    MemDC->DeleteDC();
    delete MemDC;
}
