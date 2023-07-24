// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/ArmyUnits/UnitController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>



AUnitController::AUnitController(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

	BrainComponent = BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void AUnitController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	
	//PathFollowingComponent->RequestMove();
}

void AUnitController::OnUnPossess()
{
	Super::OnUnPossess();
}