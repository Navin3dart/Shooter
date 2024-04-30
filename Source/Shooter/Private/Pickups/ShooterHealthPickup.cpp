// ShooterGame. All Rights Reserved.


#include "Pickups/ShooterHealthPickup.h"
#include "component/ShooterHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool AShooterHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn) return false;
    //UE_LOG(LogHealthPickup, Display, TEXT("Health was taken!"));
    const auto Component = PlayerPawn->GetComponentByClass(UShooterHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UShooterHealthComponent>(Component);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->AddHealth(HealthAmount);
}
