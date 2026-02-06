#include "Core/Input.h"
#include <Windows.h>

namespace KhyMario
{
    Input* Input::instance = nullptr;

    Input::Input()
    {
        instance = this;
    }

    Input::~Input()
    {
        instance = nullptr;
    }

    Input& Input::Get()
    {
        return *instance;
    }

    static bool IsValidKey(int keyCode)
    {
        return keyCode >= 0 && keyCode < 255;
    }

    bool Input::GetKeyDown(int keyCode)
    {
        if (!IsValidKey(keyCode)) return false;
        const KeyState& ks = keyStates[keyCode];
        return ks.isKeyDown && !ks.wasKeyDown;
    }

    bool Input::GetKeyUp(int keyCode)
    {
        if (!IsValidKey(keyCode)) return false;
        const KeyState& ks = keyStates[keyCode];
        return !ks.isKeyDown && ks.wasKeyDown;
    }

    bool Input::GetKey(int keyCode)
    {
        if (!IsValidKey(keyCode)) return false;
        return keyStates[keyCode].isKeyDown;
    }

    bool Input::GetMouseButtonDown(int buttonCode)
    {
        // 필요하면 구현
        return false;
    }

    bool Input::GetMouseButtonUp(int buttonCode)
    {
        return false;
    }

    bool Input::GetMouseButton(int buttonCode)
    {
        return false;
    }

    void Input::ProcessInput()
    {
        //모든 키 업데이트 (VK_LEFT/VK_RIGHT/VK_SPACE 포함)
        for (int i = 0; i < 255; ++i)
        {
            keyStates[i].isKeyDown = (GetAsyncKeyState(i) & 0x8000) != 0;
        }

        // 마우스 위치도 필요하면 여기서 갱신
        POINT p;
        GetCursorPos(&p);
        mousePosition = Vector2((float)p.x, (float)p.y);
    }

    void Input::SavePreviousInputStates()
    {
        for (int i = 0; i < 255; ++i)
        {
            keyStates[i].wasKeyDown = keyStates[i].isKeyDown;
        }
    }
}
