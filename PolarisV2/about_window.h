#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include "ui_window.h"

namespace polaris
{
    class AboutWindow : public UIWindow
    {
    public:
        void Draw() override;
    };
}

#endif // !ABOUT_WINDOW_H
