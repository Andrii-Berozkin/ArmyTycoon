// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Character/TycoonCharacter.h>

//	AbilitySystem
#include <AbilitySystemComponent.h>
#include <ArmyTycoon/Character/HealthComponent.h>
#include <ArmyTycoon/AbilitySystem/Attributes/HealthAttribute.h>
#include <ArmyTycoon/AbilitySystem/Attributes/MoneyAttribute.h>

//	Base Character
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Character.h>

//	Camera
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

//	EnhancedInput
#include <EnhancedInputComponent.h>
#include <InputMappingContext.h>
#include <EnhancedInputSubsystems.h>
#include <ArmyTycoon/Character/CharacterInputAsset.h>



//const FName ATycoonCharacter::CapsuleCollisionComponentName("CapsuleComponent");
//const FName ATycoonCharacter::CharacterMovementComponentName("CharacterMovementComponent");
const FName ATycoonCharacter::AbilitySystemComponentName("AbilitySystemComponent");
const FName ATycoonCharacter::HealthComponentName("HealthComponent");
const FName ATycoonCharacter::HealthSetName("HealthAttriute");
const FName ATycoonCharacter::MoneySetName("MoneyAttribute");
const FName ATycoonCharacter::CameraArmName("CameraArm");
const FName ATycoonCharacter::CameraName("Camera");



ATycoonCharacter::ATycoonCharacter(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	/*
	RootComponent = CapsuleComponent = StaticCast<UCapsuleComponent*>(CreateDefaultSubobject(CapsuleCollisionComponentName, 
		UCapsuleComponent::StaticClass(), UCapsuleComponent::StaticClass(), true, false));
	CapsuleComponent->SetCapsuleHalfHeight(84.0f);
	CapsuleComponent->SetCapsuleRadius(32.0f);
	CapsuleComponent->SetHiddenInGame(true);

	CharacterMovementComponent = StaticCast<UCharacterMovementComponent*>(CreateDefaultSubobject(CharacterMovementComponentName,
		UCharacterMovementComponent::StaticClass(), UCharacterMovementComponent::StaticClass(), true, false));
	if (CharacterMovementComponent && CapsuleComponent)
	{
		CharacterMovementComponent->UpdatedComponent = CapsuleComponent;
	}*/
	

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	AbilitySystemComponent = StaticCast<UAbilitySystemComponent*>(CreateDefaultSubobject(AbilitySystemComponentName,
		UAbilitySystemComponent::StaticClass(), UAbilitySystemComponent::StaticClass(), true, false));

	HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(HealthSetName, false);
	MoneySet = CreateDefaultSubobject<UMoneyAttributeSet>(MoneySetName, false);

	//AbilitySystemComponent->SetIsReplicated(true);
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	//InitializeAttributes();

	HealthComponent = StaticCast<UHealthComponent*>(CreateDefaultSubobject(HealthComponentName, UHealthComponent::StaticClass(), UHealthComponent::StaticClass(), true, false));

	CameraArm = StaticCast<USpringArmComponent*>(CreateDefaultSubobject(CameraArmName, USpringArmComponent::StaticClass(), USpringArmComponent::StaticClass(), true, false));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 500.0f;
	CameraArm->bUsePawnControlRotation = true;

	Camera = StaticCast<UCameraComponent*>(CreateDefaultSubobject(CameraName, UCameraComponent::StaticClass(), UCameraComponent::StaticClass(), true, false));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	InitialMinHealth = 0.0f;
	InitialMaxHealth = 100.0f;
	InitialHealth = 100.0;

	InitialMoney = 100.0f;

	TeamID = 0;
}

void ATycoonCharacter::InitializeAttributes()
{
	//HealthSet = CreateDefaultSubobject<UHealthAttributeSet>(HealthSetName, false);
	if (HealthSet)
	{
		HealthSet->InitMinHealth(InitialMinHealth);
		HealthSet->InitMaxHealth(InitialMaxHealth);
		HealthSet->InitHealth(InitialHealth);
	}

	//MoneySet = CreateDefaultSubobject<UMoneyAttributeSet>(MoneySetName, false);
	if (MoneySet)
	{
		MoneySet->InitMoney(InitialMoney);
	}
}

