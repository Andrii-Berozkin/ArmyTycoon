// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/CapturePoints/CapturePointComponent.h>
#include <Containers/Map.h>



UCapturePointComponent::UCapturePointComponent(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	ProgressMin = 0.0f;
	ProgressMax = 1.0f;
	
	bCaptured = false;

	bBeginNeutralIfNCaptured = true;

	BaseCaptureRate = 1.0f;

	bIncreseCaptureSpeedWithEachActor = true;
	bUseCustomCaptureSpeed = false;
	BaseSpeedIncreaseStepPerEachActor = 0.2f;

	CurrentPointOwner = nullptr;
	CurrentPointTeamID = -1;

	//CaptureProgressTimeline = FTimeline();

	BaseCurve.AddKey(ProgressMin, ProgressMin, false, FKeyHandle());
	BaseCurve.AddKey(ProgressMax, ProgressMax, false, FKeyHandle());

	ProgressCurve = CreateDefaultSubobject<UCurveFloat>("ProgressCurve");

	ProgressCurve->FloatCurve = BaseCurve;

	OnTimelineEvent.BindUFunction(this, "OnTimeLineEventHandle");
	OnTimelineFinishEventStatic.BindUObject(this, &UCapturePointComponent::OnTimelineFinishEventStaticHandle);


}

void UCapturePointComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ProgressCurve)
	{
		float TimeMin, TimeMax;
		ProgressCurve->GetTimeRange(TimeMin, TimeMax);

		CaptureProgressTimeline = FTimeline();

		//CaptureProgressTimeline.SetFloatCurve(ProgressCurve, "ProgressTrack");
		CaptureProgressTimeline.AddInterpFloat(ProgressCurve, OnTimelineFloat, "Progress", "ProgressTrack");

		CaptureProgressTimeline.SetPropertySetObject(this);
		CaptureProgressTimeline.SetPlaybackPosition(TimeMin, false);
		CaptureProgressTimeline.SetLooping(false);
		CaptureProgressTimeline.SetPlayRate(BaseCaptureRate);
		CaptureProgressTimeline.SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		CaptureProgressTimeline.SetDirectionPropertyName("TimelineDirection");

		CaptureProgressTimeline.SetTimelinePostUpdateFunc(OnTimelineEvent);
		CaptureProgressTimeline.SetTimelineFinishedFunc(OnTimelineFinishEventStatic);
	}
}

void UCapturePointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	CaptureProgressTimeline.TickTimeline(DeltaTime);
}

bool UCapturePointComponent::CanCapturePoint(AActor* ToCapturePoint, int32 TeamID/* = 0*/) const
{
	return true;
}

void UCapturePointComponent::Capture(AActor* ToCapturePoint, int32 TeamID/* = 0*/)
{
	if (!CanCapturePoint(ToCapturePoint, TeamID)) { return; }

	InvolvedInCapture.Add(ToCapturePoint, TeamID);

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		Calculate();
	}
}

void UCapturePointComponent::ReleaseCapture(AActor* ToRealeseCapture)
{
	int32 DummyInt;
	if (InvolvedInCapture.RemoveAndCopyValue(ToRealeseCapture, DummyInt))
	{

	}

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		Calculate();
	}
}

void UCapturePointComponent::OnTimeLineEventHandle()
{
	OnProgressChanged.ExecuteIfBound(this, CaptureProgressTimeline.GetPlaybackPosition(), TimelineDirection);
}

void UCapturePointComponent::OnTimelineFinishEventStaticHandle()
{
	if (TimelineDirection == ETimelineDirection::Forward)
	{
		bCaptured = true;

		AActor* NewPointOwner = nullptr;
		int32 FirstTeamID = 0;

		//	Get firs at list TeamID
		TArray<AActor*> Owners;
		InvolvedInCapture.GetKeys(Owners);
		if (Owners.Num() > 0) 
		{
			NewPointOwner = Owners[0];
			FirstTeamID = *InvolvedInCapture.Find(Owners[0]); 
		}
		
		//CurrentPointTeamID = FirstTeamID;
		//CurrentPointOwner = NewPointOwner;
		
		OnPointCaptured.Broadcast(this, NewPointOwner, FirstTeamID);
	}

	if (TimelineDirection == ETimelineDirection::Backward)
	{
		bCaptured = false;

		OnPointNeutral.Broadcast(this);
	}

	CaptureProgressTimeline.Stop();

	Calculate();
}

void UCapturePointComponent::Calculate()
{
	if(bIncreseCaptureSpeedWithEachActor) { RecalculateCaptureRate(); }

	if (InvolvedInCapture.Num() <= 0) 
	{
		if (!bCaptured && bBeginNeutralIfNCaptured && CaptureProgressTimeline.GetPlaybackPosition() > KINDA_SMALL_NUMBER)
		{
			OnReverse();
		}
		else { CaptureProgressTimeline.Stop(); }

		return;
	}



	bool bInvolvedDifferentTeams = false;

	//	Get firs at list TeamID
	TArray<AActor*> Keys;
	InvolvedInCapture.GetKeys(Keys);
	const int32 FirstTeamID = *InvolvedInCapture.Find(Keys[0]);
	
	for (TMapBase<AActor*, int32, FDefaultSetAllocator, TDefaultMapHashableKeyFuncs<AActor*, int32, false>>::TConstIterator ItPair = InvolvedInCapture.CreateConstIterator(); ItPair ; ++ItPair)
	{
		if (ItPair->Value != FirstTeamID)
		{
			bInvolvedDifferentTeams = true;
			OnInvolvedDifferentTeams();
			return;
			break;
		}
	}


	if (!bInvolvedDifferentTeams)
	{
		if (!bCaptured && CaptureProgressTimeline.GetPlaybackPosition() < KINDA_SMALL_NUMBER)
		{
			CurrentPointOwner = Keys[0];
			CurrentPointTeamID = FirstTeamID;
		}

		if (CurrentPointTeamID == FirstTeamID)
		{
			CaptureProgressTimeline.Play();
		}

		if (CurrentPointTeamID != FirstTeamID)
		{
			OnReverse();
		}

		if (bCaptured && CurrentPointTeamID == FirstTeamID)
		{
			CaptureProgressTimeline.Stop();
		}
	}

}

void UCapturePointComponent::RecalculateCaptureRate()
{
	const float BasePlayRate = 1.0f;

	TArray<AActor*> InvolvedActors;
	InvolvedInCapture.GetKeys(InvolvedActors);

	float NewSpeedRate = BasePlayRate;

	for (AActor* Actor : InvolvedActors)
	{
		if (bUseCustomCaptureSpeed)
		{
			if (ICustomCaptureSpeedIncreaseInterface* CustomIncrease = Cast<ICustomCaptureSpeedIncreaseInterface>(Actor))
			{
				NewSpeedRate += CustomIncrease->GetCustomCaptureSpeedIncrease();
			}
		}
		else
		{
			NewSpeedRate += BaseSpeedIncreaseStepPerEachActor;
		}
	}

	CaptureProgressTimeline.SetPlayRate(NewSpeedRate);
}

void UCapturePointComponent::OnInvolvedDifferentTeams()
{
	CaptureProgressTimeline.Stop();
	OnCaptureInterrupted.Broadcast(this);
}

void UCapturePointComponent::OnReverse()
{
	if (!CaptureProgressTimeline.IsReversing())
	{
		CaptureProgressTimeline.Reverse();
		OnCaptureReverse.Broadcast();
		bCaptured = false;
	}
}