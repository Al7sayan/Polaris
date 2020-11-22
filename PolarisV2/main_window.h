#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_window.h"
#include "about_window.h"
#include "world_inspector.h"
#include "watermark.h"
#include "creative_menu.h"

namespace polaris::ui::window::windows
{
    //!  The main menu bar.
    /*!
     * In charge of giving users access to the Polaris tools.
     */
    class MainWindow : public UIWindow
    {
    public:
        AboutWindow* m_pAboutWindow;
        WorldInspector* m_pWorldInspector;
        Watermark* m_pWatermark;
        CreativeMenu* m_pCreativeMenu;

        bool m_bIsUE4ConsoleEnabled;

        MainWindow();

        void Draw() override;
        void Update() override;
    };
}

#endif // !MAIN_WINDOW_H
