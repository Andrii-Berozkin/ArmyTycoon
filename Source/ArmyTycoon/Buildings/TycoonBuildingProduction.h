// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <ArmyTycoon/Buildings/TycoonBuilding.h>
#include "TycoonBuildingProduction.generated.h"


class UProductionBuildingComponent;
class USimpleProductionBuildingComponent;

UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API AProductionBuilding : public ABuilding
{
	GENERATED_BODY()

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UProductionBuildingComponent* ProductionBuildingComponent;
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USimpleProductionBuildingComponent* SimpleProductionBuildingComponent;

public:

public:

	AProductionBuilding(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//FORCEINLINE UProductionBuildingComponent* GetProductionBuildingComponent() const { return ProductionBuildingComponent; }

	FORCEINLINE USimpleProductionBuildingComponent* GetSimpleProductionBuildingComponent() const { return SimpleProductionBuildingComponent; }

};