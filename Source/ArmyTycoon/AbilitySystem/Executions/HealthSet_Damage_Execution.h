// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayEffectExecutionCalculation.h>
#include "HealthSet_Damage_Execution.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthSetDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

protected:

	struct FGameplayEffectAttributeCaptureDefinition HealthCapture;

public:

	UHealthSetDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};