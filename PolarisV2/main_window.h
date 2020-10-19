#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "ui_window.h"
#include "about_window.h"

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                //!  The main menu bar.
                /*!
                * In charge of giving users access to the Polaris tools.
                */
                class MainWindow : public UIWindow
                {
                public:
                    AboutWindow* m_pAboutWindow;

                    MainWindow();

                    void Draw() override;
                    void Update() override;
                };
            }
        }
    }
}

#endif // !MAIN_WINDOW_H
