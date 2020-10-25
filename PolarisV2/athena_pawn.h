#ifndef ATHENA_PAWN_H
#define ATHENA_PAWN_H

#include "pawn.h"
#include <map>

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
                std::map<std::string, std::string> mPickaxeAsWid
                {
                    {"DefaultPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01"},
                    {"BoltonPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_BoltOn_Athena_C_T01.WID_Harvest_Pickaxe_BoltOn_Athena_C_T01"},
                    {"HalloweenScythe", "FortWeaponMeleeItemDefinition WID_Harvest_HalloweenScythe_Athena_C_T01.WID_Harvest_HalloweenScythe_Athena_C_T01"},
                    {"HappyPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Smiley_Athena_C_T01.WID_Harvest_Pickaxe_Smiley_Athena_C_T01"},
                    {"Pickaxe_Deathvalley", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01.WID_Harvest_Pickaxe_Deathvalley_Athena_C_T01"},
                    {"Pickaxe_Flamingo", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Flamingo_Athena_C_T01.WID_Harvest_Pickaxe_Flamingo_Athena_C_T01"},
                    {"Pickaxe_Lockjaw", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01.WID_Harvest_Pickaxe_Lockjaw_Athena_C_T01"},
                    {"SickleBatPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_SickleBat_Athena_C_T01.WID_Harvest_Pickaxe_SickleBat_Athena_C_T01"},
                    {"SkiIcePickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_IcePick_Athena_C_T01.WID_Harvest_Pickaxe_IcePick_Athena_C_T01"},
                    {"SpikyPickaxe", "FortWeaponMeleeItemDefinition WID_Harvest_Pickaxe_Spikey_Athena_C_T01.WID_Harvest_Pickaxe_Spikey_Athena_C_T01"}
                };

                AthenaPawn();
                ~AthenaPawn();

                void Update() override;
                void ApplyCustomizationLoadout(); //!< Applies skin, pickaxe and glider to pawn.
                void EquipWeapon(const char* cItemDef, int iGuid);
            };
        }
    }
}

#endif // !ATHENA_PAWN_H
