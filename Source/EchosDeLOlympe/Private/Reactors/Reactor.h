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
	virtual void BeginPlay() override;	

private:
	UPROPERTY(EditAnywhere)
	FComponentReference _componentReference;

	FTimerHandle _timerHandle;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float _timerInterval;
	
	UPrimitiveComponent* _reactorOverlapComponent;

	UPROPERTY()
	TMap<AHeatSource*, float> _objectsTemperature;
	float _currentTemperature;

	UFUNCTION()
	void OnReactorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void UpdateTemperature(AHeatSource* source);

};
