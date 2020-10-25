#include "world_inspector.h"
#include "ui_renderer.h"
#include "globals.h"
#include "imgui_text_extension.h"

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                WorldInspector::WorldInspector()
                {
                }

                void WorldInspector::Draw()
                {
                    auto world = (*globals::gpWorld);
                    ImGui::SetNextWindowSize(ImVec2(820, 440), ImGuiCond_Appearing);
                    ImGui::Begin("World Inspector", &m_bIsOpen, ImGuiWindowFlags_MenuBar);
                    {
                        if (world == nullptr || (world && world->Levels.Count == 0))
                        {
                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("There's no levels loaded!").x / 1.15f);
                            ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2);
                            ImGui::Header("There's no levels loaded!");

                            ImGui::End();
                            return;
                        }

                        static int selectedLevel = 0;
                        static int selectedActor = 0;
                        {
                            ImGui::BeginChild("Hierarchy", ImVec2(300, 0), true);
                            {
                                for (int i = 0; i < world->Levels.Count; i++)
                                {
                                    if (world->Levels.IsValidIndex(i) && world->Levels[i])
                                    {
                                        auto level = world->Levels[i];
                                        auto name = level->GetFullName();
                                        auto formattedName = name.substr(0, name.find(".")).substr(5, name.length() - 1);

                                        if (ImGui::TreeNode(formattedName.c_str()))
                                        {
                                            for (int j = 0; j < level->Actors.Count; j++)
                                            {
                                                if (level->Actors.IsValidIndex(j) && level->Actors[j])
                                                {
                                                    auto actor = level->Actors[j];

                                                    if (ImGui::Selectable(actor->GetName().c_str(), selectedActor == j && selectedLevel == i))
                                                    {
                                                        selectedLevel = i;
                                                        selectedActor = j;
                                                    }
                                                }
                                            }

                                            ImGui::TreePop();
                                        }
                                    }
                                }
                            }
                        }

                        auto level = world->Levels[selectedLevel];
                        auto actor = level->Actors[selectedActor];
                        ImGui::End();
                    }
                }
            }
        }
    }
}