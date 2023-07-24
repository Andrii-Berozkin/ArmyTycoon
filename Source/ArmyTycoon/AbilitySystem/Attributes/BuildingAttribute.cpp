// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Attributes/BuildingAttribute.h>
#include <Net/UnrealNetwork.h>


UBuildingAttributeSet::UBuildingAttributeSet()
{

}

void UBuildingAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBuildingAttributeSet, BuildingLevel, COND_None, ELifetimeRepNotifyCondition::REPNOTIFY_Always);
}

void UBuildingAttributeSet::OnRep_BuildingLevel(FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBuildingAttributeSet, BuildingLevel, OldValue);
}