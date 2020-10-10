#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"

namespace polaris
{
    void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
            Initialize();
    }
    void AthenaPlate::Update()
    {
        if (m_bIsInitialized == false || m_pPlayerPawn == nullptr)
            return;

        bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(gpPlayerController)->bWantsToSprint;
        m_pPlayerPawn->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;
    }

    void AthenaPlate::OnEnabled()
    {
        gpPlayerController->SwitchLevel(TEXT("Athena_Terrain"));
    }

    void AthenaPlate::Initialize()
    {
        m_bIsInitialized = true;
        SDKUtils::InitSdk();
        SDKUtils::InitGlobals();

        gpPlayerController->CheatManager->Summon(TEXT("PlayerPawn_Athena_C"));
        m_pPlayerPawn = static_cast<SDK::AFortPlayerPawn*>(SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
        gpPlayerController->Possess(m_pPlayerPawn);

        // Load Ramirez onto the pawn and replicate character parts.
        auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(gpPlayerController->PlayerState);
        playerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
        playerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
        playerState->OnRep_CharacterParts();
        m_pPlayerPawn->OnCharacterPartsReinitialized();

        // Tell the client that we are ready to start the match, this allows the loading screen to drop.
        static_cast<SDK::AFortPlayerController*>(gpPlayerController)->ServerReadyToStartMatch();
        static_cast<SDK::AGameMode*>((*gpWorld)->AuthorityGameMode)->StartMatch();
    }
}