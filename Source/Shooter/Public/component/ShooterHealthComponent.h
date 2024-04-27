// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShooterHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

	FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;
    FTimerHandle TimerHandleAutoHeal;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health", meta = (ClampMin = "10", ClampMax = "200"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal")
    bool UseAutoHeal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal", meta = (EditCondition = "UseAutoHeal"))
    float DelayHealStart = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal", meta = (EditCondition = "UseAutoHeal"))
    float HealthForTick = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal", meta = (EditCondition = "UseAutoHeal"))
    float TimerRate = 5.0f;


	private:

    float Health = 0.0f;



	void TimerAutoHeal();
    void SetHealth(float NewHealth);

	UFUNCTION()
    void OnTakeAnyDamageHandle(
        AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);



};
