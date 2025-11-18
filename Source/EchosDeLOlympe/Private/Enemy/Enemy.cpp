#include "Enemy/Enemy.h"
#include "Reactors/HeatReactor.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	_reactor->Init();

	_reactor->OnReact.AddDynamic(this, &AEnemy::Activate);
	_reactor->OnStopReact.AddDynamic(this, &AEnemy::Deactivate);
	
}

void AEnemy::Activate()
{
	IsActive = true;
}

void AEnemy::Deactivate()
{
	IsActive = false;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AEnemy::GetBehaviorTree()
{
	return m_tree;
}