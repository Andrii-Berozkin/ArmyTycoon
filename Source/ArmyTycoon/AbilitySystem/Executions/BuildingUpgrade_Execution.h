// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/GameplayEffectExecutionCalculation.h>
#include "BuildingUpgrade_Execution.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UBuildingUpgradeExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

public:

	UBuildingUpgradeExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};