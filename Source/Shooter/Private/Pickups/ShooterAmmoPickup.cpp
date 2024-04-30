// ShooterGame. All Rights Reserved.


#include "Pickups/ShooterAmmoPickup.h"
#include "component/ShooterWeaponComponent.h"
#include "component/ShooterHealthComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool AShooterAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    if (!PlayerPawn) return false;
    const auto Component = PlayerPawn->GetComponentByClass(UShooterHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UShooterHealthComponent>(Component);
    if (!HealthComponent || HealthComponent->IsDead()) return false;


    const auto ComponentW = PlayerPawn->GetComponentByClass(UShooterWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UShooterWeaponComponent>(ComponentW);
    if (!WeaponComponent) return false;

    UE_LOG(LogAmmoPickup, Display, TEXT("Ammo was taken!"));
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
