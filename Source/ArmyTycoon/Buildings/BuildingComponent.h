// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include <ArmyTycoon/Buildings/TycoonBuildingInterface.h>
#include <ArmyTycoon/ArmyTycoonGeneral.h>
#include <GameplayAbilities/Public/GameplayEffectTypes.h>
#include "BuildingComponent.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UBuildingComponent : public UActorComponent, public ITycoonBuildingInterface, public ITycoonTeamsInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	float UpgradeCost;

protected:

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystem;

private:

public:

	UBuildingComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION()
	void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION()
	void UninitializeFromAbilitySystem();

	//	Begin ITycoonBuildingInterface
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building")
	virtual float GetCost() const override { return UpgradeCost; }
	//	End   ITycoonBuildingInterface

	//	Begin ITycoonTeamsInterface
	virtual int32 GetTeam() const;

	virtual void SetTeam(int32 NewTeamID);
	//	End of ITycoonTeamsInterface


protected:

	void OnAttributeLevelChanged(const FOnAttributeChangeData& Data);

};