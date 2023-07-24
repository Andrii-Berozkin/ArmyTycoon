// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameplayEffect.h>
#include "HealthDamageBasic.generated.h"



UCLASS(BlueprintType, Blueprintable)
class UHealthDamageBasic : public UGameplayEffect
{
	GENERATED_BODY()

public:

public:

	UHealthDamageBasic(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};