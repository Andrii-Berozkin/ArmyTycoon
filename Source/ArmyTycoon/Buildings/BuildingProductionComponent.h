// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <ArmyTycoon/Buildings/BuildingComponent.h>
#include <GameplayAbilities/Public/GameplayAbilitySpec.h>
#include "BuildingProductionComponent.generated.h"



DECLARE_DELEGATE(FProgressCompleteSignature);

UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UProductionBuildingComponent : public UBuildingComponent, public ITycoonProductionBuildingInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building| Production")
	TSubclassOf<class UGameplayEffect> ProductionEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Building| Production")
	float ProduceCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building| Production")
	/** Time to produce in seconds */
	float BaseTimeToProduce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building| Production")
	/**  */
	float EffectivenessRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building| Production")
	TArray<FGameplayAbilitySpec> ProduceAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building| Production")
	FGameplayTagContainer TagsToBlockProduction;

protected:

private:

	UPROPERTY(BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	/** What amount of goods this building producing in one minute */
	float ProductionRate;

	UPROPERTY(BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	/** How effective this building producing goods in percent */
	float Effectiveness;

	UPROPERTY(BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	/** How much time it takes to produce with current effectiveness */
	float ActualTimeToProduce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	float BaseMinEffectiveness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	float BaseMaxEffectiveness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	float BonusEffectiveness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Building| Production", meta = (AllowPrivateAccess = true))
	float Progress;

	UPROPERTY(Transient)
	/** Progress rate at second */
	float ProgressRate;

	FProgressCompleteSignature ProgressComplete;

	UPROPERTY(Transient)
	bool bProductionMarkDirty;

public:

	UProductionBuildingComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//	Begin UActorComponent Interface
	virtual void BeginPlay() override;
	//	End of UActorComponent Interface

	//	Begin ITycoonBuildingInterface
	virtual float GetCost() const override { return Super::GetCost(); }
	//	End   ITycoonBuildingInterface

	//	Begin ITycoonProductionBuildingInterface
	UFUNCTION(BlueprintCallable, Category = "Building")
	virtual void Produce() override;
	//	End   ITycoonProductionBuildingInterface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building| Production")
	float GetProductionRate() const { return ProductionRate; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Building| Production")
	float GetEffectiveness() const { return Effectiveness; }

	UFUNCTION(BlueprintCallable, Category = "Building| Production")
	void SetBaseMinEffectiveness(float NewValue) { BaseMinEffectiveness = NewValue; bProductionMarkDirty = true; }

	UFUNCTION(BlueprintCallable, Category = "Building| Production")
	void SetBaseMaxEffectiveness(float NewValue) { BaseMaxEffectiveness = NewValue; bProductionMarkDirty = true; }

	UFUNCTION(BlueprintCallable, Category = "Building| Production")
	void SetBonusEffectiveness(float NewValue) { BonusEffectiveness = NewValue; bProductionMarkDirty = true; }

	UFUNCTION()
	void RecalculateEffectiveness();

	UFUNCTION()
	void CalculateProductionRate();

protected:

	virtual void OnProgressComplete();

private:

	void AddProgress(float Value) { Progress += Value; if (Progress >= ActualTimeToProduce) { OnProgressComplete(); } }

};



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API USimpleProductionBuildingComponent : public UBuildingComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleProduction")
	float TimeToProduce;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleProduction")
	FTransform GatheringPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleProduction")
	TArray<FGameplayAbilitySpecDef> ProductionAbilities;

protected:

	UPROPERTY()
	FTimerHandle ProduceTimer;
	
	FTimerDelegate ProduceDelegate;

public:

	USimpleProductionBuildingComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin UObject Interface
	virtual void BeginPlay() override;
	//	End of UObject Interface

	void Produce();

	void BlockProduction(const FGameplayTag CallbackTag, int32 NewStackCount);

	UFUNCTION(BlueprintCallable, Category = "SimpleProduction|Helper")
	static FGameplayAbilitySpecDef MakeAbilityDef(TSubclassOf<UGameplayAbility> Ability, FScalableFloat LevelScaleFloat, int32 InputID = -1, EGameplayEffectGrantedAbilityRemovePolicy RemovalPolicy = EGameplayEffectGrantedAbilityRemovePolicy::CancelAbilityImmediately)
	{ 
		FGameplayAbilitySpecDef AbilitySpec;
		AbilitySpec.Ability = Ability;
		AbilitySpec.LevelScalableFloat = LevelScaleFloat;
		AbilitySpec.InputID = InputID;
		AbilitySpec.RemovalPolicy = RemovalPolicy;
		
		return AbilitySpec;
	}

};