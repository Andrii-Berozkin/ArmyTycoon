// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayAbilities/Public/AttributeSet.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>
#include "MoneyAttribute.generated.h"



// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



UCLASS()
class ARMYTYCOON_API UMoneyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "MoneyAttribute", ReplicatedUsing = OnRep_Money)
	FGameplayAttributeData Money;
	ATTRIBUTE_ACCESSORS(UMoneyAttributeSet, Money);

public:

	UMoneyAttributeSet();

	//	Begin UObject Interface
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//	End   UObject Interface

	UFUNCTION()
	void OnRep_Money(FGameplayAttributeData& OldMoney);
};