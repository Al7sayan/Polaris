#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

namespace polaris
{
    class UIWindow
    {
    public:
        bool m_bIsOpen;

        UIWindow();

        virtual void Draw();
        virtual void Resize();
        virtual void Update();
    };
}

#endif // !UI_WINDOW_H
