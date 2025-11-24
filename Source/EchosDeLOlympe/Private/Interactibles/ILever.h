// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactible.h"
#include "ILever.generated.h"

UCLASS()
class AILever : public AActor, public IInteractible
{
	GENERATED_BODY()
	
public:	
	AILever();

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

};
