// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Executions/BuildingUpgradeCost_Execution.h>
#include <ArmyTycoon/AbilitySystem/Attributes/MoneyAttribute.h>
#include <ArmyTycoon/Character/TycoonCharacter.h>
#include <ArmyTycoon/Buildings/TycoonBuilding.h>



UBuildingUpgradeCostExecution::UBuildingUpgradeCostExecution()
{

}

void UBuildingUpgradeCostExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle Context = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	AActor* Inistigator = Context.GetInstigator();

	FGameplayModifierEvaluatedData EvaluatedData;
	EvaluatedData.Attribute = UMoneyAttributeSet::GetMoneyAttribute();
	EvaluatedData.Magnitude = 0.0f;
	EvaluatedData.ModifierOp = EGameplayModOp::Additive;

	if (ATycoonCharacter* TycoonChar = Cast<ATycoonCharacter>(Inistigator))
	{
		if (ABuilding* SelectedBuilding = Cast<ABuilding>(TycoonChar->GetSelectedActor()))
		{
			EvaluatedData.Magnitude = -SelectedBuilding->GetUpgradeCost();
		}
	}


	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}