// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "Components/ActorComponent.h"
#include "HeatReactor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopReact);

class UHeatSourceComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHeatReactor : public UActorComponent
{
	GENERATED_BODY()

public:
	UHeatReactor();

	bool IsActive;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnReact OnReact;
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStopReact OnStopReact;


	UFUNCTION(BlueprintCallable)
	virtual void Init();

protected:

	float _currentTemperature;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _activationTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _activationDuration;

	UFUNCTION()
	virtual void OnReactorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void UpdateTemperature(UHeatSourceComponent* source);


	UFUNCTION(BlueprintNativeEvent)
	void Cool();
	virtual void Cool_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Heated();
	virtual void Heated_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void ActivateReactor();
	virtual void ActivateReactor_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void DeactivateReactor();
	virtual void DeactivateReactor_Implementation();

private:

	UPROPERTY(EditAnywhere)
	FComponentReference _componentReference;

	FTimerHandle _timerHandle;
	FTimerHandle _temperatureTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float _timerInterval;

	UPrimitiveComponent* _reactorOverlapComponent;

	UPROPERTY()
	TMap<UHeatSourceComponent*, float> _objectsTemperature;

		
};
