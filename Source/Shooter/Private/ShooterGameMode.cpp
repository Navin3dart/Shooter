// ShooterGame. All Rights Reserved.


#include "ShooterGameMode.h"
#include "Player/ShooterBaseCharacter.h"
#include "Player/ShooterPlayerController.h"
#include "UI/ShooterGameHUD.h"

AShooterGameMode::AShooterGameMode() 
{
    DefaultPawnClass = AShooterBaseCharacter::StaticClass();
    PlayerControllerClass = AShooterPlayerController::StaticClass();
    HUDClass = AShooterGameHUD::StaticClass();
}
