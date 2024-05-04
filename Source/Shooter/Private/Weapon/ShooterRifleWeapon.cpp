// ShooterGame. All Rights Reserved.


#include "Weapon/ShooterRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework//Character.h"
#include "GameFramework/Controller.h"
#include "component/ShooterWeaponVFXComponent.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapons, All, All);

AShooterRifleWeapon::AShooterRifleWeapon() 
{
    ShooterVFXComponent = CreateDefaultSubobject<UShooterWeaponVFXComponent>("ShooterWeaponComponent");
}

void AShooterRifleWeapon::StartFire()
{
    Super::StartFire();
    InitMuzzleVFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AShooterRifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void AShooterRifleWeapon::StopFire()
{
    Super::StopFire();
    SetMuzzleVFXVisibility(false);
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void AShooterRifleWeapon::BeginPlay() 
{
    Super::BeginPlay();
    check(ShooterVFXComponent);
}

void AShooterRifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) 
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        if (!IsShot) return;
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        HitResult.GetActor()->TakeDamage(WeaponDamage, FDamageEvent{}, GetPlayerController(), this);

        ShooterVFXComponent->PlayImpactVFX(HitResult);
    }
    else
    {
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }
    DecreaseAmmo();
}

void AShooterRifleWeapon::InitMuzzleVFX() 
{
    if (!MuzzleVFXComponent)
    {
        MuzzleVFXComponent = SpawnMuzzleVFX();
        MuzzleVFXComponent->SetFloatParameter("Rate", TimeBetweenShots);
    }
    SetMuzzleVFXVisibility(true);
}

void AShooterRifleWeapon::SetMuzzleVFXVisibility(bool Visibility) 
{
    if (MuzzleVFXComponent)
    {
        MuzzleVFXComponent->SetPaused(!Visibility);
        MuzzleVFXComponent->SetVisibility(Visibility, true);
    }
}
