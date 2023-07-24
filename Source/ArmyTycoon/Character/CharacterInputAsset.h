// Copyright Andrii Berozkin, Inc. All Rights Reserved.
#pragma once



#include <CoreMinimal.h>
#include <Engine/DataAsset.h>
#include <EnhancedInput/Public/InputAction.h>
#include "CharacterInputAsset.generated.h"



UCLASS(BlueprintType, Blueprintable, Const)
class ARMYTYCOON_API UCharacterInputAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInputs")
	UInputAction* Move;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInputs")
	UInputAction* Look;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInputs")
	UInputAction* LookZoom;

	UPROPERTY(EditDefaultsOnly, Category = "EnhancedInputs")
	UInputAction* LookEnable;

};