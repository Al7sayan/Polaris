#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "program.h"

namespace polaris
{
    void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (m_pPlayerPawn != nullptr)
            m_pPlayerPawn->ProcessEventHook(pObject, pFunction, pParams);

        if (m_pEquip != nullptr)
            m_pEquip->ProcessEventHook(pObject, pFunction, pParams);

        if (pFunction->GetName().find("AircraftEnteredDropZone") != std::string::npos && m_bIsInitialized == false)
            Initialize();

        if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos && static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->IsInAircraft())
        {
            m_pPlayerPawn = new pawn::pawns::AthenaPawn(static_cast<SDK::AAthena_GameState_C*>((*globals::gpWorld)->GameState)->GetAircraft()->K2_GetActorLocation(),
                reinterpret_cast<SDK::AFortPlayerControllerAthena_ServerAttemptAircraftJump_Params*>(pParams)->ClientRotation);
            m_pEquip->m_pPlayerPawn = m_pPlayerPawn;
        }
        if (pFunction->GetName().find("AircraftExitedDropZone") != std::string::npos && static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->IsInAircraft())
        {
            m_pPlayerPawn = new pawn::pawns::AthenaPawn(static_cast<SDK::AAthena_GameState_C*>((*globals::gpWorld)->GameState)->GetAircraft()->K2_GetActorLocation(),
                SDK::FRotator{ 0,0,0 });
            m_pEquip->m_pPlayerPawn = m_pPlayerPawn;
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
        gpPlayerController->SwitchLevel(TEXT("Athena_Terrain"));
    }

    void AthenaPlate::Initialize()
    {
        m_bIsInitialized = true;
        utilities::SDKUtils::InitSdk();
        utilities::SDKUtils::InitGlobals();
        utilities::SDKUtils::InitPatches();

        // Spawn a Player Pawn and setup inventory.
        m_pPlayerPawn = new pawn::pawns::AthenaPawn(globals::gpPlayerController->SpectatorPawn->K2_GetActorLocation(),
            globals::gpPlayerController->SpectatorPawn->K2_GetActorRotation());

        // Tell the client that we are ready to start the match, this allows the loading screen to drop.
        static_cast<SDK::AFortPlayerController*>(gpPlayerController)->ServerReadyToStartMatch();
        static_cast<SDK::AGameMode*>((*gpWorld)->AuthorityGameMode)->StartMatch();
    }
}