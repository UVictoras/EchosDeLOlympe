// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactible.h"
#include "IButton.generated.h"

UCLASS()
class AIButton : public AActor, public IInteractible
{
    GENERATED_BODY()

public:
    AIButton();

    virtual void Interact_Implementation(AActor* Interactor) override;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, Interactor);

    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FOnInteracted OnInteracted;
    
protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UFUNCTION()
    void Animate(AActor* Interactor);
};