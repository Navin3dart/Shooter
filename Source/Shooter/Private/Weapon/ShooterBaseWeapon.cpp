// ShooterGame. All Rights Reserved.

#include "Weapon/ShooterBaseWeapon.h"
#include "components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework//Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ShooterBaseCharacter.h"
#include "component/ShooterWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

AShooterBaseWeapon::AShooterBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void AShooterBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);

    CurrentAmmo = DefaultAmmo;
    SpreadData.CurrentSpread = SpreadData.MinSpreadCurrent;
}

APlayerController* AShooterBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;
    return Player->GetController<APlayerController>();
}

bool AShooterBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto ShooterCharacter = Cast<ACharacter>(GetOwner());
    if (!ShooterCharacter) return false;

    if (ShooterCharacter->IsPlayerControlled())
    {
        const auto Controller = GetPlayerController();
        if (!Controller) return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleWorldLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    return true;
}

FVector AShooterBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

float AShooterBaseWeapon::GetShotOffset()
{
    return ShotOffset;
}

void AShooterBaseWeapon::SetShotOffset(float NewOffset) 
{
    ShotOffset = NewOffset;
    const auto ShooterCharacter = Cast<AShooterBaseCharacter>(GetOwner());
    if (!ShooterCharacter) return;
    ShooterCharacter->AddControllerPitchInput(-NewOffset/5.0f);

}



bool AShooterBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    if (!GetWorld()) return false;
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    //TraceStart = ViewLocation;
    TraceStart = GetMuzzleWorldLocation();
    const auto HalfRad = FMath::DegreesToRadians(SpreadData.CurrentSpread);
    //const FVector ShotDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    const FVector MuzzleDirection = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();
    const FVector ShotDirection = FMath::VRandCone(MuzzleDirection, HalfRad);
    TraceEnd = TraceStart + ShotDirection * TraceMaxDistance;
    return true;
}

void AShooterBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;
    const FVector MuzzleDirection = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    IsShot = (FVector::DotProduct(MuzzleDirection, (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal())) > 0.5f;

    if (IsShot)
    {
        SpreadData.CurrentSpread = FMath::Clamp(SpreadData.CurrentSpread + SpreadData.DeltaSpread,  //
            SpreadData.MinSpreadCurrent, SpreadData.MaxSpreadCurrent);
        const auto ComponentW = GetOwner()->GetComponentByClass(UShooterWeaponComponent::StaticClass());
        const auto WeaponComponent = Cast<UShooterWeaponComponent>(ComponentW);

        WeaponComponent->PlayRecoil();
    }
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

void AShooterBaseWeapon::Tick(float DeltaTime)
{
    if (!(FMath::IsNearlyEqual(SpreadData.CurrentSpread, SpreadData.MinSpreadCurrent, 0.001)) && !IsShooting)
    {
        SpreadData.CurrentSpread = FMath::FInterpTo(SpreadData.CurrentSpread,  //
            SpreadData.MinSpreadCurrent,                                       //
            DeltaTime,                                                         //
            SpreadData.InterpSpeed);                                           //
    }
    Super::Tick(DeltaTime);
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
    UE_LOG(LogBaseWeapon, Display, TEXT("---------Change Clip--------"));
}

void AShooterBaseWeapon::SimulatePhysics()
{

    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    WeaponMesh->SetSimulatePhysics(true);
    GetWorldTimerManager().SetTimer(CollisionHandle, this, &AShooterBaseWeapon::DisableCollision, 0.1f, false, 4.0f);
}

void AShooterBaseWeapon::UpdateSpreadRadius()
{
    SpreadData.MinSpreadCurrent = SpreadData.MinSpread * SpreadAiming * WalkModifyerSpread * CrouchModifyer;
    SpreadData.MaxSpreadCurrent = SpreadData.MaxSpread * SpreadAiming * WalkModifyerSpread * CrouchModifyer;
}

bool AShooterBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool AShooterBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

    if (IsAmmoEmpty())
    {
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast();
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        CurrentAmmo.Clips = CurrentAmmo.Clips + ClipsAmount;
    }
    return true;
}

void AShooterBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo:" + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

    // UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

bool AShooterBaseWeapon::IsAmmoFull()
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips;
}

void AShooterBaseWeapon::DisableCollision()
{
    WeaponMesh->SetSimulatePhysics(false);
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UNiagaraComponent* AShooterBaseWeapon::SpawnMuzzleVFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleVFX,  //
        WeaponMesh,                                                 //
        MuzzleSocketName,                                           //
        FVector::ZeroVector,                                        //
        FRotator::ZeroRotator,                                      //
        EAttachLocation::SnapToTarget,                              //
        true);
}

void AShooterBaseWeapon::StartFire()
{
    IsShooting = true;
}
void AShooterBaseWeapon::StopFire()
{
    IsShooting = false;
}
void AShooterBaseWeapon::MakeShot() {}