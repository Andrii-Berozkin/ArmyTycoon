// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/GameplayEffectExecutionCalculation.h>
#include "BuildingUpgradeCost_Execution.generated.h"



UCLASS()
class ARMYTYCOON_API UBuildingUpgradeCostExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

public:

	UBuildingUpgradeCostExecution();

	//	Begin UGameplayEffectExecutionCalculation Interface
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	//	End of UGameplayEffectExecutionCalculation Interface

};