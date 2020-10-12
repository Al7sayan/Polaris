#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_window.h"

namespace polaris
{
    class MainWindow : public UIWindow
    {
    public:
        void Draw() override;
    };
}

#endif // !MAIN_WINDOW_H
