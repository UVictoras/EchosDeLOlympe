// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyController.h"
#include "Enemy/Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AEnemy* enemy = Cast<AEnemy>(InPawn);
	if (!enemy)
		return;

	UBehaviorTree* tree = enemy->GetBehaviorTree();

	UBlackboardComponent* board;
	UseBlackboard(tree->BlackboardAsset, board);
	Blackboard = board;
	RunBehaviorTree(tree);
}

void AEnemyController::SetupPerceptionSystem()
{
	/*m_sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	if (!m_sightConfig)
		return;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	m_sightConfig->SightRadius = 500.f;
	m_sightConfig->LoseSightRadius = m_sightConfig->SightRadius + 25.f;
	m_sightConfig->PeripheralVisionAngleDegrees = 90.f;
	m_sightConfig->SetMaxAge(5.f);
	m_sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
	m_sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	m_sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*m_sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIEnemyController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*m_sightConfig);*/

}

void AEnemyController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{

}