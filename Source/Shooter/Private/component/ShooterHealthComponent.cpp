// ShooterGame. All Rights Reserved.


#include "Component/ShooterHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Dev/ShooterEMIDamageType.h"
#include "Dev/ShooterFireDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UShooterHealthComponent::UShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

    check(MaxHealth > 0);
    SetHealth(MaxHealth);

		AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UShooterHealthComponent::OnTakeAnyDamageHandle);
	}

	
}

void UShooterHealthComponent::TimerAutoHeal() 
{
       SetHealth(Health + HealthForTick);

       if (FMath::IsNearlyEqual(Health,MaxHealth) && GetWorld())
       {
           GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutoHeal);
       }
       //UE_LOG(LogHealthComponent, Display, TEXT("Heal: %f"), Health);
}




void UShooterHealthComponent::OnTakeAnyDamageHandle(
    AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutoHeal);


	if (IsDead())
	{
        OnDeath.Broadcast();
	}
    else if (UseAutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(
            TimerHandleAutoHeal, this, &UShooterHealthComponent::TimerAutoHeal, TimerRate, true, DelayHealStart);
    }

    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

	if (DamageType)
	{
		if (DamageType->IsA<UShooterFireDamageType>())
		{
            //UE_LOG(LogHealthComponent, Display, TEXT("FIRE!!!"));
		}
        else if (DamageType->IsA<UShooterEMIDamageType>())
        {
            //UE_LOG(LogHealthComponent, Display, TEXT("XXXXX!!!XXXXX"));
        }
	}
}

void UShooterHealthComponent::SetHealth(float NewHealth) 
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}
