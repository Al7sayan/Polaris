#ifndef FRONTEND_PLATE_H
#define FRONTEND_PLATE_H

#include "peh_plate.h"

namespace polaris
{
    //!  The main Frontend PehPlate.
    /*!
    * Controls basic interaction like entering a match.
    */
    class FrontendPlate : public PehPlate
    {
    public:
        void OnEnabled() override;

        void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
    };
}

#endif // !FRONTEND_PLATE_H
