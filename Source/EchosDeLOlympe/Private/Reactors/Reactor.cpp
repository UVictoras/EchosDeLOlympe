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
		_objectsTemperature.Emplace(source, source->GetObjectTemperature(_reactorOverlapComponent));

		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUFunction(this, FName("UpdateTemperature"), source);

		GetWorld()->GetTimerManager().SetTimer(_timerHandle, TimerDelegate, _timerInterval, true);
	}
}

void AReactor::OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AHeatSource* source = Cast<AHeatSource>(OtherActor))
	{
		_objectsTemperature.Remove(source);

		if (_objectsTemperature.Num() == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
			Cool();
		}
	}
	
}

void AReactor::UpdateTemperature(AHeatSource* source)
{
	float maxTemp = 0;

	for (TPair<AHeatSource*, float>& Elem : _objectsTemperature)
	{
		AHeatSource* source = Elem.Key;
		float& temperature = Elem.Value;

		if (IsValid(source))
		{
			temperature = source->GetObjectTemperature(_reactorOverlapComponent);

			if (temperature > maxTemp)
				maxTemp = temperature;
		}

	}

	_currentTemperature = maxTemp;

	if (_currentTemperature >= _activationTemperature)
	{
		Activate();
	}

	else if (_isActive && _currentTemperature <= _activationTemperature)
	{
		Deactivate();
	}
}

void AReactor::Cool_Implementation()
{
	_currentTemperature = 0;

	Deactivate();

}

void AReactor::Activate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate"));
	_isActive = true;
}

void AReactor::Deactivate_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivate"));
	_isActive = false;
}