void ATycoonCharacter::InitializeStartupEffects()
{
	if (AbilitySystemComponent && HasAuthority())
	{
		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);

		for (TPair<TSubclassOf<UGameplayEffect>, float> Pair : StartupEffects)
		{
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Pair.Key, Pair.Value, Context);

			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATycoonCharacter::InitializeStartupAbilities()
{
	int32 BaseLevel = 1;

	if (AbilitySystemComponent && HasAuthority())
	{
		for (FGameplayAbilitySpecDef AbilitySpecDef : StartupAbilities)
		{
			FGameplayAbilitySpec AbilitySpec(AbilitySpecDef, BaseLevel);

			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void ATycoonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalEnhancedInputSubsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			LocalEnhancedInputSubsytem->ClearAllMappings();
			LocalEnhancedInputSubsytem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (InputAsset != nullptr)
		{
			EnhancedInput->BindAction(InputAsset->Move, ETriggerEvent::Triggered, this, &ThisClass::MoveInputHandle);
			EnhancedInput->BindAction(InputAsset->Look, ETriggerEvent::Triggered, this, &ThisClass::LookInputHandle);
			EnhancedInput->BindAction(InputAsset->LookZoom, ETriggerEvent::Triggered, this, &ThisClass::LookZoomInputHandle);
			EnhancedInput->BindAction(InputAsset->LookEnable, ETriggerEvent::Started, this, &ThisClass::LookEnableInputStartedHandle);
			EnhancedInput->BindAction(InputAsset->LookEnable, ETriggerEvent::Completed, this, &ThisClass::LookEnableInputCompletedHandle);
		}
	}
}

void ATycoonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	if (AbilitySystemComponent)
	{
		InitializeAttributes();
		InitializeStartupEffects();
		InitializeStartupAbilities();
	}
}

void ATycoonCharacter::Restart()
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

void ATycoonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if(AbilitySystemComponent && HealthSet) { HealthComponent->InitializerWithAbilitySystem(AbilitySystemComponent); }
}

void ATycoonCharacter::MoveInputHandle(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector2D InputVector = Value.Get<FVector2D>();
		const FTransform RootTransform = RootComponent->GetComponentTransform();
		FRotator ControlRotationYaw = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);

		
		//if (InputVector.Y != 0.0f) { AddMovementInput(RootTransform.GetUnitAxis(EAxis::X), InputVector.Y); }
		if (InputVector.Y != 0.0f) { AddMovementInput(FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X), InputVector.Y); }
		
		//if (InputVector.X != 0.0f) { AddMovementInput(RootTransform.GetUnitAxis(EAxis::Y), InputVector.X); }
		if (InputVector.X != 0.0f) { AddMovementInput(FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y), InputVector.X); }
	}
}

void ATycoonCharacter::LookInputHandle(const FInputActionValue& Value)
{
	if (Controller && bHoldMouse)
	{
		const FVector2D InputVector = Value.Get<FVector2D>();

		if (InputVector.X != 0.0f) { AddControllerYawInput(InputVector.X); }
		if (InputVector.Y != 0.0f) { AddControllerPitchInput(InputVector.Y); }
	}
}

void ATycoonCharacter::LookZoomInputHandle(const FInputActionValue& Value)
{
	const float MinArmLenght = 0.0;
	const float MaxArmLenght = 3500.0f;

	if (Controller)
	{
		const float InputFloat = Value.Get<float>();

		if (InputFloat != 0.0f)
		{
			float NewArmLenght = CameraArm->TargetArmLength + InputFloat;
			CameraArm->TargetArmLength = FMath::Clamp<float>(NewArmLenght, MinArmLenght, MaxArmLenght);
		}
	}
}

void ATycoonCharacter::LookEnableInputStartedHandle(const FInputActionValue& Value)
{
	if (Controller) 
	{ 
		bHoldMouse = Value.Get<bool>(); 

		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			PC->bShowMouseCursor = false;
		}
	}
}

void ATycoonCharacter::LookEnableInputCompletedHandle(const FInputActionValue& Value)
{
	if (Controller) 
	{ 
		bHoldMouse = Value.Get<bool>(); 

		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			PC->bShowMouseCursor = true;
		}
	}
}