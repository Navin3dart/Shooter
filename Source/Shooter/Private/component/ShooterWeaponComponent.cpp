// ShooterGame. All Rights Reserved.

#include "component/ShooterWeaponComponent.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/ShooterEquipFinishedAnimNotify.h"
#include "Animations/ShooterReloadFinishedAnimNotify.h"
#include "Weapon/ShooterBaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

UShooterWeaponComponent::UShooterWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UShooterWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    InitAnimations();
    CurrentWeaponIndex = 0;
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    /* for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
        Weapon->GetActorEnableCollision();
    }*/
    Weapons.Empty();
    Super::EndPlay(EndPlayReason);
}

void UShooterWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<AShooterBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &UShooterWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void UShooterWeaponComponent::AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UShooterWeaponComponent::DetachWeapons()
{
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->SimulatePhysics();
    }
}

void UShooterWeaponComponent::StartAiming() 
{
    CurrentWeapon->ChangeSpreadRadius(0.25f);
}

void UShooterWeaponComponent::EndAiming() 
{
    CurrentWeapon->ChangeSpreadRadius(4.0f);
}

void UShooterWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid Weapon Index"));
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }
    CurrentWeapon = Weapons[WeaponIndex];
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //
        return Data.WeaponClass == CurrentWeapon->GetClass();                                 //
    });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EqipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

FVector UShooterWeaponComponent::GetMuzzleWorldLocation() const
{
    if (!CurrentWeapon) return FVector::ZeroVector;
    return CurrentWeapon->GetMuzzleWorldLocation();
}

void UShooterWeaponComponent::StartFire()
{
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void UShooterWeaponComponent::StopFire()
{
    CurrentWeapon->StopFire();
}

void UShooterWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UShooterWeaponComponent::Reload()
{
    ChangeClip();
}

bool UShooterWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

bool UShooterWeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool UShooterWeaponComponent::TryToAddAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}

void UShooterWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void UShooterWeaponComponent::InitAnimations()
{
    auto EquipFinishedNotify = FindNotifyByClass<UShooterEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnEquipFinished);
    }
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = FindNotifyByClass<UShooterReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify) continue;
        ReloadFinishedNotify->OnNotified.AddUObject(this, &UShooterWeaponComponent::OnReloadFinished);
    }
}

void UShooterWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    EqipAnimInProgress = false;
}

void UShooterWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComponent) return;

    ReloadAnimInProgress = false;
}

bool UShooterWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EqipAnimInProgress && !ReloadAnimInProgress;
}

void UShooterWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
    if (GetWorld() && CanFire())
    {
        ACharacter* Character = Cast<ACharacter>(GetOwner());
        const auto Controller = Character->GetController<APlayerController>();


        if (Controller)
        {
            const auto WeaponMesh = CurrentWeapon->WeaponMesh;
            FName MuzzleSocketName = CurrentWeapon->MuzzleSocketName;
            const FVector MuzzleDirection = WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();

            FHitResult WeaponHitResult;
            FVector MuzzleLocation = GetMuzzleWorldLocation();
            FVector TargetLocation = MuzzleDirection * 15000.0f + MuzzleLocation;
            FVector HitLocation = TargetLocation;

            GetWorld()->LineTraceSingleByChannel(WeaponHitResult, MuzzleLocation, TargetLocation, ECollisionChannel::ECC_Visibility);
            if (WeaponHitResult.bBlockingHit)
            {
                HitLocation = WeaponHitResult.ImpactPoint;
            }
            FVector2d ScreenLocation;
            FVector ImpictLoaction = WeaponHitResult.ImpactPoint;
            bool LocationIsScreen = Controller->ProjectWorldLocationToScreen(HitLocation, ScreenLocation);
            if (LocationIsScreen)
            {
                FVector2d ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
                FVector2d NormilizeScreenLocation = ScreenLocation / ViewportSize;
                FVector2d CorrectionParam = (FMath::Abs(ScreenLocation - (ViewportSize / 2.0f)) / 6.0f);

                const bool IsXTop = NormilizeScreenLocation.X > 0.5f;
                bool IsYLeft = NormilizeScreenLocation.Y > 0.5f;
                float OffsetX;
                float OffsetY;
                if (IsXTop)
                {
                    OffsetX = -0.25f;
                }
                else
                {
                    OffsetX = 0.25f;
                }
                if (IsYLeft)
                {
                    OffsetY = 0.25f;
                }
                else
                {
                    OffsetY = -0.25f;
                }
                MultiplayerX = MultiplayerX + OffsetX * FMath::Clamp(CorrectionParam.X, 0.0f, 1.0f);
                MultiplayerY = MultiplayerY + OffsetY * FMath::Clamp(CorrectionParam.Y, 0.0f, 1.0f);

                //UE_LOG(LogWeaponComponent, Display, TEXT("%f"), NormilizeScreenLocation.X);
                //UE_LOG(LogWeaponComponent, Display, TEXT("%s"), (IsXTop ? TEXT("true"): TEXT("false")));
                //UE_LOG(LogWeaponComponent, Display, TEXT("%f"), MultiplayerX);
            }
        }
    }

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UShooterWeaponComponent::GetAimingOffsetX() const
{
    return MultiplayerX;
}

float UShooterWeaponComponent::GetAimingOffsetY() const
{
    return MultiplayerY;
}



bool UShooterWeaponComponent::CanReload() const
{
    return CurrentWeapon             //
           && !EqipAnimInProgress    //
           && !ReloadAnimInProgress  //
           && CurrentWeapon->CanReload();
}

void UShooterWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void UShooterWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UShooterWeaponComponent::CanEquip() const
{
    return !EqipAnimInProgress && !ReloadAnimInProgress;
}
