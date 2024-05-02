// ShooterGame. All Rights Reserved.


#include "component/ShooterAIPerceptionComponent.h"
#include "AIController.h"
#include "component/ShooterHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UShooterAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    if (PercieveActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;
    
    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (const auto PercieveActor : PercieveActors)
    {
        //const auto Component = PercieveActor->GetComponentByClass(UShooterHealthComponent::StaticClass());
        //const auto HealthComponent = Cast<UShooterHealthComponent>(Component);
        const auto HealthComponent = PercieveActor->FindComponentByClass<UShooterHealthComponent>();
        if (HealthComponent && !HealthComponent->IsDead()) //check if enemies or not
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;

}
