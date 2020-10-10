#include "base_plate.h"
#include "util.h"
#include "program.h"
#include "frontend_plate.h"

namespace polaris
{
    DWORD LoadThread(LPVOID lpParam)
    {
        Util::InitSdk();

        // NOTE: This does not seem to crash while loading, unlike putting it in util.
        uintptr_t pBaseAddress = Util::BaseAddress();
        if (!pBaseAddress)
        {
            MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        if (!polaris::Globals::gpWorld)
        {
            MessageBoxA(NULL, "An unknown error has occured. Please relaunch Fortnite and try again!", "Error", MB_ICONERROR);
            ExitProcess(EXIT_FAILURE);
        }

        // NOTE: We wait until this is not null. This becomes a valid pointer as soon as
        // the initial loading screen drops. From then, we can continue initializing Polaris.
        while ((*polaris::Globals::gpWorld) == nullptr)
            Sleep(1000 / 60);

        polaris::Globals::gpLevel = (*polaris::Globals::gpWorld)->PersistentLevel;
        polaris::Globals::gpGameInstance = (*polaris::Globals::gpWorld)->OwningGameInstance;
        polaris::Globals::gpLocalPlayers = polaris::Globals::gpGameInstance->LocalPlayers;
        polaris::Globals::gpLocalPlayer = polaris::Globals::gpLocalPlayers[0];
        polaris::Globals::gpPlayerController = polaris::Globals::gpLocalPlayer->PlayerController;

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