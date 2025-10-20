// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Reactor.generated.h"

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

	
	UPrimitiveComponent* _reactorOverlapComponent;

	UFUNCTION()
	void OnReactorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnReactorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
