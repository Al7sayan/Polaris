#include "peh_table.h"
#include "SDK.hpp"
#include "sdk_utils.h"
#include "peh_plate.h"
#include "error_utils.h"

#include <windows.h>
#include <MinHook.h>
#pragma comment(lib, "libMinHook.x64.lib")

polaris::PehTable* gpPehTable;

namespace polaris
{
    PVOID(*ProcessEvent)(SDK::UObject*, SDK::UFunction*, PVOID) = nullptr;
    PVOID ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (pObject != nullptr && pFunction != nullptr)
        {
            if (gpPehTable->m_lpPlates.size() > 0)
            {
                PehPlate* primaryTable = dynamic_cast<PehPlate*>(gpPehTable->m_lpPlates.front());

                primaryTable->Update();
                primaryTable->ProcessEventHook(pObject, pFunction, pParams);
            }
        }

        return ProcessEvent(pObject, pFunction, pParams);
    }

    PehTable::PehTable()
    {
        if (gpPehTable != nullptr)
            ErrorUtils::ThrowException(L"gpPehTable is not null.");

        gpPehTable = this;

        auto pProcessEventAddress = SDKUtils::FindPattern("\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8D\x6C\x24\x00\x48\x89\x9D\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC5\x48\x89\x85\x00\x00\x00\x00\x48\x63\x41\x0C", "xxxxxxxxxxxxxxx????xxxx?xxx????xxx????xxxxxx????xxxx");
        if (!pProcessEventAddress)
            ErrorUtils::ThrowException(L"ProcessEvent address was not found.");

        MH_CreateHook(static_cast<LPVOID>(pProcessEventAddress), ProcessEventHook, reinterpret_cast<LPVOID*>(&ProcessEvent));
        MH_EnableHook(static_cast<LPVOID>(pProcessEventAddress));
    }
}