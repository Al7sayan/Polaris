#include "base_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "frontend_plate.h"
#include "error_utils.h"

namespace polaris
{
    DWORD LoadThread(LPVOID lpParam)
    {
        SDKUtils::InitSdk();

        uintptr_t pBaseAddress = SDKUtils::BaseAddress();
        if (!pBaseAddress)
            ErrorUtils::ThrowException(L"BaseAddress was not found.");

        // NOTE: We wait until this is not null. This becomes a valid pointer as soon as
        // the initial loading screen drops. From then, we can continue initializing Polaris.
        while ((*polaris::gpWorld) == nullptr)
            Sleep(1000 / 60);

        // NOTE: For some reason if you don't wait a bit here, everything will be nullptr.
        Sleep(300);

        polaris::gpLevel = (*polaris::gpWorld)->PersistentLevel;
        polaris::gpGameInstance = (*polaris::gpWorld)->OwningGameInstance;
        polaris::gpLocalPlayers = polaris::gpGameInstance->LocalPlayers;
        polaris::gpLocalPlayer = polaris::gpLocalPlayers[0];
        polaris::gpActors = &polaris::gpLevel->Actors;
        polaris::gpPlayerController = polaris::gpLocalPlayer->PlayerController;

        gpProgram->m_pMainTable->PushPlate(new FrontendPlate);
    }

    void BasePlate::OnEnabled()
    {
        std::cout << "We're now in the Base State.\nWaiting for game to load..." << std::endl;

        // Check the NOTE in LoadThread to see the reasoning as to
        // why this is in its separate thread.
        CreateThread(0, 0, LoadThread, 0, 0, 0);
    }
}