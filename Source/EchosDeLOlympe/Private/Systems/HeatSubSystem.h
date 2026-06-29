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
	UFUNCTION(BlueprintCallable)
	void RegisterSource(UHeatSourceComponent* source);
	UFUNCTION(BlueprintCallable)
	void UnregisterSource(UHeatSourceComponent* source);

	UFUNCTION(BlueprintCallable)
	UHeatSourceComponent* GetNearestHeatSource(FVector location);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTemperatureAtLocation(FVector location);

private:
	TSet<UHeatSourceComponent*> _registeredHeatSources;
};
