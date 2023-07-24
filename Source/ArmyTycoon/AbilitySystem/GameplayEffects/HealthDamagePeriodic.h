// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/HealthDamageBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include "HealthDamagePeriodic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthDamagePeriodic : public UHealthDamageBasic
{
	GENERATED_BODY()

public:

public:

	UHealthDamagePeriodic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		DurationPolicy = EGameplayEffectDurationType::Infinite;

		InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_DamagePeriodic);
		InheritableGameplayEffectTags.Added.AddTag(FGE_DamagePeriodic);
	}
};