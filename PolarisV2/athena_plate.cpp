#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "program.h"
#include "build.h"

#include <MinHook.h>

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
            {
                Maybe(m_pPlayerPawn)->ProcessEventHook(pObject, pFunction, pParams);

                if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
                    Initialize();

                if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos)
                    m_pPlayerPawn = new pawn::pawns::AthenaPawn;
            }
            void AthenaPlate::Update()
            {
                Maybe(m_pPlayerPawn)->Update();

                // TEMP: Go back to Frontend.
                if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000)
                    gpProgram->m_pMainTable->PopPlate();
            }

            void AthenaPlate::OnEnabled()
            {
                Maybe(globals::gpPlayerController)->SwitchLevel(TEXT("Athena_Terrain"));
            }

            PVOID(*CollectGarbageInternal)(uint32_t, bool) = nullptr;
            PVOID CollectGarbageInternalHook(uint32_t KeepFlags, bool bPerformFullPurge)
            {
                return NULL;
            }

            void AthenaPlate::Initialize()
            {
                m_bIsInitialized = true;

                //From: Wiktor, TEMPORARY UNTIL WE PUT THIS IN A SEPERATE CLASS
                //NOTE: (irma) Wiktor, this is OK to put in Athena Plate.
                auto pCollectGarbageInternalAddress = utilities::SDKUtils::FindPattern("\x48\x8B\xC4\x48\x89\x58\x08\x88\x50\x10", "xxxxxxxxxx");
                if (!pCollectGarbageInternalAddress)
                    utilities::ErrorUtils::ThrowException(L"Finding pattern for CollectGarbageInternal has failed. Please relaunch Fortnite and try again!");

                MH_CreateHook(static_cast<LPVOID>(pCollectGarbageInternalAddress), CollectGarbageInternalHook, reinterpret_cast<LPVOID*>(&CollectGarbageInternal));
                MH_EnableHook(static_cast<LPVOID>(pCollectGarbageInternalAddress));

                utilities::SDKUtils::InitSdk();
                utilities::SDKUtils::InitGlobals();
                utilities::SDKUtils::InitPatches();

                m_pPlayerPawn = new pawn::pawns::AthenaPawn;

                // Tell the client that we are ready to start the match, this allows the loading screen to drop.
                static_cast<SDK::AFortPlayerController*>(globals::gpPlayerController)->ServerReadyToStartMatch();
                static_cast<SDK::AGameMode*>((*globals::gpWorld)->AuthorityGameMode)->StartMatch();
            }
        }
    }
}