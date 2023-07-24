// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once
#include <CoreMinimal.h>
#include <GameplayAbilities/Public/AttributeSet.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>
#include "BuildingAttribute.generated.h"



// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UBuildingAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "BuildingAttribute", ReplicatedUsing = OnRep_BuildingLevel)
	FGameplayAttributeData BuildingLevel;
	ATTRIBUTE_ACCESSORS(UBuildingAttributeSet, BuildingLevel);

public:

	UBuildingAttributeSet();

	UFUNCTION()
	virtual void OnRep_BuildingLevel(FGameplayAttributeData& OldValue);

	//	Begin UObject Interface
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//	End of UObject Interface

	//	Begin UAttributeSet Interface
	//	End   UAttributeSet Interface

};