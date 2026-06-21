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

	//if (_heatZone)
	//	_heatZone->SetSphereRadius(_emissionRadius);

	//else
	//	UE_LOG(LogTemp, Warning, TEXT("Collision Sphere not set"));

}

float UHeatSourceComponent::GetTemperatureAtLocation(FVector position)
{
	if (!_isActive)
		return 0;

	float distance = FVector::Distance(_heatZone->GetComponentLocation(),position);

	float temperature = FMath::Clamp(_minTemperature + (_maxTemperature - _minTemperature) * (1 - (distance - _minDistance) / (_emissionRadius - _minDistance)),0,_maxTemperature);

	return temperature;
}

void UHeatSourceComponent::DeactivateSource()
{
	_isActive = false;
}

void UHeatSourceComponent::ActivateSource()
{
	_isActive = true;
}

bool UHeatSourceComponent::IsActive()
{
	return _isActive;
}