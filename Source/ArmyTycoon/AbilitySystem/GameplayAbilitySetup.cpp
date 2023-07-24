// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/AbilitySystem/GameplayAbilitySetup.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>
#include <Engine/AssetManager.h>


UAbilitySet::UAbilitySet(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	
}

void UAbilitySet::GrantAbilitySetToComponent(UAbilitySystemComponent* ASC, FAbilitySetHandle& OutAbilitySetHandle) const
{
	check(ASC);
	//check(AbilitySet);
	
	UAssetManager* AssetManager = UAssetManager::GetIfValid();
	//AssetManager->

	if (!ASC->IsOwnerActorAuthoritative() || !AssetManager) { return; }

	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(ASC->GetAvatarActor());

	for (TSubclassOf<UAttributeSet> Attribute : GrantedAttributes)
	{
		UAttributeSet* NewAttribute = NewObject<UAttributeSet>(ASC->GetAvatarActor(), Attribute);

		if (NewAttribute)
		{
			OutAbilitySetHandle.Attributes.Add(ASC->AddAttributeSetSubobject(NewAttribute));
		}
	}


	//	Grant GameplayEffects
	for (FGameplayEffectSetup EffectSetup : GrantedEffects)
	{
		FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectSetup.GameplayEffectClass, EffectSetup.EffectLevel, Context);

		if (EffectSpec.IsValid())
		{
			OutAbilitySetHandle.ActiveEffects.Add(ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get(), FPredictionKey()));
		}
	}


	//	Grant GameplayAbilities
	for (FGameplayAbilitySetup AbilitySetup : GrantedAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilitySetup.GameplayAbilityClass, AbilitySetup.AbilityLevel, INDEX_NONE, (UObject*)nullptr);
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySetup.InputID);

		OutAbilitySetHandle.AbilitiesHandle.Add(ASC->GiveAbility(AbilitySpec));
	}
}