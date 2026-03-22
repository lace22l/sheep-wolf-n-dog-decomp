#include <windows.h>
#include <cstring>

extern HINSTANCE ghInstance;
extern LOGFONTA GLOBAL_FONT;

HBITMAP drawTextOverBitmap(HWND handler, int bitmap_id, LPCSTR text_to_write, COLORREF text_color)
{
    HDC hdcWindow = GetDC(handler);
    if (!hdcWindow) return NULL;

    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    if (!hdcMem)
    {
        ReleaseDC(handler, hdcWindow);
        return NULL;
    }

    // Load original bitmap
    HBITMAP hbmOriginal = (HBITMAP)LoadImageA(
        ghInstance,
        MAKEINTRESOURCE(bitmap_id),
        IMAGE_BITMAP,
        0, 0,
        LR_CREATEDIBSECTION
    );

    if (!hbmOriginal)
    {
        DeleteDC(hdcMem);
        ReleaseDC(handler, hdcWindow);
        return NULL;
    }

    // Select bitmap into memory DC
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmOriginal);

    // Create font
    LONG savedWeight = GLOBAL_FONT.lfWeight;
    GLOBAL_FONT.lfWeight = FW_BOLD;

    HFONT hFont = CreateFontIndirectA(&GLOBAL_FONT);
    HFONT hOldFont = (HFONT)SelectObject(hdcMem, hFont);

    // Text settings
    SetBkMode(hdcMem, TRANSPARENT);
    SetTextColor(hdcMem, text_color);
    SetTextAlign(hdcMem, TA_CENTER | TA_TOP);

    BITMAP bm;
    GetObjectA(hbmOriginal, sizeof(BITMAP), &bm);

    TEXTMETRIC tm;
    GetTextMetricsA(hdcMem, &tm);

    size_t textLen = strlen(text_to_write);

    // Draw text onto bitmap
    TextOutA(
        hdcMem,
        bm.bmWidth / 2,
        (bm.bmHeight / 2) - (tm.tmHeight / 2),
        text_to_write,
        (int)textLen
    );

    // Restore font weight
    GLOBAL_FONT.lfWeight = savedWeight;

    // Create a copy of the modified bitmap to return
    HDC hdcFinal = CreateCompatibleDC(hdcWindow);
    HBITMAP hbmResult = CreateCompatibleBitmap(hdcWindow, bm.bmWidth, bm.bmHeight);

    if (hbmResult)
    {
        HBITMAP hbmFinalOld = (HBITMAP)SelectObject(hdcFinal, hbmResult);
        BitBlt(hdcFinal, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcFinal, hbmFinalOld);
        DeleteDC(hdcFinal);
    }

    // Cleanup
    SelectObject(hdcMem, hOldFont);
    SelectObject(hdcMem, hbmOld);

    DeleteObject(hFont);
    DeleteObject(hbmOriginal);
    DeleteDC(hdcMem);
    ReleaseDC(handler, hdcWindow);

    return hbmResult;
}