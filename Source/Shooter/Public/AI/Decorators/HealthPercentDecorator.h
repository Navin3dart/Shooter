// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "HealthPercentDecorator.generated.h"

UCLASS()
class SHOOTER_API UHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
    UHealthPercentDecorator();
	
protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.5f;
};
