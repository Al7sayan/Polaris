#ifndef CREATIVE_MENU
#define CREATIVE_MENU

#include "ui_window.h"

#include <string>
#include <filesystem>

namespace polaris::ui::window::windows
{
    class CreativeMenu : public UIWindow
    {
    public:
        bool m_bCreatingNewProject;

        void Draw() override;

        void LoadCreation(std::filesystem::path path);
        void SaveCreation(std::string name);
        void CreateNew();
    };
}

#endif // !CREATIVE_MENU
