// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include "Weapon_Projectile.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FDamageAppliedSignature, AWeapon_Projectile, OnDamageApplied, AActor*, OtherActor, UAbilitySystemComponent*, OtherAbilitySystem);


class UStaticMeshComponent;
class UProjectileMovementComponent;



UCLASS(BlueprintType, Blueprintable)
class ARMYTYCOON_API AWeapon_Projectile : public AActor
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	USceneComponent* SceneRootComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovementComponent;


public:

	static const FName SceneRootComponentName;
	static const FName StaticMeshComponentName;
	static const FName ProjectileMovementComponentName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<class UGameplayEffect> GE_DamageNative;

	UPROPERTY(BlueprintAssignable, Category = "Damage")
	FDamageAppliedSignature OnDamageApplied;

private:

public:

	AWeapon_Projectile(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//	Begin AActor Interface
	virtual void PostInitializeComponents() override;
	//	End of AActor Interface
	
	UFUNCTION()
	virtual void OnProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnActorBeginOverlapHandle(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void ProcessedProjectileDamage(AActor* OtherActor);

private:
};