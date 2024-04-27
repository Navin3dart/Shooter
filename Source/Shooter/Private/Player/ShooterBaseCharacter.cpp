// ShooterGame. All Rights Reserved.


#include "Player/ShooterBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/ShooterHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"


DEFINE_LOG_CATEGORY_STATIC(LogShooter, All, All)


AShooterBaseCharacter::AShooterBaseCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<UShooterHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());

}


void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
    CurrentSpeed = 300.0f;
    TargetSpeed = 300.0f;
    InterpSpeed = 4.0f;
    //ErrorTolerance = 1.0f;

    //GetWorldTimerManager().SetTimer(TimerHandleAutoHeal, this, &UShooterHealthComponent::TimerAutoHeal, TimerRate, true);

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &AShooterBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &AShooterBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &AShooterBaseCharacter::OnGroundLanded);
}


void AShooterBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (!(FMath::IsNearlyEqual(TargetSpeed, CurrentSpeed, 0.1)))
    {
        CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, InterpSpeed);
        GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
        //UE_LOG(LogShooter, Display, TEXT("Tick"));
    }
}

void AShooterBaseCharacter::OnHealthChanged(float Health) 
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

// Called to bind functionality to input
void AShooterBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AShooterBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AShooterBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AShooterBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShooterBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShooterBaseCharacter::Run);
    PlayerInputComponent->BindAction("Walk", IE_Released, this, &AShooterBaseCharacter::Walk);
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
        UE_LOG(LogShooter, Display, TEXT("Run"));
        TargetSpeed = 600.0f;
        //GetCharacterMovement()->MaxWalkSpeed = 600;
    }
}

void AShooterBaseCharacter::Walk() 
{
    UE_LOG(LogShooter, Display, TEXT("Walk"));
    TargetSpeed = 300.0f;
    //GetCharacterMovement()->MaxWalkSpeed = 300;
}

void AShooterBaseCharacter::OnDeath()
{
    UE_LOG(LogShooter, Display, TEXT("Player %s is dead"), *GetName());

    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}


void AShooterBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(LogShooter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return;
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(LogShooter, Display, TEXT("FinalDamage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

