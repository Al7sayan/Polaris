#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_window.h"
#include "about_window.h"

namespace polaris
{
    class MainWindow : public UIWindow
    {
    public:
        AboutWindow* m_pAboutWindow;

        MainWindow();

        void Draw() override;
    };
}

#endif // !MAIN_WINDOW_H
