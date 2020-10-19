#include "pawn.h"

namespace polaris
{
    namespace pawn
    {
        void Pawn::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
        {
        }
        void Pawn::Update()
        {
            if (m_pPawnActor == nullptr)
                delete this;
        }
    }
}