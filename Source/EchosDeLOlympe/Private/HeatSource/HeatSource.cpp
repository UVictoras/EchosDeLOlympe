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

}

void AHeatSource::BeginPlay()
{
	Super::BeginPlay();

	_heatZone->bHiddenInGame = false;
	_heatZone->SetSphereRadius(_emissionRadius, true);
	
}

void AHeatSource::OnHeatZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeatZone overlap with: %s"), *OtherActor->GetName());
	}
}
