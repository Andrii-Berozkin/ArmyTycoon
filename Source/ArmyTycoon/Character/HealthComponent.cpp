// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Character/HealthComponent.h>
#include <AbilitySystemComponent.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>
#include <GameplayEffectExtension.h>



UHealthComponent::UHealthComponent(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

}

void UHealthComponent::InitializerWithAbilitySystem(UAbilitySystemComponent* AbilitySystemComponent)
{
	AActor* Owner = GetOwner();

	if (!AbilitySystemComponent) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent of %s not found"), *GetNameSafe(Owner));
		return; 
	}
	AbilitySystem = AbilitySystemComponent;
	HealthSet = AbilitySystemComponent->GetSet<UHealthAttributeSet>();
	if (!HealthSet)
	{
		UE_LOG(LogTemp, Error, TEXT("%s doesn`t have UHealthAttributeSet as DefaultSubobject"), *GetNameSafe(Owner));
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute()).AddUObject(this, &UHealthComponent::OnHealthChangedHandle);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UHealthComponent::OnMaxHealthChangedHandle);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetMinHealthAttribute()).AddUObject(this, &UHealthComponent::OnMinHealthChangedHandle);

	HealthSet->HealthReachMinimal.AddUObject(this, &ThisClass::ReachMinimalHandle);
}

void UHealthComponent::UninitializeFromAbilitySystem()
{
	AbilitySystem = nullptr;
	HealthSet = nullptr;
}

void UHealthComponent::ReachMinimalHandle(AActor* AttributeOwner, const FGameplayEffectSpec& EffectSpec, float EffectMagnitude)
{
	OnDeath.Broadcast();
}

float UHealthComponent::GetCurrentHealth() const
{
	return HealthSet ? HealthSet->GetHealth() : 0.0f;
}

void UHealthComponent::OnHealthChangedHandle(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChange.Broadcast(this, UHealthComponent::GetOriginalInistigatorFromModData(ChangeData.GEModData), ChangeData.OldValue, ChangeData.NewValue);
}

void UHealthComponent::OnMaxHealthChangedHandle(const FOnAttributeChangeData& ChangeData)
{
	OnMaxHealthChange.Broadcast(this, UHealthComponent::GetOriginalInistigatorFromModData(ChangeData.GEModData), ChangeData.OldValue, ChangeData.NewValue);
}

void UHealthComponent::OnMinHealthChangedHandle(const FOnAttributeChangeData& ChangeData)
{
	OnMinHealthChange.Broadcast(this, UHealthComponent::GetOriginalInistigatorFromModData(ChangeData.GEModData), ChangeData.OldValue, ChangeData.NewValue);
}

AActor* UHealthComponent::GetOriginalInistigatorFromModData(const FGameplayEffectModCallbackData* ModData)
{
	if (ModData)
	{
		const FGameplayEffectContextHandle& ContextHandle = ModData->EffectSpec.GetContext();
		return ContextHandle.GetOriginalInstigator();
	}
	return nullptr;
}