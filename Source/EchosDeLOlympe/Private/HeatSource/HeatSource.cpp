// Fill out your copyright notice in the Description page of Project Settings.


#include "HeatSource/HeatSource.h"


AHeatSource::AHeatSource()
{
	_heatZone = CreateDefaultSubobject<USphereComponent>(TEXT("Heat Zone"));

	RootComponent = GetRootComponent();

	_heatZone->SetupAttachment(RootComponent);

	_heatZone->InitSphereRadius(_emissionRadius);
	_heatZone->SetCollisionProfileName("OverlapAll");

	_heatZone->OnComponentBeginOverlap.AddDynamic(this, &AHeatSource::OnHeatZoneOverlap);
	_heatZone->OnComponentEndOverlap.AddDynamic(this, &AHeatSource::OnHeatZoneEndOverlap);

}

void AHeatSource::BeginPlay()
{
	Super::BeginPlay();

	_heatZone->bHiddenInGame = false;
	_heatZone->SetSphereRadius(_emissionRadius, true);
	
}

void AHeatSource::OnHeatZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AHeatSource::OnHeatZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

float AHeatSource::GetDistance(UPrimitiveComponent* component)
{
	FVector ClosestPoint;

	float Distance = component->GetClosestPointOnCollision(_heatZone->GetComponentLocation(),ClosestPoint);

	return Distance;
}

float AHeatSource::GetObjectTemperature(UPrimitiveComponent* component)
{
	float distance = GetDistance(component);

	float temperature = _minTemperature + (_maxTemperature - _minTemperature) * (1 - (distance - _minDistance) / (_emissionRadius - _minDistance));

	return temperature;
}
