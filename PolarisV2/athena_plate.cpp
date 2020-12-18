#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "inventory.h"
#include "quickbars.h"
#include "program.h"
#include <MinHook.h>
#include <vector>
#include <map>
#include <iostream>
PVOID(*CollectGarbageInternal)(uint32_t, bool) = nullptr;
PVOID CollectGarbageInternalHook(uint32_t KeepFlags, bool bPerformFullPurge)
{

    return NULL;
}

namespace polaris::tables::plates
{
    void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pFunction->GetName().find("ClientReportKill") != std::string::npos) { //kill stuff
            auto controller = static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController);
            auto playerstate = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState);
            if (playerstate->KillScore >= 99)
                controller->ClientNotifyWon();
            playerstate->KillScore++;
            playerstate->OnRep_Kills();
        }
        if (pFunction->GetName().find("ServerAddItemInternal") != std::string::npos)
        {
            auto params = reinterpret_cast<SDK::AFortQuickBars_ServerAddItemInternal_Params*>(pParams);
            int slot = params->Slot;
            SDK::EFortQuickBars quickbar = params->InQuickBar;
            SDK::FGuid guid = params->Item;
            if (slot != -1 && quickbar == SDK::EFortQuickBars::Secondary)
            {
                for (auto it = m_pInventory->m_mItems.begin(); it != m_pInventory->m_mItems.end(); it++)
                {
                    if (m_pEquip->AreGuidsTheSame((*it->first), guid))
                    {
                        SDK::FLinearColor color{ 100,100,100,100 };
                        if (!m_pHud)
                            m_pHud = SDK::UObject::FindObject<SDK::UAthenaHUD_C>("AthenaHUD_C Transient.FortEngine_1.FortGameInstance_1.AthenaHUD_C_1");
                        if (m_pHud != nullptr)
                        {
                            SDK::FSlateBrush brush = it->second->GetSmallPreviewImageBrush();
                            if (&brush)
                            {
                                m_pHud->QuickbarSecondary->QuickbarSlots[params->Slot]->Empty->SetBrush(brush);
                                m_pHud->QuickbarSecondary->QuickbarSlots[params->Slot]->Empty->SetColorAndOpacity(color);
                            }
                        }
                    }
                }
            }
        }
        if (m_pPlayerPawn != nullptr)
            m_pPlayerPawn->ProcessEventHook(pObject, pFunction, pParams);

        if (m_pEquip != nullptr)
            m_pEquip->ProcessEventHook(pObject, pFunction, pParams);

        if (pFunction->GetName().find("Tick") != std::string::npos && m_bHasLoadingScreenDropped && m_pPlayerPawn->m_pPawnActor != nullptr)
        {
            for (std::list<pawn::pawns::BotPawn*>::iterator it = globals::glBotList.begin(); it != globals::glBotList.end(); it++) {
                if (!(*it)->m_bHasConstructed) {
                    (*it)->Construct();
                    m_pHud->TeamInfo->AppendTeamMember((*it)->m_pPlayerState, globals::glBotList.size());
                    //(*it)->m_pBotPawnController->NetPlayerIndex = globals::glBotList.size();
                    //static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->PlayerToSpectateOnDeath = (*it)->m_pBotPawnActor;
                }
                else if ((*it) != nullptr && (*it)->m_pBotPawnActor != nullptr && !(*it)->m_pBotPawnController->IsInAircraft() && (*it)->m_pBotPawnActor->GetHealth() != 0) {
                    SDK::FVector loc = m_pKismetmathlibrary->STATIC_Subtract_VectorVector(
                        m_pPlayerPawn->m_pPawnActor->K2_GetActorLocation(),
                        (*it)->m_pBotPawnActor->K2_GetActorLocation());
                    (*it)->m_pBotPawnActor->AddMovementInput(loc, 1, true);
                    SDK::FRotator rot = m_pKismetmathlibrary->STATIC_Conv_VectorToRotator(loc);
                    (*it)->m_pBotPawnController->ControlRotation = rot;
                    rot.Pitch = 0;
                    (*it)->m_pBotPawnActor->K2_SetActorRotation(rot, false);
                    (*it)->m_pBotPawnActor->CurrentMovementStyle = SDK::EFortMovementStyle::Sprinting;
                    (*it)->m_pBotPawnActor->PawnStartFire(0);
                }
                else {
                    m_lBots.erase(it);
                }
            }
        }

        // Victory Royale screen
        if (pFunction->GetName().find("BndEvt__LeaveButton_K2Node_ComponentBoundEvent_76_CommonButtonClicked__DelegateSignature") != std::string::npos)
        {
            if (!uiContext)
            {
                uiContext = SDK::UObject::FindObject<SDK::UFortGlobalUIContext>("FortGlobalUIContext Transient.FortEngine_1.FortLocalPlayer_1.FortGlobalUIContext_1");
            }
            // the below function just straight-up exits the engine without any confirmation.
            // UFortGlobalUIContext::AllowQuit() does jack shit
            // POSSIBLE ANTI-CRASH SOLUTION: invoke a MessageBox asking to exit the game instead?
            uiContext->QuitGame();
        }

        if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
            Initialize();

        if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos && static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->IsInAircraft())
        {
            m_pPlayerPawn = new pawn::pawns::AthenaPawn(static_cast<SDK::AAthena_GameState_C*>((*globals::gpWorld)->GameState)->GetAircraft()->K2_GetActorLocation(),
                reinterpret_cast<SDK::AFortPlayerControllerAthena_ServerAttemptAircraftJump_Params*>(pParams)->ClientRotation);
            m_pEquip->m_pPlayerPawn = m_pPlayerPawn;
        }
        if (pFunction->GetName().find("AircraftExitedDropZone") != std::string::npos && static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->IsInAircraft())
        {
            m_pPlayerPawn = new pawn::pawns::AthenaPawn(static_cast<SDK::AAthena_GameState_C*>((*globals::gpWorld)->GameState)->GetAircraft()->K2_GetActorLocation(),
                SDK::FRotator{ 0,0,0 });
            m_pEquip->m_pPlayerPawn = m_pPlayerPawn;
        }
        if (pFunction->GetName().find("ServerLoadingScreenDropped") != std::string::npos && !m_bHasDoneQuickbars) {
            m_pQuickbars->SetupQuickbars();
            m_bHasDoneQuickbars = true;
        }
    }
    void AthenaPlate::Update()
    {
        if (m_pPlayerPawn != nullptr)
            m_pPlayerPawn->Update();
    }

    void AthenaPlate::OnEnabled()
    {
        utilities::SDKUtils::InitSdk();
        utilities::SDKUtils::InitGlobals();

        globals::gpPlayerController->SwitchLevel(TEXT("Athena_Terrain"));
    }

    void AthenaPlate::Initialize()
    {
        //Initialize the SDK again.
        m_bIsInitialized = true;
        utilities::SDKUtils::InitSdk();
        utilities::SDKUtils::InitGlobals();
        //Load Husks Into Memory.
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn.HuskPawn_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Fire.HuskPawn_Fire_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Ice.HuskPawn_Ice_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Lightning.HuskPawn_Lightning_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Beehive.HuskPawn_Beehive_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Bombshell.HuskPawn_Bombshell_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Bombshell_Poison.HuskPawn_Bombshell_Poison_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf.HuskPawn_Dwarf_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Fire.HuskPawn_Dwarf_Fire_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Ice.HuskPawn_Dwarf_Ice_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Characters/Enemies/Husk/Blueprints/HuskPawn_Dwarf_Lightning.HuskPawn_Dwarf_Lightning_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/Trap_Floor_Spikes.Trap_Floor_Spikes_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/Trap_Floor_Player_Jump_Free_Direction_Pad.Trap_Floor_Player_Jump_Free_Direction_Pad_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/Trap_Floor_Player_Jump_Pad.Trap_Floor_Player_Jump_Pad_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/Trap_Ceiling_ElectricWeak_Athena.Trap_Ceiling_ElectricWeak_Athena_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/Trap_Floor_Spikes.Trap_Floor_Spikes_C");
        utilities::SDKUtils::FindOrLoadObject("/Game/Items/Traps/Blueprints/Trap_Ceiling_Falling.Trap_Ceiling_Falling_C");

        // Disable garbage collection.

        m_pCollectGarbageInternalAddress = utilities::SDKUtils::FindPattern("\x48\x8B\xC4\x48\x89\x58\x08\x88\x50\x10", "xxxxxxxxxx");
        if (!m_pCollectGarbageInternalAddress)
            utilities::ErrorUtils::ThrowException(L"Finding pattern for CollectGarbageInternal has failed. Please relaunch Fortnite and try again!");

        MH_CreateHook(static_cast<LPVOID>(m_pCollectGarbageInternalAddress), CollectGarbageInternalHook, reinterpret_cast<LPVOID*>(&CollectGarbageInternal));
        MH_EnableHook(static_cast<LPVOID>(m_pCollectGarbageInternalAddress));
        static_cast<SDK::AFortGameModeAthena*>((*globals::gpWorld)->AuthorityGameMode)->bSquadPlay = true;
        // Spawn a Player Pawn and setup inventory.
        m_pPlayerPawn = new pawn::pawns::AthenaPawn(globals::gpPlayerController->SpectatorPawn->K2_GetActorLocation(),
            globals::gpPlayerController->SpectatorPawn->K2_GetActorRotation());

        m_pInventory = new inventory::Inventory;
        m_pInventory->m_pPickaxeDef = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>(m_pPlayerPawn->mPickaxeAsWid[static_cast<SDK::AFortPlayerPawnAthena*>(m_pPlayerPawn->m_pPawnActor)->CustomizationLoadout.Character->GetName()].c_str());
        m_pQuickbars = new inventory::quickbars;
        m_pInventory->SetupInventory();

        m_pQuickbars->m_pgPickaxe = m_pInventory->m_pgPickaxe;
        m_pQuickbars->m_pgWallBuild = m_pInventory->m_pgWallBuild;
        m_pQuickbars->m_pgFloorBuild = m_pInventory->m_pgFloorBuild;
        m_pQuickbars->m_pgStairBuild = m_pInventory->m_pgStairBuild;
        m_pQuickbars->m_pgRoofBuild = m_pInventory->m_pgRoofBuild;

        m_pEquip = new inventory::equip;
        m_pEquip->m_pInventory = m_pInventory;
        m_pEquip->m_pPlayerPawn = m_pPlayerPawn;

        // Tell the client that we are ready to start the match, this allows the loading screen to drop.
        static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController)->ServerReadyToStartMatch();
        static_cast<SDK::AGameMode*>((*globals::gpWorld)->AuthorityGameMode)->StartMatch();
        m_bHasLoadingScreenDropped = true;

        m_pKismetmathlibrary = SDK::UObject::FindObject<SDK::UKismetMathLibrary>("KismetMathLibrary Engine.Default__KismetMathLibrary");
        globals::glBotList.clear();
    }
}