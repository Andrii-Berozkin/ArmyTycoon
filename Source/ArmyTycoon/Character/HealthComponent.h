// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include "HealthComponent.generated.h"


struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAttributeChangeSignature, UHealthComponent*, HealthComponent, AActor*, Inistigator, float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FDeathSignature, UHealthComponent, OnDeath);



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FAttributeChangeSignature OnHealthChange;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangeSignature OnMaxHealthChange;

	UPROPERTY(BlueprintAssignable)
	FAttributeChangeSignature OnMinHealthChange;

	UPROPERTY(BlueprintAssignable)
	FDeathSignature OnDeath;

protected:

	UPROPERTY()
	const class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY()
	const class UHealthAttributeSet* HealthSet;

private:

	void ReachMinimalHandle(AActor* AttributeOwner, const FGameplayEffectSpec& EffectSpec, float EffectMagnitude);

public:

	UHealthComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void InitializerWithAbilitySystem(class UAbilitySystemComponent* AbilitySystemComponent);
	void UninitializeFromAbilitySystem();

	float GetCurrentHealth() const;

protected:

	void OnHealthChangedHandle(const FOnAttributeChangeData& ChangeData);
	void OnMaxHealthChangedHandle(const FOnAttributeChangeData& ChangeData);
	void OnMinHealthChangedHandle(const FOnAttributeChangeData& ChangeData);

private:

	static AActor* GetOriginalInistigatorFromModData(const struct FGameplayEffectModCallbackData* ModData);
};