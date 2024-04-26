// ShooterGame. All Rights Reserved.


#include "ShooterGameMode.h"
#include "Player/ShooterBaseCharacter.h"
#include "Player/ShooterPlayerController.h"

AShooterGameMode::AShooterGameMode() 
{
    DefaultPawnClass = AShooterBaseCharacter::StaticClass();
    PlayerControllerClass = AShooterPlayerController::StaticClass();

}
