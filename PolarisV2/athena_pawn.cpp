#include "athena_pawn.h"
#include "globals.h"
#include "sdk_utils.h"

struct AFortAsQuickBars
{
public:
    unsigned char                                      UnknownData00[0x1A88];
    class SDK::AFortQuickBars* QuickBars;
};
struct AFortAsBuildPreviewMID
{
public:
    unsigned char UnknownData00[0x1928];
    class SDK::UMaterialInstanceDynamic* BuildPreviewMarkerMID;
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
SDK::UStaticMesh* m_pStaticRoof;
SDK::UStaticMesh* m_pStaticWall;
SDK::UStaticMesh* m_pStaticFloor;
SDK::UBuildingEditModeMetadata_Wall* m_pMetadataWall;
SDK::UBuildingEditModeMetadata_Roof* m_pMetadataRoof;
SDK::UBuildingEditModeMetadata_Stair* m_pMetadataStair;
SDK::UBuildingEditModeMetadata_Floor* m_pMetadataFloor;
SDK::UStaticMesh* m_pStaticStair;
SDK::UFortWeaponItemDefinition* m_pEditToolDef;
SDK::UFortWeaponItemDefinition* m_pWallBuildDef;
SDK::UFortWeaponItemDefinition* m_pFloorBuildDef;
SDK::UFortWeaponItemDefinition* m_pStairBuildDef;
SDK::UFortWeaponItemDefinition* m_pRoofBuildDef;
bool bLoad_defs = false;
namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            static SDK::UCustomCharacterPart* pCharacterPartBody;
            static SDK::UCustomCharacterPart* pCharacterPartHead;
            static SDK::UCustomCharacterPart* pCharacterPartHat;

            template<typename T>
            T* FindObject(const std::string& sClassName, const std::string& sQuery)
            {
                for (int i = 0; i < SDK::UObject::GetGlobalObjects().Num(); ++i)
                {
                    auto pObject = SDK::UObject::GetGlobalObjects().GetByIndex(i);
                    if (pObject != nullptr && pObject->GetFullName().find("F_Med_Head1") == std::string::npos)
                    {
                        if (pObject->GetFullName().rfind(sClassName, 0) == 0 && pObject->GetFullName().find(sQuery) != std::string::npos)
                            return static_cast<T*>(pObject);
                    }
                }

                return nullptr;
            }

            AthenaPawn::AthenaPawn()
            {
                globals::gpPlayerController->CheatManager->Summon(TEXT("PlayerPawn_Athena_C"));
                m_pPawnActor = static_cast<SDK::AFortPlayerPawn*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
                globals::gpPlayerController->Possess(m_pPawnActor);

                auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState);
                playerState->TeamIndex = SDK::EFortTeam::HumanPvP_Team69; // funny number go brrrrrr
                playerState->OnRep_TeamIndex();

                //Reset the pawn's actor rotation.
                SDK::FRotator actorRotation = m_pPawnActor->K2_GetActorRotation();
                actorRotation.Pitch = 0;
                actorRotation.Roll = 0;

                m_pPawnActor->K2_SetActorLocationAndRotation(m_pPawnActor->K2_GetActorLocation(), actorRotation, false, true, new SDK::FHitResult());

                auto pawn = static_cast<SDK::AFortPlayerPawnAthena*>(m_pPawnActor);
                pawn->OnRep_CustomizationLoadout();

                // Give the player a pickaxe.
                EquipWeapon(mPickaxeAsWid[pawn->CustomizationLoadout.Character->GetName()].c_str(), 0);
                CreateBuildPreviews();

