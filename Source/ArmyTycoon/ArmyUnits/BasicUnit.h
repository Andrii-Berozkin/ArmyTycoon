// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Pawn.h>
#include <GameFramework/Character.h>
#include <GameplayAbilities/Public/GameplayAbilitySpec.h>
#include <GameplayAbilities/Public/AbilitySystemInterface.h>
#include <ArmyTycoon/ArmyTycoonGeneral.h>
#include "BasicUnit.generated.h"



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API ABasicUnit : public ACharacter, public IAbilitySystemInterface, public ITycoonTeamsInterface
{
	GENERATED_BODY()

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	//class UCharacterMovementComponent* CharacterMovementComponent;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	//class UPawnMovementComponent* PawnMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UHealthAttributeSet* HealthSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	class UHealthComponent* HealthComponent;

public:

	static const FName CharacterMovementComponentName;
	static const FName PawnMovementComponentName;
	static const FName AbilitySystemComponentName;
	static const FName HealtComponentName;
	static const FName HealthSetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilitySystem")
	TArray<struct FGameplayAbilitySpecDef> StartupAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Teams")
	int32 TeamID;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Health")
	float InitialMinHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Health")
	float InitialMaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Unit|Health")
	float InitialHealth;

protected:

	//	
	UPROPERTY(BlueprintReadOnly, Category = "Unit|Weapon")
	AActor* Weapon;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AttackAbilitiesHandle;

	/** responds to weapon 'muzzle' */
	UPROPERTY(BlueprintReadOnly, Category = "Unit|Weapon|Targeting")
	TSoftObjectPtr<USceneComponent> TargetingComponent;

	/** Normilized direction of weapon 'muzzle' */
	UE_DEPRECATED(4.27, "Variable not in usage anymore");
	UPROPERTY(BlueprintReadOnly, Category = "Unit|Weapon|Targeting")
	FVector TargetingDirection;

public:

	ABasicUnit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin AActor Interface
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
	//	End of AActor Interface

	//	Begin APawn Interface
	virtual void Restart() override;
	//	End of APawn Interface

	//	Begin IAbilitySystemInterface Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	//	End of IAbilitySystemInterface Interface

	//	Begin ITycoonTeamsInterface Interface
	virtual int32 GetTeam_Implementation() const { return TeamID; }

	virtual void SetTeam_Implementation(int32 NewTeamID) { TeamID = NewTeamID; }
	//	End of ITycoonTeamsInterface Interface

	UFUNCTION(BlueprintCallable, Category = "Unit|Weapon")
	void GiveWeapon(TSubclassOf<class UWeaponDefinition> WeaponDefinition);

	UFUNCTION(BlueprintCallable, Category = "Unit|Weapon")
	void RemoveAllWeapon();

	UFUNCTION(BlueprintCallable, Category = "Unit|Weapon")
	bool HasAbilityToAttack() const;

	UFUNCTION(BlueprintCallable, Category = "Unit|Weapon")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Unit")
	/**	Try to find any scene component at "TargetingComponent" variable
	* if fail to find any, then Prodvide Current actor transform + BaseEyeHeight
	*/
	virtual FTransform GetTargetingSourceTransform() const;


	UFUNCTION(BlueprintCallable, Category = "Unit|Weapon|Targeting")
	/** Set Scene component which will be used as location and direction to perform shoot */
	void SetTargetingComponent(const USceneComponent* NewTargetingComponent) { TargetingComponent = NewTargetingComponent; }

protected:

	void InitializeAttributes();
	void InitializeStartupEffects();
	void InitializeStartupAbilities();

};