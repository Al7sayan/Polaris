#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"

namespace polaris
{
    void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
        {
            Initialize();
        }
    }
    void AthenaPlate::Update()
    {

    }

    void AthenaPlate::OnEnabled()
    {
        gpPlayerController->SwitchLevel(m_sMapName);
    }

    void AthenaPlate::Initialize()
    {
        m_bIsInitialized = true;
        SDKUtils::InitSdk();

        // TODO: Make this a function of some kind
        polaris::gpLevel = (*polaris::gpWorld)->PersistentLevel;
        polaris::gpGameInstance = (*polaris::gpWorld)->OwningGameInstance;
        polaris::gpLocalPlayers = polaris::gpGameInstance->LocalPlayers;
        polaris::gpLocalPlayer = polaris::gpLocalPlayers[0];
        polaris::gpActors = &polaris::gpLevel->Actors;
        polaris::gpPlayerController = polaris::gpLocalPlayer->PlayerController;

        //gpPlayerController->CheatManager->Summon(m_sPawnName);
        //m_pPlayerPawn = static_cast<SDK::AFortPlayerPawn*>(SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));
        //gpPlayerController->Possess(m_pPlayerPawn);

        // Tell the client that we are ready to start the match, this allows the loading screen to drop.
        static_cast<SDK::AFortPlayerController*>(gpPlayerController)->ServerReadyToStartMatch();
        static_cast<SDK::AGameMode*>((*gpWorld)->AuthorityGameMode)->StartMatch();
    }
}