#ifndef _PROD
#ifndef _BETA

#include "farmstead_plate.h"
#include "sdk_utils.h"
#include "program.h"

namespace polaris
{
    void FarmsteadPlate::ProcessEventHook(SDK::UObject* pObject, SDK::UFunction* pFunction, PVOID pParams)
    {
        if (m_pPlayerPawn != nullptr)
            m_pPlayerPawn->ProcessEventHook(pObject, pFunction, pParams);

        if (pFunction->GetName().find("ReadyToStartMatch") != std::string::npos && m_bIsInitialized == false)
            Initialize();
    }
    void FarmsteadPlate::Update()
    {
        if (m_pPlayerPawn != nullptr)
            m_pPlayerPawn->Update();

        // TEMP: Go back to Frontend.
        if (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000)
            gpProgram->m_pMainTable->PopPlate();
    }

    void FarmsteadPlate::OnEnabled()
    {
        gpPlayerController->SwitchLevel(TEXT("Zone_Onboarding_FarmsteadFort"));
    }

    void FarmsteadPlate::Initialize()
    {
        m_bIsInitialized = true;
        SDKUtils::InitSdk();
        SDKUtils::InitGlobals();
        SDKUtils::InitPatches();

        std::string sZoneClassName = "fortgamestatezone";
        gpPlayerController->CheatManager->Summon(SDK::FString(std::wstring(sZoneClassName.begin(), sZoneClassName.end()).c_str()));

        m_pPlayerPawn = new FarmsteadPawn;

        auto pickaxe = SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>("FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_SR_T05.WID_Harvest_Pickaxe_SR_T05");
        m_pPlayerPawn->m_pPawnActor->EquipWeaponDefinition(pickaxe, SDK::FGuid());

        static_cast<SDK::AGameMode*>((*gpWorld)->AuthorityGameMode)->StartMatch();
    }
}

#endif
#endif