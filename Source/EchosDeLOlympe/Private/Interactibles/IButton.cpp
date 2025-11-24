// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactibles/IButton.h"

AIButton::AIButton()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    OnInteracted.AddDynamic(this, &AIButton::Animate);
}

void AIButton::Interact_Implementation(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("Bouton pressé par %s"), *Interactor->GetName());
    OnInteracted.Broadcast(Interactor);
}

void AIButton::Animate(AActor* Interactor)
{

}