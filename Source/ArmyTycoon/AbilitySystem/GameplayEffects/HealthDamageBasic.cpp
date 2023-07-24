// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/HealthDamageBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>
#include <ArmyTycoon/AbilitySystem/Executions/HealthSet_Damage_Execution.h>



UHealthDamageBasic::UHealthDamageBasic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset;
	DurationPolicy = EGameplayEffectDurationType::Instant;

	InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_Damage);
	InheritableGameplayEffectTags.Added.AddTag(FGE_Damage);

	GrantedApplicationImmunityTags.IgnoreTags.AddTag(FHealth_Cheats_GodMode);

	/*
	FGameplayModifierInfo ModInfo;
	ModInfo.Attribute = UHealthAttributeSet::GetHealthAttribute();
	//	Magnitude is Deprecated
	ModInfo.Magnitude = -1.0f;
	ModInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-1.0f));
	ModInfo.ModifierOp = EGameplayModOp::Additive;
	Modifiers.Add(ModInfo);
	*/

	FGameplayEffectExecutionDefinition DamageExecutionDefinition;

	DamageExecutionDefinition.CalculationClass = UHealthSetDamageExecution::StaticClass();

	FGameplayEffectExecutionScopedModifierInfo ScopedModifierInfo;
	ScopedModifierInfo.AggregatorType = EGameplayEffectScopedModifierAggregatorType::CapturedAttributeBacked;
	ScopedModifierInfo.CapturedAttribute = FGameplayEffectAttributeCaptureDefinition(UHealthAttributeSet::GetHealthAttribute(), EGameplayEffectAttributeCaptureSource::Target, false);
	ScopedModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-1.0f));
	ScopedModifierInfo.ModifierOp = EGameplayModOp::Additive;

	DamageExecutionDefinition.CalculationModifiers.Add(ScopedModifierInfo);

	Executions.Add(DamageExecutionDefinition);
}