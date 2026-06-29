// Fill out your copyright notice in the Description page of Project Settings.

#include "Systems/HeatSubSystem.h"
#include "HeatSource/HeatSourceComponent.h"
#include "HeatSubSystem.h"

void UHeatSubSystem::RegisterSource(UHeatSourceComponent* source)
{
	_registeredHeatSources.Add(source);
}

void UHeatSubSystem::UnregisterSource(UHeatSourceComponent* source)
{
	_registeredHeatSources.Remove(source);
}

UHeatSourceComponent* UHeatSubSystem::GetNearestHeatSource(FVector location)
{
	float minDist = 999999;

	UHeatSourceComponent* nearestSource = nullptr;

	for (UHeatSourceComponent* source : _registeredHeatSources)
	{
		float currentDist = FVector::Distance(source->GetOwner()->GetActorLocation(), location);
		if (currentDist <= minDist)
		{
			minDist = currentDist;
			nearestSource = source;
		}
	}

	return nearestSource;
}

float UHeatSubSystem::GetTemperatureAtLocation(FVector location)
{
	UHeatSourceComponent* source = GetNearestHeatSource(location);

	if (source == nullptr)
		return 0;

	return source->GetTemperatureAtLocation(location);
}
