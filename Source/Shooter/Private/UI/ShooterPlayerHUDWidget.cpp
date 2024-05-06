// ShooterGame. All Rights Reserved.


#include "UI/ShooterPlayerHUDWidget.h"
#include "component/ShooterWeaponComponent.h"
#include "component/ShooterHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWidget, All, All)

float UShooterPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = GetHealthComponent();
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

float UShooterPlayerHUDWidget::GetStaminaPercent() const
{
    const auto HealthComponent = GetHealthComponent();
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetStaminaPercent();
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

float UShooterPlayerHUDWidget::GetWeaponSpreadRadius() const
{

    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;

    const auto Component = Player->GetComponentByClass(UShooterWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UShooterWeaponComponent>(Component);
    if (!WeaponComponent) return 0.0f;

    return WeaponComponent->GetWeaponSpreadRadius();
}



bool UShooterPlayerHUDWidget::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return false;

    const auto Component = Player->GetComponentByClass(UShooterWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UShooterWeaponComponent>(Component);
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

bool UShooterPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = GetHealthComponent();
    return HealthComponent && !HealthComponent->IsDead();
}

bool UShooterPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UShooterPlayerHUDWidget::Initialize()
{
    const auto HealthComponent = GetHealthComponent();
    if (HealthComponent)
    {
        HealthComponent->OnHealthChanged.AddUObject(this, &UShooterPlayerHUDWidget::OnHealthChanged);
    }
    return Super::Initialize();
}

UShooterHealthComponent* UShooterPlayerHUDWidget::GetHealthComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;

    const auto Component = Player->GetComponentByClass(UShooterHealthComponent::StaticClass());
    const auto HealthComponent = Cast<UShooterHealthComponent>(Component);

    return HealthComponent;
}

void UShooterPlayerHUDWidget::OnHealthChanged(float Health) 
{
    OnTakeDamage();
}









