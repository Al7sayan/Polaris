#include "ui_window.h"
#include "ui_renderer.h"

namespace polaris
{
    UIWindow::UIWindow()
    {
        gpRenderer->m_vpWindows.push_back(this);
    }

    void UIWindow::Draw()
    {
    }
    void UIWindow::Resize()
    {
    }
    void UIWindow::Update()
    {
    }
}