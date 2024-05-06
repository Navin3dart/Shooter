// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UShooterHealthComponent;
class UTextRenderComponent;
class UShooterWeaponComponent;


UCLASS()
class SHOOTER_API AShooterBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:

    AShooterBaseCharacter(const FObjectInitializer& ObjInit);

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UShooterHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UShooterWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation|Montage")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(20.0f, 100.0f);



    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float WalkSpeed = 250.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Settings|Movement")
    float RunSpeed = 500.0f;

    virtual void BeginPlay() override;
    virtual void OnDeath();

public:
    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    FVector GetMuzzleSoketLocation() const;



private:

    float CurrentSpeed;
    float TargetSpeed;
    float InterpSpeed;

    float CameraInterpSpeed = 10.0f;
    float CurrentCameraParam = 0.0f;
    float TargetCameraParam = 0.0f;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void Run();
    void Walk();

    void Aiming(bool IsAiming);

    void OnHealthChanged(float Health);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);


};
