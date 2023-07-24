// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Buildings/BuildingProductionComponent.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>



UProductionBuildingComponent::UProductionBuildingComponent(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bRunOnAnyThread = true;
	PrimaryComponentTick.bTickEvenWhenPaused = false;

	ProduceCount = 1.0f;
	BaseTimeToProduce = 15.0f;

	TagsToBlockProduction.AddTag(FGE_ProduceBlocked);
	TagsToBlockProduction.AddTag(FGE_BuildingUpgradeInProgress);
	TagsToBlockProduction.AddTag(FGE_UpgradeInProgress);

	ProductionRate = 0.0f;
	Effectiveness = 0.0f;
	ActualTimeToProduce = BaseTimeToProduce;

	BaseMinEffectiveness = 0.0f;
	BaseMaxEffectiveness = 100.0f;
	BonusEffectiveness = 0.0f;

	Progress = 0.0f;
	ProgressRate = 0.0f;

	bProductionMarkDirty = true;
}

void UProductionBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Effectiveness != BaseMaxEffectiveness + BonusEffectiveness)
	{
		Effectiveness += EffectivenessRate * DeltaTime;
	}

	if (bProductionMarkDirty)
	{
		RecalculateEffectiveness();
		CalculateProductionRate();

		ProgressRate = (1.0f / ActualTimeToProduce);

		bProductionMarkDirty = false;
	}

	float AddValue = ProgressRate * DeltaTime;

	if (!AbilitySystem->HasAnyMatchingGameplayTags(TagsToBlockProduction))
	{
		AddProgress(AddValue);
	}
}

void UProductionBuildingComponent::BeginPlay()
{
	AActor* Owner = GetOwner();
	UWorld* WorldPtr = Owner->GetWorld();
}

void UProductionBuildingComponent::Produce()
{
	if (!AbilitySystem || !GetOwner()->HasAuthority() || ProduceAbilities.Num() <= 0) { return; }

	for (FGameplayAbilitySpec Ability : ProduceAbilities)
	{
		AbilitySystem->GiveAbilityAndActivateOnce(Ability);
	}

}

void UProductionBuildingComponent::RecalculateEffectiveness()
{
	float NewMaxEffectiveness = BaseMaxEffectiveness + BonusEffectiveness;

	Effectiveness = FMath::Clamp(Effectiveness, BaseMinEffectiveness, BaseMaxEffectiveness);
}

void UProductionBuildingComponent::CalculateProductionRate()
{
	ActualTimeToProduce = BaseTimeToProduce * (Effectiveness / 100.0f);
	ProductionRate = ActualTimeToProduce * ProduceCount;
}

void UProductionBuildingComponent::OnProgressComplete()
{
	Produce();
	Progress = 0.0f;
}



USimpleProductionBuildingComponent::USimpleProductionBuildingComponent(const FObjectInitializer& ObjectInitializer/*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	TimeToProduce = 10.0f;

	GatheringPoint = FTransform::Identity;

	ProduceDelegate.BindUObject(this, &USimpleProductionBuildingComponent::Produce);
}

void USimpleProductionBuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ProduceTimer, ProduceDelegate, TimeToProduce, true);

	if (AbilitySystem)
	{
		AbilitySystem->RegisterGameplayTagEvent(FGE_UpgradeInProgress, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::BlockProduction);
	}
}

void USimpleProductionBuildingComponent::Produce()
{
	if (AbilitySystem && GetOwner()->HasAuthority())
	{
		int32 GameplayEffectLevel = 1;

		for (FGameplayAbilitySpecDef AbilityDef : ProductionAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityDef, GameplayEffectLevel);

			AbilitySystem->GiveAbilityAndActivateOnce(AbilitySpec);
		}
	}
}

void USimpleProductionBuildingComponent::BlockProduction(const FGameplayTag CallbackTag, int32 NewStackCount)
{
	bool bTagAdded = false;
	if (AbilitySystem && CallbackTag.IsValid())
	{
		bTagAdded = AbilitySystem->HasMatchingGameplayTag(CallbackTag);
	}
	UWorld* WorldPtr = GetWorld();



	//	Block production
	if (bTagAdded)
	{
		FGameplayTagContainer BlockTags;
		BlockTags.AddTag(FGE_ProduceBlocked);

		if (AbilitySystem && GetOwner()->HasAuthority())
		{
			AbilitySystem->AddMinimalReplicationGameplayTags(BlockTags);
		}

		if(ProduceTimer.IsValid()){ WorldPtr->GetTimerManager().PauseTimer(ProduceTimer); }
	}
	


	//	Unlock production
	if (!bTagAdded)
	{
		FGameplayTagContainer ToRemoveTags;
		ToRemoveTags.AddTag(FGE_ProduceBlocked);

		if (AbilitySystem && GetOwner()->HasAuthority())
		{
			AbilitySystem->RemoveMinimalReplicationGameplayTags(ToRemoveTags);
		}

		if (ProduceTimer.IsValid()) { WorldPtr->GetTimerManager().UnPauseTimer(ProduceTimer); }
	}
}