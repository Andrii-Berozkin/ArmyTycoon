// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Character/ArmyTycoonPlayerController.h>



AArmyTycoonPlayerController::AArmyTycoonPlayerController(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	bEnableClickEvents = true;
	bEnableTouchEvents = true;

	bShowMouseCursor = true;
}