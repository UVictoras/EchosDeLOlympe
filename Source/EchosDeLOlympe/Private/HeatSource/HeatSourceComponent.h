// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "HeatSourceComponent.generated.h"


UCLASS( Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHeatSourceComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _minTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _maxTemperature;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _emissionRadius;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float _minDistance;

	bool _isSourceActive = true;

	UPrimitiveComponent* _heatZone;

	UPROPERTY(EditAnywhere)
	FComponentReference _componentReference;

	UFUNCTION(BlueprintCallable)
	void Init();

public:

	UHeatSourceComponent();

	float GetTemperatureAtLocation(FVector position);

	UFUNCTION(BlueprintCallable)
	void DeactivateSource();
	UFUNCTION(BlueprintCallable)
	void ActivateSource();
	bool IsSourceActive();

protected:
	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
