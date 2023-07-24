// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Buildings/TycoonBuilding.h>
#include <GameplayAbilities/Public/GameplayEffect.h>
#include <GameplayAbilities/Public/Abilities/GameplayAbility.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/AbilitySystem/GameplayEffects/UpgradeCostBasic.h>

//	UI
#include <Blueprint/UserWidget.h>

#include <ArmyTycoon/Character/TycoonCharacter.h>




const FName ABuilding::AbilitySystemName("AbilitySystemComponent");
const FName ABuilding::BuildingComponentName("BuildingComponent");
const FName ABuilding::BuildingAttributeSetName("BuildingAttributeSet");



ABuilding::ABuilding(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(AbilitySystemName, false);
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->ReplicationMode = EGameplayEffectReplicationMode::Minimal;

	BuildingSet = CreateDefaultSubobject<UBuildingAttributeSet>(BuildingAttributeSetName, false);	
	

	BuildingComponent = CreateDefaultSubobject<UBuildingComponent>(BuildingComponentName, false);
	BuildingComponent->InitializeWithAbilitySystem(AbilitySystem);

	TeamID = 0;

	OnClicked.AddDynamic(this, &ThisClass::OnClickedHandle);
	OnInputTouchBegin.AddDynamic(this, &ThisClass::OnInputTouchBeginHandle);
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystem) { AbilitySystem->InitAbilityActorInfo(this, this); }

	if (AbilitySystem)
	{
		InitializeAttributes();
		InitializeStartupEffects();
		InitializeStartupAbilities();
	}
}

void ABuilding::Reset()
{
	Super::Reset();

	if (AbilitySystem) { AbilitySystem->InitAbilityActorInfo(this, this); }


	if (AbilitySystem)
	{
		InitializeAttributes();
		InitializeStartupEffects();
		InitializeStartupAbilities();
	}
}

void ABuilding::InitializeAttributes()
{
	BuildingSet->InitBuildingLevel(0.0f);

	if (BuildingSet && AbilitySystem)
	{
		AbilitySystem->GetGameplayAttributeValueChangeDelegate(UBuildingAttributeSet::GetBuildingLevelAttribute()).AddUObject(this, &ThisClass::OnBuildingLevelAttibuteChanges);
	}
}

void ABuilding::InitializeStartupEffects()
{
	if (AbilitySystem && HasAuthority())
	{
		FGameplayEffectContextHandle Context = AbilitySystem->MakeEffectContext();
		Context.AddSourceObject(this);

		for (TPair<TSubclassOf<UGameplayEffect>, float> Pair : StartupEffects)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(Pair.Key, Pair.Value, Context);

			AbilitySystem->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), AbilitySystem);
		}
	}
}

void ABuilding::InitializeStartupAbilities()
{
	int32 BasicaAbilityLevel = 1;

	if (AbilitySystem && HasAuthority())
	{
		for (FGameplayAbilitySpecDef AbilityDef : StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityDef, BasicaAbilityLevel);

			AbilitySystem->GiveAbility(AbilitySpec);
		}
	}
}

void ABuilding::OnBuildingLevelAttibuteChanges(const FOnAttributeChangeData& Data)
{
	OnBuildingLevelChange.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}

float ABuilding::GetUpgradeCost() const
{
	if (AbilitySystem && BuildingSet)
	{
		float NextLvl = BuildingSet->GetBuildingLevel() + 1.0f;
		for (FLevelCost LevelCost : UpgradeCost)
		{
			if (LevelCost.Level == NextLvl) { return LevelCost.Cost; }
		}
	}
	return 0.0f;
}

float ABuilding::GetDowngradeCost() const
{
	if (AbilitySystem && BuildingSet)
	{
		float PreviousLevel = BuildingSet->GetBuildingLevel() - 1.0f;
		for (FLevelCost LevelCost : UpgradeCost)
		{
			if (LevelCost.Level == PreviousLevel) { return LevelCost.Cost; }
		}
	}
	return 0.0f;
}

void ABuilding::OnClickedHandle(AActor* TouchedActor, FKey ButtonPressed)
{
	ATycoonCharacter* TycoonChar = GetBuildingOwner() != nullptr ? Cast<ATycoonCharacter>(GetBuildingOwner()) : Cast<ATycoonCharacter>(GetOwner());

	if (TycoonChar != nullptr && TycoonChar->GetTeam() == GetTeam())
	{
		TycoonChar->SetSelectedActor(this);

		ShowBuildingUI(TouchedActor);
	}
}

void ABuilding::OnInputTouchBeginHandle(ETouchIndex::Type FingerIndex, AActor* TouchedActor)
{
	ATycoonCharacter* TycoonChar = GetBuildingOwner() != nullptr ? Cast<ATycoonCharacter>(GetBuildingOwner()) : Cast<ATycoonCharacter>(GetOwner());

	if (TycoonChar != nullptr && TycoonChar->GetTeam() == GetTeam())
	{
		TycoonChar->SetSelectedActor(this);

		ShowBuildingUI(TouchedActor);
	}
}

void ABuilding::ShowBuildingUI(AActor* TouchedActor)
{
	if (BuildingWidgetAsset && BuildingWidgetAsset->PopUpWidget)
	{
		PopUpWidgetHandle = CreateWidget<UUserWidget>(GetWorld(), BuildingWidgetAsset->PopUpWidget);

		if (PopUpWidgetHandle) { PopUpWidgetHandle->AddToViewport(0); }//PopUpWidgetHandle->SetFocus();}
	}
}

void ABuilding::HideBuildingUI()
{
	if (BuildingWidgetAsset && BuildingWidgetAsset->PopUpWidget)
	{
		if (PopUpWidgetHandle)
		{
			PopUpWidgetHandle->RemoveFromViewport();
		}
	}
}