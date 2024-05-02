// ShooterGame. All Rights Reserved.

#include "AI/ShooterAIController.h"
#include "AI/ShooterAICharacter.h"
#include "component/ShooterAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIController, All, All);

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
    const auto AimActor = GetFocusOnActor();
    SetFocus(AimActor);
}

AActor* AShooterAIController::GetFocusOnActor() const
{
    //UE_LOG(LogAIController, Display, TEXT("Start"));
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));

}
