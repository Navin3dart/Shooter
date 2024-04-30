// ShooterGame. All Rights Reserved.


#include "UI/ShooterPlayerHUDWidget.h"
#include "component/ShooterWeaponComponent.h"
#include "component/ShooterHealthComponent.h"

float UShooterPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;

    const auto Component = Player->GetComponentByClass(UShooterHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UShooterHealthComponent>(Component);
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool UShooterPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return false;

    const auto Component = Player->GetComponentByClass(UShooterWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UShooterWeaponComponent>(Component);
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}
