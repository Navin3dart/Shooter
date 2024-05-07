// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UShooterWeaponVFXComponent;

UCLASS()
class SHOOTER_API AShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterProjectile();

	void SetShootDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon");
    USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon");
    UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage");
    float DamageRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage");
    float DamageAmount = 50.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Damage");
    bool DoFullDamage = false;

	UPROPERTY(VisibleDefaultsOnly, Category = "Damage");
    float LifeSeconds = 5.0f;

	UPROPERTY(VisibleAnywhere, Category = "Components")
    UShooterWeaponVFXComponent* ShooterVFXComponent;

private:
    FVector ShotDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;

};
