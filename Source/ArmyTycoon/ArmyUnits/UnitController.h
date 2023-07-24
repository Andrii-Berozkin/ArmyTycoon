// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <AIController.h>
#include "UnitController.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API AUnitController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UBehaviorTreeComponent* BehaviorComponent;


public:

private:


public:

	AUnitController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	//	Begin AAIcontroller Interface
	virtual void OnPossess(class APawn* InPawn) override;
	virtual void OnUnPossess() override;
	//	End   AAIcontroller Interface
};