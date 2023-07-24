// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/HealthHealBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include "HealthHealPeriodic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthHealPeriodic : public UHealthHealBasic
{
	GENERATED_BODY()

public:

public:

	UHealthHealPeriodic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		DurationPolicy = EGameplayEffectDurationType::Infinite;

		InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_HealPeriodic);
		InheritableGameplayEffectTags.Added.AddTag(FGE_HealPeriodic);
	}
};