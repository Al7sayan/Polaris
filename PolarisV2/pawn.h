#ifndef PAWN_H
#define PAWN_H

#include "SDK.hpp"

#include <windows.h>

namespace polaris
{
    namespace pawn
    {
        //!  Pawn class for a PehPlate.
        /*!
        * Used to separate Pawn logic from the a PehPlate's update loops. Call a pawn's Update and ProcessEventHook functions in a PehPlate.
        */
        class Pawn
        {
        public:
            SDK::AFortPlayerPawn* m_pPawnActor;

            virtual void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams); //!< ProcessEventHook is used to hook behavior to ProcessEvent calls.
            virtual void Update(); //!< Update should be called in a PehPlate's Update function.
        };
    }
}

#endif // !PAWN_H
