// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "component/ShooterWeaponComponent.h"
#include "ShooterAIWeaponComponent.generated.h"

UCLASS()
class SHOOTER_API UShooterAIWeaponComponent : public UShooterWeaponComponent
{
	GENERATED_BODY()
	
public:

	virtual void StartFire() override;
    virtual void NextWeapon() override;
};
