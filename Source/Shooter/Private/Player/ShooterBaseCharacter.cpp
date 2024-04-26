// ShooterGame. All Rights Reserved.


#include "Player/ShooterBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogShooter, All, All)

// Sets default values
AShooterBaseCharacter::AShooterBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

}


// Called when the game starts or when spawned
void AShooterBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
    CurrentSpeed = 300.0f;
    TargetSpeed = 300.0f;
    InterpSpeed = 4.0f;
    //ErrorTolerance = 1.0f;
	
}

// Called every frame
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
