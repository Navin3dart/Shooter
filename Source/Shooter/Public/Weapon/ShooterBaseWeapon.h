// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework//Character.h"
#include "ShooterBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

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

USTRUCT(BlueprintType)
struct FWeaponSpreadData
{
    GENERATED_USTRUCT_BODY()

    float CurrentSpread;
    float MinSpreadCurrent;
    float MaxSpreadCurrent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spread")
    float MinSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spread")
    float MaxSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spread")
    float DeltaSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spread")
    float InterpSpeed;
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
    void SimulatePhysics();

    //void ChangeSpreadRadius(float ModifyerSpread); //aiming
    void UpdateSpreadRadius();

    bool CanReload() const;

    FWeaponUIData GetUIData() const { return UIData; }

    FAmmoData GetAmmoData() const { return CurrentAmmo; }

    float GetCurrentSpreadRadius() const { return SpreadData.CurrentSpread; }

    bool TryToAddAmmo(int32 ClipsAmount);
    bool IsAmmoEmpty() const;

    virtual void Tick(float DeltaTime) override;
    FVector GetMuzzleWorldLocation() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket"; 

    float SpreadAiming = 1.0f;
    float WalkModifyerSpread = 1.0f;

    float GetShotOffset();
    virtual void SetShotOffset(float NewOffset);

protected:

    	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 15000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float WeaponDamage = 10.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FWeaponSpreadData SpreadData{0.0f, 0.2f, 3.0f, 0.1f, 2.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|VFX")
    UNiagaraSystem* MuzzleVFX;

    bool IsShot;
    bool IsShooting = false;

    FAmmoData CurrentAmmo;

    virtual void BeginPlay() override;

	virtual void MakeShot();

    APlayerController* GetPlayerController() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
  
    float ShotOffset;


    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
    
    void DecreaseAmmo();
    bool IsClipEmpty() const;
    void LogAmmo();
    bool IsAmmoFull();
    void DisableCollision();

    UNiagaraComponent* SpawnMuzzleVFX();

private:
    //FAmmoData CurrentAmmo;
    FTimerHandle CollisionHandle;



};
