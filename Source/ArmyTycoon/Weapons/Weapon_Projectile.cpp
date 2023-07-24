// Copyright Andrii Berozkin, Inc. All Rights Reserved.

#include <ArmyTycoon/Weapons/Weapon_Projectile.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

#include <ArmyTycoon/ArmyTycoonGeneral.h>
#include <ArmyTycoon/ArmyUnits/BasicUnit.h>
#include <GameplayAbilities/Public/AbilitySystemComponent.h>



const FName AWeapon_Projectile::SceneRootComponentName("RootComponent");
const FName AWeapon_Projectile::StaticMeshComponentName("StaticMeshComonent");
const FName AWeapon_Projectile::ProjectileMovementComponentName("ProjectileMovementComponent");

AWeapon_Projectile::AWeapon_Projectile(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get())*/)
	: Super(ObjectInitializer)
{
	RootComponent = SceneRootComponent = CreateDefaultSubobject<USceneComponent>(SceneRootComponentName, false);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(StaticMeshComponentName, false);
	FCollisionResponseContainer DefaultProjectileCollision;
	DefaultProjectileCollision.Camera = ECollisionResponse::ECR_Block;
	DefaultProjectileCollision.Visibility = ECollisionResponse::ECR_Block;
	DefaultProjectileCollision.Pawn = ECollisionResponse::ECR_Block;
	DefaultProjectileCollision.Destructible = ECollisionResponse::ECR_Ignore;
	DefaultProjectileCollision.PhysicsBody = ECollisionResponse::ECR_Ignore;
	DefaultProjectileCollision.Vehicle = ECollisionResponse::ECR_Block;
	DefaultProjectileCollision.WorldDynamic = ECollisionResponse::ECR_Ignore;
	DefaultProjectileCollision.WorldStatic = ECollisionResponse::ECR_Block;

	StaticMeshComponent->SetCollisionResponseToChannels(DefaultProjectileCollision);
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(ProjectileMovementComponentName, false);
	ProjectileMovementComponent->UpdatedComponent = StaticMeshComponent;
	ProjectileMovementComponent->InitialSpeed = 2500.0f;
	ProjectileMovementComponent->MaxSpeed = 2500.0f;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->OnProjectileStop;
	ProjectileMovementComponent->ProjectileGravityScale = 0.1f;

	this->OnActorHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	this->OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnActorBeginOverlapHandle);
}

void AWeapon_Projectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AWeapon_Projectile::OnProjectileHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	ProcessedProjectileDamage(OtherActor);
}

void AWeapon_Projectile::OnActorBeginOverlapHandle(AActor* OverlappedActor, AActor* OtherActor)
{
	ProcessedProjectileDamage(OtherActor);
}

void AWeapon_Projectile::ProcessedProjectileDamage(AActor* OtherActor)
{
	UTeamSubsystem* TeamSubsytem = GetWorld()->GetSubsystem<UTeamSubsystem>();

	AActor* OwnerPtr = GetOwner();

	if (ABasicUnit* BasicUnit = Cast<ABasicUnit>(OtherActor))
	{
		if (!BasicUnit->IsPendingKill() && TeamSubsytem->IsFromDifferentTeam(BasicUnit, OwnerPtr))
		{
			ABasicUnit* OwnerUnit = Cast<ABasicUnit>(OwnerPtr);

			if (UAbilitySystemComponent* OwnerAbilitySystem = OwnerUnit->GetAbilitySystemComponent())
			{
				FGameplayEffectContextHandle Context = OwnerAbilitySystem->MakeEffectContext();
				Context.AddSourceObject(OwnerUnit);

				FGameplayEffectSpecHandle EffectSpec = OwnerAbilitySystem->MakeOutgoingSpec(GE_DamageNative, 1.0f, Context);

				OwnerUnit->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), BasicUnit->GetAbilitySystemComponent());

				OnDamageApplied.Broadcast(OtherActor, OwnerUnit->GetAbilitySystemComponent());
			}

			this->Destroy();
		}
	}



	//this->Destroy();
	return;
}