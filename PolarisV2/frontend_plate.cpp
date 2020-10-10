#include "frontend_plate.h"
#include "globals.h"
#include "program.h"
#include "athena_plate.h"

#include <iostream>

namespace polaris
{
    void FrontendPlate::OnEnabled()
    {
        std::cout << "We're now in the Frontend State." << std::endl;
    }

    void FrontendPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pFunction->GetName().find("BndEvt__BP_PlayButton_K2Node_ComponentBoundEvent_1_CommonButtonClicked__DelegateSignature") != std::string::npos)
        {
            AthenaPlate* athenaPlate = new AthenaPlate;
            athenaPlate->m_sMapName = TEXT("Athena_Terrain");
            athenaPlate->m_sPawnName = TEXT("PlayerPawn_Athena_C");

            gpProgram->m_pMainTable->PushPlate(athenaPlate);
        }
    }
}