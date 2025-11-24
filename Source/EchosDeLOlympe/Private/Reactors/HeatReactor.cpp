// Fill out your copyright notice in the Description page of Project Settings.


#include "Reactors/HeatReactor.h"

#include "HeatSource/HeatSourceComponent.h"

UHeatReactor::UHeatReactor()
{

}

void UHeatReactor::Init()
{

	_reactorOverlapComponent = Cast<UPrimitiveComponent>(_componentReference.GetComponent(GetOwner()));

	if (_reactorOverlapComponent)
	{
		_reactorOverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &UHeatReactor::OnReactorOverlap);
		_reactorOverlapComponent->OnComponentEndOverlap.AddDynamic(this, &UHeatReactor::OnReactorEndOverlap);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Reactor Overlap Component not Defined"));
	}
}

void UHeatReactor::OnReactorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UHeatSourceComponent* source = OtherActor->GetComponentByClass<UHeatSourceComponent>())
	{
		_objectsTemperature.Emplace(source, source->GetObjectTemperature(_reactorOverlapComponent));

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("UpdateTemperature"), source);

		GetWorld()->GetTimerManager().SetTimer(_timerHandle, TimerDelegate, _timerInterval, true);
	}
}

void UHeatReactor::OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UHeatSourceComponent* source = OtherActor->GetComponentByClass<UHeatSourceComponent>())
	{
		_objectsTemperature.Remove(source);

		if (_objectsTemperature.Num() == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
			Cool();
		}
	}

}

void UHeatReactor::UpdateTemperature(UHeatSourceComponent* source)
{
	float maxTemp = 0;

	for (TPair<UHeatSourceComponent*, float>& Elem : _objectsTemperature)
	{
		UHeatSourceComponent* source = Elem.Key;
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
		ActivateReactor();
	}

	else if (IsActive && _currentTemperature <= _activationTemperature)
	{
		DeactivateReactor();
	}
}

void UHeatReactor::Cool_Implementation()
{
	_currentTemperature = 0;

	DeactivateReactor();

}

void UHeatReactor::ActivateReactor_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Activate"));

	OnReact.Broadcast();
	IsActive = true;
}

void UHeatReactor::DeactivateReactor_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Deactivate"));
	OnStopReact.Broadcast();
	IsActive = false;
}


