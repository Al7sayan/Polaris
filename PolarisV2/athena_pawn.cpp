#include "athena_pawn.h"
#include "globals.h"
#include "sdk_utils.h"
#include "globals.h"

namespace polaris
{
    AthenaPawn::AthenaPawn()
    {
        globals::gpPlayerController->CheatManager->Summon(TEXT("PlayerPawn_Athena_C"));
        m_pPawnActor = static_cast<SDK::AFortPlayerPawn*>(utilities::SDKUtils::FindActor(SDK::AFortPlayerPawn::StaticClass()));

        //Reset the pawn's actor rotation.
        SDK::FRotator actorRotation = m_pPawnActor->K2_GetActorRotation();
        actorRotation.Pitch = 0;
        actorRotation.Roll = 0;

        m_pPawnActor->K2_SetActorRotation(actorRotation, true);
        globals::gpPlayerController->Possess(m_pPawnActor);

        auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState);
        playerState->TeamIndex = SDK::EFortTeam::HumanPvP_Team69; // funny number go brrrrrr
        playerState->OnRep_PlayerTeam();

        auto pawn = static_cast<SDK::AFortPlayerPawnAthena*>(m_pPawnActor);
        pawn->OnRep_CustomizationLoadout();

        EquipWeapon("FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01", 0);

        // Apply customization loadout.
        ApplyCustomizationLoadout();

        // Destroy the HLOD actors.
        globals::gpPlayerController->CheatManager->DestroyAll(SDK::AFortHLODSMActor::StaticClass());
    }
    AthenaPawn::~AthenaPawn()
    {
        if (m_pPawnActor != nullptr)
            m_pPawnActor->K2_DestroyActor();
    }

    void AthenaPawn::Update()
    {
        Pawn::Update();

        // Sprinting keybind
        bool wantsToSprint = static_cast<SDK::AFortPlayerControllerAthena*>(globals::gpPlayerController)->bWantsToSprint;
        if (m_bSprint == false)
        {
            m_bSprint = true;
            m_pPawnActor->CurrentMovementStyle = wantsToSprint ? SDK::EFortMovementStyle::Sprinting : SDK::EFortMovementStyle::Running;
        }
        else
            m_bSprint = false;

        // Glider redeploy keybind
        auto athenaPawn = static_cast<SDK::AFortPlayerPawnAthena*>(m_pPawnActor);
        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            // We do this hacky bool check to avoid the holding button issue.
            if (m_bTryingToDeployGlider == false)
            {
                m_bTryingToDeployGlider = true;
                if (athenaPawn->IsSkydiving() && !athenaPawn->IsParachuteOpen() && !athenaPawn->IsParachuteForcedOpen())
                    athenaPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Custom, 2U);
                else if (athenaPawn->IsParachuteOpen() && !athenaPawn->IsParachuteForcedOpen())
                    athenaPawn->CharacterMovement->SetMovementMode(SDK::EMovementMode::MOVE_Custom, 3U);

                athenaPawn->OnRep_IsParachuteOpen(athenaPawn->IsParachuteOpen());
            }
        }
        else
            m_bTryingToDeployGlider = false;
    }
    void AthenaPawn::ApplyCustomizationLoadout()
    {
        auto playerState = static_cast<SDK::AFortPlayerStateAthena*>(globals::gpPlayerController->PlayerState);

        playerState->CharacterParts[0] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Head1.F_Med_Head1");
        playerState->CharacterParts[1] = SDK::UObject::FindObject<SDK::UCustomCharacterPart>("CustomCharacterPart F_Med_Soldier_01.F_Med_Soldier_01");
        playerState->OnRep_CharacterParts();
    }
    void AthenaPawn::EquipWeapon(const char* cItemDef, int iGuid)
    {
        SDK::FGuid guid;
        guid.A = iGuid;
        guid.B = iGuid;
        guid.C = iGuid;
        guid.D = iGuid;

        m_pPawnActor->EquipWeaponDefinition(SDK::UObject::FindObject<SDK::UFortWeaponItemDefinition>(cItemDef), guid)->SetOwner(m_pPawnActor);
    }
}