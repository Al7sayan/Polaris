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

                    ImGui::Begin("Changelog", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar);
                    {
                        ImGui::Header2("What's new");
                        ImGui::SmallText(ImColor(1.0f, 1.0f, 1.0f, 0.5f), "October 27th, 2020");

                        ImGui::BeginGroup();
                        ImGui::Dummy(ImVec2(0, 10));
                        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() - 1));
                        ImGui::Header2("New features");
                        ImGui::SameLine();
                        ImGui::Separator();
                        ImGui::EndGroup();

                        ImGui::BoldText("- Building is finally here! ");
                        ImGui::TextWrapped("Additional info about building, poggers");

                        ImGui::Dummy(ImVec2(0, 10));
                        ImGui::Header2("Fixes and updates");

                        ImGui::BoldText("- Polaris v2 codebase is now in action! ");
                        ImGui::TextWrapped("The Polaris v2 codebase is a remastered version of the Polaris codebase with many improvements that contribute to a better experience.");

                        ImGui::Dummy(ImVec2(0, 10));
                        ImGui::Header2("Notes");

                        ImGui::BoldText("- Join our discord! ");
                        ImGui::TextWrapped("Join and talk to us at discord.gg/polaris");

                        ImGui::End();
                    }
                }
            }
        }
    }
}