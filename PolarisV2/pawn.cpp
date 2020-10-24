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
            {
                delete this;
                return;
            }

            if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            {
                if (m_bHasJumped == false)
                {
                    m_bHasJumped = true;
                    m_pPawnActor->Jump();
                }
            }
            else
                m_bHasJumped = false;
        }
    }
}