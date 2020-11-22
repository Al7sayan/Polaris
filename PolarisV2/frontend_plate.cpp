#include "frontend_plate.h"
#include "globals.h"
#include "program.h"
#include "athena_plate.h"

#include <iostream>

namespace polaris::tables::plates
{
    void FrontendPlate::OnEnabled()
    {
        std::cout << "We're now in the Frontend State." << std::endl;

        if (globals::gpLevel->URL.Map.ToString() != "/Game/Maps/FortniteEntry")
            globals::gpPlayerController->SwitchLevel(TEXT("Frontend"));
    }

    void FrontendPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pFunction->GetName().find("BP_PlayButton") != std::string::npos)
            gpProgram->m_pMainTable->PushPlate(new AthenaPlate);
    }
}