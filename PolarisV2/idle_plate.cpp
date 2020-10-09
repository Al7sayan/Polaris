#include "idle_plate.h"

#include <iostream>

namespace polaris
{
    void IdlePlate::OnEnabled()
    {
        std::cout << "Welcome to Polaris!" << std::endl;
    }
}