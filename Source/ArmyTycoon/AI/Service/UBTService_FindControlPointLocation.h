// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <BehaviorTree/BTService.h>
//#include <BehaviorTree/Services/BTService_BlueprintBase.h>
#include "UBTService_FindControlPointLocation.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UBTService_FindControlPointLocation : public UBTService
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeLocationAtNavRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector PointLocation;

protected:

	UPROPERTY(Transient)
	AAIController* AIController;

	UPROPERTY(Transient)
	AActor* ActorOwner;

public:

	UBTService_FindControlPointLocation(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	//	Begin UBTAuxiliaryNode
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* MemoryNode) override;
	//	End of UBTAuxiliaryNode

	void FilterCapturedPoints(TArray<AActor*> ListOfPoints, AController* Controller, OUT TArray<AActor*>& OutArr);
	void FilterCapturedPoints(TArray<AActor*> ListOfPoints, AActor* OwnerActor, OUT TArray<AActor*>& OutArr);

	AActor* GetClosestActor(FVector TargetLocation, TArray<AActor*> ActorList);

};