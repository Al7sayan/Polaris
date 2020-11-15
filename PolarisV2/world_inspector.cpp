#include "world_inspector.h"
#include "ui_renderer.h"
#include "globals.h"
#include "imgui_text_extension.h"
#include "building.h"
#include "sdk_utils.h"

#include <iostream>
#include <fstream>
#include "..\..\..\..\json.hpp"

using json = nlohmann::json;

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                std::map<std::string, SDK::UBlueprintGeneratedClass*> classCache;

                struct FActorSpawnParameters
                {
                    unsigned char Unk00[0x40];
                };

                SDK::AActor* (*SpawnActor)(SDK::UWorld* World, SDK::UClass* Class, SDK::FVector* Location, SDK::FRotator* Rotation, FActorSpawnParameters& SpawnParameters) = nullptr;

                WorldInspector::WorldInspector()
                {
                    auto pSpawnActorOffset = utilities::SDKUtils::FindPattern("\xE8\x00\x00\x00\x00\x0F\x10\x04\x3E", "x????xxxx");
                    if (!pSpawnActorOffset)
                    {
                        MessageBox(NULL, static_cast<LPCWSTR>(L"Finding pattern for SpawnActor has failed, please re-open Fortnite and try again!"), static_cast<LPCWSTR>(L"Error"), MB_ICONERROR);
                        ExitProcess(EXIT_FAILURE);
                    }

                    SpawnActor = reinterpret_cast<decltype(SpawnActor)>(pSpawnActorOffset + 5 + *reinterpret_cast<uint32_t*>(pSpawnActorOffset + 1));
                }

                void WorldInspector::Draw()
                {
                    auto world = (*globals::gpWorld);
                    ImGui::SetNextWindowSize(ImVec2(820, 440), ImGuiCond_Appearing);
                    ImGui::Begin("World Inspector", &m_bIsOpen);
                    {
                        if (ImGui::Button("Save"))
                        {
                            json json;
                            int buildActorIndex = 0;
                            for (int i = 0; i < globals::gpLevel->Actors.Count; i++)
                            {
                                auto actor = globals::gpLevel->Actors[i];
                                if (actor && actor->IsA(SDK::ABuildingSMActor::StaticClass()) && actor->GetFullName().find("PBWA") != std::string::npos)
                                {
                                    auto name = actor->GetFullName();
                                    auto formattedName = name.substr(0, name.find(" ")).substr(0, name.length() - 1).substr(0, name.find("_C"));

                                    json[std::to_string(buildActorIndex)]["name"] = formattedName;
                                    json[std::to_string(buildActorIndex)]["location"]["x"] = actor->K2_GetActorLocation().X;
                                    json[std::to_string(buildActorIndex)]["location"]["y"] = actor->K2_GetActorLocation().Y;
                                    json[std::to_string(buildActorIndex)]["location"]["z"] = actor->K2_GetActorLocation().Z;
                                    json[std::to_string(buildActorIndex)]["rotation"]["pitch"] = actor->K2_GetActorRotation().Pitch;
                                    json[std::to_string(buildActorIndex)]["rotation"]["roll"] = actor->K2_GetActorRotation().Roll;
                                    json[std::to_string(buildActorIndex)]["rotation"]["yaw"] = actor->K2_GetActorRotation().Yaw;
                                    json[std::to_string(buildActorIndex)]["health"] = static_cast<SDK::ABuildingSMActor*>(actor)->GetHealth();

                                    buildActorIndex++;
                                }
                            }

                            json["amountOfBuilds"] = buildActorIndex;

                            std::ofstream o("MyCreation.polaris");
                            o << json.dump(4) << std::endl;
                            o.close();
                        }

                        if (ImGui::Button("Load"))
                        {
                            std::ifstream i("MyCreation.polaris");
                            json j;
                            i >> j;
                            
                            int amountOfBuilds = j["amountOfBuilds"];
                            for (int i = 0; i < amountOfBuilds; i++)
                            {
                                SDK::FVector location;
                                location.X = j[std::to_string(i)]["location"]["x"];
                                location.Y = j[std::to_string(i)]["location"]["y"];
                                location.Z = j[std::to_string(i)]["location"]["z"];

                                SDK::FRotator rotation;
                                rotation.Pitch = j[std::to_string(i)]["rotation"]["pitch"];
                                rotation.Yaw = j[std::to_string(i)]["rotation"]["yaw"];
                                rotation.Roll = j[std::to_string(i)]["rotation"]["roll"];

                                FActorSpawnParameters params{};
                                SDK::UBlueprintGeneratedClass* buildClass = nullptr;
                                if (classCache.find(j[std::to_string(i)]["name"]) == classCache.end())
                                {
                                    buildClass = SDK::UObject::FindObject<SDK::UBlueprintGeneratedClass>("BlueprintGeneratedClass " + std::string(j[std::to_string(i)]["name"]) + "." + std::string(j[std::to_string(i)]["name"]) + "_C");
                                    classCache.insert({ j[std::to_string(i)]["name"], buildClass });
                                }
                                else
                                    buildClass = classCache[j[std::to_string(i)]["name"]];

                                auto build = SpawnActor(*globals::gpWorld, buildClass, &location, &rotation, params);
                                auto damageType = new SDK::UDamageType;
                                damageType->bCausedByWorld = true;

                                static_cast<SDK::ABuildingSMActor*>(build)->ForceBuildingHealth(j[std::to_string(i)]["health"]);
                                static_cast<SDK::ABuildingSMActor*>(build)->ForceBuildingHealth(static_cast<SDK::ABuildingSMActor*>(build)->GetMaxHealth());
                            }
                            i.close();
                        }

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
                                ImGui::EndChild();
                            }
                        }

                        ImGui::SameLine();

                        auto level = world->Levels[selectedLevel];
                        auto actor = level->Actors[selectedActor];
                        if (actor)
                        {
                            bool actorHasAuthority = actor->HasAuthority();

                            ImGui::SameLine();

                            ImGui::BeginGroup();
                            ImGui::BeginChild("Inspector", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
                            {
                                ImGui::Header2(actor->GetName().c_str());

                                ImGui::Separator();
                                {
                                    ImGui::TextWrapped("ID: %s", actor->GetFullName().c_str());

                                    auto location = actor->K2_GetActorLocation();
                                    ImGui::TextWrapped("Position: (%d, %d, %d)", location.X, location.Y, location.Z);

                                    auto rotation = actor->K2_GetActorRotation();
                                    ImGui::TextWrapped("Rotation: (%d, %d, %d)", rotation.Pitch, rotation.Roll, rotation.Yaw);

                                    auto scale = actor->GetActorScale3D();
                                    ImGui::TextWrapped("Scale: (%d, %d, %d)", scale.X, scale.Y, scale.Z);

                                    ImGui::Separator();
                                }
                                {
                                    auto ownerLabel = actor->Owner != nullptr ? actor->Owner->GetName().c_str() : "None";
                                    ImGui::TextWrapped("Owner: %s", ownerLabel);

                                    auto authorityLabel = actor->HasAuthority() ? "Yes" : "No";
                                    ImGui::TextWrapped("Has Authority: %s", authorityLabel);
                                }

                                ImGui::EndChild();
                            }

                            if (ImGui::Button("Destroy"))
                                actor->K2_DestroyActor();

                            ImGui::SameLine();

                            if (ImGui::Button("Toggle Authority"))
                                actor->Role = actorHasAuthority ? SDK::ENetRole::ROLE_None : SDK::ENetRole::ROLE_Authority;

                            ImGui::EndGroup();
                        }

                        ImGui::End();
                    }
                }
            }
        }
    }
}