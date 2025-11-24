// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interactibles/Interactible.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
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

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouch);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("SideStrafe", this, &AMainCharacter::StrafeSide);

	PlayerInputComponent->BindAxis("TurnCamera", this, &AMainCharacter::TurnCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	PlayerInputComponent->BindAxis("Interact", this, &AMainCharacter::Interact);
}

void AMainCharacter::MoveForward(float InputValue)
{
	if (Controller && InputValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction =  FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, InputValue);

		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = FMath::RInterpTo(CurrentRotation, YawRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
		SetActorRotation(TargetRotation);
	}
}

void AMainCharacter::StrafeSide(float InputValue)
{
	if (Controller && InputValue != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, InputValue);

		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = FMath::RInterpTo(CurrentRotation, YawRotation, GetWorld()->GetDeltaSeconds(), 10.0f);
		SetActorRotation(TargetRotation);
	}
}

void AMainCharacter::TurnCamera(float InputValue)
{
	AddControllerYawInput(InputValue);
}

void AMainCharacter::LookUp(float InputValue)
{
	AddControllerPitchInput(InputValue);
}

void AMainCharacter::StartCrouch()
{
	Crouch();
}

void AMainCharacter::StopCrouch()
{
	UnCrouch();
}

void AMainCharacter::Interact(float InputValue)
{
	if (InputValue == 0.0f)
	{
		return;
	}

	if (!Camera)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Camera found on MainCharacter"));
		return;
	}

	FCollisionShape CollisionBox = FCollisionShape::MakeBox(FVector(50.f, 50.f, 50.f));

	FHitResult Hit;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * 1000.f;

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		Camera->GetComponentQuat(),
		ECC_Visibility,
		CollisionBox
	);

	if (bHit && Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor->GetClass()->ImplementsInterface(UInteractible::StaticClass()))
		{
			IInteractible::Execute_Interact(HitActor, this);
		}
	}
}