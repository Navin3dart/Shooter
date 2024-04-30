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

	void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload();

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<AShooterBaseWeapon> WeaponType, int32 ClipsAmount);

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

private:

	UPROPERTY()
    AShooterBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
    TArray<AShooterBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0;
    bool EqipAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void SpawnWeapons();
    void AttachWeaponToSocket(AShooterBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
    void EquipWeapon(int32 WeaponIndex);

    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimations();
    void OnEquipFinished(USkeletalMeshComponent* MeshComponent);
    void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

    bool CanFire() const;
    bool CanEquip() const;
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
