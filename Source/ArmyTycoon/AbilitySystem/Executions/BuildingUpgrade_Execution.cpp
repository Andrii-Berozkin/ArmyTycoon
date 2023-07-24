// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Executions/BuildingUpgrade_Execution.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/Buildings/TycoonBuilding.h>
#include <TimerManager.h>




UBuildingUpgradeExecution::UBuildingUpgradeExecution()
{

}

void UBuildingUpgradeExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
#if WITH_SERVER_CODE

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle Context = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();



	AActor* Inistigator = Context.GetInstigator();
	UWorld* WorldPtr = Inistigator->GetWorld();

	TSet<UActorComponent*> ActorComponents = Inistigator->GetComponents();
	for (UActorComponent* Component : ActorComponents)
	{
		if (UBuildingComponent* BuildComponent = Cast<UBuildingComponent>(Component))
		{
			FActorSpawnParameters SpawnParam;
			SpawnParam.bHideFromSceneOutliner = false;
			SpawnParam.Name = NAME_None;
			SpawnParam.bDeferConstruction = false;
			SpawnParam.OverrideLevel = WorldPtr->GetCurrentLevel();

			//WorldPtr->SpawnActor(BuildComponent->UpgradeClass, &Inistigator->GetTransform(), SpawnParam);
		}
	}

	const FGameplayEffectModifierMagnitude& DurationMag = Spec.Def->DurationMagnitude;
	{
		float DurationFloat = 0.0f;
		DurationMag.AttemptCalculateMagnitude(Spec, DurationFloat);

		FTimerHandle TimerHandle;
		FTimerDelegate DestroyTimer;
		DestroyTimer.BindLambda([Inistigator] {Inistigator->Destroy(); });
		WorldPtr->GetTimerManager().SetTimer(TimerHandle, DestroyTimer, DurationFloat, false, 0.0f);
	}


#endif // WITH_SERVER_CODE
}


//	TO DO: Make Upgrade Ability to work with money attribute