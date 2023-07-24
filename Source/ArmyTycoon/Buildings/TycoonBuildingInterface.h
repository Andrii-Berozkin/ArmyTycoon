// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "TycoonBuildingInterface.generated.h"



UINTERFACE(MinimalAPI, Blueprintable)
class UTycoonConstructionSiteInteface : public UInterface
{
	GENERATED_BODY()
};

class ITycoonConstructionSiteInteface
{
	GENERATED_BODY()

public:

	virtual bool Build() = 0;

	virtual void Destroy() = 0;
};

UINTERFACE(MinimalAPI, Blueprintable)
class UTycoonBuildingInterface : public UInterface
{
	GENERATED_BODY()
};

class ITycoonBuildingInterface
{
	GENERATED_BODY()

public:

	virtual float GetCost() const = 0;

};


UINTERFACE(MinimalAPI, Blueprintable)
class UTycoonProductionBuildingInterface : public UTycoonBuildingInterface
{
	GENERATED_BODY()
};

class ITycoonProductionBuildingInterface : public ITycoonBuildingInterface
{
	GENERATED_BODY()
	
public:

	virtual void Produce() {};

};