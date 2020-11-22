#include "creative_menu.h"
#include "imgui_text_extension.h"
#include "athena_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "json.hpp"
#include "SDK.hpp"
#include "trap_build.h";
#include "trap_abilityset.h"

#include <iostream>
#include <fstream>

namespace fs = std::filesystem;
using trapbuild = polaris::models::offsetfixes::TrapBuild;
using json = nlohmann::json;

namespace polaris
{
    namespace ui
    {
        namespace window
        {
            namespace windows
            {
                std::map<std::string, SDK::UBlueprintGeneratedClass*> mpClassCache;
                std::map<SDK::FGuid*, SDK::ABuildingSMActor*> mpBuildActors;

                void CreativeMenu::Draw()
                {
                    if (m_bIsOpen == true)
                    {
                        globals::gpPlayerController->CheatManager->Slomo(0);
                    }

                    ImGuiIO* io = &ImGui::GetIO();

                    // Background darkener window
                    ImGui::SetNextWindowSize(ImVec2(5000, 5000));
                    ImGui::SetNextWindowPos(ImVec2(-(io->DisplaySize.x / 2), -(io->DisplaySize.y / 2)));
                    ImGui::SetNextWindowBgAlpha(0.65f);
                    ImGui::Begin("Poglaris Moment", &m_bIsOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);
                    {
                        ImGui::End();
                    }

                    ImGui::SetNextWindowSize(ImVec2(631, 410), ImGuiCond_Always);
                    ImGui::SetNextWindowPos(ImVec2(((io->DisplaySize.x / 2) - (631 / 2)), ((io->DisplaySize.y / 2) - (410 / 2))));
                    ImGui::Begin("Creations Hub", &m_bIsOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings); 
                    {
                        ImGui::Header2("Creations Hub");
                        ImGui::Text("What would you like to play?");

                        ImGui::BeginChild("Gamemode List", ImVec2(600, 250), true);
                        {
                            std::string path = "creations";

                            for (const auto& entry : fs::directory_iterator(path))
                            {
                                std::ifstream streamData(entry.path());
                                json jsonData;
                                streamData >> jsonData;

                                if (ImGui::Selectable(std::string(jsonData["info"]["name"]).c_str()))
                                {
                                    LoadCreation(entry.path());
                                }
                            }
                            ImGui::EndChild();
                        }

                        ImGui::Button("Open creations folder");
                        ImGui::SameLine();
                        if (ImGui::Button("Debug Quick Save"))
                        {
                            SaveCreation("Debug");
                        }
                        ImGui::End();
                    }
                }

