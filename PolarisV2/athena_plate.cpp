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
struct AFortAsBuildPreview
{
public:
    unsigned char UnknownData00[0x1788];
    class SDK::ABuildingPlayerPrimitivePreview* BuildPreviewMarker;
};
struct AFortAsCurrentBuildable
{
public:
    unsigned char UnknownData00[0x1940];
    class SDK::UClass* CurrentBuildableClass;
};

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
            {
                if (m_pPlayerPawn != nullptr)
                    m_pPlayerPawn->ProcessEventHook(pObject, pFunction, pParams);

                if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
                    Initialize();

                if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos ||
                    (pFunction->GetName().find("AircraftExitedDropZone") != std::string::npos && static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->IsInAircraft()))
                {
                    if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos)
                        m_pPlayerPawn->~AthenaPawn();

                    m_pPlayerPawn = new pawn::pawns::AthenaPawn;
                }

                if (pFunction->GetName().find("ServerLoadingScreenDropped") != std::string::npos)
                    m_pQuickbars->SetupQuickbars();
                if (pFunction->GetName().find("ServerExecuteInventoryItem") != std::string::npos) {
                    SDK::FGuid* paramGuid = reinterpret_cast<SDK::FGuid*>(pParams);
                    for (auto it = m_pInventory->m_mItems.begin(); it != m_pInventory->m_mItems.end(); it++) {
                        if (AreGuidsTheSame((*it->first), (*paramGuid)) && !AreGuidsTheSame((*m_pInventory->m_pgWallBuild), (*paramGuid)) && !AreGuidsTheSame((*m_pInventory->m_pgFloorBuild), (*paramGuid)) && !AreGuidsTheSame((*m_pInventory->m_pgStairBuild), (*paramGuid)) && !AreGuidsTheSame((*m_pInventory->m_pgRoofBuild), (*paramGuid))) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(it->second, (*it->first));
                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    for (auto it = m_pInventory->m_mTraps.begin(); it != m_pInventory->m_mTraps.end(); it++) {
                        if (AreGuidsTheSame((*it->first), (*paramGuid))) {
                            if (m_pInventory->bTrapDone == false) {
                                m_pInventory->m_pTrapC = SDK::UObject::FindObject<SDK::ABuildingActor>("Trap_Floor_Player_Jump_Free_Direction_Pad_C Athena_Terrain.Athena_Terrain.PersistentLevel.Trap_Floor_Player_Jump_Free_Direction_Pad_C_1");
                                m_pInventory->bTrapDone = true;
                            }
                            m_pPlayerPawn->m_pPawnActor->PickUpActor(m_pInventory->m_pTrapC, it->second);
                            m_pPlayerPawn->m_pPawnActor->CurrentWeapon->ItemEntryGuid = (*it->first);
                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgWallBuild))) {
                        if (!m_bOnWall) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pWallBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_Solid_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewWall;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledWall)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledWall = true;
                            }

                            m_bOnWall = true;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgFloorBuild))) {
                        if (!m_bOnFloor) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pFloorBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_Floor_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewFloor;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledFloor)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledFloor = true;
                            }

                            m_bOnWall = false;
                            m_bOnFloor = true;
                            m_bOnStair = false;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgStairBuild))) {
                        if (!m_bOnStair) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pStairBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_StairW_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewStair;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledStair)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledStair = true;
                            }

                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = true;
                            m_bOnRoof = false;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgRoofBuild))) {
                        if (!m_bOnRoof) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pRoofBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_RoofC_C::StaticClass();
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewRoof;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(false);

                            // building position fix
                            if (!m_pPlayerPawn->m_bHasAutoCycledRoof)
                            {
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Stone"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Metal"));
                                static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager)->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasAutoCycledRoof = true;
                            }

                            m_bOnWall = false;
                            m_bOnFloor = false;
                            m_bOnStair = false;
                            m_bOnRoof = true;
                        }
                    }
                }
            }
            void AthenaPlate::Update()
            {
                if (m_pPlayerPawn != nullptr)
                    m_pPlayerPawn->Update();

                // TEMP: Go back to Frontend.
                if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000)
                    gpProgram->m_pMainTable->PopPlate();
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
                utilities::SDKUtils::InitPatches();
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

                // Disable garbage collection.
                auto pCollectGarbageInternalAddress = utilities::SDKUtils::FindPattern("\x48\x8B\xC4\x48\x89\x58\x08\x88\x50\x10", "xxxxxxxxxx");
                if (!pCollectGarbageInternalAddress)
                    utilities::ErrorUtils::ThrowException(L"Finding pattern for CollectGarbageInternal has failed. Please relaunch Fortnite and try again!");
                
                MH_CreateHook(static_cast<LPVOID>(pCollectGarbageInternalAddress), CollectGarbageInternalHook, reinterpret_cast<LPVOID*>(&CollectGarbageInternal));
                MH_EnableHook(static_cast<LPVOID>(pCollectGarbageInternalAddress));

                // Spawn a Player Pawn and setup inventory.
                m_pPlayerPawn = new pawn::pawns::AthenaPawn;
                
                m_pInventory = new inventory::Inventory;
                m_pInventory->m_pPickaxeDef = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>(m_pPlayerPawn->mPickaxeAsWid[static_cast<SDK::AFortPlayerPawnAthena*>(m_pPlayerPawn->m_pPawnActor)->CustomizationLoadout.Character->GetName()].c_str());
                m_pQuickbars = new inventory::quickbars;
                m_pInventory->SetupInventory();
                m_pQuickbars->m_pgPickaxe = m_pInventory->m_pgPickaxe;
                m_pQuickbars->m_pgWallBuild = m_pInventory->m_pgWallBuild;
                m_pQuickbars->m_pgFloorBuild = m_pInventory->m_pgFloorBuild;
                m_pQuickbars->m_pgStairBuild = m_pInventory->m_pgStairBuild;
                m_pQuickbars->m_pgRoofBuild = m_pInventory->m_pgRoofBuild;

                // Tell the client that we are ready to start the match, this allows the loading screen to drop.
                static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController)->ServerReadyToStartMatch();
                static_cast<SDK::AGameMode*>((*globals::gpWorld)->AuthorityGameMode)->StartMatch();
            }
            bool AthenaPlate::AreGuidsTheSame(SDK::FGuid guidA, SDK::FGuid guidB)
            {
                if (guidA.A == guidB.A && guidA.B == guidB.B && guidA.C == guidB.C && guidA.D == guidB.D) 
                    return true;
                else
                    return false;
            }
        }
    }
}