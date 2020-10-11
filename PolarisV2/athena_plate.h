#ifndef ATHENA_PLATE_H
#define ATHENA_PLATE_H

#include "peh_plate.h"
#include "athena_pawn.h"

namespace polaris
{
    class AthenaPlate : public PehPlate
    {
    private:
        AthenaPawn* m_pPlayerPawn;
        bool m_bIsInitialized;

    public:
        void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
        void Update() override;

        void OnEnabled() override;

        void Initialize();
    };
}

#endif // !ATHENA_PLATE_H
