#include "base_plate.h"
#include "util.h"

#include <iostream>

#include "program.h"

namespace polaris
{
    void BasePlate::OnEnabled()
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

        polaris::Globals::gpLevel = (*polaris::Globals::gpWorld)->PersistentLevel;

        polaris::Globals::gpGameInstance = (*polaris::Globals::gpWorld)->OwningGameInstance;

        polaris::Globals::gpLocalPlayers = polaris::Globals::gpGameInstance->LocalPlayers;
        polaris::Globals::gpLocalPlayer = polaris::Globals::gpLocalPlayers[0];

        polaris::Globals::gpPlayerController = polaris::Globals::gpLocalPlayer->PlayerController;

        // HERE GOES ADDING THE FRONTEND PLATE!!
    }
}