#ifndef _PROD
#ifndef _BETA

#ifndef FARMSTEAD_PLATE_H
#define FARMSTEAD_PLATE_H

#include "peh_plate.h"
#include "farmstead_pawn.h"

namespace polaris
{
    class FarmsteadPlate : public PehPlate
    {
    private:
        FarmsteadPawn* m_pPlayerPawn;
        bool m_bIsInitialized;

    public:
        void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
        void Update() override;

        void OnEnabled() override;

        void Initialize();
    };
}

#endif // !FARMSTEAD_PLATE_H

#endif
#endif