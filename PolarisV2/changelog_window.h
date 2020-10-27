#ifndef CHANGELOG_WINDOW_H
#define CHANGELOG_WINDOW_H

#include "ui_window.h"

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                class ChangelogWindow : public UIWindow
                {
                public:
                    ChangelogWindow();
                    
                    void Draw() override;
                };
            }
        }
    }
}

#endif // !CHANGELOG_WINDOW_H
