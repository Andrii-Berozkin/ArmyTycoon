// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/ArmyUnits/BasicUnit.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/PawnMovementComponent.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>
#include <ArmyTycoon/Character/HealthComponent.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>
#include <ArmyTycoon/ArmyUnits/UnitController.h>
#include <ArmyTycoon/AbilitySystem/ArmyTycoonTags.h>
#include <ArmyTycoon/Weapons/WeaponDefinition.h>
#include <ArmyTycoon/AbilitySystem/GameplayAbilitySetup.h>



const FName ABasicUnit::CharacterMovementComponentName("CharacterMovementComponent");
const FName ABasicUnit::PawnMovementComponentName("PawnMovementComponent");
const FName ABasicUnit::AbilitySystemComponentName("AbilitySystemComponent");
const FName ABasicUnit::HealtComponentName("HealthComponent");
const FName ABasicUnit::HealthSetName("HealthSet");

ABasicUnit::ABasicUnit(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	//CharacterMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(CharacterMovementComponentName, false);
	//CharacterMovementComponent->UpdatedComponent = RootComponent;

	//PawnMovementComponent = CreateDefaultSubobject<UPawnMovementComponent>(PawnMovementComponentName, false);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(AbilitySystemComponentName, false);

	//InitializeAttributes
	HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(HealthSetName, false);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(HealtComponentName, false);

	AIControllerClass = AUnitController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TeamID = 0;

	InitialMinHealth = 0.0f;
	InitialMaxHealth = 10.0f;
	InitialHealth = 10.0f;

}

void ABasicUnit::InitializeAttributes()
{
	if (HealthSet)
	{
		HealthSet->InitMinHealth(InitialMinHealth);
		HealthSet->InitMaxHealth(InitialMaxHealth);
		HealthSet->InitHealth(InitialHealth);
	}
}

void ABasicUnit::InitializeStartupEffects()
{
	float BaseEffectLvl = 1.0f;

	if (AbilitySystemComponent && HasAuthority())
	{
		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);

		for (TSubclassOf<UGameplayEffect> Effect : StartupEffects)
		{
			FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(Effect, BaseEffectLvl, Context);

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
		}
	}
}

void ABasicUnit::InitializeStartupAbilities()
{
	int32 BaseAbilityLvl = 1;

	if (AbilitySystemComponent && HasAuthority())
	{
		for (FGameplayAbilitySpecDef AbilityDef : StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityDef, BaseAbilityLvl);

			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void ABasicUnit::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent) { AbilitySystemComponent->InitAbilityActorInfo(this, this); }

	if (AbilitySystemComponent)
	{
		InitializeAttributes();
		InitializeStartupEffects();
		InitializeStartupAbilities();
	}
}

void ABasicUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HealthSet && HealthComponent) { HealthComponent->InitializerWithAbilitySystem(AbilitySystemComponent); }
}

void ABasicUnit::Restart()
{
	Super::Restart();

	if (AbilitySystemComponent) { AbilitySystemComponent->InitAbilityActorInfo(this, this); }

	if (AbilitySystemComponent)
	{
		InitializeAttributes();
		InitializeStartupEffects();
		InitializeStartupAbilities();
	}
}

void ABasicUnit::GiveWeapon(TSubclassOf<UWeaponDefinition> WeaponDefinition)
{
	const UWeaponDefinition* WeaponInst = GetDefault<UWeaponDefinition>(WeaponDefinition);//WeaponDefinition.GetDefaultObject();
	
	check(WeaponInst);


	FAbilitySetHandle OutSetHandle;

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.bDeferConstruction = false;
		SpawnParameters.Instigator = this;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Weapon = GetWorld()->SpawnActor(WeaponInst->WeaponSetup.WeaponActor, &GetActorTransform(), SpawnParameters);

		if (Weapon)
		{
			//Weapon->FinishSpawning(GetActorTransform(), false);

			//Weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponInst->WeaponSetup.SocketToAttach);
			Weapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponInst->WeaponSetup.SocketToAttach);
			Weapon->SetActorRelativeTransform(WeaponInst->WeaponSetup.RelativeTransform);
		}

	}

	for (TSoftObjectPtr<const UAbilitySet> AbilitySet : WeaponInst->GrantedAbilitySets)
	{
		if (!AbilitySet.IsValid()) { AbilitySet.LoadSynchronous(); }
		
		AbilitySet->GrantAbilitySetToComponent(AbilitySystemComponent, OutSetHandle);
	}

	AttackAbilitiesHandle.Append(OutSetHandle.AbilitiesHandle);

	AbilitySystemComponent->AddMinimalReplicationGameplayTag(FGE_Weapon_HasAnyWeapon);
}

void ABasicUnit::RemoveAllWeapon()
{
	if (Weapon) { Weapon->Destroy(); }
	Weapon = nullptr;

	if (AttackAbilitiesHandle.Num() > 0)
	{
		for (FGameplayAbilitySpecHandle AbilitySpecHandle : AttackAbilitiesHandle)
		{
			AbilitySystemComponent->CancelAbilityHandle(AbilitySpecHandle);
		}
	}

	AttackAbilitiesHandle.Empty();

	AbilitySystemComponent->RemoveMinimalReplicationGameplayTag(FGE_Weapon_HasAnyWeapon);
}

bool ABasicUnit::HasAbilityToAttack() const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer WeaponBlockedTags;
		WeaponBlockedTags.AddTag(FGE_Weapon_NoFire);

		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(WeaponBlockedTags)) { return false; }

		FGameplayTagContainer AnyWeaponTags;
		AnyWeaponTags.AddTag(FGE_Weapon_HasAnyWeapon);

		if (AbilitySystemComponent->HasAnyMatchingGameplayTags(AnyWeaponTags)) { return true; }
	}
	return true;
}

void ABasicUnit::Attack()
{
	if (AbilitySystemComponent)
	{
		for (FGameplayAbilitySpecHandle AbilitySpecHandle : AttackAbilitiesHandle)
		{
			AbilitySystemComponent->TryActivateAbility(AbilitySpecHandle, true);
		}
	}
}

FTransform ABasicUnit::GetTargetingSourceTransform() const
{
	if (TargetingComponent) { return TargetingComponent->GetComponentTransform(); }

	FVector EyeLoc = GetActorLocation() + FVector(0.0f, 0.0f, BaseEyeHeight);

	return FTransform(GetActorQuat(), EyeLoc);
}