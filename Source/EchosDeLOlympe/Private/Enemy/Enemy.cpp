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

	_reactor->Init();

	_reactor->OnReact.AddDynamic(this, &AEnemy::Activate);
	_reactor->OnStopReact.AddDynamic(this, &AEnemy::Deactivate);

	AEnemyController* controller = Cast<AEnemyController>(GetController());
	
	if (!controller)
		return;
	
	_blackboard = controller->GetBlackboardComponent();
	_blackboard->SetValueAsBool("IsStatic", _isStatic);
	_blackboard->SetValueAsBool("IsActive", IsActive);
}

void AEnemy::Activate()
{
	IsActive = true;
	_blackboard->SetValueAsBool("IsActive", IsActive);
}

void AEnemy::Deactivate()
{
	IsActive = false;
	_blackboard->SetValueAsBool("IsActive", IsActive);
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