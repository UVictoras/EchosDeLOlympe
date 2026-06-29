#include "HeatSource/HeatSourceComponent.h"
#include "Systems/HeatSubSystem.h"
#include "Components/SphereComponent.h"

UHeatSourceComponent::UHeatSourceComponent()
{
}

void UHeatSourceComponent::Init()
{

	_heatZone = Cast<UPrimitiveComponent>(_componentReference.GetComponent(GetOwner()));

	if (Cast<USphereComponent>(_heatZone))
	{
		Cast<USphereComponent>(_heatZone)->SetSphereRadius(_emissionRadius);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Heat Zone not defined !"));
	}
}

void UHeatSourceComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UHeatSubSystem>()->RegisterSource(this);

}

void UHeatSourceComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (!IsValid(GetWorld())) return;
	if (!IsValid(GetWorld()->GetSubsystem<UHeatSubSystem>())) return;

	GetWorld()->GetSubsystem<UHeatSubSystem>()->UnregisterSource(this);
}

float UHeatSourceComponent::GetTemperatureAtLocation(FVector position)
{
	if (!_isSourceActive)
		return 0;

	float distance = FVector::Distance(_heatZone->GetComponentLocation(),position);

	float temperature = FMath::Clamp(_minTemperature + (_maxTemperature - _minTemperature) * (1 - (distance - _minDistance) / (_emissionRadius - _minDistance)),0,_maxTemperature);

	return temperature;
}

void UHeatSourceComponent::DeactivateSource()
{
	_isSourceActive = false;
	GetWorld()->GetSubsystem<UHeatSubSystem>()->UnregisterSource(this);
}

void UHeatSourceComponent::ActivateSource()
{
	_isSourceActive = true;
	GetWorld()->GetSubsystem<UHeatSubSystem>()->RegisterSource(this);

}

bool UHeatSourceComponent::IsSourceActive()
{
	return _isSourceActive;
}