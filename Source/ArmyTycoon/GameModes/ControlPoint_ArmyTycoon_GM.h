// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <ArmyTycoon/ArmyTycoonGameModeBase.h>
#include "ControlPoint_ArmyTycoon_GM.generated.h"




UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API AControlPoint_ArmyTycoon_GM : public AArmyTycoonGameModeBase
{
	GENERATED_BODY()

public:

	/** Allows GamedMode to find all Control Points at Begin play */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameConfig|ControlPoints")
	bool bAllowAutoScanControlPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameConfig|ControlPoints")
	TArray<AActor*> ControlPoints;

public:

	AControlPoint_ArmyTycoon_GM(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	//	Begin AActor Interface
	virtual void BeginPlay() override;
	//	End of AActor Interface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ControlPoints")
	const TArray<AActor*> GetControlPoints() const { return ControlPoints; }

};