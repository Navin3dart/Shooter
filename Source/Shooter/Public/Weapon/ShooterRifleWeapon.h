// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterRifleWeapon.generated.h"


UCLASS()
class SHOOTER_API AShooterRifleWeapon : public AShooterBaseWeapon
{
	GENERATED_BODY()
	
public:
    virtual void StartFire() override;
    virtual void StopFire() override;


protected:
    virtual void MakeShot() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;



private:
    FTimerHandle ShotTimerHandle;



};
