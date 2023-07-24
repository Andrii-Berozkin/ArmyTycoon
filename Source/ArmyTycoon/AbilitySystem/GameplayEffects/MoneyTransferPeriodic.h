// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/MoneyTransferBasic.h>
#include "MoneyTransferPeriodic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UMoneyTransferPeriodic : public UMoneyTransferBasic
{
	GENERATED_BODY()

public:

public:

	UMoneyTransferPeriodic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};