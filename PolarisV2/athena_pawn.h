#ifndef ATHENA_PAWN_H
#define ATHENA_PAWN_H

#include <iostream>
#include <fstream>

#include "pawn.h"

namespace polaris
{
    class AthenaPawn : public Pawn
    {
    public:
        AthenaPawn();
        ~AthenaPawn();

        void Update() override;
    };
}

#endif // !ATHENA_PAWN_H
