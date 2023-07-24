// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Executions/HealthSet_Damage_Execution.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>



UHealthSetDamageExecution::UHealthSetDamageExecution()
{
	HealthCapture = FGameplayEffectAttributeCaptureDefinition(UHealthAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);

	RelevantAttributesToCapture.Add(HealthCapture);
}

void UHealthSetDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;


	//	CaptureDefinition
	//FGameplayEffectAttributeCaptureDefinition HealthCapture(UHealthAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);

	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealthCapture, EvaluateParameters, CurrentHealth);

	//	Actual damage
	float DamageDone = 0.0f;

	DamageDone = FMath::Clamp<float>(DamageDone, 0.0f, CurrentHealth);

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UHealthAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, DamageDone));

#endif // WITH_SERVER_CODE

}