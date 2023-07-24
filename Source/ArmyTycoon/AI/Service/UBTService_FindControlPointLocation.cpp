// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AI/Service/UBTService_FindControlPointLocation.h>
#include <ArmyTycoon/CapturePoints/CapturePointBasic.h>
#include <ArmyTycoon/CapturePoints/CapturePointComponent.h>
#include <ArmyTycoon/GameModes/ControlPoint_ArmyTycoon_GM.h>
#include <ArmyTycoon/ArmyTycoonGeneral.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <Components/SphereComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <NavigationSystem.h>
#include <AIController.h>



UBTService_FindControlPointLocation::UBTService_FindControlPointLocation(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	

	bRandomizeLocationAtNavRadius = false;

	PointLocation.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindControlPointLocation, PointLocation));
}


void UBTService_FindControlPointLocation::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* MemoryNode)
{
	Super::OnBecomeRelevant(OwnerComp, MemoryNode);

	UWorld* WorldPtr = GetWorld();
	AIController = OwnerComp.GetAIOwner();
	ActorOwner = AIController->GetPawn();

	AControlPoint_ArmyTycoon_GM* ArmyTycoonGM = Cast<AControlPoint_ArmyTycoon_GM>(WorldPtr->GetAuthGameMode());
	AActor* SelectedControlPoint = nullptr;
	FNavLocation ControlPointNavLocation;

	if (!ActorOwner || !AIController) { return; }

	if (ArmyTycoonGM)
	{
		TArray<AActor*> ControlPoints = ArmyTycoonGM->GetControlPoints();
		TArray<AActor*> FilteredControlPoints;

		FilterCapturedPoints(ControlPoints, AIController->GetPawn(), FilteredControlPoints);

		if (FilteredControlPoints.Num() > 0)
		{
			SelectedControlPoint = GetClosestActor(AIController->GetPawn()->GetActorLocation(), FilteredControlPoints);
		}
		
	}

	if (SelectedControlPoint) { ControlPointNavLocation.Location = SelectedControlPoint->GetActorLocation(); }

	ACapturePointBasic* CapturePoint = Cast<ACapturePointBasic>(SelectedControlPoint);

	if (bRandomizeLocationAtNavRadius && CapturePoint)
	{
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(WorldPtr);
		ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance();
		FSharedConstNavQueryFilter NavQueryFilter = 0;

		if (NavData) { NavQueryFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, AIController->GetDefaultNavigationFilterClass()); }
		
		const USphereComponent* SphereCollision = Cast<USphereComponent>(CapturePoint->GetCollisionComponent());

		if (SphereCollision)
		{
			NavSystem->GetRandomPointInNavigableRadius(CapturePoint->GetActorLocation(), SphereCollision->GetScaledSphereRadius(), ControlPointNavLocation, NavData, NavQueryFilter);
		}
	}

	if (ControlPointNavLocation.Location != FVector::ZeroVector)
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(PointLocation.SelectedKeyName, ControlPointNavLocation.Location);
	}
}

void UBTService_FindControlPointLocation::FilterCapturedPoints(TArray<AActor*> ListOfPoints, AController* Controller, OUT TArray<AActor*>& OutArr)
{
	FilterCapturedPoints(ListOfPoints, Controller->GetPawn(), OutArr);
}

void UBTService_FindControlPointLocation::FilterCapturedPoints(TArray<AActor*> ListOfPoints, AActor* OwnerActor, OUT TArray<AActor*>& OutArr)
{
	UTeamSubsystem* TeamSubsytem = GetWorld()->GetSubsystem<UTeamSubsystem>();

	OutArr.Empty();

	if (ListOfPoints.Num() <= 0) { return; }

	for (int32 i = 0, iMax = ListOfPoints.Num(); i < iMax; i++)
	{
		if (ACapturePointBasic* CapturePoint = Cast<ACapturePointBasic>(ListOfPoints[i]))
		{
			//	Check if not captured
			if (!CapturePoint->IsCaptured())
			{
				OutArr.Add(ListOfPoints[i]);
				continue;
			}

			//	In common will be a 'ABasicUnit'
			AActor* PointOwner = CapturePoint->GetCapturePointComponent()->GetPointOwner();

			//	Check if captured by other team
			if (CapturePoint->IsCaptured() && TeamSubsytem->IsFromDifferentTeam(OwnerActor, PointOwner))
			{
				OutArr.Add(ListOfPoints[i]);
				continue;
			}

		}
	}
}

AActor* UBTService_FindControlPointLocation::GetClosestActor(FVector TargetLocation, TArray<AActor*> ActorList)
{
	float LastClosestDistance = INT16_MAX;
	int32 ClosestIndex = 0;

	if (ActorList.Num() <= 0) { return nullptr; }

	for (int32 i = 0, iMax = ActorList.Num(); i < iMax; i++)
	{
		float Distance = (ActorList[i]->GetActorLocation() - TargetLocation).Size();

		if (Distance < LastClosestDistance)
		{
			LastClosestDistance = Distance;
			ClosestIndex = i;
		}
	}

	return ActorList[ClosestIndex];
}