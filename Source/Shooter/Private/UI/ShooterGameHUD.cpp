// ShooterGame. All Rights Reserved.

#include "UI/ShooterGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"
#include "UI/ShooterPlayerHUDWidget.h"

void AShooterGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrossHair();
}

void AShooterGameHUD::BeginPlay()
{
    Super::BeginPlay();
    PlayerHUDWidget = CreateWidget<UShooterPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }

}

void AShooterGameHUD::DrawCrossHair()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);
    const float HalfLineSize = 10.0f;
    const float LineThickness = 1.0f;
    const FLinearColor LineColor = FLinearColor::Black;
    float Offset = (PlayerHUDWidget->GetWeaponSpreadRadius())*20.0f;

    DrawLine(Center.Min - HalfLineSize - Offset,  // Start X
        Center.Max,                               // Start Y
        Center.Min - Offset,                      // End X
        Center.Max,                               // End Y
        LineColor,                                //
        LineThickness);                           //
    DrawLine(Center.Min + Offset,                 // Start X
        Center.Max,                               // Start Y
        Center.Min + HalfLineSize + Offset,       // End X
        Center.Max,                               // End Y
        LineColor,                                //
        LineThickness);                           //
    DrawLine(Center.Min,                          // Start X
        Center.Max + Offset,                      // Start Y
        Center.Min,                               // End X
        Center.Max + HalfLineSize + Offset,       // End Y
        LineColor,                                //
        LineThickness);                           //
}
