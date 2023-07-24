// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/BuildingUpgradeBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/AbilitySystem/Executions/BuildingUpgrade_Execution.h>



UBuildingUpgradeBasic::UBuildingUpgradeBasic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FGameplayEffectModifierMagnitude DurationModifier(FScalableFloat(1.0f));
	DurationMagnitude = DurationModifier;

	FGameplayEffectExecutionDefinition ExecutionDefinition;
	ExecutionDefinition.CalculationClass = UBuildingUpgradeExecution::StaticClass();

	bExecutePeriodicEffectOnApplication = false;
	//Executions.Add(ExecutionDefinition);

	InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_UpgradeInProgress);
	InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_BuildingUpgradeInProgress);
	InheritableGameplayEffectTags.Added.AddTag(FGE_UpgradeInProgress);
	InheritableGameplayEffectTags.Added.AddTag(FGE_BuildingUpgradeInProgress);

	InheritableOwnedTagsContainer.CombinedTags.AddTag(FGE_UpgradeInProgress);
	InheritableOwnedTagsContainer.CombinedTags.AddTag(FGE_BuildingUpgradeInProgress);
	InheritableOwnedTagsContainer.Added.AddTag(FGE_UpgradeInProgress);
	InheritableOwnedTagsContainer.Added.AddTag(FGE_BuildingUpgradeInProgress);
}