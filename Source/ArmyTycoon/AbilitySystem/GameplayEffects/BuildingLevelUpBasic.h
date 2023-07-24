// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/GameplayEffect.h>
#include <ArmyTycoon/AbilitySystem/Attributes/BuildingAttribute.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include "BuildingLevelUpBasic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UBuildingLevelUpBasic : public UGameplayEffect
{
	GENERATED_BODY()

public:

public:

	UBuildingLevelUpBasic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		DurationPolicy = EGameplayEffectDurationType::Infinite;

		FGameplayModifierInfo ModInfo;
		ModInfo.Attribute = UBuildingAttributeSet::GetBuildingLevelAttribute();
		ModInfo.Magnitude = FScalableFloat(1.0f);
		ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(1.0f));
		ModInfo.ModifierOp = EGameplayModOp::Additive;
		
		Modifiers.Add(ModInfo);
	}

};