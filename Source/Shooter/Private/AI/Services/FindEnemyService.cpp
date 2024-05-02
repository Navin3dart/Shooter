// ShooterGame. All Rights Reserved.


#include "AI/Services/FindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "component/ShooterAIPerceptionComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIService, All, All);

UFindEnemyService::UFindEnemyService() 
{
    NodeName = "FindEnemy";
}

void UFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceprionComponent = Controller->FindComponentByClass<UShooterAIPerceptionComponent>();
        if (PerceprionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceprionComponent->GetClosestEnemy());
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
