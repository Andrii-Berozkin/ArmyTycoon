// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/HealthHealBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>
#include <ArmyTycoon/AbilitySystem/Executions/HealthSet_Heal_Execution.h>



UHealthHealBasic::UHealthHealBasic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset;
	DurationPolicy = EGameplayEffectDurationType::Instant;

	InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_Heal);
	InheritableGameplayEffectTags.Added.AddTag(FGE_Heal);


	/*
	FGameplayModifierInfo ModInfo;
	ModInfo.Attribute = UHealthAttributeSet::GetHealthAttribute();
	//	Magnitude is Deprecated
	ModInfo.Magnitude = 1.0f;
	ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(1.0f));
	ModInfo.ModifierOp = EGameplayModOp::Additive;
	Modifiers.Add(ModInfo);
	*/

	FGameplayEffectExecutionDefinition HealExecutionDefinition;
	HealExecutionDefinition.CalculationClass = UHealthSetHealExecution::StaticClass();

	FGameplayEffectExecutionScopedModifierInfo ExecutionScopedInfo;
	ExecutionScopedInfo.AggregatorType = EGameplayEffectScopedModifierAggregatorType::CapturedAttributeBacked;
	ExecutionScopedInfo.ModifierOp = EGameplayModOp::Additive;
	ExecutionScopedInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(1.0f));
	ExecutionScopedInfo.CapturedAttribute = FGameplayEffectAttributeCaptureDefinition(UHealthAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	HealExecutionDefinition.CalculationModifiers.Add(ExecutionScopedInfo);

	Executions.Add(HealExecutionDefinition);
}