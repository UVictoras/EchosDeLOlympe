// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/SoundSystem.h"

// Sets default values
ASoundSystem::ASoundSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASoundSystem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

