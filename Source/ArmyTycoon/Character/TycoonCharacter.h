// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <AbilitySystemInterface.h>
#include <GameplayAbilities/Public/GameplayAbilitySpec.h>
#include <ArmyTycoon/ArmyTycoonGeneral.h>

#include "TycoonCharacter.generated.h"




UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API ATycoonCharacter : public ACharacter, public IAbilitySystemInterface, public ITycoonTeamsInterface
{
	GENERATED_BODY()

public:

	//const static FName CapsuleCollisionComponentName;
	//const static FName CharacterMovementComponentName;
	const static FName AbilitySystemComponentName;
	const static FName HealthComponentName;
	const static FName HealthSetName;
	const static FName MoneySetName;
	const static FName CameraArmName;
	const static FName CameraName;
	//const static FName InteractTriggerName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TMap<TSubclassOf<class UGameplayEffect>, float> StartupEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TArray<struct FGameplayAbilitySpecDef> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "TycoonCharacter|Health")
	float InitialMinHealth;

	UPROPERTY(EditDefaultsOnly, Category = "TycoonCharacter|Health")
	float InitialMaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "TycoonCharacter|Health")
	float InitialHealth;

	UPROPERTY(EditDefaultsOnly, Category = "TycoonCharacter|Money")
	float InitialMoney;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TycoonCharacter|Teams")
	int32 TeamID;

protected:
	/*
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	class UCharacterMovementComponent* CharacterMovementComponent;*/

	UPROPERTY(BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = true))
	class UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components| AttributeSet", Meta = (AllowPrivateAccess = true))
	class UHealthAttributeSet* HealthSet;

	UPROPERTY(BlueprintReadOnly, Category = "Components| AttributeSet", Meta = (AllowPrivateAccess = true))
	class UMoneyAttributeSet* MoneySet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", Meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;

	UPROPERTY(BlueprintReadOnly, Category = "TycoonCharacter", Transient)
	/** Used in ability system to get Cost of Upgrade of building inside UpgradeAbility which applies to character */
	AActor* SelectedActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	class UCharacterInputAsset* InputAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	class UInputMappingContext* InputMappingContext;

private:

	UPROPERTY(Transient)
	bool bHoldMouse = false;

public:

	ATycoonCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeAttributes();

	virtual void InitializeStartupEffects();

	virtual void InitializeStartupAbilities();

	//	Begin APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//	End of APawn interface

	//	Begin AActor Interface
	virtual void BeginPlay() override;

	virtual void Restart() override;

	virtual void PostInitializeComponents() override;
	//	End   AActor Interface

	//	Getter
	//UFUNCTION(BlueprintCallable)
	//class UCharacterMovementComponent* GetCharacterMovementComponent() const { return CharacterMovementComponent; }

	//	Begin IAbilitySystemInterface
	//UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	//	End   IAbilitySystemInterface

	//	Begin ITycoonTeamsInterface
	virtual int32 GetTeam() const { return TeamID; }

	virtual void SetTeam(int32 NewTeamID) { TeamID = NewTeamID; }
	//	End of ITycoonTeamsInterface

	class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	class UHealthAttributeSet* GetHealthAttributeSet() const { return HealthSet; }

	class UMoneyAttributeSet* GetMoneyAttributeSet() const { return MoneySet; }

	class USpringArmComponent* GetCameraArmComponent() const { return CameraArm; }

	class UCameraComponent* GetCameraComponent() const { return Camera; }

	void SetSelectedActor(class AActor* NewActor) { SelectedActor = NewActor; }

	/** return an actor (mainly ABuilding) */
	class AActor* GetSelectedActor() const { return SelectedActor; }

protected:

	void MoveInputHandle(const struct FInputActionValue& Value);

	void LookInputHandle(const struct FInputActionValue& Value);

	void LookZoomInputHandle(const struct FInputActionValue& Value);

	void LookEnableInputStartedHandle(const struct FInputActionValue& Value);

	void LookEnableInputCompletedHandle(const struct FInputActionValue& Value);

private:
};