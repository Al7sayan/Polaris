#ifndef TRAPBUILD_H
#define TRAPBUILD_H

#include "SDK.hpp"

namespace polaris
{
    namespace models
    {
        namespace offsetfixes
        {
            struct TrapBuild
            {
                unsigned char UnknownData00[0x0FE0];
                class SDK::UFortTrapItemDefinition* TrapData;
            };
        }
    }
}

#endif // !TRAPBUILD_H
