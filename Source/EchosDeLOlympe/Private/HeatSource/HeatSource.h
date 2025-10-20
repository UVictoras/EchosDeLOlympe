// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "HeatSource.generated.h"

UCLASS()
class AHeatSource : public AActor
{
	GENERATED_BODY()


private :
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _minTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _maxTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _emissionRadius;

	UPROPERTY(EditAnywhere, Category = "Timer")
	float _timerInterval;

	USphereComponent* _heatZone;

	FTimerHandle _timerHandle;


	UFUNCTION()
	void OnHeatZoneOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	UFUNCTION()
	void OnHeatZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void GetDistance(UPrimitiveComponent* component);
	
public:	
	AHeatSource();

protected:
	virtual void BeginPlay() override;


};
