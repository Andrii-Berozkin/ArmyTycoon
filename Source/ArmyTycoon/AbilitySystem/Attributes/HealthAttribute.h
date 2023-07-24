// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <AttributeSet.h>
#include <AbilitySystemComponent.h>
#include "HealthAttribute.generated.h"



DECLARE_MULTICAST_DELEGATE_ThreeParams(FHealthBelowMinimalSignature, AActor* /*AttributeOwner*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/ );

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)




UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "HealthAttribute", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);


	UPROPERTY(BlueprintReadOnly, Category = "HealthAttribute", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "HealthAttribute", ReplicatedUsing = OnRep_MinHealth)
	FGameplayAttributeData MinHealth;
	ATTRIBUTE_ACCESSORS(UHealthAttributeSet, MinHealth);

	//	Delegates

	mutable FHealthBelowMinimalSignature HealthReachMinimal;

public:

	UHealthAttributeSet();

	UFUNCTION()
	virtual void OnRep_Health(FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_MinHealth(FGameplayAttributeData& OldMinHealth);

	//	Begin UAttributeSet Interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	
	//	Endd  UAttributeSet Interface

protected:

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};