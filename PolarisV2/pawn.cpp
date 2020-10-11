#include "pawn.h"

namespace polaris
{
    Pawn::Pawn()
    {
    }
    Pawn::~Pawn()
    {
    }

    void Pawn::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
    }
    void Pawn::Update()
    {
        if (m_pPawnActor == nullptr)
            delete this;
    }
}