// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <Net/UnrealNetwork.h>
#include <GameplayEffectExtension.h>
#include <GameplayEffectTypes.h>




UHealthAttributeSet::UHealthAttributeSet()
{

}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, MinHealth, COND_None, REPNOTIFY_OnChanged);
}

void UHealthAttributeSet::OnRep_Health(FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, OldHealth);
}

void UHealthAttributeSet::OnRep_MaxHealth(FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MaxHealth, OldMaxHealth);
}

void UHealthAttributeSet::OnRep_MinHealth(FGameplayAttributeData& OldMinHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, MinHealth, OldMinHealth);
}

bool UHealthAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data)) { return false; }

	if (Data.Target.HasMatchingGameplayTag(FHealth_Cheats_GodMode))
	{
		Data.EvaluatedData.Magnitude = 0.0f;
		return false;
	}

	return true;
}

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (GetHealth() <= GetMinHealth())
	{
		const FGameplayEffectContextHandle& EffectHandle = Data.EffectSpec.GetEffectContext();
		AActor* Initigator = EffectHandle.GetOriginalInstigator();
		HealthReachMinimal.Broadcast(Initigator, Data.EffectSpec, Data.EvaluatedData.Magnitude);
	}
}

void UHealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UHealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UHealthAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		 NewValue = FMath::Clamp<float>(NewValue, GetMinHealth(), GetMaxHealth());
	}
}