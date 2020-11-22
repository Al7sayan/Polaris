#ifndef CREATIVE_MENU
#define CREATIVE_MENU

#include "ui_window.h"

#include <string>
#include <filesystem>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                class CreativeMenu : public UIWindow
                {
                public:
                    void Draw() override;

                    void LoadCreation(std::filesystem::path path);
                    void SaveCreation(std::string name);
                };
            }
        }
    }
}

#endif // !CREATIVE_MENU
