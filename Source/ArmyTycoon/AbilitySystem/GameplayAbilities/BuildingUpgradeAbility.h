// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/Abilities/GameplayAbility.h>
#include "BuildingUpgradeAbility.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UBuildingUpgradeAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	/** Array of effects which would be applied to target if abilily woulde be activated */
	TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	float BasicEffectLevel;

public:

	UBuildingUpgradeAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin UGameplayAbility Interface
		virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const override;
	//	End   UGameplayAbility Interface

protected:

	//	Begin UGameplayAbility Interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//	End   UGameplayAbility Interface

};