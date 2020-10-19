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
                SetupInventoryAndQuickBars();
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
                //gaming rgb inventory
                auto pTrapDef = SDK::UObject::FindObject<SDK::UFortTrapItemDefinition>("FortTrapItemDefinition TID_Floor_Player_Jump_Pad_Free_Direction.TID_Floor_Player_Jump_Pad_Free_Direction");
                auto pPickaxeDef = SDK::UObject::FindObject<SDK::UFortWeaponMeleeItemDefinition>("FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
                auto pWallBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
                auto pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
                auto pStairBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
                auto pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
                auto pWood = SDK::UObject::FindObject<SDK::UFortResourceItemDefinition>("FortResourceItemDefinition WoodItemData.WoodItemData");
                auto pMetal = SDK::UObject::FindObject<SDK::UFortResourceItemDefinition>("FortResourceItemDefinition MetalItemData.MetalItemData");
                auto pStone = SDK::UObject::FindObject<SDK::UFortResourceItemDefinition>("FortResourceItemDefinition StoneItemData.StoneItemData");
                auto pLight = SDK::UObject::FindObject<SDK::UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
                auto pMed = SDK::UObject::FindObject<SDK::UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
                auto pShot = SDK::UObject::FindObject<SDK::UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataShells.AthenaAmmoDataShells");
                auto pHeavy = SDK::UObject::FindObject<SDK::UFortAmmoItemDefinition>("FortAmmoItemDefinition AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
                SDK::FGuid gLight;
                SDK::FGuid gMed;
                SDK::FGuid gShot;
                SDK::FGuid gHeavy;
                SDK::FGuid gWall;
                SDK::FGuid gRamp;
                SDK::FGuid gFloor;
                SDK::FGuid gCone;
                SDK::FGuid gTrap;
                SDK::FGuid gPickaxe;
                gTrap.A = 1;
                gTrap.B = 0;
                gTrap.C = 0;
                gTrap.D = 0;
                gPickaxe.A = 2;
                gPickaxe.B = 0;
                gPickaxe.C = 0;
                gPickaxe.D = 0;
                gLight.A = 123;
                gLight.B = 420;
                gLight.C = 420;
                gLight.D = 420;
                gMed.A = 420;
                gMed.B = 123;
                gMed.C = 420;
                gMed.D = 420;
                gShot.A = 420;
                gShot.B = 420;     //why guids, why?
                gShot.C = 123;
                gShot.D = 420;
                gHeavy.A = 420;
                gHeavy.B = 420;
                gHeavy.C = 420;
                gHeavy.D = 123;
                gWall.A = 420;
                gWall.B = 420;
                gWall.C = 420;
                gWall.D = 420;
                gRamp.A = 4210;
                gRamp.B = 420;
                gRamp.C = 420;
                gRamp.D = 420;
                gFloor.A = 420;
                gFloor.B = 420;
                gFloor.C = 4210;
                gFloor.D = 420;
                gCone.A = 4210;
                gCone.B = 420;
                gCone.C = 420;
                gCone.D = 4210;
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
                    auto myInventory = pActualInv;
                    SDK::FFortItemList* inv = &myInventory->Inventory;
                    auto TrapEntry = new SDK::FFortItemEntry;
                    TrapEntry->Count = 1;
                    TrapEntry->ItemDefinition = pTrapDef;
                    TrapEntry->Durability = 100;
                    TrapEntry->Level = 69;
                    TrapEntry->ItemGuid = gTrap;
                    TrapEntry->bIsDirty = false;

                    auto PickaxeEntry = new SDK::FFortItemEntry;
                    PickaxeEntry->Count = 1;
                    PickaxeEntry->ItemDefinition = pPickaxeDef;
                    PickaxeEntry->Durability = 1;
                    PickaxeEntry->Level = 1;
                    PickaxeEntry->ItemGuid = gPickaxe;
                    PickaxeEntry->bIsDirty = false;

                    auto WallEntry = new SDK::FFortItemEntry;
                    WallEntry->Count = 1;
                    WallEntry->ItemDefinition = pWallBuildDef;
                    WallEntry->Durability = 1;
                    WallEntry->Level = 1;
                    WallEntry->ItemGuid = gWall;
                    WallEntry->bIsDirty = false;

                    auto FloorEntry = new SDK::FFortItemEntry;
                    FloorEntry->Count = 1;
                    FloorEntry->ItemDefinition = pFloorBuildDef;
                    FloorEntry->Durability = 1;
                    FloorEntry->Level = 1;
                    FloorEntry->ItemGuid = gFloor;
                    FloorEntry->bIsDirty = false;

                    auto RampEntry = new SDK::FFortItemEntry;
                    RampEntry->Count = 1;
                    RampEntry->ItemDefinition = pStairBuildDef;
                    RampEntry->Durability = 1;
                    RampEntry->Level = 1;
                    RampEntry->ItemGuid = gRamp;
                    RampEntry->bIsDirty = false;

                    auto ConeEntry = new SDK::FFortItemEntry;
                    ConeEntry->Count = 1;
                    ConeEntry->ItemDefinition = pRoofBuildDef;
                    ConeEntry->Durability = 1;
                    ConeEntry->Level = 1;
                    ConeEntry->ItemGuid = gCone;
                    ConeEntry->bIsDirty = false;

                    auto LightEntry = new SDK::FFortItemEntry;
                    LightEntry->Count = 1;
                    LightEntry->ItemDefinition = pLight;
                    LightEntry->Durability = 100;
                    LightEntry->Level = 1;
                    LightEntry->ItemGuid = gPickaxe;
                    LightEntry->bIsDirty = false;

                    auto MedEntry = new SDK::FFortItemEntry;
                    MedEntry->Count = 100;
                    MedEntry->ItemDefinition = pMed;
                    MedEntry->Durability = 100;
                    MedEntry->Level = 1;
                    MedEntry->ItemGuid = gPickaxe;
                    MedEntry->bIsDirty = false;

                    auto HeavyEntry = new SDK::FFortItemEntry;
                    HeavyEntry->Count = 100;
                    HeavyEntry->ItemDefinition = pHeavy;
                    HeavyEntry->Durability = 100;
                    HeavyEntry->Level = 1;
                    HeavyEntry->ItemGuid = gPickaxe;
                    HeavyEntry->bIsDirty = false;

                    auto WoodEntry = new SDK::FFortItemEntry;
                    WoodEntry->Count = 100;
                    WoodEntry->ItemDefinition = pWood;
                    WoodEntry->Durability = 100;
                    WoodEntry->Level = 1;
                    WoodEntry->ItemGuid = gPickaxe;
                    WoodEntry->bIsDirty = false;

                    auto StoneEntry = new SDK::FFortItemEntry;
                    StoneEntry->Count = 100;
                    StoneEntry->ItemDefinition = pStone;
                    StoneEntry->Durability = 100;
                    StoneEntry->Level = 1;
                    StoneEntry->ItemGuid = gPickaxe;
                    StoneEntry->bIsDirty = false;

                    auto MetalEntry = new SDK::FFortItemEntry;
                    MetalEntry->Count = 100;
                    MetalEntry->ItemDefinition = pMetal;
                    MetalEntry->Durability = 100;
                    MetalEntry->Level = 1;
                    MetalEntry->ItemGuid = gPickaxe;
                    MetalEntry->bIsDirty = false;
                
                    SDK::TArray<class SDK::UFortWorldItem*>* iteminst = &inv->ItemInstances;
                    iteminst->Count = 12;
                    iteminst->Max = 12;
                    iteminst->Data = (class SDK::UFortWorldItem**)::malloc(iteminst->Count * sizeof(SDK::UFortWorldItem*));

                    auto pTrapWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pTrapWorldItem->OwnerInventory = myInventory;
                    pTrapWorldItem->ItemEntry = *TrapEntry;
                    pTrapWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pPickaxeWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pPickaxeWorldItem->OwnerInventory = myInventory;
                    pPickaxeWorldItem->ItemEntry = *PickaxeEntry;
                    pPickaxeWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pWallWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pWallWorldItem->OwnerInventory = myInventory;
                    pWallWorldItem->ItemEntry = *WallEntry;
                    pWallWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pFloorWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pFloorWorldItem->OwnerInventory = myInventory;
                    pFloorWorldItem->ItemEntry = *FloorEntry;
                    pFloorWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pRampWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pRampWorldItem->OwnerInventory = myInventory;
                    pRampWorldItem->ItemEntry = *RampEntry;
                    pRampWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pConeWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pConeWorldItem->OwnerInventory = myInventory;
                    pConeWorldItem->ItemEntry = *ConeEntry;
                    pConeWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pLightWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pLightWorldItem->OwnerInventory = myInventory;
                    pLightWorldItem->ItemEntry = *LightEntry;
                    pLightWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pMedWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pMedWorldItem->OwnerInventory = myInventory;
                    pMedWorldItem->ItemEntry = *MedEntry;
                    pMedWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pHeavyWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pHeavyWorldItem->OwnerInventory = myInventory;
                    pHeavyWorldItem->ItemEntry = *HeavyEntry;
                    pHeavyWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pWoodWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pWoodWorldItem->OwnerInventory = myInventory;
                    pWoodWorldItem->ItemEntry = *WoodEntry;
                    pWoodWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pStoneWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pStoneWorldItem->OwnerInventory = myInventory;
                    pStoneWorldItem->ItemEntry = *StoneEntry;
                    pStoneWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    auto pMetalWorldItem = reinterpret_cast<SDK::UFortWorldItem*>(polaris::globals::StaticConstructObject_Internal(SDK::UFortWorldItem::StaticClass(), myInventory, SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                    pMetalWorldItem->OwnerInventory = myInventory;
                    pMetalWorldItem->ItemEntry = *MetalEntry;
                    pMetalWorldItem->SetOwningControllerForTemporaryItem(static_cast<SDK::AFortPlayerControllerAthena*>(polaris::globals::gpPlayerController));

                    iteminst->operator[](0) = pTrapWorldItem;
                    iteminst->operator[](1) = pPickaxeWorldItem;
                    iteminst->operator[](2) = pWallWorldItem;
                    iteminst->operator[](3) = pFloorWorldItem;
                    iteminst->operator[](4) = pRampWorldItem;
                    iteminst->operator[](5) = pConeWorldItem;
                    iteminst->operator[](6) = pLightWorldItem;
                    iteminst->operator[](7) = pMedWorldItem;
                    iteminst->operator[](8) = pHeavyWorldItem;
                    iteminst->operator[](9) = pWoodWorldItem;
                    iteminst->operator[](10) = pStoneWorldItem;
                    iteminst->operator[](11) = pMetalWorldItem;

                    pQuickBars->EnableSlot(SDK::EFortQuickBars::Primary, 0);
                    pQuickBars->ServerAddItemInternal(gWall,SDK::EFortQuickBars::Secondary,0);
                    pQuickBars->ServerAddItemInternal(gFloor,SDK::EFortQuickBars::Secondary,1);
                    pQuickBars->ServerAddItemInternal(gRamp,SDK::EFortQuickBars::Secondary,2);
                    pQuickBars->ServerAddItemInternal(gCone,SDK::EFortQuickBars::Secondary,3);
                    pQuickBars->ServerAddItemInternal(gTrap,SDK::EFortQuickBars::Secondary,4);
                    pQuickBars->ServerAddItemInternal(gPickaxe, SDK::EFortQuickBars::Primary, 0);
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