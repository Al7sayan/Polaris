#ifndef ATHENA_PAWN_H
#define ATHENA_PAWN_H

#include "pawn.h"

namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            //!  Player Pawn class for an AthenaPlate.
            class AthenaPawn : public Pawn
            {
            public:
                AthenaPawn();
                ~AthenaPawn();

                void Update() override;
            };
        }
    }
}

#endif // !ATHENA_PAWN_H
