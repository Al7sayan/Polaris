#ifndef ATHENA_PLATE_H
#define ATHENA_PLATE_H

#include "peh_plate.h"
#include "athena_pawn.h"
#include "inventory.h"
#include "quickbars.h"

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
                inventory::Inventory* m_pInventory;
                inventory::quickbars* m_pQuickbars;
                bool m_bIsInitialized;
            public:
                bool m_bOnWall = false;
                bool m_bOnFloor = false;
                bool m_bOnStair = false;
                bool m_bOnRoof = false;

                void ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams) override;
                void Update() override;

                void OnEnabled() override;

                void Initialize(); //!< Initialize spawns a pawn and drops the loading screen.

                bool AreGuidsTheSame(SDK::FGuid guidA,SDK::FGuid guidB); //!< AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            };
        }
    }
}

#endif // !ATHENA_PLATE_H
