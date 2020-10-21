#include "frontend_plate.h"
#include "globals.h"
#include "program.h"
#include "athena_plate.h"

#include <iostream>

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            void FrontendPlate::OnEnabled()
            {
                std::cout << "We're now in the Frontend State." << std::endl;
            }

            void FrontendPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
            {
                if (pFunction->GetName().find("SetColorAndOpacity") != std::string::npos)
                    gpProgram->m_pMainTable->PushPlate(new AthenaPlate);
            }
        }
    }
}