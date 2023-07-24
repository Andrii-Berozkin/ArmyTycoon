// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayEffects/MoneyTransferBasic.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/AbilitySystem/Attributes/MoneyAttribute.h>



UMoneyTransferBasic::UMoneyTransferBasic(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	FGameplayModEvaluationChannelSettings ChannelSettings;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UMoneyAttributeSet::GetMoneyAttribute();
	ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(1.0f));
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	Modifiers.Add(ModifierInfo);

	//InheritableGameplayEffectTags.CombinedTags.AddTag(FGE_MoneyGet);
	//InheritableGameplayEffectTags.Added.AddTag(FGE_MoneyGet);
}