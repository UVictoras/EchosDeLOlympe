// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "Components/ActorComponent.h"
#include "HeatReactor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopReact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeating);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCooling);

class UHeatSourceComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHeatReactor : public UActorComponent
{
	GENERATED_BODY()

public:
	UHeatReactor();

	UPROPERTY(BlueprintReadOnly)
	bool IsReactorActive;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnReact OnReact;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStopReact OnStopReact;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHeating OnHeating;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCooling OnCooling;


	UFUNCTION(BlueprintCallable)
	virtual void Init();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetCurrentTemperature();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	virtual float GetCurrentCoolDuration();

	UFUNCTION(BlueprintCallable)
	virtual float GetBaseCoolDuration();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual float GetActivationTemperature();

protected:

	float _currentTemperature;
	float _targetTemperature;
	bool _isCooling;

	UPROPERTY(EditAnywhere)
	FVector _reactorRelativeLocation;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _activationTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _activationDuration;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _baseCoolDuration;

	float _currentCoolDuration;

	UFUNCTION()
	virtual void UpdateTemperature();


	UFUNCTION()
	void Cool();

	UFUNCTION()
	void Heated();

	UFUNCTION()
	void ActivateReactor();

	UFUNCTION()
	void DeactivateReactor();

private:

	FTimerHandle _timerHandle;
	FTimerHandle _temperatureTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _temperatureUpdateTimer = 0.1; // At 0 temperature is not updating

	UPROPERTY()
	int _overlappedSourceCount = 1;

		
};
