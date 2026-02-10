//#pragma once
//#include <windows.h>
//
//class D2DWindow
//{
//public:
//    bool Init(HINSTANCE hInstance, const wchar_t* title = L"D2D - Mario");
//    void PumpMessages();     // 창 이벤트 처리
//    void Render();           // 매 프레임 그리기
//    bool IsOpen() const { return _open; }
//
//    // (선택) 마리오 좌표 연동용
//    void SetMarioPos(float x, float y) { _marioX = x; _marioY = y; }
//
//    void Cleanup();
//
//private:
//    HWND _hwnd = nullptr;
//    bool _open = true;
//
//    float _marioX = 200.0f;
//    float _marioY = 250.0f;
//};
