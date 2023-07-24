// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <ArmyTycoon/CapturePoints/CapturePointInterface.h>
#include "CapturePointBasic.generated.h"


class UPrimitiveComponent;

UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API ACapturePointBasic : public AActor
{
	GENERATED_BODY()

public:

	const static FName CapturePointComponentName;
	const static FName CollisionComponentName;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivatAccess = true))
	class UCapturePointComponent* CapturePointComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UPrimitiveComponent* CollisionComponent;

private:

public:

	ACapturePointBasic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin AActor Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	//	End   AActor Interface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "")
	bool IsCaptured() const;

	const UCapturePointComponent* GetCapturePointComponent() const { return CapturePointComponent; }

	const UPrimitiveComponent* GetCollisionComponent() const { return CollisionComponent; }

protected:

	UFUNCTION()
	virtual void OnPointCapturedHandle(class UCapturePointComponent* CapturePointComponentInistigator, AActor* NewOwner, int32 TeamID);

	UFUNCTION()
	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnComponentEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};