// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/ShooterBaseWeapon.h"
#include "ShooterWeaponComponent.generated.h"

class AShooterBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AShooterBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UShooterWeaponComponent();

	virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void Reload();
    void DetachWeapons();
    void Aiming(bool IsAiming);

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;
    float GetWeaponSpreadRadius() const { return CurrentWeapon->GetCurrentSpreadRadius(); }

    bool TryToAddAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType, int32 ClipsAmount);

    UPROPERTY()
    AShooterBaseWeapon* CurrentWeapon = nullptr;

    UFUNCTION(Category = "Weapon")
    FVector GetMuzzleWorldLocation() const;

    bool CanFire() const;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "AIMING")
    float GetAimingOffsetX() const;

    UFUNCTION(BlueprintCallable, Category = "AIMING")
    float GetAimingOffsetY() const;

    void CalculateAimingOffset();

    FVector2d GetHitWeaponLocationOnScreen();

    FVector2d ScreenLocation;

    float ShotOffset;

protected:

	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Socket")
    FName WeaponEquipSocketName = "weapon_rHandSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Socket")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon|Animation")
    UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    TArray<AShooterBaseWeapon*> Weapons;

    //bool CanFire() const;
    bool CanEquip() const;
    void EquipWeapon(int32 WeaponIndex);

    void UpdateSpreadRadiusForSpeed();


    float MultiplayerX;
    float MultiplayerY;
    int32 CurrentWeaponIndex = 0;

private:

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;


    bool EqipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void SpawnWeapons();
    void AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();


    template<typename T>
    T* FindNotifyByClass(UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;

        const auto NotifyEvents = Animation->Notifies;

        for (auto NotifyEvent : NotifyEvents)
        {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify);
            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};
