// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Buildings/BuildingComponent.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>
#include <ArmyTycoon/AbilitySystem/Attributes/BuildingAttribute.h>



UBuildingComponent::UBuildingComponent(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	UpgradeCost = 0.0f;
}

void UBuildingComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	AActor* Owner = GetOwner();
	check(Owner);

	AbilitySystem = AbilitySystemComponent;

	if (!AbilitySystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot Initializer AbilitySystemComponent with owner %s with NULL"), *Owner->GetName());
	}


	//	if has BuildingAttributeSet add delegate to listen Building Lvl
	for (UAttributeSet* Set : AbilitySystem->GetSpawnedAttributes())
	{
		if (UBuildingAttributeSet* BuildingSet = Cast<UBuildingAttributeSet>(Set))
		{
			AbilitySystem->GetGameplayAttributeValueChangeDelegate(UBuildingAttributeSet::GetBuildingLevelAttribute()).AddUObject(this, &ThisClass::OnAttributeLevelChanged);
		}
	}
}

void UBuildingComponent::UninitializeFromAbilitySystem()
{
	AbilitySystem = nullptr;
}

int32 UBuildingComponent::GetTeam() const
{
	ITycoonTeamsInterface* TeamInterface = Cast<ITycoonTeamsInterface>(GetOwner());

	return  TeamInterface != nullptr ? TeamInterface->Execute_GetTeam(GetOwner()) : 0;
}

void UBuildingComponent::SetTeam(int32 NewTeamID)
{
	if (ITycoonTeamsInterface* TeamInterface = Cast<ITycoonTeamsInterface>(GetOwner())) { TeamInterface->Execute_SetTeam(GetOwner(), NewTeamID); }
}

void UBuildingComponent::OnAttributeLevelChanged(const FOnAttributeChangeData& Data)
{
}