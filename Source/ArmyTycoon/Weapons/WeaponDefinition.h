// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "WeaponDefinition.generated.h"



class UAbilitySet;



USTRUCT(BlueprintType, Blueprintable)
struct FWeaponSetup
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVisual")
		TSubclassOf<AActor> WeaponActor;

	/** Socket to which weapon will be attached */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVisual")
		FName SocketToAttach;

	/** Relative transform to socket attach */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponVisual")
		FTransform RelativeTransform;

public:

	FWeaponSetup()
	{
	};
};

UCLASS(BlueprintType, Blueprintable, Const, Abstract)
class ARMYTYCOON_API UWeaponDefinition : public UObject
{
	GENERATED_BODY()

public:

	UWeaponDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get())
		: Super(ObjectInitializer)
	{
	}

	/* Gameplay abilities, effect, attributes which reffers to this weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<TSoftObjectPtr<const UAbilitySet>> GrantedAbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FWeaponSetup WeaponSetup;

};