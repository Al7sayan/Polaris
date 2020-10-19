#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "program.h"

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

                if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos)
                    m_pPlayerPawn = new pawn::pawns::AthenaPawn;
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
                globals::gpPlayerController->SwitchLevel(TEXT("Athena_Terrain"));
            }

            void AthenaPlate::Initialize()
            {
                m_bIsInitialized = true;
                utilities::SDKUtils::InitSdk();
                utilities::SDKUtils::InitGlobals();

                m_pPlayerPawn = new pawn::pawns::AthenaPawn;

                // Tell the client that we are ready to start the match, this allows the loading screen to drop.
                static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController)->ServerReadyToStartMatch();
                static_cast<SDK::AGameMode*>((*globals::gpWorld)->AuthorityGameMode)->StartMatch();
            }
        }
    }
}