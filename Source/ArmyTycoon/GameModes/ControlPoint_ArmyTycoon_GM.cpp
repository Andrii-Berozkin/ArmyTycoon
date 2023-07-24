// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/GameModes/ControlPoint_ArmyTycoon_GM.h>
#include <EngineUtils.h>
#include <ArmyTycoon/CapturePoints/CapturePointBasic.h>



AControlPoint_ArmyTycoon_GM::AControlPoint_ArmyTycoon_GM(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	
	//	ControlPoints
	bAllowAutoScanControlPoint = true;
}

void AControlPoint_ArmyTycoon_GM::BeginPlay()
{
	Super::BeginPlay();

	UWorld* WorldPtr = GetWorld();

	if (bAllowAutoScanControlPoint)
	{
		for (TActorIterator<AActor> It(WorldPtr, ACapturePointBasic::StaticClass()); It; ++It)
		{
			ControlPoints.Add(*It);
		}
	}

}