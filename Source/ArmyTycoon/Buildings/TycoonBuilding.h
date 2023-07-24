// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <AbilitySystemInterface.h>
#include <GameplayAbilities/Public/AttributeSet.h>
#include <GameplayAbilities/Public/GameplayAbilitySpec.h>
#include <ArmyTycoon/AbilitySystem/Attributes/BuildingAttribute.h>
#include <ArmyTycoon/Buildings/BuildingComponent.h>
#include <ArmyTycoon/ArmyTycoonGeneral.h>
#include <Engine/DataAsset.h>

#include "TycoonBuilding.generated.h"



class UAbilitySystemComponent;
class ABuilding;



DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_ThreeParams(FBuildingAttributeChangeSignature, ABuilding, OnBuildingLevelChange, FGameplayAttribute, Attribute, int32, NewValue, int32, OldValue);

USTRUCT(BlueprintType, Blueprintable)
struct FLevelCost
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost;

public:

	FLevelCost()
		: Level(0),
		Cost(0.0f)
	{
	};

	FLevelCost(int32 NewLevel, float NewCost)
		: Level(NewLevel),
		Cost(NewCost)
	{
	};
};



UCLASS(BlueprintType, Blueprintable, Const)
class ARMYTYCOON_API UBuildingWidgetAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Building|UI")
	TSubclassOf<class UUserWidget> PopUpWidget;

};



//		======
class UBuildingComponent;
class UGameplayEffect;
class UGameplayAbility;



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API ABuilding : public AActor, public IAbilitySystemInterface, public ITycoonTeamsInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UBuildingComponent* BuildingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = true))
	UBuildingAttributeSet* BuildingSet;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	TArray<FLevelCost> UpgradeCost;

	UPROPERTY(BlueprintAssignable, Category = "Buidling")
	FBuildingAttributeChangeSignature OnBuildingLevelChange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Building|Teams")
	int32 TeamID;

protected:

	//	Components names
	const static FName AbilitySystemName;
	const static FName BuildingComponentName;
	const static FName BuildingAttributeSetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building")
	UObject* BuildingOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<UGameplayEffect>, float> StartupEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = true))
	TArray<struct FGameplayAbilitySpecDef> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building|UI", meta = (AllowPrivateAccess = true))
	UBuildingWidgetAsset* BuildingWidgetAsset;

	UPROPERTY(Transient)
	class UUserWidget* PopUpWidgetHandle;

public:

	ABuilding(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin AActor Interface
	virtual void BeginPlay() override;

	virtual void Reset() override;
	//	End of AActor Interface

	//	Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }
	//	End of IAbilitySystemInterface

	//	Begin ITycoonTeamsInterface
	virtual int32 GetTeam() const { return TeamID; }

	virtual void SetTeam(int32 NewTeamID) { TeamID = NewTeamID; }
	//	End of ITycoonTeamsInterface

	FORCEINLINE UBuildingComponent* GetBuildingComponent() const { return BuildingComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building")
	UObject* GetBuildingOwner() const { return BuildingOwner; }

	UFUNCTION(BlueprintCallable, Category = "Building")
	void SetBuildingOwner(UObject* NewOwner) { BuildingOwner = NewOwner; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building")
	float GetUpgradeCost() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building")
	float GetDowngradeCost() const;

	UFUNCTION()
	void OnClickedHandle(AActor* TouchedActor, FKey ButtonPressed);

	UFUNCTION()
	void OnInputTouchBeginHandle(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

protected:

	void InitializeAttributes();

	void InitializeStartupEffects();

	void InitializeStartupAbilities();

	virtual void OnBuildingLevelAttibuteChanges(const FOnAttributeChangeData& Data);

	void ShowBuildingUI(AActor* TouchedActor);

	void HideBuildingUI();

};

//		=======