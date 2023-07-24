// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>


//	Health
UE_DEFINE_GAMEPLAY_TAG(FHealth_OnDeath, "Health.OnDeath")

//	Health.Cheats
UE_DEFINE_GAMEPLAY_TAG(FHealth_Cheats_GodMode, "Health.Cheats.GodMode");

//	GE
UE_DEFINE_GAMEPLAY_TAG(FGE_Damage, "GameplayEffect.Damage");
UE_DEFINE_GAMEPLAY_TAG(FGE_DamagePeriodic, "GameplayEffect.DamagePeriodic");
UE_DEFINE_GAMEPLAY_TAG(FGE_Heal, "GameplayEffect.Heal");
UE_DEFINE_GAMEPLAY_TAG(FGE_HealPeriodic, "GameplayEffect.HealPeriodic");
UE_DEFINE_GAMEPLAY_TAG(FGE_UpgradeInProgress, "GameplayEffect.UpgradeInProgress");
UE_DEFINE_GAMEPLAY_TAG(FGE_ProduceBlocked, "GameplayEffect.ProduceBlocked");
UE_DEFINE_GAMEPLAY_TAG(FGE_UnitProduce, "GameplayEffect.UnitProduce");
UE_DEFINE_GAMEPLAY_TAG(FGE_UnitProduceBlocked, "GameplayEffect.UnitProduceBlocked");
UE_DEFINE_GAMEPLAY_TAG(FGE_UnitProducePeriodic, "GameplayEffect.UnitProducePeriodic");
UE_DEFINE_GAMEPLAY_TAG(FGE_BuildingUpgradeInProgress, "GameplayEffect.BuildingUpgradeInProgress");
UE_DEFINE_GAMEPLAY_TAG(FGE_MoneyGet, "GameplayEffect.MoneyGet");
UE_DEFINE_GAMEPLAY_TAG(FGE_MoneyLost, "GameplayEffect.MoneyLost");
UE_DEFINE_GAMEPLAY_TAG(FGE_MoneyGetPeriodic, "GameplayEffect.MoneyGetPeriodic");
UE_DEFINE_GAMEPLAY_TAG(FGE_MoneyLostPeriodic, "GameplayEffect.MoneyLostPeriodic");

//	GameplayCue
UE_DEFINE_GAMEPLAY_TAG(FGC_Character_DamageTaken, "GameplayCue.Character.DamageTaken");
UE_DEFINE_GAMEPLAY_TAG(FGC_UnitShoot, "GameplayCue.UnitShoot");
UE_DEFINE_GAMEPLAY_TAG(FGC_HeliFlight, "GameplayCue.HeliFlight");

//	Weapons
UE_DEFINE_GAMEPLAY_TAG(FGE_Weapon_NoFire, "Weapon.NoFire");
UE_DEFINE_GAMEPLAY_TAG(FGE_Weapon_HasAnyWeapon, "Weapon.HasAnyWeapon");