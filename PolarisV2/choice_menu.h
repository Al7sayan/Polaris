#ifndef CHOICE_MENU_H
#define CHOICE_MENU_H

#include "ui_window.h"

namespace polaris::ui::window::windows
{
    class ChoiceMenu : public UIWindow
    {
    public:
        void Draw() override;
    };
}

#endif // !CHOICE_MENU_H
