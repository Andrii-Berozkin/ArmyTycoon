// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayAbilities/UnitProduceAbility.h>
#include <ArmyTycoon/Buildings/BuildingProductionComponent.h>



UUnitProduceAbility::UUnitProduceAbility(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

bool UUnitProduceAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) { return false; }

	if (ActorToProduce == NULL) { return false; }

	return true;
}

void UUnitProduceAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
#if WITH_SERVER_CODE

	AActor* Inistigator = ActorInfo->AvatarActor.Get();
	UWorld* WorldPtr = Inistigator->GetWorld();

	FTransform PointToSpawn = Inistigator->GetActorTransform();
	
	if (CommitCheck(Handle, ActorInfo, ActivationInfo))
	{
		TSet<UActorComponent*> Components = Inistigator->GetComponents();
		for (UActorComponent* Comp : Components)
		{
			if (UProductionBuildingComponent* ProductionBuildComponent = Cast<UProductionBuildingComponent>(Comp))
			{
				FActorSpawnParameters SpawnParam;
				SpawnParam.bDeferConstruction = false;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				SpawnParam.Owner = Inistigator; 

				if (HasAuthority(&ActivationInfo)) { WorldPtr->SpawnActor(ActorToProduce, &PointToSpawn, SpawnParam); }
			}

			if (USimpleProductionBuildingComponent* SimpleProductionBuildingComponent = Cast<USimpleProductionBuildingComponent>(Comp))
			{
				PointToSpawn = SimpleProductionBuildingComponent->GatheringPoint;

				FActorSpawnParameters SpawnParam;
				SpawnParam.bDeferConstruction = false;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				SpawnParam.Owner = Inistigator;

				if(HasAuthority(&ActivationInfo)) { WorldPtr->SpawnActor(ActorToProduce, &PointToSpawn, SpawnParam); }
			}
		}

		CommitAbility(Handle, ActorInfo, ActivationInfo);

		//ApplyCost(Handle, ActorInfo, ActivationInfo);
		//ApplyCooldown(Handle, ActorInfo, ActivationInfo);
	}
	

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

#endif // WITH_SERVER_CODE
}

void UUnitProduceAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCanceled)
{
}