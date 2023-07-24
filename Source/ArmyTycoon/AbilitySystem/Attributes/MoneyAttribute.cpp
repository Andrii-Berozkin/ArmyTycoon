// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/Attributes/MoneyAttribute.h>
#include <Net/UnrealNetwork.h>



UMoneyAttributeSet::UMoneyAttributeSet()
{

}

void UMoneyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UMoneyAttributeSet, Money, COND_None, ELifetimeRepNotifyCondition::REPNOTIFY_Always);
}

void UMoneyAttributeSet::OnRep_Money(FGameplayAttributeData& OldMoney)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMoneyAttributeSet, Money, OldMoney);
}