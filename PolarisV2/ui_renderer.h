#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace polaris
{
    class UIRenderer
    {
    public:
        ID3D11Device* m_pCurrentDevice = nullptr;
        ID3D11DeviceContext* m_pCurrentContext = nullptr;
        ID3D11RenderTargetView* m_pCurrentView = nullptr;
        HWND m_hWnd = 0;
        WNDPROC m_lpPrevWndFunc;

        UIRenderer();
        ~UIRenderer();
    };
}

#endif // !UI_RENDERER_H

extern polaris::UIRenderer* gpRenderer;