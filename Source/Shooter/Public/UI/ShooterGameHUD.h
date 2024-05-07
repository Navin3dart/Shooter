// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/ShooterPlayerHUDWidget.h"
#include "ShooterGameHUD.generated.h"

class UShooterPlayerHUDWidget;

UCLASS()
class SHOOTER_API AShooterGameHUD : public AHUD
{
	GENERATED_BODY()


public:
    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UShooterPlayerHUDWidget> PlayerHUDWidgetClass;

    virtual void BeginPlay() override;

    UShooterPlayerHUDWidget* PlayerHUDWidget;

private:
    void DrawCrossHair();

    float XPosition;
    float YPosition;

	
};
