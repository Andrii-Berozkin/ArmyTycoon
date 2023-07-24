// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/UnitProducePeriodic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>


UUnitProducePeriodic::UUnitProducePeriodic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	Period = FScalableFloat(10.0f);

	InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_UnitProducePeriodic);
	InheritableGameplayEffectTags.Added.AddTag(FGE_UnitProducePeriodic);
}