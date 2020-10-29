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
                    const char* BUILD_DATE = "October 30th, 2020";

                    ChangelogField* m_aFields[3] = {
                        new ChangelogField("New features", new ChangelogEntry * [2] {
                            new ChangelogEntry("Building!", "Build to your heart's content, and make something unbelievable."),
                            new ChangelogEntry("Inventory", "The inventory mapper is no longer needed, simply open your inventory (normally the *i* key) and drag your favorite gun onto your hotbar.")
                        }),
                        new ChangelogField("Fixes and updates", new ChangelogEntry * [2] {
                            new ChangelogEntry("Codebase Rewrite", "The entire Polaris codebase has been rewritten (codenamed v2), and provides more stability and ease of use for the developers."),
                            new ChangelogEntry("Helmet Bug", "Skins that normally don't have a helmet, should no longer have a helmet when using some private servers.")
                        }),
                        new ChangelogField("Notes", new ChangelogEntry*[2] {
                            new ChangelogEntry("Join our Discord!", "Join and talk to us over at discord.gg/polaris!"),
                            new ChangelogEntry("About that tweet...", "i lied. there's no sex."),
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
