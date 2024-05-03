// ShooterGame. All Rights Reserved.

#include "AI/Decorators/HealthPercentDecorator.h"
#include "AIController.h"
#include "component/ShooterHealthComponent.h"

UHealthPercentDecorator::UHealthPercentDecorator() 
{
    NodeName = "Health Percent";
}

bool UHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HealthComponent = Controller->GetPawn()->FindComponentByClass<UShooterHealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() <= HealthPercent;
}
