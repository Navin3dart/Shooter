// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework//Character.h"
#include "ShooterBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTexture2D* MainIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    UTexture2D* CrossHairIcon;
};

UCLASS()
class SHOOTER_API AShooterBaseWeapon : public AActor
{
	GENERATED_BODY()

public:	
	AShooterBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();

    bool CanReload() const;

    FWeaponUIData GetUIData() const { return UIData; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket"; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float WeaponDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|UI")
    FWeaponUIData UIData;

    bool IsShot;



	virtual void BeginPlay() override;

	virtual void MakeShot();

    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    FVector GetMuzzleWorldLocation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void LogAmmo();

private:
    FAmmoData CurrentAmmo;

};