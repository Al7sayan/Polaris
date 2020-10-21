#include "athena_plate.h"
#include "globals.h"
#include "error_utils.h"
#include "sdk_utils.h"
#include "program.h"
#include <MinHook.h>
#include <fstream>

typedef SDK::UObject* (__fastcall* o_StaticConstructObject_Internal)(
    SDK::UClass* Class,
    SDK::UObject* InOuter,
    void* Name,
    int32_t SetFlags,
    int32_t InternalSetFlags,
    SDK::UObject* Template,
    bool  bCopyTransientsFromClassDefaults,
    void* InstanceGraph,
    bool  bAssumeTemplateIsArchetype
    );
static o_StaticConstructObject_Internal StaticConstructObject_Internal;

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            void AthenaPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
            {
                //std::cout << pFunction->GetName() << std::endl;
                if (m_pPlayerPawn != nullptr)
                    m_pPlayerPawn->ProcessEventHook(pObject, pFunction, pParams);

                if (pFunction->GetName().find("Handle Parachute Audio State") != std::string::npos && m_bIsInitialized == false)
                    Initialize();

                if (pFunction->GetName().find("ServerAttemptAircraftJump") != std::string::npos)
                    m_pPlayerPawn = new pawn::pawns::AthenaPawn;
            }
            void AthenaPlate::Update()
            {
                if (m_pPlayerPawn != nullptr)
                    m_pPlayerPawn->Update();
            }

            void AthenaPlate::OnEnabled()
            {
                globals::gpPlayerController->SwitchLevel(TEXT("Apollo_Terrain"));
            }

            void AthenaPlate::Initialize()
            {
                m_bIsInitialized = true;

                uint64_t SCOIAddr = reinterpret_cast<uint64_t>(utilities::SDKUtils::FindPattern(
                    "\x4C\x89\x44\x24\x18\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4",
                    "xxxxxxxxxxxxxxxxxxxxx????xxx????xxx????xxx"
                ));
                StaticConstructObject_Internal = (o_StaticConstructObject_Internal)(SCOIAddr);

                reinterpret_cast<joeNuts*>((*globals::gpWorld)->OwningGameInstance->LocalPlayers[0]->PlayerController)->CheatClass = SDK::UCheatManager::StaticClass();

                SDK::UCheatManager* CheatManager = reinterpret_cast<SDK::UCheatManager*>(StaticConstructObject_Internal(
                    reinterpret_cast<joeNuts*>((*globals::gpWorld)->OwningGameInstance->LocalPlayers[0]->PlayerController)->CheatClass,
                    reinterpret_cast<SDK::UObject*>((*globals::gpWorld)->OwningGameInstance->LocalPlayers[0]->PlayerController),
                    nullptr,
                    0,
                    0,
                    nullptr,
                    false,
                    nullptr,
                    false
                ));
                CheatManager->ReceiveInitCheatManager();

                reinterpret_cast<joeNuts*>((*globals::gpWorld)->OwningGameInstance->LocalPlayers[0]->PlayerController)->CheatManager = CheatManager;

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