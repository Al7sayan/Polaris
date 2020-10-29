#include "changelog_window.h"
#include "build.h"
#include "imgui_text_extension.h"
#include "ui_renderer.h"

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
                    gpRenderer->m_bLockFortInput = true;
                }

                void ChangelogWindow::Draw()
                {
                    ImGuiStyle* style = &ImGui::GetStyle();
                    auto prevSpacing = style->ItemSpacing.y;

                    style->ItemSpacing = ImVec2(style->ItemSpacing.x, 0.15f);

                    ImGui::SetNextWindowSize(ImVec2(405, 540), ImGuiCond_Appearing);
                    ImGui::Begin("Changelog", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar);
                    {
                        ImGui::Header2("What's new");
                        ImGui::SmallText(ImColor(1.0f, 1.0f, 1.0f, 0.5f), BUILD_DATE);

                        auto prevCursorLocation = ImGui::GetCursorPos();

                        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() - 70), ImGui::GetCursorPosY() - 35));
                        if (ImGui::Button("X", ImVec2(33, 33)))
                        {
                            m_bIsOpen = false;
                            m_bShouldUnlockFortUIInput = true;
                        }
                        ImGui::SetCursorPos(prevCursorLocation);

                        for (int i = 0; i < *(&m_aFields + 1) - m_aFields; i++)
                        {
                            ChangelogField* field = m_aFields[i];

                            ImGui::Dummy(ImVec2(0, 15));
                            ImGui::Header2(field->m_sHeader.c_str());

                            for (int j = 0; j < field->m_vEntries.size(); j++)
                            {
                                ChangelogEntry* entry = field->m_vEntries[j];
                                char buffer[2000];

                                ImGui::Dummy(ImVec2(0, 5));

                                // Format the field's header and display it.
                                sprintf_s(buffer, sizeof(buffer), FIELD_HEADER_FORMAT, entry->m_ccHeader);
                                ImGui::BoldText(buffer);

                                // Format the field's body and display it.
                                sprintf_s(buffer, sizeof(buffer), FIELD_BODY_FORMAT, entry->m_ccBody);
                                ImGui::TextWrapped(buffer);
                            }
                        }

                        ImGui::End();
                        style->ItemSpacing = ImVec2(style->ItemSpacing.x, prevSpacing);
                    }
                }
                void ChangelogWindow::Update()
                {
                    if (m_bShouldUnlockFortUIInput && gpRenderer->m_bLockFortInput)
                    {
                        gpRenderer->m_bLockFortInput = false;
                        m_bShouldUnlockFortUIInput = false;
                    }
                }
            }
        }
    }
}