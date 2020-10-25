#ifndef ATHENA_PLATE_H
#define ATHENA_PLATE_H

#include "peh_plate.h"
#include "athena_pawn.h"

namespace polaris
{
    namespace tables
    {
        namespace plates
        {
            //!  Manages Athena's custom behavior.
            /*!
            * This Plate is in charge of essential Athena stuff, like spawning a pawn or dropping the loading screen.
            */
            class AthenaPlate : public PehPlate
            {
            private:
                pawn::pawns::AthenaPawn* m_pPlayerPawn;
                bool m_bIsInitialized;

            public:
                void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
                void Update() override;

                void OnEnabled() override;

                void Initialize(); //!< Initialize spawns a pawn and drops the loading screen.
                void SetupInventory(); //Inventory
                void SetupQuickbars(); //Inventory
                SDK::FGuid GenerateGuid(); //generates an item guid
            };
        }
    }
}

#endif // !ATHENA_PLATE_H
