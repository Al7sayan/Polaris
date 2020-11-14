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
struct AFortAsLastBuildable
{
public:
    unsigned char UnknownData00[0x1948];
    class SDK::UClass* PreviousBuildableClass;
};
struct AFortAsEditActor
{
public:
    unsigned char UnknownData00[0x1A48];
    class SDK::ABuildingSMActor* EditBuildingActor;
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
                if (pFunction->GetName().find("HandleBuildingMaterialChanged") != std::string::npos) {
                    auto controller = static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController);
                    auto buildtool = reinterpret_cast<SDK::AFortWeap_BuildingTool*>(controller->MyFortPawn->CurrentWeapon);
                    auto cba = reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass;
                    switch (buildtool->LastResourceType) {
                        case SDK::EFortResourceType::Wood: //stone
                            m_pPlayerPawn->m_sPendingMaterials = TEXT("Stone");
                            break;
                        case SDK::EFortResourceType::Stone: //metal
                            m_pPlayerPawn->m_sPendingMaterials = TEXT("Metal");
                            break;
                        case SDK::EFortResourceType::Metal: //wood
                            m_pPlayerPawn->m_sPendingMaterials = TEXT("Wood");
                            break;
                    }
                    m_pPlayerPawn->m_bHasCycledWall = false;
                    m_pPlayerPawn->m_bHasCycledFloor = false;
                    m_pPlayerPawn->m_bHasCycledStair = false;
                    m_pPlayerPawn->m_bHasCycledRoof = false;
                }
                if (pFunction->GetName().find("ServerEditBuildingActor") != std::string::npos) {
                    auto editactor = reinterpret_cast<AFortAsEditActor*>(globals::gpPlayerController);
                    if (editactor->EditBuildingActor != reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker) 
                        m_iCurrentBuildPiece = 0;
                }
                if (pFunction->GetName().find("OnSuccessfulMatchInteractComplete") != std::string::npos) {
                    auto editactor = reinterpret_cast<AFortAsEditActor*>(globals::gpPlayerController);
                    if (editactor->EditBuildingActor != reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker) 
                        m_iCurrentBuildPiece = 0;
                }
                if (pFunction->GetName().find("ServerEndEditingBuildingActor") != std::string::npos) {
                    auto editactor = reinterpret_cast<AFortAsEditActor*>(globals::gpPlayerController);
                    if (editactor->EditBuildingActor != reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker) 
                        m_iCurrentBuildPiece = 0;
                }
                if (pFunction->GetName().find("ServerHandleMissionEvent_ToggledEditMode") != std::string::npos) {
                    auto cba = reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass;
                    auto lba = reinterpret_cast<AFortAsLastBuildable*>(globals::gpPlayerController)->PreviousBuildableClass;
                    auto controller = reinterpret_cast<SDK::AFortPlayerController*>(globals::gpPlayerController);
                    auto editactor = reinterpret_cast<AFortAsEditActor*>(globals::gpPlayerController);
                    if (editactor->EditBuildingActor == reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker) {
                        switch (m_iCurrentBuildPiece) {
                        case 1:
                            m_pLastBuildClassForWall = cba;
                            lba = m_pLastBuildClassForWall;
                            break;
                        case 2:
                            m_pLastBuildClassForFloor = cba;
                            lba = m_pLastBuildClassForFloor;
                            break;
                        case 3:
                            m_pLastBuildClassForStair = cba;
                            lba = m_pLastBuildClassForStair;
                            break;
                        case 4:
                            m_pLastBuildClassForRoof = cba;
                            lba = m_pLastBuildClassForRoof;
                            break;
                        }
                    }
                }
                if (pFunction->GetName().find("ServerLoadingScreenDropped") != std::string::npos)
                    m_pQuickbars->SetupQuickbars();
                if (pFunction->GetName().find("ServerExecuteInventoryItem") != std::string::npos) {
                    SDK::FGuid* paramGuid = reinterpret_cast<SDK::FGuid*>(pParams);
                    for (auto it = m_pInventory->m_mItems.begin(); it != m_pInventory->m_mItems.end(); it++) {
                        if (AreGuidsTheSame((*it->first), (*paramGuid)) && !static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController)->IsInBuildMode()) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(it->second, (*it->first));
                            m_iCurrentBuildPiece = 0;
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
                            m_iCurrentBuildPiece = 0;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgWallBuild))) {
                        if (m_iCurrentBuildPiece != 1) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pWallBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = m_pLastBuildClassForWall;
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewWall;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            auto cheatman = static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager);
                            if (!m_pPlayerPawn->m_bHasCycledWallOnce) {
                                cheatman->BuildWith(TEXT("Wood"));
                                cheatman->BuildWith(TEXT("Stone"));
                                cheatman->BuildWith(TEXT("Metal"));
                                cheatman->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasCycledWallOnce = true;
                            }
                            if (m_pPlayerPawn->m_bHasCycledWall != true) {
                                cheatman->BuildWith(m_pPlayerPawn->m_sPendingMaterials);
                                m_pPlayerPawn->m_bHasCycledWall = true;
                            }
                            m_iCurrentBuildPiece = 1;
                            m_pLastBuildClassForWall = reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgFloorBuild))) {
                        if (m_iCurrentBuildPiece != 2) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pFloorBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = m_pLastBuildClassForFloor;
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewFloor;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            auto cheatman = static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager);
                            if (!m_pPlayerPawn->m_bHasCycledFloorOnce) {
                                cheatman->BuildWith(TEXT("Wood"));
                                cheatman->BuildWith(TEXT("Stone"));
                                cheatman->BuildWith(TEXT("Metal"));
                                cheatman->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasCycledFloorOnce = true;
                            }
                            if (m_pPlayerPawn->m_bHasCycledFloor != true) {
                                cheatman->BuildWith(m_pPlayerPawn->m_sPendingMaterials);
                                m_pPlayerPawn->m_bHasCycledFloor = true;
                            }
                            m_iCurrentBuildPiece = 2;
                            m_pLastBuildClassForFloor = reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgStairBuild))) {
                        if (m_iCurrentBuildPiece != 3) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pStairBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = m_pLastBuildClassForStair;
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewStair;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(false);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(true);

                            // building position fix
                            auto cheatman = static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager);
                            if (!m_pPlayerPawn->m_bHasCycledStairOnce) {
                                cheatman->BuildWith(TEXT("Wood"));
                                cheatman->BuildWith(TEXT("Stone"));
                                cheatman->BuildWith(TEXT("Metal"));
                                cheatman->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasCycledStairOnce = true;
                            }
                            if (m_pPlayerPawn->m_bHasCycledStair != true) {
                                cheatman->BuildWith(m_pPlayerPawn->m_sPendingMaterials);
                                m_pPlayerPawn->m_bHasCycledStair = true;
                            }
                            m_iCurrentBuildPiece = 3;
                            m_pLastBuildClassForStair = reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass;
                        }
                    }
                    if (AreGuidsTheSame((*paramGuid), (*m_pInventory->m_pgRoofBuild))) {
                        if (m_iCurrentBuildPiece != 4) {
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(m_pInventory->m_pRoofBuildDef, (*paramGuid));
                            reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = m_pLastBuildClassForRoof;
                            reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->m_pBuildPreviewRoof;
                            m_pPlayerPawn->m_pBuildPreviewWall->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewFloor->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewStair->SetActorHiddenInGame(true);
                            m_pPlayerPawn->m_pBuildPreviewRoof->SetActorHiddenInGame(false);

                            // building position fix
                            auto cheatman = static_cast<SDK::UFortCheatManager*>(polaris::globals::gpPlayerController->CheatManager);
                            if (!m_pPlayerPawn->m_bHasCycledRoofOnce) {
                                cheatman->BuildWith(TEXT("Wood"));
                                cheatman->BuildWith(TEXT("Stone"));
                                cheatman->BuildWith(TEXT("Metal"));
                                cheatman->BuildWith(TEXT("Wood"));
                                m_pPlayerPawn->m_bHasCycledRoofOnce = true;
                            }
                            if (m_pPlayerPawn->m_bHasCycledRoof != true) {
                                cheatman->BuildWith(m_pPlayerPawn->m_sPendingMaterials);
                                m_pPlayerPawn->m_bHasCycledRoof = true;
                            }
                            m_iCurrentBuildPiece = 4;
                            m_pLastBuildClassForRoof = reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass;
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