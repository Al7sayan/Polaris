#include "base_plate.h"
#include "sdk_utils.h"
#include "program.h"
#include "frontend_plate.h"
#include "error_utils.h"
#include "ui_renderer.h"
#include "main_window.h"
#include "athena_plate.h"

namespace polaris
{
    DWORD LoadThread(LPVOID lpParam)
    {
        SDKUtils::InitSdk();

        // NOTE: We wait until this is not null. This becomes a valid pointer as soon as
        // the initial loading screen drops. From then, we can continue initializing Polaris.
        while ((*polaris::gpWorld) == nullptr)
            Sleep(1000 / 60);

        // Initialize the renderer and main window.
        std::cout << "Initializing UI." << std::endl;
        new ui::UIRenderer;
        new ui::window::windows::MainWindow;

        // NOTE: For some reason if you don't wait a bit here, everything will be nullptr.
        Sleep(500);

        utilities::SDKUtils::InitGlobals();
        gpProgram->m_pMainTable->PushPlate(new FrontendPlate);
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