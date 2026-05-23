#include "Enemy/Enemy.h"
#include "Enemy/EnemyController.h"
#include "Reactors/HeatReactor.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	_reactor = CreateDefaultSubobject<UHeatReactor>(TEXT("Enemy Reactor"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AEnemyController* controller = Cast<AEnemyController>(GetController());;

	if (!controller)
		return;

	_blackboard = controller->GetBlackboardComponent();
	_blackboard->SetValueAsBool("IsStatic", _isStatic);
	_blackboard->SetValueAsBool("IsActive", IsActive);

	_reactor->Init();

	_reactor->OnReact.AddDynamic(this, &AEnemy::Activate);
	_reactor->OnStopReact.AddDynamic(this, &AEnemy::Deactivate);
	_reactor->OnHeating.AddDynamic(this, &AEnemy::Heat);
	_reactor->OnCooling.AddDynamic(this, &AEnemy::Cool);

}

void AEnemy::Activate()
{
	IsActive = true;
	_blackboard->SetValueAsBool("IsActive", IsActive);
	_blackboard->SetValueAsBool("NeedToHeat", false);
}

void AEnemy::Deactivate()
{
	IsActive = false;
	_blackboard->SetValueAsBool("IsActive", IsActive);
	_coolDuration = 0;
}

void AEnemy::Heat()
{
}

void AEnemy::Cool()
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Cool"));
	_coolDuration = _reactor->GetBaseCoolDuration();
	_currentCoolDuration = _reactor->GetCurrentCoolDuration();

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("NeedHeat"), this);
	GetWorld()->GetTimerManager().SetTimer(_needHeatHandle, TimerDelegate, _currentCoolDuration * 0.4, false);

}

void AEnemy::NeedHeat()
{
	_blackboard->SetValueAsBool("NeedToHeat", true);
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

float AEnemy::GetCurrentTemperature()
{
	return _reactor->GetCurrentTemperature();
}

float AEnemy::GetCoolDuration()
{
	return _coolDuration;
}

float AEnemy::GetCurrentCoolDuration()
{
	return _currentCoolDuration;
}
