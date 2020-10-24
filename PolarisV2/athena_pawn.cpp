#include "athena_pawn.h"
#include "globals.h"
#include "sdk_utils.h"

struct AFortAsQuickBars
{
public:
    unsigned char                                      UnknownData00[0x1A88];
    class SDK::AFortQuickBars* QuickBars;
};
namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            AthenaPawn::AthenaPawn()
            {
                globals::gpPlayerController->CheatManager->Summon(TEXT("PlayerPawn_Athena_C"));
                m_pPawnActor = static_cast<SDK::AFortPlayerPawn*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
                globals::gpPlayerController->Possess(m_pPawnActor);

                // Load Ramirez onto the pawn and replicate character parts.
                auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState);
                playerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
                playerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
                playerState->OnRep_CharacterParts();
                m_pPawnActor->OnCharacterPartsReinitialized();

                playerState->TeamIndex = SDK::EFortTeam::HumanPvP_Team69; // funny number go brrrrrr
                playerState->OnRep_TeamIndex();

                //Reset the pawn's actor rotation.
                SDK::FRotator actorRotation = m_pPawnActor->K2_GetActorRotation();
                actorRotation.Pitch = 0;
                actorRotation.Roll = 0;

                m_pPawnActor->K2_SetActorLocationAndRotation(m_pPawnActor->K2_GetActorLocation(), actorRotation, false, true, new SDK::FHitResult());
                //SetupInventoryAndQuickBars();
            }
            AthenaPawn::~AthenaPawn()
            {
                if (m_pPawnActor != nullptr)
                    m_pPawnActor->K2_DestroyActor();
            }

            void AthenaPawn::SetupInventoryAndQuickBars()
            {
                //Setting up quickbars
                polaris::globals::gpPlayerController->CheatManager->Summon(TEXT("FortQuickBars"));

                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars = static_cast<SDK::AFortQuickBars*>(utilities::SDKUtils::FindActor(SDK::AFortQuickBars::StaticClass()));
                reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars->SetOwner(polaris::globals::gpPlayerController);
                auto pQuickBars = reinterpret_cast<AFortAsQuickBars*>(polaris::globals::gpPlayerController)->QuickBars;
                static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController)->Role = SDK::ENetRole::ROLE_None;
                static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController)->OnRep_QuickBar();
                static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController)->Role = SDK::ENetRole::ROLE_Authority;
                //Defining Item Definitions
                auto pTrapDef = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortTrapItemDefinition TID_Floor_Player_Jump_Pad_Free_Direction.TID_Floor_Player_Jump_Pad_Free_Direction");
                auto pPickaxeDef = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
                auto pWallBuildDef = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
                auto pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
                auto pStairBuildDef = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
                auto pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
                auto pWood = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
                auto pMetal = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
                auto pStone = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
                auto pLight = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
                auto pMed = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
                auto pShot = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataShells.AthenaAmmoDataShells");
                auto pHeavy = SDK::UObject::FindObject<SDK::UFortItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
                // funny item array
                SDK::UFortItemDefinition* pItemDefs[12] = {};
                pItemDefs[0] = pTrapDef;
                pItemDefs[1] = pPickaxeDef;
                pItemDefs[2] = pWallBuildDef;
                pItemDefs[3] = pFloorBuildDef;
                pItemDefs[4] = pStairBuildDef;
                pItemDefs[5] = pRoofBuildDef;
                pItemDefs[6] = pWood;
                pItemDefs[7] = pMetal;
                pItemDefs[8] = pStone;
                pItemDefs[9] = pLight;
                pItemDefs[10] = pMed;
                pItemDefs[11] = pShot;
                pItemDefs[12] = pHeavy;
                auto pInventory1 = static_cast<SDK::AFortInventory*>(polaris::utilities::SDKUtils::FindActor(SDK::AFortInventory::StaticClass()));
                auto pInventory2 = static_cast<SDK::AFortInventory*>(polaris::utilities::SDKUtils::FindActor(SDK::AFortInventory::StaticClass(),1));
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
                    SDK::TArray<class SDK::UFortWorldItem*>* iteminst = &inv->ItemInstances;
                    iteminst->Count = 12;
                    iteminst->Max = 12;
                    iteminst->Data = (class SDK::UFortWorldItem**)::malloc(iteminst->Count * sizeof(SDK::UFortWorldItem*));
                    for (int i = 0; SDK::UFortItemDefinition* itemdef = pItemDefs[i]; i != 12) {
                        SDK::FGuid Guid;
                        Guid.A = rand() % 10;
                        Guid.B = rand() % 10;
                        Guid.C = rand() % 10;
                        Guid.D = rand() % 10;
                        auto ItemEntry = new SDK::FFortItemEntry;
                        switch (i) {
                            default:
                                ItemEntry->Count = 100;
                                break;
                            case 1:
                                ItemEntry->Count = 1;
                                break;
                            case 2:
                                ItemEntry->Count = 1;
                                break;
                            case 3:
                                ItemEntry->Count = 1;
                                break;
                            case 4:
                                ItemEntry->Count = 1;
                                break;
                            case 5:
                                ItemEntry->Count = 1;
                                break;
                        }
                        ItemEntry->ItemDefinition = itemdef;
                        ItemEntry->Durability = 1000;
                        ItemEntry->Level = 1;
                        ItemEntry->ItemGuid = Guid;
                        ItemEntry->bIsDirty = false;
                        auto pWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                        pWorldItem->OwnerInventory = myInventory;
                        pWorldItem->ItemEntry = *ItemEntry;
                        pWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerController*>(polaris::globals::gpPlayerController));
                        iteminst->operator[](i) = pWorldItem;
                        i++;
                    }
                    //pQuickBars->EnableSlot(SDK::EFortQuickBars::Primary, 0);
                    //pQuickBars->ServerAddItemInternal(inv->ItemInstances.operator[](2)->ItemEntry.ItemGuid, SDK::EFortQuickBars::Secondary, 0);
                    //pQuickBars->ServerAddItemInternal(inv->ItemInstances.operator[](3)->ItemEntry.ItemGuid, SDK::EFortQuickBars::Secondary, 1);
                    //pQuickBars->ServerAddItemInternal(inv->ItemInstances.operator[](4)->ItemEntry.ItemGuid, SDK::EFortQuickBars::Secondary, 2);
                    //pQuickBars->ServerAddItemInternal(inv->ItemInstances.operator[](5)->ItemEntry.ItemGuid, SDK::EFortQuickBars::Secondary, 3);
                    //pQuickBars->ServerAddItemInternal(inv->ItemInstances.operator[](0)->ItemEntry.ItemGuid, SDK::EFortQuickBars::Secondary, 4);
                    //pQuickBars->ServerAddItemInternal(inv->ItemInstances.operator[](1)->ItemEntry.ItemGuid, SDK::EFortQuickBars::Primary, 0);
                }
            }
            void AthenaPawn::Update()
            {
                Pawn::Update();

                bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->bWantsToSprint;
                m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;
            }
        }
    }
}