// Fill out your copyright notice in the Description page of Project Settings.


#include "Reactors/HeatReactor.h"

#include "HeatSource/HeatSourceComponent.h"
#include "Systems/HeatSubSystem.h"

UHeatReactor::UHeatReactor()
{

}

void UHeatReactor::Init()
{

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("UpdateTemperature"));

	GetWorld()->GetTimerManager().SetTimer(_timerHandle, TimerDelegate, _temperatureUpdateTimer, true);
}

float UHeatReactor::GetCurrentTemperature()
{
	return _currentTemperature;
}

float UHeatReactor::GetCurrentCoolDuration()
{
	return _currentCoolDuration;
}

float UHeatReactor::GetBaseCoolDuration()
{
	return _baseCoolDuration;
}

float UHeatReactor::GetActivationTemperature()
{
	return _activationTemperature;
}

void UHeatReactor::UpdateTemperature()
{
	_targetTemperature = GetWorld()->GetSubsystem<UHeatSubSystem>()->GetTemperatureAtLocation(GetOwner()->GetActorLocation() + _reactorRelativeLocation);

	if (_targetTemperature < _currentTemperature)
	{
		_currentTemperature = FMath::FInterpConstantTo(_currentTemperature, _targetTemperature, _temperatureUpdateTimer, _activationTemperature / _baseCoolDuration);
	}
	else if (_targetTemperature > _currentTemperature)
	{
		_currentTemperature = FMath::FInterpConstantTo(_currentTemperature, _targetTemperature, _temperatureUpdateTimer, _activationTemperature / _activationDuration);
	}

	if (_currentTemperature >= _activationTemperature && !IsReactorActive)
	{
		Heated();
	}

	else if (IsReactorActive && _currentTemperature <= _activationTemperature && !_isCooling)
	{
		Cool();
	}
}

void UHeatReactor::Cool()
{
	_isCooling = true;
	IsReactorActive = false;


	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("DeactivateReactor"), this);
	_currentCoolDuration = (_activationTemperature - _targetTemperature) / (_activationTemperature / _baseCoolDuration);
	
	GetWorld()->GetTimerManager().SetTimer(_temperatureTimerHandle, TimerDelegate, _currentCoolDuration, false);

	OnCooling.Broadcast();

}

void UHeatReactor::Heated()
{
	_isCooling = false;
	IsReactorActive = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ActivateReactor"), this);

	GetWorld()->GetTimerManager().SetTimer(_temperatureTimerHandle, TimerDelegate, _activationDuration, false);

	OnHeating.Broadcast();

}

void UHeatReactor::ActivateReactor()
{

	OnReact.Broadcast();
}

void UHeatReactor::DeactivateReactor()
{
	OnStopReact.Broadcast();
	IsReactorActive = false;
}


