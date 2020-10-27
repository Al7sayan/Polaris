#include "changelog_window.h"
#include "build.h"
#include "imgui_text_extension.h"

#include <string>

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                ChangelogWindow::ChangelogWindow()
                {
                    m_bIsOpen = true;
                }

                void ChangelogWindow::Draw()
                {
                    ImGuiWindow* window = ImGui::GetCurrentWindow();
                    std::string verNum = POLARIS_VER;
                    std::string verEnv = POLARIS_ID;
                    std::string combined = verNum + "-" + verEnv;

                    ImGui::Begin("Changelog", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar);
                    {
                        ImGui::Header2("What's new - ");
                        ImGui::SameLine(0, 0);
                        ImGui::Header2(combined.c_str());
                        ImGui::SmallText(ImColor(1.0f, 1.0f, 1.0f, 0.5f), "October 27th, 2020");

                        ImGui::Dummy(ImVec2(0, 10));
                        ImGui::Header2("New features");

                        ImGui::Text("- Polaris is done. Everything is done.");
                        ImGui::Text("- Matchmaking is now here!");

                        ImGui::Dummy(ImVec2(0, 10));
                        ImGui::Header2("Fixes and updates");

                        ImGui::Text("- Nothing was fixed. Absolutely nothing.");
                        ImGui::Text("- Did I mention that nothing got fixed? Get fucked.");

                        ImGui::Dummy(ImVec2(0, 10));
                        ImGui::Header2("Notes");

                        ImGui::Text("- Coca");

                        ImGui::End();
                    }
                }
            }
        }
    }
}