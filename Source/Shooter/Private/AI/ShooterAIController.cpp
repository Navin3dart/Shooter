// ShooterGame. All Rights Reserved.

#include "AI/ShooterAIController.h"
#include "AI/ShooterAICharacter.h"

void AShooterAIController::OnPossess(APawn* InPawn) 
{
    Super::OnPossess(InPawn);

    const auto SCharacter = Cast<AShooterAICharacter>(InPawn);
    if (SCharacter)
    {
        RunBehaviorTree(SCharacter->BehaviorTreeAsset);
    }
}
