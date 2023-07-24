// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayEffectExecutionCalculation.h>
#include "HealthSet_Heal_Execution.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthSetHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

protected:

	struct FGameplayEffectAttributeCaptureDefinition HealthCapture;

public:

	UHealthSetHealExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};