                void CreativeMenu::LoadCreation(std::filesystem::path path)
                {
                    std::ifstream streamData(path);
                    json jsonData;
                    streamData >> jsonData;

                    mpBuildActors.clear();

                    for (int i = 0; i < jsonData["buildPieces"].size(); i++)
                    {
                        SDK::UBlueprintGeneratedClass* buildClass = nullptr;
                        SDK::ABuildingSMActor* buildPiece = nullptr;
                        SDK::FVector location;
                        SDK::FRotator rotation;

                        // Place the build in the correct location
                        location.X = jsonData["buildPieces"][i]["location"]["x"];
                        location.Y = jsonData["buildPieces"][i]["location"]["y"];
                        location.Z = jsonData["buildPieces"][i]["location"]["z"];

                        // Rotate the build properly
                        rotation.Pitch = jsonData["buildPieces"][i]["rotation"]["pitch"];
                        rotation.Yaw = jsonData["buildPieces"][i]["rotation"]["yaw"];
                        rotation.Roll = jsonData["buildPieces"][i]["rotation"]["roll"];

                        // Cache the actor class or cache it now
                        if (mpClassCache.find(jsonData["buildPieces"][i]["name"]) == mpClassCache.end())
                        {
                            buildClass = SDK::UObject::FindObject<SDK::UBlueprintGeneratedClass>("BlueprintGeneratedClass " + std::string(jsonData["buildPieces"][i]["name"]) + "." + std::string(jsonData["buildPieces"][i]["name"]) + "_C");
                            mpClassCache.insert({ jsonData["buildPieces"][i]["name"], buildClass });
                        }
                        else
                            buildClass = mpClassCache[jsonData["buildPieces"][i]["name"]];

                        // Summon the build piece at the correct location and rotation
                        buildPiece = static_cast<SDK::ABuildingSMActor*>(utilities::SDKUtils::SpawnActor(buildClass, &location, &rotation));

                        // Apply its health property from storage
                        buildPiece->ForceBuildingHealth(jsonData["buildPieces"][i]["health"]);

                        // Apply GUID
                        buildPiece->MyGuid.A = jsonData["buildPieces"][i]["guid"]["a"];
                        buildPiece->MyGuid.B = jsonData["buildPieces"][i]["guid"]["b"];
                        buildPiece->MyGuid.C = jsonData["buildPieces"][i]["guid"]["c"];
                        buildPiece->MyGuid.D = jsonData["buildPieces"][i]["guid"]["d"];

                        mpBuildActors.insert({ &buildPiece->MyGuid, buildPiece });
                    }
                    for (int i = 0; i < jsonData["traps"].size(); i++)
                    {
                        SDK::UBlueprintGeneratedClass* buildClass = nullptr;
                        SDK::ABuildingSMActor* buildPiece = nullptr;
                        SDK::FVector location;
                        SDK::FRotator rotation;

                        std::cout << "Loading trap into memory. Path is " << "/Game/Items/Traps/Blueprints/" + std::string(jsonData["traps"][i]["name"]) + "." + std::string(jsonData["traps"][i]["name"]) << "_C" << std::endl;
                        auto result = utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/" + std::string(jsonData["traps"][i]["name"]) + "." + std::string(jsonData["traps"][i]["name"]) + "_C");
                        std::cout << "Load complete. Result is " << result->GetName() << std::endl;

                        // Place the build in the correct location
                        location.X = jsonData["traps"][i]["location"]["x"];
                        location.Y = jsonData["traps"][i]["location"]["y"];
                        location.Z = jsonData["traps"][i]["location"]["z"];

                        // Rotate the build properly
                        rotation.Pitch = jsonData["traps"][i]["rotation"]["pitch"];
                        rotation.Yaw = jsonData["traps"][i]["rotation"]["yaw"];
                        rotation.Roll = jsonData["traps"][i]["rotation"]["roll"];

                        // Cache the actor class or cache it now
                        if (mpClassCache.find(jsonData["traps"][i]["name"]) == mpClassCache.end())
                        {
                            buildClass = SDK::UObject::FindObject<SDK::UBlueprintGeneratedClass>("BlueprintGeneratedClass " + std::string(jsonData["traps"][i]["name"]) + "." + std::string(jsonData["traps"][i]["name"]) + "_C");
                            mpClassCache.insert({ jsonData["traps"][i]["name"], buildClass });
                        }
                        else
                            buildClass = mpClassCache[jsonData["traps"][i]["name"]];

                        // Summon the build piece at the correct location and rotation
                        buildPiece = static_cast<SDK::ABuildingSMActor*>(utilities::SDKUtils::SpawnActor(buildClass, &location, &rotation));

                        auto trapActor = static_cast<SDK::ABuildingTrap*>(buildPiece);
                        SDK::FGuid* buildGuid = new SDK::FGuid;

                        buildGuid->A = jsonData["traps"][i]["buildGuid"]["a"];
                        buildGuid->B = jsonData["traps"][i]["buildGuid"]["b"];
                        buildGuid->C = jsonData["traps"][i]["buildGuid"]["c"];
                        buildGuid->D = jsonData["traps"][i]["buildGuid"]["d"];

                        trapActor->AttachedTo = mpBuildActors[buildGuid];
                        reinterpret_cast<models::offsetfixes::TrapBuild*>(trapActor)->TrapData = SDK::UObject::FindObject<SDK::UFortTrapItemDefinition>(jsonData["traps"][i]["tid"]);

                        trapActor->InitializeKismetSpawnedBuildingActor(mpBuildActors[buildGuid], static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController));
                    }

                    auto pawnActor = static_cast<SDK::AFortPlayerPawn*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
                    SDK::FVector location;
                    SDK::FRotator rotation;

                    location.X = jsonData["player"]["location"]["x"];
                    location.Y = jsonData["player"]["location"]["y"];
                    location.Z = jsonData["player"]["location"]["z"];

                    rotation.Pitch = jsonData["player"]["rotation"]["pitch"];
                    rotation.Yaw = jsonData["player"]["rotation"]["yaw"];
                    rotation.Roll = jsonData["player"]["rotation"]["roll"];

                    pawnActor->K2_SetActorLocation(location, false, true, new SDK::FHitResult);
                    pawnActor->K2_SetActorRotation(rotation, true);

                    streamData.close();

