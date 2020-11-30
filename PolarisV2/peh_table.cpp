#include "peh_table.h"
#include "SDK.hpp"
#include "sdk_utils.h"
#include "peh_plate.h"
#include "error_utils.h"

#include <windows.h>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

polaris::tables::PehTable* gpPehTable;

namespace polaris::tables
{
    PVOID(*ProcessEvent)(SDK::UObject*, SDK::UFunction*, PVOID) = nullptr;
    PVOID ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pObject != nullptr && pFunction != nullptr)
        {
            if (gpPehTable->m_lpPlates.size() > 0)
            {
                tables::plates::PehPlate* primaryTable = dynamic_cast<tables::plates::PehPlate*>(gpPehTable->m_lpPlates.front());

                primaryTable->Update();
                primaryTable->ProcessEventHook(pObject, pFunction, pParams);
            }
        }

        return ProcessEvent(pObject, pFunction, pParams);
    }

    PehTable::PehTable()
    {
        if (gpPehTable != nullptr)
            utilities::ErrorUtils::ThrowException(L"gpPehTable is not null.");

        gpPehTable = this;

        auto pProcessEventAddress = utilities::SDKUtils::FindPattern("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x85\xD2\x75\x30\x33\xC0\x48\x89\x01\x48\xC7\x41\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x0B\x48\x8D\x15\x00\x00\x00\x00\x41\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4\x20\x5B\xC3\x48\x8B\x42\x18", "xxxxxxxxxxxxxxxxxxxxxx?????x????xxxxxx????xx????x????xxxxxxxxxxxxx");
        if (!pProcessEventAddress)
            utilities::ErrorUtils::ThrowException(L"ProcessEvent address was not found.");

        MH_CreateHook(static_cast<LPVOID>(pProcessEventAddress), ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
        MH_EnableHook(static_cast<LPVOID>(pProcessEventAddress));
    }
}