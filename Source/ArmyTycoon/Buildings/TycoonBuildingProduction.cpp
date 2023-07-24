// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Buildings/TycoonBuildingProduction.h>
#include <ArmyTycoon/Buildings/BuildingProductionComponent.h>


AProductionBuilding::AProductionBuilding(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USimpleProductionBuildingComponent>(ABuilding::BuildingComponentName))
{
	SimpleProductionBuildingComponent = StaticCast<USimpleProductionBuildingComponent*>(GetBuildingComponent());
	//ProductionBuildingComponent = StaticCast<UProductionBuildingComponent*>(GetBuildingComponent());
}