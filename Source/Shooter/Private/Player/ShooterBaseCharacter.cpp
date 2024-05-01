// ShooterGame. All Rights Reserved.


#include "Player/ShooterBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/ShooterHealthComponent.h"
#include "component/ShooterWeaponComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"



DEFINE_LOG_CATEGORY_STATIC(LogShooter, All, All)


AShooterBaseCharacter::AShooterBaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");

    WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("WeaponComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

}

void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
    CurrentSpeed = WalkSpeed;
    TargetSpeed = WalkSpeed;
    GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
    InterpSpeed = 4.0f;
    //ErrorTolerance = 1.0f;

    //GetWorldTimerManager().SetTimer(TimerHandleAutoHeal, this, &UShooterHealthComponent::TimerAutoHeal, TimerRate, true);

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    check(WeaponComponent);
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AShooterBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanded);

}

void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

   // UE_LOG(LogShooter, Display, TEXT("Speed: %f"), CurrentSpeed);
    if (!(FMath::IsNearlyEqual(TargetSpeed, CurrentSpeed, 0.1)))
    {
        CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, InterpSpeed);
        GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
        //UE_LOG(LogShooter, Display, TEXT("Speed: %f"), CurrentSpeed);
    }
}

void AShooterBaseCharacter::OnHealthChanged(float Health) 
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AShooterBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AShooterBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AShooterBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShooterBaseCharacter::Run);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AShooterBaseCharacter::Walk);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShooterWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShooterWeaponComponent::Reload);

}

float AShooterBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AShooterBaseCharacter::MoveForward(float Amount) 
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void AShooterBaseCharacter::MoveRight(float Amount) 
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void AShooterBaseCharacter::Run() 
{
    if (!GetVelocity().IsZero())
    {
        //UE_LOG(LogShooter, Display, TEXT("Run"));
        TargetSpeed = RunSpeed;
        //GetCharacterMovement()->MaxWalkSpeed = 600;
    }
}

void AShooterBaseCharacter::Walk() 
{
    //UE_LOG(LogShooter, Display, TEXT("Walk"));
    TargetSpeed = WalkSpeed;
    //GetCharacterMovement()->MaxWalkSpeed = 300;
}

void AShooterBaseCharacter::OnDeath()
{
    UE_LOG(LogShooter, Display, TEXT("Player %s is dead"), *GetName());

    //PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    //SetLifeSpan(1.0f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
    WeaponComponent->StopFire();
    WeaponComponent->DetachWeapons();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    //UE_LOG(LogShooter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    //UE_LOG(LogShooter, Display, TEXT("FinalDamage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}


