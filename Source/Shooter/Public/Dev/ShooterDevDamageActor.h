// ShooterGame. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterDevDamageActor.generated.h"

UCLASS()
class SHOOTER_API AShooterDevDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterDevDamageActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float Radius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FColor SphereColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Damage")
    float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Damage")
    bool DoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug|Damage")
    TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
