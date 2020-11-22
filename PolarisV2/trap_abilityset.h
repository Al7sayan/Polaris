#ifndef TRAP_ABILITYSET_H
#define TRAP_ABILITYSET_H

#include "SDK.hpp"

namespace polaris::models::offsetfixes
{
    struct TrapAbilitySet
    {
        unsigned char UnknownData00[0x0030];
        SDK::TArray<class SDK::UClass*> GameplayAbilities;
    };
}

#endif // !TRAP_ABILITYSET_H