                    m_bIsOpen = false;
                    globals::gpPlayerController->CheatManager->Slomo(1);
                }

                void CreativeMenu::SaveCreation(std::string name)
                {
                    auto pawnActor = static_cast<SDK::AFortPlayerPawn*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
                    json creationJson;
                    int buildActorIndex = 0;
                    int trapActorIndex = 0;

                    for (int i = 0; i < globals::gpLevel->Actors.Count; i++)
                    {
                        auto actor = globals::gpLevel->Actors[i];
                        if (actor && (actor->IsA(SDK::ABuildingSMActor::StaticClass())) && (actor->GetFullName().find("PBWA") != std::string::npos))
                        {
                            auto name = actor->GetFullName();
                            auto formattedName = name.substr(0, name.find(" ")).substr(0, name.length() - 1).substr(0, name.find("_C"));
                            auto smActor = static_cast<SDK::ABuildingSMActor*>(actor);

                            creationJson["buildPieces"][buildActorIndex]["name"] = formattedName;
                            creationJson["buildPieces"][buildActorIndex]["guid"] =
                            {
                                { "a", smActor->MyGuid.A },
                                { "b", smActor->MyGuid.B },
                                { "c", smActor->MyGuid.C },
                                { "d", smActor->MyGuid.D }
                            };
                            creationJson["buildPieces"][buildActorIndex]["location"] = 
                            {
                                { "x", actor->K2_GetActorLocation().X },
                                { "y", actor->K2_GetActorLocation().Y },
                                { "z", actor->K2_GetActorLocation().Z }
                            };
                            creationJson["buildPieces"][buildActorIndex]["rotation"] = 
                            {
                                { "pitch", actor->K2_GetActorRotation().Pitch },
                                { "yaw", actor->K2_GetActorRotation().Yaw },
                                { "roll", actor->K2_GetActorRotation().Roll }
                            };
                            creationJson["buildPieces"][buildActorIndex]["health"] = static_cast<SDK::ABuildingSMActor*>(actor)->GetHealth();

                            buildActorIndex++;
                        }
                        if (actor && (actor->IsA(SDK::ABuildingTrap::StaticClass())) && (actor->GetFullName().find("Trap") != std::string::npos))
                        {
                            auto name = actor->GetFullName();
                            auto formattedName = name.substr(0, name.find(" ")).substr(0, name.length() - 1).substr(0, name.find("_C"));
                            auto smActor = static_cast<SDK::ABuildingSMActor*>(actor);
                            auto trapActor = reinterpret_cast<models::offsetfixes::TrapBuild*>(actor);

                            creationJson["traps"][trapActorIndex]["name"] = formattedName;
                            creationJson["traps"][trapActorIndex]["tid"] = trapActor->TrapData->GetFullName();
                            creationJson["traps"][trapActorIndex]["buildGuid"] =
                            {
                                { "a", smActor->MyGuid.A },
                                { "b", smActor->MyGuid.B },
                                { "c", smActor->MyGuid.C },
                                { "d", smActor->MyGuid.D }
                            };
                            creationJson["traps"][trapActorIndex]["location"] =
                            {
                                { "x", actor->K2_GetActorLocation().X },
                                { "y", actor->K2_GetActorLocation().Y },
                                { "z", actor->K2_GetActorLocation().Z }
                            };
                            creationJson["traps"][trapActorIndex]["rotation"] =
                            {
                                { "pitch", actor->K2_GetActorRotation().Pitch },
                                { "yaw", actor->K2_GetActorRotation().Yaw },
                                { "roll", actor->K2_GetActorRotation().Roll }
                            };

                            trapActorIndex++;
                        }
                    }

                    // Info
                    creationJson["info"]["name"] = name;
                    creationJson["player"]["location"] = 
                    {
                        { "x", pawnActor->K2_GetActorLocation().X },
                        { "y", pawnActor->K2_GetActorLocation().Y },
                        { "z", pawnActor->K2_GetActorLocation().Z }
                    };
                    creationJson["player"]["rotation"] = 
                    {
                        { "pitch", pawnActor->K2_GetActorRotation().Pitch },
                        { "yaw", pawnActor->K2_GetActorRotation().Yaw },
                        { "roll", pawnActor->K2_GetActorRotation().Roll }
                    };

                    std::ofstream o("creations\\" + name + ".pcf");
                    o << creationJson.dump(4) << std::endl;
                    o.close();
                }
            }
        }
    }
}
