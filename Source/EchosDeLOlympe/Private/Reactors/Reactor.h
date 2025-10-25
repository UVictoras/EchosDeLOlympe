// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "Reactor.generated.h"

class AHeatSource;

UCLASS()
class AReactor : public AActor
{
	GENERATED_BODY()
	
public:	
	AReactor();

protected:

	float _currentTemperature;

	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _activationTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _coolingDuration;

	bool _isActive;

	virtual void BeginPlay() override;	

	UFUNCTION()
	virtual void OnReactorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void UpdateTemperature(AHeatSource* source);
	
	
	UFUNCTION(BlueprintNativeEvent)
	void Cool();
	virtual void Cool_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void Activate();
	virtual void Activate_Implementation();
	UFUNCTION(BlueprintNativeEvent)
	void Deactivate();
	virtual void Deactivate_Implementation();

private:
	UPROPERTY(EditAnywhere)
	FComponentReference _componentReference;

	FTimerHandle _timerHandle;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float _timerInterval;
	
	UPrimitiveComponent* _reactorOverlapComponent;

	UPROPERTY()
	TMap<AHeatSource*, float> _objectsTemperature;

};
