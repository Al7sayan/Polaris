#ifndef CHANGELOG_WINDOW_H
#define CHANGELOG_WINDOW_H

#include "ui_window.h"

#include <vector>
#include <string>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                class ChangelogEntry
                {
                public:
                    const char* m_ccHeader;
                    const char* m_ccBody;

                    ChangelogEntry(const char* header, const char* body)
                    {
                        m_ccHeader = header;
                        m_ccBody = body;
                    }
                };

                class ChangelogField
                {
                public:
                    std::string m_sHeader;
                    std::vector<ChangelogEntry*> m_vEntries;

                    ChangelogField(std::string header, ChangelogEntry* entries[])
                    {
                        m_sHeader = header;

                        for (int i = 0; i < (sizeof(entries) / sizeof(*entries) + 1); i++)
                            m_vEntries.push_back(entries[i]);
                    }
                };

                class ChangelogWindow : public UIWindow
                {
                private:
                    bool m_bShouldUnlockFortUIInput = false;
                public:
                    const char* FIELD_HEADER_FORMAT = "-  %s";
                    const char* FIELD_BODY_FORMAT = "     %s";
                    const char* BUILD_DATE = "October 28th, 2020";

                    ChangelogField* m_aFields[3] = {
                        new ChangelogField("New features", new ChangelogEntry*[2] {
                            new ChangelogEntry("Test entry!", "Test body!"),
                            new ChangelogEntry("Oh no", "There's 2 of them")
                        }),
                        new ChangelogField("Fixes and updates", new ChangelogEntry*[2] {
                            new ChangelogEntry("Test entry!", "Test body!"),
                            new ChangelogEntry("Oh no", "There's 2 of them")
                        }),
                        new ChangelogField("Notes", new ChangelogEntry*[2] {
                            new ChangelogEntry("Test entry!", "Test body!"),
                            new ChangelogEntry("Oh no", "There's 2 of them")
                        })
                    };

                    ChangelogWindow();
                    
                    void Draw() override;
                    void Update() override;
                };
            }
        }
    }
}

#endif // !CHANGELOG_WINDOW_H
