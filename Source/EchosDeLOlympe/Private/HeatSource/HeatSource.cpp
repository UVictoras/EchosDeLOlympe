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
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindUFunction(this, FName("GetDistance"), OtherComp);

	GetWorld()->GetTimerManager().SetTimer(_timerHandle, TimerDelegate, _timerInterval, true);
}

void AHeatSource::OnHeatZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
}

void AHeatSource::GetDistance(UPrimitiveComponent* component)
{
	FVector ClosestPoint;

	float Distance = component->GetClosestPointOnCollision(_heatZone->GetComponentLocation(),ClosestPoint);

	UE_LOG(LogTemp, Warning, TEXT("Distance à l'objet: %f"), Distance);
}
