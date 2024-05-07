// ShooterGame. All Rights Reserved.


#include "Component/ShooterHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Dev/ShooterEMIDamageType.h"
#include "Dev/ShooterFireDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
#include "Player/ShooterBaseCharacter.h"


DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

UShooterHealthComponent::UShooterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UShooterHealthComponent::AddHealth(int32 HealthAmount)
{
    if (Health == MaxHealth) return false;
    Health = FMath::Clamp(Health + HealthAmount, 0, MaxHealth);
    return true;
}

void UShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

    check(MaxHealth > 0);
    SetHealth(MaxHealth);
    check(MaxStamina > 0);
    SetStamina(MaxStamina);

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

void UShooterHealthComponent::TimerStaminaRegeneration() 
{
    SetStamina(Stamina + StaminaPerSecond);
    if (FMath::IsNearlyEqual(Stamina, MaxStamina) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutoRegenStamina);
    }
}

void UShooterHealthComponent::TimerStaminaDecrease() 
{
    SetStamina(Stamina - DecreaseStaminaPerSecond*0.1f);
    const auto Player = Cast<AShooterBaseCharacter>(GetOwner());
    if ((FMath::IsNearlyEqual(Stamina, 0.0f)) || !(Player->GetVelocity().Length() > 0.0f))
    {
        Player->Walk();
        StopRunStamina();
    }
}

void UShooterHealthComponent::StartRunStamina() 
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandleDecreaseStamina, this, &UShooterHealthComponent::TimerStaminaDecrease, 0.1f, true);
    GetWorld()->GetTimerManager().ClearTimer(TimerHandleAutoRegenStamina);
}

void UShooterHealthComponent::StopRunStamina() 
{
    GetWorld()->GetTimerManager().ClearTimer(TimerHandleDecreaseStamina);
    StartRegenerationStamina();
}

void UShooterHealthComponent::StartRegenerationStamina() 
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandleAutoRegenStamina, this, &UShooterHealthComponent::TimerStaminaRegeneration,
        TimerRateStaminaRegen, true, DelayStaminaRegenStart);
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

    PlayCameraShake();

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

void UShooterHealthComponent::SetStamina(float NewStamina) 
{
    Stamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);
}

void UShooterHealthComponent::PlayCameraShake()
{
    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
