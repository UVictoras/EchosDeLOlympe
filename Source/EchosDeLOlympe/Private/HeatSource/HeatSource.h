// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
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

	USphereComponent* _heatZone;


	UFUNCTION()
	void OnHeatZoneOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	
public:	
	AHeatSource();

protected:
	virtual void BeginPlay() override;


};
