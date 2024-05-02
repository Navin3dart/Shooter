// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

class UShooterAIPerceptionComponent;

UCLASS()
class SHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()

public:
    AShooterAIController();
	
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UShooterAIPerceptionComponent* ShooterAIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* GetFocusOnActor() const;
};
