// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "HeatSubSystem.generated.h"


class UHeatSourceComponent;

UCLASS()
class UHeatSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterSource(UHeatSourceComponent* source);
	void UnregisterSource(UHeatSourceComponent* source);

	UFUNCTION(BlueprintCallable)
	UHeatSourceComponent* GetNearestHeatSource(FVector location);
	UFUNCTION(BlueprintCallable)
	float GetTemperatureAtLocation(FVector location);

private:
	TSet<UHeatSourceComponent*> _registeredHeatSources;
};
