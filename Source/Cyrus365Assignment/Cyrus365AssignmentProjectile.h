#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cyrus365AssignmentProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS(config=Game)
class ACyrus365AssignmentProjectile : public AActor
{
	GENERATED_BODY()

	//Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	//Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	ACyrus365AssignmentProjectile();

	//called when projectile hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Returns CollisionComp subobject
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	//Returns ProjectileMovement subobject
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	//Mesh component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;
};

