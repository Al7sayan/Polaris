#include "bot_creator.h"
#include "bot_pawn.h"
#include "imgui_text_extension.h"
#include "athena_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "json.hpp"
#include "SDK.hpp"
#include "trap_build.h"

#include <iostream>
#include <fstream>

namespace polaris::ui::window::windows
{
    static char cName[128];
    static int iHealth = 100;

    BotCreator::BotCreator()
    {
    }

    void BotCreator::Draw()
    {
        ImGuiIO* io = &ImGui::GetIO();
        ImGui::SetNextWindowSize(ImVec2(380, 459), ImGuiCond_Appearing);
        //ImGui::SetNextWindowPos(ImVec2(((io->DisplaySize.x / 2) - (631 / 2)), ((io->DisplaySize.y / 2) - (410 / 2))));
        ImGui::Begin("New Bot", &m_bIsOpen);
        {
            ImGui::Header2("New Bot");
            ImGui::Text("Create a bot");
            ImGui::InputTextWithHint("Name", "Joe717", cName, IM_ARRAYSIZE(cName));
            ImGui::InputInt("Health", &iHealth);
            if (ImGui::Button("Create!"))
            {
                SDK::FVector Pos = globals::gpPlayerController->Pawn->K2_GetActorLocation();
                Pos.X = Pos.X + globals::gpPlayerController->Pawn->GetActorForwardVector().X;
                Pos.Y = Pos.Y + globals::gpPlayerController->Pawn->GetActorForwardVector().Y;
                Pos.Z = Pos.Z + globals::gpPlayerController->Pawn->GetActorForwardVector().Z;
                globals::glBotList.push_back(new pawn::pawns::BotPawn(Pos, globals::gpPlayerController->Pawn->K2_GetActorRotation(), cName, iHealth, static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState)->TeamIndex, SDK::UObject::FindObject<SDK::UFortWeaponRangedItemDefinition>("FortWeaponRangedItemDefinition WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Launcher_Rocket_Athena_VR_Ore_T03"), static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController), static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->OutpostInventory, true));
            }
            ImGui::SameLine();

            ImGui::End();
        }
    }
}
