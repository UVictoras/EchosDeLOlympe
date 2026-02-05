// Fill out your copyright notice in the Description page of Project Settings.


#include "Reactors/HeatReactor.h"

#include "HeatSource/HeatSourceComponent.h"
#include "Systems/HeatSubSystem.h"

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
		_overlappedSourceCount++;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("UpdateTemperature"), source);

		GetWorld()->GetTimerManager().SetTimer(_timerHandle, TimerDelegate, _timerInterval, true);
	}
}

void UHeatReactor::OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UHeatSourceComponent* source = OtherActor->GetComponentByClass<UHeatSourceComponent>())
	{
		_overlappedSourceCount = FMath::Clamp(_overlappedSourceCount - 1,0,_overlappedSourceCount);

		if (_overlappedSourceCount == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
			Cool();
		}
	}

}

void UHeatReactor::UpdateTemperature(UHeatSourceComponent* source)
{
	float maxTemp = 0;

	_currentTemperature = GetWorld()->GetSubsystem<UHeatSubSystem>()->GetTemperatureAtLocation(GetOwner()->GetActorLocation());

	if (_currentTemperature >= _activationTemperature && !IsActive )
	{
		Heated();
	}

	else if (IsActive && _currentTemperature <= _activationTemperature)
	{
		Cool();
	}
}

void UHeatReactor::Cool_Implementation()
{
	IsActive = false;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DeactivateReactor"), this);

	float duration = _activationDuration / (_activationTemperature / _currentTemperature);

	GetWorld()->GetTimerManager().SetTimer(_temperatureTimerHandle, TimerDelegate, duration, false);

}

void UHeatReactor::Heated_Implementation()
{
	IsActive = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ActivateReactor"), this);

	GetWorld()->GetTimerManager().SetTimer(_temperatureTimerHandle, TimerDelegate, _activationDuration, false);

}

void UHeatReactor::ActivateReactor_Implementation()
{

	OnReact.Broadcast();
}

void UHeatReactor::DeactivateReactor_Implementation()
{
	OnStopReact.Broadcast();
	IsActive = false;
}


