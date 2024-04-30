// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterPlayerHUDWidget.generated.h"



UCLASS()
class SHOOTER_API UShooterPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetWeaponUIData(FWeaponUIData& UIData) const;
};
