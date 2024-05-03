// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTER_API AShooterBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AShooterBasePickup();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Pickup")
    float RespawnTime = 5.0f;

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;



public:	
	virtual void Tick(float DeltaTime) override;

	bool CouldBeTaken() const;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn);
    void PickupWasTaken(); 
	void Respawn();

	FTimerHandle RespawnTimerHandle;

};
