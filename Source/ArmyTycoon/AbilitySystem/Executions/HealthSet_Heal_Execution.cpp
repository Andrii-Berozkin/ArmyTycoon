// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Executions/HealthSet_Heal_Execution.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>



UHealthSetHealExecution::UHealthSetHealExecution()
{
	HealthCapture = FGameplayEffectAttributeCaptureDefinition(UHealthAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);

	RelevantAttributesToCapture.Add(HealthCapture);
}

void UHealthSetHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = &Spec.CapturedSourceTags.GetActorTags();
	const FGameplayTagContainer* TargetTags = &Spec.CapturedTargetTags.GetActorTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;


	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthCapture, EvaluateParameters, CurrentHealth);


	float AddHealth = 10.0f;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UHealthAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, AddHealth));

#endif // WITH_SERVER_CODE
}