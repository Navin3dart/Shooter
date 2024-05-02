// ShooterGame. All Rights Reserved.

#include "AI/ShooterAIController.h"
#include "AI/ShooterAICharacter.h"
#include "component/ShooterAIPerceptionComponent.h"

AShooterAIController::AShooterAIController() 
{
    ShooterAIPerceptionComponent = CreateDefaultSubobject<UShooterAIPerceptionComponent>("SPerceptionComponent");
    SetPerceptionComponent(*ShooterAIPerceptionComponent);
}

void AShooterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto SCharacter = Cast<AShooterAICharacter>(InPawn);
    if (SCharacter)
    {
        RunBehaviorTree(SCharacter->BehaviorTreeAsset);
    }
}

void AShooterAIController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);
    const auto AimActor = ShooterAIPerceptionComponent->GetClosestEnemy();
    SetFocus(AimActor);
}
