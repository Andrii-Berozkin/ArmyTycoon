// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/Abilities/GameplayAbility.h>
#include <GameplayAbilities/Public/GameplayEffect.h>
#include "GameplayAbilitySetup.generated.h"


//class UGameplayEffect;
class UAttributeSet;

USTRUCT()
struct FAbilitySetHandle
{
	GENERATED_BODY()

public:

	UPROPERTY()
		TArray<const UAttributeSet*> Attributes;

	UPROPERTY()
		TArray<FActiveGameplayEffectHandle> ActiveEffects;

	UPROPERTY()
		TArray<FGameplayAbilitySpecHandle> AbilitiesHandle;

public:

	FAbilitySetHandle() {};

};

USTRUCT(BlueprintType, Blueprintable)
struct FGameplayAbilitySetup
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilitySetup")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilitySetup")
	int32 AbilityLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilitySetup")
	FGameplayTag InputID;

public:

	FGameplayAbilitySetup()
	{
	};

};


USTRUCT(BlueprintType, Blueprintable)
struct FGameplayEffectSetup
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayEffectSetup")
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayEffectSetup")
	float EffectLevel;

public:

	FGameplayEffectSetup()
	{
	};

};

UCLASS(BlueprintType, Blueprintable, Const)
class ARMYTYCOON_API UAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet")
	TArray<FGameplayAbilitySetup> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet")
	TArray<FGameplayEffectSetup> GrantedEffects;

	UPROPERTY(EditDefaultsOnly, Category = "AbilitySet")
	TArray<TSubclassOf<UAttributeSet>> GrantedAttributes;

public:

	UAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GrantAbilitySetToComponent(UAbilitySystemComponent* ASC, FAbilitySetHandle& OutAbilitySetHandle) const;

};