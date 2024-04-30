// ShooterGame. All Rights Reserved.

#include "Pickups/ShooterBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

AShooterBasePickup::AShooterBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);

}


void AShooterBasePickup::BeginPlay()
{
	Super::BeginPlay();

    check(CollisionComponent);
	
}

void AShooterBasePickup::NotifyActorBeginOverlap(AActor* OtherActor) 
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
    //UE_LOG(LogBasePickup, Display, TEXT("Pickup!"));
}


void AShooterBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AShooterBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void AShooterBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetRootComponent()->SetVisibility(false, true);

    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AShooterBasePickup::Respawn, RespawnTime);
}

void AShooterBasePickup::Respawn() 
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    GetRootComponent()->SetVisibility(true, true);
}

