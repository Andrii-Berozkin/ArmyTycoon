// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/CapturePoints/CapturePointBasic.h>
#include <ArmyTycoon/CapturePoints/CapturePointComponent.h>
#include <Components/SphereComponent.h>
#include <ArmyTycoon/ArmyTycoonGeneral.h>




const FName ACapturePointBasic::CapturePointComponentName("CapturePointComponent");
const FName ACapturePointBasic::CollisionComponentName("CollisionComponent");

ACapturePointBasic::ACapturePointBasic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

	CapturePointComponent = StaticCast<UCapturePointComponent*>(CreateDefaultSubobject(CapturePointComponentName,
		UCapturePointComponent::StaticClass(), UCapturePointComponent::StaticClass(), true, false));


	CollisionComponent = StaticCast<USphereComponent*>(CreateDefaultSubobject(CollisionComponentName,
		USphereComponent::StaticClass(), USphereComponent::StaticClass(), true, false));

	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->SetCollisionProfileName("OverlapAll", true);

}

void ACapturePointBasic::BeginPlay()
{
	CollisionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACapturePointBasic::OnComponentBeginOverlapHandle);
	CollisionComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnComponentEndOverlapHandle);

	CapturePointComponent->OnPointCaptured.AddUniqueDynamic(this, &ThisClass::OnPointCapturedHandle);

	Super::BeginPlay();
}

void ACapturePointBasic::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool ACapturePointBasic::IsCaptured() const
{
	return CapturePointComponent->IsCaptured();
}

void ACapturePointBasic::OnPointCapturedHandle(UCapturePointComponent* CapturePointComponentInistigator, AActor* NewOwner, int32 TeamID)
{
	CapturePointComponent->IsCaptured();
}

void ACapturePointBasic::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (!CapturePointComponent || !OverlappedComponent || !OtherActor) { return; }

	int32 TeamId = 0;

	//if (ITycoonTeamsInterface* TeamActor = Cast<ITycoonTeamsInterface>(OtherActor)) { TeamId = TeamActor->Execute_GetTeam(OtherActor); }

	
	if (OtherActor->Implements<UTycoonTeamsInterface>())
	{
		ITycoonTeamsInterface* TeamActor = Cast<ITycoonTeamsInterface>(OtherActor);
		TeamId = TeamActor->Execute_GetTeam(OtherActor);
	}
	
	CapturePointComponent->Capture(OtherActor, TeamId);
}

void ACapturePointBasic::OnComponentEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CapturePointComponent->ReleaseCapture(OtherActor);
}