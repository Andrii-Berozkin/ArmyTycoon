// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/UnitProduceBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>




UUnitProduceBasic::UUnitProduceBasic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PeriodicInhibitionPolicy = EGameplayEffectPeriodInhibitionRemovedPolicy::NeverReset;
	DurationPolicy = EGameplayEffectDurationType::Instant;

	InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_UnitProduce);
	InheritableGameplayEffectTags.Added.AddTag(FGE_UnitProduce);

	FGameplayEffectExecutionDefinition UnitProduceExecutionDefinition;
	//UnitProduceExecutionDefinition.CalculationClass = UProdBuildSet_Produce_Execution::StaticClass();

	/*
	FGameplayEffectExecutionScopedModifierInfo ModifierInfo;
	ModifierInfo.AggregatorType = EGameplayEffectScopedModifierAggregatorType::CapturedAttributeBacked;
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(0.0));
	ModifierInfo.ModifierOp = EGameplayModOp::Override;
	UnitProduceExecutionDefinition.CalculationModifiers.Add(ModifierInfo);
	*/
	
	Executions.Add(UnitProduceExecutionDefinition);
}