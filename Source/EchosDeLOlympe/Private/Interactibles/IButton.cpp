// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactibles/IButton.h"

// Sets default values
AIButton::AIButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIButton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

