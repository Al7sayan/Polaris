#include "athena_pawn.h"
#include "globals.h"
#include "sdk_utils.h"

namespace polaris
{
    AthenaPawn::AthenaPawn()
    {
        gpPlayerController->CheatManager->Summon(TEXT("PlayerPawn_Athena_C"));
        m_pPawnActor = static_cast<SDK::AFortPlayerPawn*>(SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
        gpPlayerController->Possess(m_pPawnActor);

        // Load Ramirez onto the pawn and replicate character parts.
        auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(gpPlayerController->PlayerState);
        playerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
        playerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
        playerState->OnRep_CharacterParts();
        m_pPawnActor->OnCharacterPartsReinitialized();

        //Reset the pawn's actor rotation.
        SDK::FRotator actorRotation = m_pPawnActor->K2_GetActorRotation();
        actorRotation.Pitch = 0;
        actorRotation.Roll = 0;

        m_pPawnActor->K2_SetActorLocationAndRotation(m_pPawnActor->K2_GetActorLocation(), actorRotation, false, true, new SDK::FHitResult());
    }
    AthenaPawn::~AthenaPawn()
    {
        if (m_pPawnActor != nullptr)
            m_pPawnActor->K2_DestroyActor();
    }

    void AthenaPawn::Update()
    {
        Pawn::Update();

        bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(gpPlayerController)->bWantsToSprint;
        m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;
    }
}