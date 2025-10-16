// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("SideStrafe", this, &AMainCharacter::StrafeSide);
	PlayerInputComponent->BindAxis("TurnCamera", this, &AMainCharacter::TurnCamera);
}

void AMainCharacter::MoveForward(float InputValue)
{
	FVector ForwardDirection = GetActorForwardVector();

	AddMovementInput(ForwardDirection, InputValue);
}

void AMainCharacter::StrafeSide(float InputValue)
{
	FVector RightDirection = GetActorRightVector();

	AddMovementInput(RightDirection, InputValue);
}

void AMainCharacter::TurnCamera(float InputValue)
{
	AddControllerYawInput(InputValue);
}