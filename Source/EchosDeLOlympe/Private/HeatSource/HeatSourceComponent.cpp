#include "HeatSource/HeatSourceComponent.h"
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

}

void UHeatSourceComponent::BeginPlay()
{
	Super::BeginPlay();

	//if (_heatZone)
	//	_heatZone->SetSphereRadius(_emissionRadius);

	//else
	//	UE_LOG(LogTemp, Warning, TEXT("Collision Sphere not set"));

}

float UHeatSourceComponent::GetDistance(UPrimitiveComponent* component)
{
	FVector ClosestPoint;

	float Distance = component->GetClosestPointOnCollision(_heatZone->GetComponentLocation(), ClosestPoint);

	return Distance;
}

float UHeatSourceComponent::GetObjectTemperature(UPrimitiveComponent* component)
{
	float distance = GetDistance(component);

	float temperature = _minTemperature + (_maxTemperature - _minTemperature) * (1 - (distance - _minDistance) / (_emissionRadius - _minDistance));

	return temperature;
}

