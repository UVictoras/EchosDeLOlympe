// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactibles/ILever.h"

AILever::AILever()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
}

void AILever::Interact_Implementation(AActor* Interactor)
{
    UE_LOG(LogTemp, Warning, TEXT("Bouton pressé par %s"), *Interactor->GetName());

}