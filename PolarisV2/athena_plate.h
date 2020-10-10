#ifndef ATHENA_PLATE_H
#define ATHENA_PLATE_H

#include "peh_plate.h"

namespace polaris
{
    class AthenaPlate : public PehPlate
    {
    private:
        SDK::AFortPlayerPawn* m_pPlayerPawn;
        bool m_bIsInitialized;

    public:
        SDK::FString m_sMapName;
        SDK::FString m_sPawnName;

        void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
        void Update() override;

        void OnEnabled() override;

        void Initialize();
    };
}

#endif // !ATHENA_PLATE_H
