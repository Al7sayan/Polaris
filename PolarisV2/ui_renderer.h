#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "ui_window.h"

#include <vector>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace polaris
{
    class UIRenderer
    {
    public:
        std::vector<UIWindow*> m_vpWindows;
        ID3D11Device* m_pCurrentDevice = nullptr;
        ID3D11DeviceContext* m_pCurrentContext = nullptr;
        ID3D11RenderTargetView* m_pCurrentView = nullptr;
        HWND m_hWnd = 0;
        WNDPROC m_lpPrevWndFunc;

        UIRenderer();
        ~UIRenderer();

        void SetTheme();
    };
}

#endif // !UI_RENDERER_H

extern polaris::UIRenderer* gpRenderer;