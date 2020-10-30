#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
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
struct AFortAsQuickBars
{
public:
    unsigned char                                      UnknownData00[0x1A88];
    class SDK::AFortQuickBars* QuickBars;
};
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

SDK::FGuid* pgPickaxe;
SDK::FGuid* pgWallBuild;
SDK::FGuid* pgFloorBuild;
SDK::FGuid* pgStairBuild;
SDK::FGuid* pgRoofBuild;
SDK::UFortWeaponMeleeItemDefinition* pPickaxeDef;
SDK::UFortBuildingItemDefinition* pWallBuildDef;
SDK::UFortBuildingItemDefinition* pFloorBuildDef;
SDK::UFortBuildingItemDefinition* pStairBuildDef;
SDK::UFortBuildingItemDefinition* pRoofBuildDef;
SDK::ABuildingActor* pTrapC;
bool bTrapDone = false;
std::map<SDK::FGuid*, SDK::UFortWeaponItemDefinition*> mItems;
std::map<SDK::FGuid*, SDK::UFortTrapItemDefinition*> mTraps;
int iInventoryIteration = 0;
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
                    SetupQuickbars();
                if (pFunction->GetName().find("ServerExecuteInventoryItem") != std::string::npos) {
                    //printf(" %s, %s, %s, %s\n", /*pObject->GetFullName().c_str(),*/ std::to_string(reinterpret_cast<SDK::FGuid*>(pParams)->A), std::to_string(reinterpret_cast<SDK::FGuid*>(pParams)->B), std::to_string(reinterpret_cast<SDK::FGuid*>(pParams)->C), std::to_string(reinterpret_cast<SDK::FGuid*>(pParams)->D));
                    SDK::FGuid guid;
                    SDK::FGuid* paramGuid = reinterpret_cast<SDK::FGuid*>(pParams);
                    guid.A = paramGuid->A;
                    guid.B = paramGuid->B;
                    guid.C = paramGuid->C;
                    guid.D = paramGuid->D;
                    for (auto it = mItems.begin(); it != mItems.end(); it++) {
                        //printf(" %s, %s, %s, %s\n", std::to_string(it->first->A), std::to_string(it->first->B), std::to_string(it->first->C), std::to_string(it->first->D));
                        if (AreGuidsTheSame((*it->first), (*paramGuid))) {
                            //printf("\n Attempting To Equip item! \n");
                            m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(it->second, (*it->first));
                        }
                    }
                    for (auto it = mTraps.begin(); it != mTraps.end(); it++) {
                        //printf(" %s, %s, %s, %s\n", std::to_string(it->first->A), std::to_string(it->first->B), std::to_string(it->first->C), std::to_string(it->first->D));
                        if (AreGuidsTheSame((*it->first), (*paramGuid))) {
                            //printf("\n Attempting To Equip item! \n");
                            if (bTrapDone == false) {
                                pTrapC = SDK::UObject::FindObject<SDK::ABuildingActor>("Trap_Floor_Player_Jump_Free_Direction_Pad_C Athena_Terrain.Athena_Terrain.PersistentLevel.Trap_Floor_Player_Jump_Free_Direction_Pad_C_1");
                                bTrapDone = true;
                            }
                            m_pPlayerPawn->m_pPawnActor->PickUpActor(pTrapC, it->second);
                            m_pPlayerPawn->m_pPawnActor->CurrentWeapon->ItemEntryGuid = (*it->first);
                            //m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(it->second, (*it->first));
                        }
                    }
                    if (AreGuidsTheSame(guid, (*pgWallBuild))) {
                        reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_Solid_C::StaticClass();
                        reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->pBuildPreviewWall;
                        m_pPlayerPawn->pBuildPreviewWall->SetActorHiddenInGame(false);
                        m_pPlayerPawn->pBuildPreviewFloor->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewStair->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewRoof->SetActorHiddenInGame(true);
                    }
                    if (AreGuidsTheSame(guid, (*pgFloorBuild))) {
                        reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_Floor_C::StaticClass();
                        reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->pBuildPreviewFloor;
                        m_pPlayerPawn->pBuildPreviewWall->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewFloor->SetActorHiddenInGame(false);
                        m_pPlayerPawn->pBuildPreviewStair->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewRoof->SetActorHiddenInGame(true);
                    }
                    if (AreGuidsTheSame(guid, (*pgStairBuild))) {
                        reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_StairW_C::StaticClass();
                        reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->pBuildPreviewStair;
                        m_pPlayerPawn->pBuildPreviewWall->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewFloor->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewStair->SetActorHiddenInGame(false);
                        m_pPlayerPawn->pBuildPreviewRoof->SetActorHiddenInGame(true);
                    }
                    if (AreGuidsTheSame(guid, (*pgRoofBuild))) {
                        reinterpret_cast<AFortAsCurrentBuildable*>(globals::gpPlayerController)->CurrentBuildableClass = SDK::APBWA_W1_RoofC_C::StaticClass();
                        reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker = m_pPlayerPawn->pBuildPreviewRoof;
                        m_pPlayerPawn->pBuildPreviewWall->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewFloor->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewStair->SetActorHiddenInGame(true);
                        m_pPlayerPawn->pBuildPreviewRoof->SetActorHiddenInGame(false);
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

                // Disable garbage collection.
                auto pCollectGarbageInternalAddress = utilities::SDKUtils::FindPattern("\x48\x8B\xC4\x48\x89\x58\x08\x88\x50\x10", "xxxxxxxxxx");
                if (!pCollectGarbageInternalAddress)
                    utilities::ErrorUtils::ThrowException(L"Finding pattern for CollectGarbageInternal has failed. Please relaunch Fortnite and try again!");

                MH_CreateHook(static_cast<LPVOID>(pCollectGarbageInternalAddress), CollectGarbageInternalHook, reinterpret_cast<LPVOID*>(&CollectGarbageInternal));
                MH_EnableHook(static_cast<LPVOID>(pCollectGarbageInternalAddress));
                // Spawn a Player Pawn and setup inventory.
                m_pPlayerPawn = new pawn::pawns::AthenaPawn;
                // Defining Item Definitions
                pPickaxeDef = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>(m_pPlayerPawn->mPickaxeAsWid[static_cast<SDK::AFortPlayerPawnAthena*>(m_pPlayerPawn->m_pPawnActor)->CustomizationLoadout.Character->GetName()].c_str());
                pWallBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
                pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
                pStairBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
                pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortBuildingItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
                auto pWood = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
                auto pMetal = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
                auto pStone = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
                auto pLight = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
                auto pMed = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
                auto pShot = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataShells.AthenaAmmoDataShells");
                auto pHeavy = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
                auto pRockets = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AmmoDataRockets.AmmoDataRockets");
                auto pEnergyCells = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataEnergyCell.AthenaAmmoDataEnergyCell");
                pgPickaxe = utilities::SDKUtils::GenerateGuidPtr();
                pgWallBuild = utilities::SDKUtils::GenerateGuidPtr();
                pgFloorBuild = utilities::SDKUtils::GenerateGuidPtr();
                pgStairBuild = utilities::SDKUtils::GenerateGuidPtr();
                pgRoofBuild = utilities::SDKUtils::GenerateGuidPtr();
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pWood);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pMetal);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pStone);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pLight);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pMed);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pShot);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pHeavy);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pRockets);
                mItems.insert_or_assign(utilities::SDKUtils::GenerateGuidPtr(), pEnergyCells);
                mItems.insert_or_assign(pgPickaxe, pPickaxeDef);
                mItems.insert_or_assign(pgWallBuild, pWallBuildDef);
                mItems.insert_or_assign(pgFloorBuild, pFloorBuildDef);
                mItems.insert_or_assign(pgStairBuild, pStairBuildDef);
                mItems.insert_or_assign(pgRoofBuild, pRoofBuildDef);
                for (int i = 0; i < SDK::UObject::GetGlobalObjects().Num(); ++i)
                {
                    auto pObject = SDK::UObject::GetGlobalObjects().GetByIndex(i);
                    if (pObject != nullptr && pObject->GetFullName().find("FortniteGame") == std::string::npos)
                    {
                        if (pObject->GetFullName().rfind("FortWeaponRangedItemDefinition", 0) == 0)
                        {
                            if (pObject->GetFullName().find("Athena") != std::string::npos) {
                                mItems[utilities::SDKUtils::GenerateGuidPtr()] = reinterpret_cast<SDK::UFortWeaponItemDefinition*>(pObject);
                            }
                        }
                    }
                }
                for (int i = 0; i < SDK::UObject::GetGlobalObjects().Num(); ++i)
                {
                    auto pObject = SDK::UObject::GetGlobalObjects().GetByIndex(i);
                    if (pObject != nullptr && pObject->GetFullName().find("FortniteGame") == std::string::npos)
                    {
                        if (pObject->GetFullName().rfind("FortTrapItemDefinition", 0) == 0)
                        {
                            if (pObject->GetFullName().find("Athena") != std::string::npos) {
                                auto guid = utilities::SDKUtils::GenerateGuidPtr();
                                mTraps[guid] = reinterpret_cast<SDK::UFortTrapItemDefinition*>(pObject);
                                mItems[guid] = reinterpret_cast<SDK::UFortTrapItemDefinition*>(pObject);
                            }
                        }
                    }
                }
                SetupInventory();

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
            void AthenaPlate::SetupQuickbars()
            {
                polaris::globals::gpPlayerController->CheatManager->Summon(TEXT("FortQuickBars"));
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->SetOwner(polaris::globals::gpPlayerController);
                auto pQuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
                static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController)->Role = SDK::ENetRole::ROLE_None;
                static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController)->OnRep_QuickBar();
                static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController)->Role = SDK::ENetRole::ROLE_Authority;
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 0);
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 1);
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 2);
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 3);
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 4);
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Secondary, 5);
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->EnableSlot(SDK::EFortQuickBars::Primary, 0);
                pQuickBars->ServerAddItemInternal((*pgWallBuild), SDK::EFortQuickBars::Secondary, 0);
                pQuickBars->ServerAddItemInternal((*pgFloorBuild), SDK::EFortQuickBars::Secondary, 1);
                pQuickBars->ServerAddItemInternal((*pgStairBuild), SDK::EFortQuickBars::Secondary, 2);
                pQuickBars->ServerAddItemInternal((*pgRoofBuild), SDK::EFortQuickBars::Secondary, 3);
                pQuickBars->ServerAddItemInternal((*pgPickaxe), SDK::EFortQuickBars::Primary, 0);
                pQuickBars->ServerActivateSlotInternal(SDK::EFortQuickBars::Primary,0,0,true);
            }
            void AthenaPlate::SetupInventory()
            {
                auto pInventory1 = static_cast<SDK::AFortInventory*>(polaris::utilities::SDKUtils::FindActor(SDK::AFortInventory::StaticClass()));
                auto pInventory2 = static_cast<SDK::AFortInventory*>(polaris::utilities::SDKUtils::FindActor(SDK::AFortInventory::StaticClass(), 1));
                SDK::AFortInventory* pActualInv = nullptr;

                // joe nuts
                if (pInventory1 && pInventory1->InventoryType == SDK::EFortInventoryType::World)
                {
                    pActualInv = pInventory1;
                }

                if (pInventory2 && pInventory2->InventoryType == SDK::EFortInventoryType::World)
                {
                    pActualInv = pInventory2;
                }

                if (pActualInv)
                {
                    //gaming rgb inventory
                    auto myInventory = pActualInv;
                    SDK::FFortItemList* inv = &myInventory->Inventory;
                    SDK::TArray<class SDK::UFortWorldItem*>* pItemInsts = &inv->ItemInstances;
                    pItemInsts->Count = mItems.size();
                    pItemInsts->Max = mItems.size();
                    pItemInsts->Data = (class SDK::UFortWorldItem**)::malloc(pItemInsts->Count * sizeof(SDK::UFortWorldItem*));
                    for (auto it = mItems.begin(); it != mItems.end(); it++) {
                        auto pItemEntry = new SDK::FFortItemEntry;
                        pItemEntry->Count = 100; //thank god this is ok and doesn't fuck up guns
                        pItemEntry->ItemDefinition = it->second;
                        pItemEntry->Durability = 1000;
                        pItemEntry->Level = 1;
                        pItemEntry->ItemGuid = (*it->first);
                        pItemEntry->bIsDirty = false;
                        pItemEntry->LoadedAmmo = 0;
                        auto pWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                        pWorldItem->OwnerInventory = myInventory;
                        pWorldItem->ItemEntry = *pItemEntry;
                        pItemInsts->operator[](iInventoryIteration) = pWorldItem;
                        iInventoryIteration++;
                    }
                }
            }
        }
    }
}