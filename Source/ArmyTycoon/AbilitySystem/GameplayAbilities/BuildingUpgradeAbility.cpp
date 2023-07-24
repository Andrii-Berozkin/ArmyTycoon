// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayAbilities/BuildingUpgradeAbility.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/BuildingUpgradeBasic.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/UpgradeCostBasic.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/BuildingLevelUpBasic.h>
#include <ArmyTycoon/Character/TycoonCharacter.h>
#include <ArmyTycoon/Buildings/TycoonBuilding.h>
#include <AbilitySystemInterface.h>



UBuildingUpgradeAbility::UBuildingUpgradeAbility(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	CostGameplayEffectClass = UUpgradeCostBasic::StaticClass();

	BasicEffectLevel = 1.0f;
}

bool UBuildingUpgradeAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor->IsValidLowLevelFast()) { return false; }
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags)) { return false; }
	
	if (ATycoonCharacter* TycoonCharacter = Cast<ATycoonCharacter>(ActorInfo->AvatarActor))
	{
		ABuilding* BuildingActor = Cast<ABuilding>(TycoonCharacter->GetSelectedActor());

		//	Check cost 
		if (BuildingActor)
		{
			return TycoonCharacter->GetMoneyAttributeSet()->GetMoney() - BuildingActor->GetUpgradeCost() >= 0.0 ? true : false;
		}
		
		//	Dosen`t make any sense, because it dosen`t check "GameplayEffect" "Executions" variable to check value
		//if(!CheckCost(Handle, ActorInfo)) { return false; }		

	}

	return true;
}

void UBuildingUpgradeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ATycoonCharacter* TycoonCharacter = Cast<ATycoonCharacter>(ActorInfo->AvatarActor);

	IAbilitySystemInterface* SelectedActor = Cast<IAbilitySystemInterface>(TycoonCharacter->GetSelectedActor());
	

	if (SelectedActor != nullptr && CommitCheck(Handle, ActorInfo, ActivationInfo))
	{
		//	Do more stuff

		UAbilitySystemComponent* TargetAbilitySystem = SelectedActor->GetAbilitySystemComponent();

		
		/**	Important: Cost must be applied before gameplay effect of lvl up.
		*	Because Cost uses Execution effect which calculate cost of upgrade based on current lvl
		*	If apply lvl up before apply cost, it will try to get cost of next lvl
		*	As "GetUpgradeCost()" uses CurrentBuildingLvl + 1.0f : I.e. next lvl of current
		*	E.g. Current lvl building 1.0f -> apply effect lvl up : new lvl 1.0 -> apply cost will get cost of upgrade for lvl 2.0f
		*/	
		ApplyCost(Handle, ActorInfo, ActivationInfo);
		ApplyCooldown(Handle, ActorInfo, ActivationInfo);

		if (TargetAbilitySystem)
		{
			for (TSubclassOf<UGameplayEffect> Effect : EffectsToApply)
			{
				FGameplayEffectContextHandle Context = TargetAbilitySystem->MakeEffectContext();
				Context.AddSourceObject(TycoonCharacter);

				FGameplayEffectSpecHandle EffectSpec = TargetAbilitySystem->MakeOutgoingSpec(Effect, BasicEffectLevel, Context);

				TargetAbilitySystem->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
			}
		}
		

		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
	else { CancelAbility(Handle, ActorInfo, ActivationInfo, false);}
}

void UBuildingUpgradeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


//	TO DO : Change BuildingComponent to use GameplayAbility instead GampleyEffectExecution