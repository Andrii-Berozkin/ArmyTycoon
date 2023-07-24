// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayEffect.h>
#include "HealthHealBasic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API UHealthHealBasic : public UGameplayEffect
{
	GENERATED_BODY()
	
public:

public:

	UHealthHealBasic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};