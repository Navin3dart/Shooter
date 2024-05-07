// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifleWeapon.generated.h"

class UShooterWeaponVFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SHOOTER_API AShooterRifleWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()
	
public:

    AShooterRifleWeapon();


    virtual void StartFire() override;
    virtual void StopFire() override;

    virtual void Tick(float DeltaTime) override;

    virtual void SetShotOffset(float NewOffset);

protected:
    virtual void BeginPlay() override;
    virtual void MakeShot() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UShooterWeaponVFXComponent* ShooterVFXComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|ShotAimOffset")
    float ShotOffset = 0.1f;

private:
    FTimerHandle ShotTimerHandle;

    UPROPERTY()
    UNiagaraComponent* MuzzleVFXComponent;



    void InitMuzzleVFX();
    void SetMuzzleVFXVisibility(bool Visibility);

};
