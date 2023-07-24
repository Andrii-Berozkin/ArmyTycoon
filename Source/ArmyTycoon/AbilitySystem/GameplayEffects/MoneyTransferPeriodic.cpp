// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/MoneyTransferPeriodic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>



UMoneyTransferPeriodic::UMoneyTransferPeriodic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	Period = FScalableFloat(10.0f);

	//InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_MoneyGetPeriodic);
	//InheritableGameplayEffectTags.Added.AddTag(FGE_MoneyGetPeriodic);
}