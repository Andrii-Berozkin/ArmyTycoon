// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/GameplayEffect.h>
#include <ArmyTycoon/AbilitySystem/Attributes/MoneyAttribute.h>
#include <ArmyTycoon/AbilitySystem/Executions/BuildingUpgradeCost_Execution.h>
#include "UpgradeCostBasic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UUpgradeCostBasic : public UGameplayEffect
{
	GENERATED_BODY()

public:

public:

	UUpgradeCostBasic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
		DurationPolicy = EGameplayEffectDurationType::Instant;

		/*
		FGameplayModifierInfo ModInfo;
		ModInfo.Attribute = UMoneyAttributeSet::GetMoneyAttribute();
		ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-1.0f));
		ModInfo.ModifierOp = EGameplayModOp::Additive;

		Modifiers.Add(ModInfo);
		*/

		FGameplayEffectExecutionDefinition ExecutionDef;
		ExecutionDef.CalculationClass = UBuildingUpgradeCostExecution::StaticClass();

		Executions.Add(ExecutionDef);
	}

};