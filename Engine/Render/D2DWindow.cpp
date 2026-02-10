//#include "D2DWindow.h"
//#include <d2d1.h>
//#include <wincodec.h>
//
//#pragma comment(lib, "d2d1.lib")
//#pragma comment(lib, "windowscodecs.lib")
//
//template<typename T>
//static void SafeRelease(T*& p) { if (p) { p->Release(); p = nullptr; } }
//
//static ID2D1Factory* gFactory = nullptr;
//static ID2D1HwndRenderTarget* gRT = nullptr;
//static IWICImagingFactory* gWic = nullptr;
//static ID2D1Bitmap* gMario = nullptr;
//
//static bool LoadBitmap(const wchar_t* path)
//{
//    SafeRelease(gMario);
//    IWICBitmapDecoder* decoder = nullptr;
//    IWICBitmapFrameDecode* frame = nullptr;
//    IWICFormatConverter* conv = nullptr;
//
//    HRESULT hr = gWic->CreateDecoderFromFilename(path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
//    if (FAILED(hr)) goto FAIL;
//    hr = decoder->GetFrame(0, &frame);
//    if (FAILED(hr)) goto FAIL;
//    hr = gWic->CreateFormatConverter(&conv);
//    if (FAILED(hr)) goto FAIL;
//
//    hr = conv->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
//    if (FAILED(hr)) goto FAIL;
//
//    hr = gRT->CreateBitmapFromWicBitmap(conv, nullptr, &gMario);
//    if (FAILED(hr)) goto FAIL;
//
//    SafeRelease(conv); SafeRelease(frame); SafeRelease(decoder);
//    return true;
//
//FAIL:
//    SafeRelease(conv); SafeRelease(frame); SafeRelease(decoder);
//    return false;
//}
//
//static void CreateRT(HWND hwnd)
//{
//    if (gRT) return;
//
//    RECT rc{};
//    GetClientRect(hwnd, &rc);
//    auto size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
//
//    HRESULT hr = gFactory->CreateHwndRenderTarget(
//        D2D1::RenderTargetProperties(),
//        D2D1::HwndRenderTargetProperties(hwnd, size),
//        &gRT);
//
//    if (FAILED(hr))
//    {
//        wchar_t buf[256];
//        wsprintf(buf, L"CreateHwndRenderTarget failed. hr=0x%08X", (unsigned)hr);
//        MessageBox(nullptr, buf, L"D2D", MB_OK);
//    }
//}
//
//
//static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//    switch (msg)
//    {
//    case WM_SIZE:
//        if (gRT)
//        {
//            UINT w = LOWORD(lParam);
//            UINT h = HIWORD(lParam);
//            gRT->Resize(D2D1::SizeU(w, h));
//        }
//        return 0;
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//    }
//    return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//
//bool D2DWindow::Init(HINSTANCE hInstance, const wchar_t* title)
//{
//    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED))) return false;
//    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&gWic)))) return false;
//    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &gFactory))) return false;
//
//    WNDCLASS wc{};
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = L"D2D_SUB_WINDOW";
//    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
//
//    if (!RegisterClass(&wc) && GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
//        return false;
//
//    _hwnd = CreateWindow(
//        wc.lpszClassName, title,
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600,
//        nullptr, nullptr, hInstance, nullptr);
//
//    if (!_hwnd) return false;
//
//    ShowWindow(_hwnd, SW_SHOW);
//    UpdateWindow(_hwnd);
//
//    CreateRT(_hwnd);
//
//    // png는 exe 기준 폴더에서 찾음 (Bin 옆)
//    if (!LoadBitmap(L"Mario.png"))
//    {
//        MessageBox(nullptr, L"Failed to load Mario.png", L"PNG LOAD ERROR", MB_OK);
//    }
//    
//    return true;
//}
//
//void D2DWindow::PumpMessages()
//{
//    MSG msg{};
//    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//    {
//        if (msg.message == WM_QUIT)
//        {
//            _open = false;
//            return;
//        }
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//}
//
//void D2DWindow::Render()
//{
//    if (!_open) return;
//    if (!_hwnd) { MessageBox(nullptr, L"_hwnd is null", L"D2D", MB_OK); return; }
//
//    // 렌더타겟 없으면 무조건 생성 시도
//    if (!gRT)
//    {
//        CreateRT(_hwnd);
//        if (!gRT) { MessageBox(nullptr, L"CreateRT failed (gRT is still null)", L"D2D", MB_OK); return; }
//    }
//
//    // BeginDraw 결과 체크
//    gRT->BeginDraw();
//
//    // 배경
//    gRT->Clear(D2D1::ColorF(0.1f, 0.1f, 0.2f));
//
//    // 무조건 보이는 테스트: 노란색 큰 사각형
//    ID2D1SolidColorBrush* brush = nullptr;
//    HRESULT hrBrush = gRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush);
//    if (FAILED(hrBrush) || !brush)
//    {
//        wchar_t buf[256];
//        wsprintf(buf, L"CreateSolidColorBrush failed. hr=0x%08X", (unsigned)hrBrush);
//        MessageBox(nullptr, buf, L"D2D", MB_OK);
//        gRT->EndDraw();
//        return;
//    }
//
//    gRT->FillRectangle(D2D1::RectF(20, 20, 400, 300), brush);
//    brush->Release();
//
//    // EndDraw 결과 체크
//    HRESULT hrEnd = gRT->EndDraw();
//    if (FAILED(hrEnd))
//    {
//        wchar_t buf[256];
//        wsprintf(buf, L"EndDraw failed. hr=0x%08X", (unsigned)hrEnd);
//        MessageBox(nullptr, buf, L"D2D", MB_OK);
//
//        // 타겟 재생성 요구면 버리고 다음 프레임에 다시 만듦
//        if (hrEnd == D2DERR_RECREATE_TARGET)
//        {
//            SafeRelease(gRT);
//        }
//        return;
//    }
//}
//
//void D2DWindow::Cleanup()
//{
//    SafeRelease(gMario);
//    SafeRelease(gRT);
//    SafeRelease(gFactory);
//    SafeRelease(gWic);
//    CoUninitialize();
//}
