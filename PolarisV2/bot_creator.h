#ifndef BOT_CREATOR
#define BOT_CREATOR

#include "ui_window.h"

#include <string>
#include <filesystem>

namespace polaris::ui::window::windows
{
    class BotCreator : public UIWindow
    {
    private:
        
    public:
        BotCreator();

        void Draw() override;
        
    };
}

#endif // !BOT_CREATOR
