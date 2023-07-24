// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>
#include <ArmyTycoon/CapturePoints/CapturePointInterface.h>
#include <Components/TimelineComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include "CapturePointComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPointCaptureChangedSignature, UCapturePointComponent*, CapturePointComponent, AActor*, NewOwner, int32, TeamID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointStateChangedSignature, UCapturePointComponent*, CapturePointComponent);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FPointProgressChangedSignature, UCapturePointComponent*, CapturePointComponent, float, CaptureProgress, ETimelineDirection::Type, CaptureDirection);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FPointStateSparseSignature, UCapturePointComponent, OnCaptureReverse);


UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class ARMYTYCOON_API UCapturePointComponent : public UActorComponent, public ICapturePointInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "CapturePoint| Delegate")
	FPointStateChangedSignature OnPointNeutral;

	UPROPERTY(BlueprintAssignable, Category = "CapturePoint| Delegate")
	FPointStateChangedSignature OnCaptureInterrupted;

	UPROPERTY(BlueprintAssignable, Category = "CapturePoint| Delegate")
	FPointStateSparseSignature OnCaptureReverse;

	UPROPERTY(BlueprintReadWrite, Category = "CapturePoint| Delegate")
	FPointProgressChangedSignature OnProgressChanged;

	UPROPERTY(BlueprintAssignable, Category = "CapturePoint| Delegate")
	FPointCaptureChangedSignature OnPointCaptured;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint")
	float ProgressMin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint")
	float ProgressMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint")
	/* If point not captured begin reverse to being neutral */
	bool bBeginNeutralIfNCaptured;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint| Capture Rate")
	float BaseCaptureRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint| Capture Rate")
	bool bIncreseCaptureSpeedWithEachActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CapturePoint| Capture Rate")
	/* Check if Actor has implemented interface to get custom capture speed */
	bool bUseCustomCaptureSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CapturePoint| Capture Rate")
	float BaseSpeedIncreaseStepPerEachActor;

protected:

	bool bCaptured;

	UPROPERTY()
	AActor* CurrentPointOwner;

	UPROPERTY()
	int32 CurrentPointTeamID;

	UPROPERTY()
	/* List of all unique involved in capture actors */
	TMap<AActor*, int32> InvolvedInCapture;


	//	Timeline section

	UPROPERTY()
	FRichCurve BaseCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* ProgressCurve;

	UPROPERTY()
	FOnTimelineFloat OnTimelineFloat;

	UPROPERTY()
	FOnTimelineEvent OnTimelineEvent;

	FOnTimelineEventStatic OnTimelineFinishEventStatic;
	FOnTimelineEvent OnTimelineFinishEvent;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	UPROPERTY()
	float Progress;

	UPROPERTY()
	FTimeline CaptureProgressTimeline;
	//	-----

public:

	UCapturePointComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin UActorComponent Interface
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//	End   UActorComponent Interface

	//	 Begin ICapturePointInterface
	UFUNCTION(BlueprintCallable)
	virtual bool CanCapturePoint(AActor* ToCapturePoint, int32 TeamID = 0) const override;

	UFUNCTION(BlueprintCallable)
	virtual void Capture(AActor* ToCapturePoint, int32 TeamID = 0) override;

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseCapture(AActor* ToRealeseCapture) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual AActor* GetPointOwner() const override { return CurrentPointOwner; }
	//	 End   ICapturePointInterface

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CapturePoint")
	int32 GetPointTeamID() const { return CurrentPointTeamID; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CapturePoint")
	bool IsCaptured() const { return bCaptured; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CapturePoint")
	float GetCaptureProgress() const { return CaptureProgressTimeline.GetPlaybackPosition(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CapturePoint")
 	const TMap<AActor*, int32> GetInvolvedActorsPair() const { return InvolvedInCapture; }

protected:

	void OnTimeLineEventHandle();

	void OnTimelineFinishEventStaticHandle();

	virtual void Calculate();

	virtual void RecalculateCaptureRate();

	void OnInvolvedDifferentTeams();

	void OnReverse();
};