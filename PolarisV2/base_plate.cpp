#include "base_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "frontend_plate.h"
#include "error_utils.h"
#include "ui_renderer.h"
#include "main_window.h"

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            DWORD LoadThread(LPVOID lpParam)
            {
                utilities::SDKUtils::InitSdk();

                // NOTE: We wait until this is not null. This becomes a valid pointer as soon as
                // the initial loading screen drops. From then, we can continue initializing Polaris.
                while ((*globals::gpWorld) == nullptr)
                    Sleep(1000 / 60);

                // NOTE: For some reason if you don't wait a bit here, everything will be nullptr.
                Sleep(500);

                utilities::SDKUtils::InitGlobals();
                gpProgram->m_pMainTable->PushPlate(new FrontendPlate);

                // Initialize the console.
                //auto pConsole = SDK::UConsole::StaticClass()->CreateDefaultObject<SDK::UConsole>();

                //pConsole->Outer = globals::gpLocalPlayer->ViewportClient;
                //globals::gpLocalPlayer->ViewportClient->ViewportConsole = pConsole;
                return NULL;
            }

            void BasePlate::OnEnabled()
            {
                std::cout << "We're now in the Base State.\nWaiting for game to load..." << std::endl;

                // Check the NOTE in LoadThread to see the reasoning as to
                // why this is in its separate thread.
                CreateThread(0, 0, LoadThread, 0, 0, 0);
            }
        }
    }
}