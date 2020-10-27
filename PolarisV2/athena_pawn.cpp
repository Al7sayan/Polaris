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
            static SDK::UCustomCharacterPart* pCharacterPartBody;
            static SDK::UCustomCharacterPart* pCharacterPartHead;
            static SDK::UCustomCharacterPart* pCharacterPartHat;

            template<typename T>
            T* FindObject(const std::string& sClassName, const std::string& sQuery)
            {
                for (int i = 0; i < SDK::UObject::GetGlobalObjects().Num(); ++i)
                {
                    auto pObject = SDK::UObject::GetGlobalObjects().GetByIndex(i);
                    if (pObject != nullptr && pObject->GetFullName().find("F_Med_Head1") == std::string::npos
                        && pObject->GetFullName().find("Helmet") == std::string::npos)
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

                // Apply customization loadout.
                ApplyCustomizationLoadout();

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
            }
            AthenaPawn::~AthenaPawn()
            {
                if (m_pPawnActor != nullptr)
                    m_pPawnActor->K2_DestroyActor();
            }

            void AthenaPawn::Update()
            {
                Pawn::Update();

                bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->bWantsToSprint;
                m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;
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
        }
    }
}