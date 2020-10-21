#include "athena_pawn.h"
#include "globals.h"
#include "sdk_utils.h"
#include "athena_plate.h"

namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            AthenaPawn::AthenaPawn()
            {
                reinterpret_cast<joeNuts*>((*globals::gpWorld)->OwningGameInstance->LocalPlayers[0]->PlayerController)->CheatManager->Summon(TEXT("PlayerPawn_Athena_C"));

                for (int i = 0; i < (*globals::gpWorld)->PersistentLevel->Actors.Num(); i++)
                {
                    SDK::AActor* pActor = (*globals::gpWorld)->PersistentLevel->Actors[i];

                    if (pActor != nullptr)
                    {
                        if (pActor->IsA(SDK::AFortPlayerPawnAthena::StaticClass()))
                        {
                            m_pPawnActor = static_cast<SDK::AFortPlayerPawn*>(pActor);
                            (*globals::gpWorld)->OwningGameInstance->LocalPlayers[0]->PlayerController->Possess(reinterpret_cast<SDK::APawn*>(pActor));
                            break;
                        }
                    }
                }

                static_cast<SDK::AFortPlayerStateAthena*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerStateAthena::StaticClass()))->CharacterData.Parts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart CP_Body_Commando_F_RebirthDefaultA.CP_Body_Commando_F_RebirthDefaultA");
                static_cast<SDK::AFortPlayerStateAthena*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerStateAthena::StaticClass()))->CharacterData.Parts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart CP_Head_F_RebirthDefaultA.CP_Head_F_RebirthDefaultA");
                static_cast<SDK::AFortPlayerStateAthena*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerStateAthena::StaticClass()))->OnRep_CharacterData();
            }
            AthenaPawn::~AthenaPawn()
            {
                if (m_pPawnActor != nullptr)
                    m_pPawnActor->K2_DestroyActor();
            }

            void AthenaPawn::Update()
            {
                Pawn::Update();

                bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->bHoldingSprint;
                m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;
            }
        }
    }
}