                // Apply customization loadout.
                ApplyCustomizationLoadout();
            }
            AthenaPawn::~AthenaPawn()
            {
                if (m_pPawnActor != nullptr)
                    m_pPawnActor->K2_DestroyActor();
            }

            void AthenaPawn::Update()
            {
                Pawn::Update();

                // Sprinting keybind
                bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->bWantsToSprint;
                m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;

                // Glider redeploy keybind
                auto athenaPawn = static_cast<SDK::AFortPlayerPawnAthena*>(m_pPawnActor);
                
                if (GetAsyncKeyState(VK_SPACE) & 0x8000)
                {
                    // We do this hacky bool check to avoid the holding button issue.
                    if (m_bTryingToDeployGlider == false)
                    {
                        m_bTryingToDeployGlider = true;

                        if (athenaPawn->IsSkydiving() && !athenaPawn->IsParachuteOpen() && !athenaPawn->IsParachuteForcedOpen())
                            athenaPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Custom, 2U);
                        else if (athenaPawn->IsParachuteOpen() && !athenaPawn->IsParachuteForcedOpen())
                            athenaPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Custom, 3U);

                        athenaPawn->OnRep_IsParachuteOpen(athenaPawn->IsParachuteOpen());
                    }
                }
                else
                    m_bTryingToDeployGlider = false;
            }
            void AthenaPawn::ApplyCustomizationLoadout()
            {
                auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState);

                if (!pCharacterPartHead)
                    pCharacterPartHead = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Head");
                if (!pCharacterPartBody)
                    pCharacterPartBody = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Body");
                if (!pCharacterPartHat)
                    pCharacterPartHat = FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart", "Hat_");

                // Assign custom character parts to the player.
                playerState->CharacterParts[0] = pCharacterPartHead;
                playerState->CharacterParts[1] = pCharacterPartBody;
                playerState->CharacterParts[3] = pCharacterPartHat;

                // If no head was found, force Ramirez's head.
                if (!playerState->CharacterParts[0])
                    playerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");

                // If no body was found, force Ramirez's body.
                if (!playerState->CharacterParts[1])
                    playerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");

                playerState->OnRep_CharacterParts();
            }
            void AthenaPawn::EquipWeapon(const char* cItemDef, int iGuid)
            {
                SDK::FGuid guid;
                guid.A = iGuid;
                guid.B = iGuid;
                guid.C = iGuid;
                guid.D = iGuid;

                m_pPawnActor->EquipWeaponDefinition(SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>(cItemDef), guid)->SetOwner(m_pPawnActor);
            }
            void AthenaPawn::CreateBuildPreviews()
            {
                m_pEditToolDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortEditToolItemDefinition EditTool.EditTool");
                m_pWallBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Wall.BuildingItemData_Wall");
                m_pFloorBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Floor.BuildingItemData_Floor");
                m_pStairBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_Stair_W.BuildingItemData_Stair_W");
                m_pRoofBuildDef = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortBuildingItemDefinition BuildingItemData_RoofS.BuildingItemData_RoofS");
                SDK::AFortPlayerController* playerController = static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController);
                playerController->CheatManager->Summon(TEXT("BuildingPlayerPrimitivePreview"));
                pBuildPreviewRoof = static_cast<SDK::ABuildingPlayerPrimitivePreview*>(polaris::utilities::SDKUtils::FindActor(SDK::ABuildingPlayerPrimitivePreview::StaticClass()));
                auto pBuildingEditSupportRoof = reinterpret_cast<SDK::UBuildingEditModeSupport_Roof*>(globals::StaticConstructObject_Internal(SDK::UBuildingEditModeSupport_Roof::StaticClass(), (*globals::gpWorld), SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                pBuildingEditSupportRoof->Outer = pBuildPreviewRoof;

                pBuildPreviewRoof->EditModeSupport = pBuildingEditSupportRoof;

                auto pComponent = pBuildPreviewRoof->GetBuildingMeshComponent();

                if (!m_pStaticRoof)
                {
                    m_pStaticRoof = SDK::UObject::FindObject<SDK::UStaticMesh>("StaticMesh PBW_W1_RoofC.PBW_W1_RoofC");
                }

                pComponent->SetStaticMesh(m_pStaticRoof);
                pComponent->SetMaterial(0, reinterpret_cast<AFortAsBuildPreviewMID*>(globals::gpPlayerController)->BuildPreviewMarkerMID);

                pBuildPreviewRoof->BuildingType = SDK::EFortBuildingType::Roof;

                if (!m_pMetadataRoof)
                {
                    m_pMetadataRoof = SDK::UObject::FindObject<SDK::UBuildingEditModeMetadata_Roof>("BuildingEditModeMetadata_Roof EMP_Roof_RoofC.EMP_Roof_RoofC");
                }

                pBuildPreviewRoof->EditModePatternData = m_pMetadataRoof;
                pBuildPreviewRoof->EditModeSupportClass = SDK::UBuildingEditModeSupport_Roof::StaticClass();
                pBuildPreviewRoof->OnBuildingActorInitialized(SDK::EFortBuildingInitializationReason::PlacementTool, SDK::EFortBuildingPersistentState::New);

                
                playerController->CheatManager->Summon(TEXT("BuildingPlayerPrimitivePreview"));
                pBuildPreviewStair = static_cast<SDK::ABuildingPlayerPrimitivePreview*>(polaris::utilities::SDKUtils::FindActor(SDK::ABuildingPlayerPrimitivePreview::StaticClass(),1));
                auto pBuildingEditSupportStair = reinterpret_cast<SDK::UBuildingEditModeSupport_Stair*>(globals::StaticConstructObject_Internal(SDK::UBuildingEditModeSupport_Stair::StaticClass(), (*globals::gpWorld), SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                pBuildingEditSupportStair->Outer = pBuildPreviewStair;

                pBuildPreviewStair->EditModeSupport = pBuildingEditSupportStair;

                auto pComponent1 = pBuildPreviewStair->GetBuildingMeshComponent();

                if (!m_pStaticStair)
                {
                    m_pStaticStair = SDK::UObject::FindObject<SDK::UStaticMesh>("StaticMesh PBW_W1_StairW.PBW_W1_StairW");
                }

                pComponent1->SetStaticMesh(m_pStaticStair);
                pComponent1->SetMaterial(0, reinterpret_cast<AFortAsBuildPreviewMID*>(globals::gpPlayerController)->BuildPreviewMarkerMID);

                pBuildPreviewStair->BuildingType = SDK::EFortBuildingType::Stairs;

                if (!m_pMetadataStair)
                {
                    m_pMetadataStair = SDK::UObject::FindObject<SDK::UBuildingEditModeMetadata_Stair>("BuildingEditModeMetadata_Stair EMP_Stair_StairW.EMP_Stair_StairW");
                }

                pBuildPreviewStair->EditModePatternData = m_pMetadataStair;
                pBuildPreviewStair->EditModeSupportClass = SDK::UBuildingEditModeSupport_Stair::StaticClass();
                pBuildPreviewStair->OnBuildingActorInitialized(SDK::EFortBuildingInitializationReason::PlacementTool, SDK::EFortBuildingPersistentState::New);

                
                playerController->CheatManager->Summon(TEXT("BuildingPlayerPrimitivePreview"));
                pBuildPreviewFloor = static_cast<SDK::ABuildingPlayerPrimitivePreview*>(polaris::utilities::SDKUtils::FindActor(SDK::ABuildingPlayerPrimitivePreview::StaticClass(),2));
                auto pBuildingEditSupportFloor = reinterpret_cast<SDK::UBuildingEditModeSupport_Floor*>(globals::StaticConstructObject_Internal(SDK::UBuildingEditModeSupport_Floor::StaticClass(), (*globals::gpWorld), SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                pBuildingEditSupportFloor->Outer = reinterpret_cast<AFortAsBuildPreview*>(globals::gpPlayerController)->BuildPreviewMarker;

                pBuildPreviewFloor->EditModeSupport = pBuildingEditSupportFloor;

                auto pComponent2 = pBuildPreviewFloor->GetBuildingMeshComponent();

                if (!m_pStaticFloor)
                {
                    m_pStaticFloor = SDK::UObject::FindObject<SDK::UStaticMesh>("StaticMesh PBW_W1_Floor.PBW_W1_Floor");
                }

                pComponent2->SetStaticMesh(m_pStaticFloor);
                pComponent2->SetMaterial(0, reinterpret_cast<AFortAsBuildPreviewMID*>(globals::gpPlayerController)->BuildPreviewMarkerMID);

                pBuildPreviewFloor->BuildingType = SDK::EFortBuildingType::Floor;

                if (!m_pMetadataFloor)
                {
                    m_pMetadataFloor = SDK::UObject::FindObject<SDK::UBuildingEditModeMetadata_Floor>("BuildingEditModeMetadata_Floor EMP_Floor_Floor.EMP_Floor_Floor");
                }

                pBuildPreviewFloor->EditModePatternData = m_pMetadataFloor;
                pBuildPreviewFloor->EditModeSupportClass = SDK::UBuildingEditModeSupport_Floor::StaticClass();
                pBuildPreviewFloor->OnBuildingActorInitialized(SDK::EFortBuildingInitializationReason::PlacementTool, SDK::EFortBuildingPersistentState::New);

                playerController->CheatManager->Summon(TEXT("BuildingPlayerPrimitivePreview"));
                pBuildPreviewWall = static_cast<SDK::ABuildingPlayerPrimitivePreview*>(polaris::utilities::SDKUtils::FindActor(SDK::ABuildingPlayerPrimitivePreview::StaticClass(),3));
                auto pBuildingEditSupportWall = reinterpret_cast<SDK::UBuildingEditModeSupport_Wall*>(globals::StaticConstructObject_Internal(SDK::UBuildingEditModeSupport_Wall::StaticClass(), (*globals::gpWorld), SDK::FName("None"), 0, SDK::FUObjectItem::ObjectFlags::None, NULL, false, NULL, false));
                pBuildingEditSupportWall->Outer = pBuildPreviewWall;

                pBuildPreviewWall->EditModeSupport = pBuildingEditSupportWall;

                auto pComponent3 = pBuildPreviewWall->GetBuildingMeshComponent();

                if (!m_pStaticWall)
                {
                    m_pStaticWall = SDK::UObject::FindObject<SDK::UStaticMesh>("StaticMesh PBW_W1_Solid.PBW_W1_Solid");
                }

                pComponent3->SetStaticMesh(m_pStaticWall);
                pComponent3->SetMaterial(0, reinterpret_cast<AFortAsBuildPreviewMID*>(globals::gpPlayerController)->BuildPreviewMarkerMID);

                pBuildPreviewWall->BuildingType = SDK::EFortBuildingType::Wall;

                if (!m_pMetadataWall)
                {
                    m_pMetadataWall = SDK::UObject::FindObject<SDK::UBuildingEditModeMetadata_Wall>("BuildingEditModeMetadata_Wall EMP_Wall_Solid.EMP_Wall_Solid");
                }

                pBuildPreviewWall->EditModePatternData = m_pMetadataWall;
                pBuildPreviewWall->EditModeSupportClass = SDK::UBuildingEditModeSupport_Wall::StaticClass();
                pBuildPreviewWall->OnBuildingActorInitialized(SDK::EFortBuildingInitializationReason::PlacementTool, SDK::EFortBuildingPersistentState::New);
                pBuildPreviewWall->SetActorHiddenInGame(true);
                pBuildPreviewFloor->SetActorHiddenInGame(true);
                pBuildPreviewStair->SetActorHiddenInGame(true);
                pBuildPreviewRoof->SetActorHiddenInGame(true);
            }
        }
    }
}