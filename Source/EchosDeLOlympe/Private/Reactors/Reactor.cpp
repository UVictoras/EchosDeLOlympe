// Fill out your copyright notice in the Description page of Project Settings.


#include "Reactors/Reactor.h"
#include "HeatSource/HeatSource.h"

AReactor::AReactor()
{

}

void AReactor::BeginPlay()
{
	Super::BeginPlay();

	_reactorOverlapComponent = Cast<UPrimitiveComponent>(_componentReference.GetComponent(this));
	
	if (_reactorOverlapComponent)
	{
		_reactorOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AReactor::OnReactorOverlap);
		_reactorOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &AReactor::OnReactorEndOverlap);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Reactor Overlap Component not Defined"));
	}

	
	
}

void AReactor::OnReactorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHeatSource* source = Cast<AHeatSource>(OtherActor))
	{
		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUFunction(this, FName("UpdateTemperature"), source, OverlappedComponent);

		GetWorld()->GetTimerManager().SetTimer(_timerHandle, TimerDelegate, _timerInterval, true);
	}
}

void AReactor::OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
}

void AReactor::UpdateTemperature(AHeatSource* source, UPrimitiveComponent* myComp)
{
	float temp = source->GetObjectTemperature(myComp);

	UE_LOG(LogTemp, Warning, TEXT("Temperature: %f"), temp);
}


