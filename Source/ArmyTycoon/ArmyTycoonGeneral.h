// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Subsystems/WorldSubsystem.h>
#include "ArmyTycoonGeneral.generated.h"



UINTERFACE(MinimalAPI, Blueprintable)
class UTycoonTeamsInterface : public UInterface
{
	GENERATED_BODY()
};

class ITycoonTeamsInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (KeyWords = "Team TycoonTeam"))
	int32 GetTeam() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (KeyWords = "Team TycoonTeam"))
	void SetTeam(int32 NewTeamID);

};

UENUM()
namespace ETeamColor
{
	enum Type
	{
		Red,
		Green,
	};
}



UCLASS()
class ARMYTYCOON_API UTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UTeamSubsystem() 
	{
	};

	UFUNCTION(BlueprintCallable, Category = "Team")
	bool IsFromDifferentTeam(AActor* A1, AActor* A2)
	{
		ITycoonTeamsInterface* A1Team = Cast<ITycoonTeamsInterface>(A1);
		ITycoonTeamsInterface* A2Team = Cast<ITycoonTeamsInterface>(A2);

		if (A1Team && A2Team)
		{
			return A1Team->Execute_GetTeam(A1) == A2Team->Execute_GetTeam(A2) ? false : true;
		}

		return false;
	}

};