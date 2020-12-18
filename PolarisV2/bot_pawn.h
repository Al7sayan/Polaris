#ifndef BOT_PAWN_H
#define BOT_PAWN_H

#include "pawn.h"
#include <map>

namespace polaris
{
    namespace pawn
    {
        namespace pawns
        {
            //!  Player Pawn class for an Bot pawn.
            class BotPawn : public Pawn
            {
            private:
                SDK::FVector m_pPos;
                SDK::FRotator m_pRot;
                int m_iHealth = 100;
                std::string m_sName = ""; 
                SDK::TEnumAsByte<SDK::EFortTeam> m_pTeam; 
                SDK::UFortWeaponItemDefinition* m_pWep = nullptr; 
                SDK::AFortPlayerController* m_pControllerToGetProfile = nullptr;
                SDK::AFortInventory* m_pInventory = nullptr;
                bool m_bIsFiring = false;
                
            public:
                BotPawn(SDK::FVector pos, SDK::FRotator rot, std::string name, int health,SDK::TEnumAsByte<SDK::EFortTeam> Team, SDK::UFortWeaponItemDefinition* wep, SDK::AFortPlayerController* controllerToGetProfile, SDK::AFortInventory* inventory, bool isFiring);
                ~BotPawn();

                SDK::AFortPlayerPawnAthena* m_pBotPawnActor;
                SDK::AFortPlayerControllerAthena* m_pBotPawnController;
                SDK::AFortPlayerStateAthena* m_pPlayerState;

                bool m_bHasConstructed = false;

                void Construct();
                void EquipWeapon(const char* cItemDef, int iGuid);
                void ApplyCustomizationLoadout();
            };
        }
    }
}

#endif // !BOT_PAWN_H
