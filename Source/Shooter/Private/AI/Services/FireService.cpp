// ShooterGame. All Rights Reserved.


#include "AI/Services/FireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "component/ShooterWeaponComponent.h"

UFireService::UFireService() 
{
    NodeName = "Fire";
}

void UFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<UShooterWeaponComponent>();
        if (WeaponComponent)
        {
            HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }
    }



    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
