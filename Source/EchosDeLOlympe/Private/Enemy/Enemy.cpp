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
	
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsBool("IsStatic", )
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