// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/HealthGodMode.h>


UHealthGodMode::UHealthGodMode(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{
	PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset;
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	//InheritableOwnedTagsContainer.CombinedTags.AddTag(FHealth_Cheats_GodMode);
	//InheritableOwnedTagsContainer.Added.AddTag(FHealth_Cheats_GodMode);

	InheritableGameplayEffectTags.CombinedTags.AddTag(FHealth_Cheats_GodMode);
	InheritableGameplayEffectTags.Added.AddTag(FHealth_Cheats_GodMode);

	
	//Executions.Add()
}