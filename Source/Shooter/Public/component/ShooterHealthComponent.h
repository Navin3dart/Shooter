// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

class UCameraShakeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShooterHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, category = "Health")
    float GetHealthPercent() const { return Health / MaxHealth; }

    UFUNCTION(BlueprintCallable, category = "Health")
    float GetStaminaPercent() const { return Stamina / MaxStamina; }

	bool AddHealth(int32 HealthAmount);

	FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;
    FTimerHandle TimerHandleAutoHeal;

    FTimerHandle TimerHandleAutoRegenStamina;
    FTimerHandle TimerHandleDecreaseStamina;

    float Stamina = 0.0f;

	void SetStamina(float NewStamina); 
	void StartRunStamina();
    void StopRunStamina();
    void StartRegenerationStamina();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health", meta = (ClampMin = "10", ClampMax = "200"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Stamina", meta = (ClampMin = "10", ClampMax = "200"))
    float MaxStamina = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Stamina", meta = (ClampMin = "10", ClampMax = "200"))
    float StaminaPerSecond = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Stamina", meta = (ClampMin = "1", ClampMax = "20"))
    float DecreaseStaminaPerSecond = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Stamina")
    float DelayStaminaRegenStart = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Stamina")
    float TimerRateStaminaRegen = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal")
    bool UseAutoHeal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal", meta = (EditCondition = "UseAutoHeal"))
    float DelayHealStart = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal", meta = (EditCondition = "UseAutoHeal"))
    float HealthForTick = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|Health|AutoHeal", meta = (EditCondition = "UseAutoHeal"))
    float TimerRate = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings|CameraShake", meta = (EditCondition = "UseAutoHeal"))
    TSubclassOf<UCameraShakeBase> CameraShake;

	private:

    float Health = 0.0f;




	void TimerAutoHeal();
    void TimerStaminaRegeneration();
    void TimerStaminaDecrease();

    void SetHealth(float NewHealth);


	void PlayCameraShake();

	UFUNCTION()
    void OnTakeAnyDamageHandle(
        AActor* DamageActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);



};
