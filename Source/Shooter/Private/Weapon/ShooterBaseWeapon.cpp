// ShooterGame. All Rights Reserved.

#include "Weapon/ShooterBaseWeapon.h"
#include "components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework//Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AShooterBaseWeapon::AShooterBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}
void AShooterBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    check(WeaponMesh);

    CurrentAmmo = DefaultAmmo;
}

APlayerController* AShooterBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;
    return Player->GetController<APlayerController>();
}

bool AShooterBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const 
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector AShooterBaseWeapon::GetMuzzleWorldLocation() const 
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool AShooterBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraceEnd = TraceStart + ShotDirection * TraceMaxDistance;
    return true;
}

void AShooterBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;
    const FVector MuzzleDirection = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    IsShot = (FVector::DotProduct(MuzzleDirection, (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal())) > 0.5f;
}

void AShooterBaseWeapon::DecreaseAmmo() 
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Ammo is empty"));
        return;
    }
    CurrentAmmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast();
    }
}

bool AShooterBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool AShooterBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void AShooterBaseWeapon::ChangeClip() 
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(LogBaseWeapon, Display, TEXT ("---------Change Clip--------"));
}

bool AShooterBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void AShooterBaseWeapon::LogAmmo() 
{
    FString AmmoInfo = "Ammo:" + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

    UE_LOG(LogBaseWeapon, Display, TEXT ("%s"), *AmmoInfo);
}

void AShooterBaseWeapon::StartFire()
{
}
void AShooterBaseWeapon::StopFire() 
{
}
void AShooterBaseWeapon::MakeShot() 
{
}