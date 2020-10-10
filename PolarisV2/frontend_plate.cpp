#include "frontend_plate.h"
#include "globals.h"

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
            std::cout << "Athena should be loaded now." << std::endl;
        }
    }
}