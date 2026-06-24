// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyController.h"
#include "Enemy/Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"


AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyController::SetupPerceptionSystem()
{
	m_sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	m_hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	if (!m_sightConfig || !m_hearingConfig)
		return;

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	m_sightConfig->SightRadius = 10000.f;
	m_sightConfig->LoseSightRadius = m_sightConfig->SightRadius + 5000.f;
	m_sightConfig->PeripheralVisionAngleDegrees = 75.f;
	m_sightConfig->SetMaxAge(0.f);
	m_sightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
	m_sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	m_sightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	m_hearingConfig->HearingRange = 1000.f;
	m_hearingConfig->SetMaxAge(0.f);
	m_hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	m_hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	m_hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*m_hearingConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*m_sightConfig);
	GetPerceptionComponent()->ConfigureSense(*m_hearingConfig);

}

void AEnemyController::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
	if (!actor->ActorHasTag("Player")) return;


	if (stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (stimulus.WasSuccessfullySensed())
		{
			Blackboard.Get()->SetValueAsBool("PlayerInSight", true);
		}

		else if (!stimulus.WasSuccessfullySensed())
		{
			Blackboard.Get()->SetValueAsBool("PlayerInSight", false);
		}
	}

	else if (stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	{
		if (stimulus.Tag == "Escape")
		{
			Blackboard.Get()->SetValueAsBool("NeedToEscape", true);

			GetWorldTimerManager().ClearTimer(_escapeTimerHandle);

			GetWorldTimerManager().SetTimer(
				_escapeTimerHandle,
				this,
				&AEnemyController::StopEscaping,
				_escapeDuration,
				false 
			);
		}
	}
}

void AEnemyController::OnTargetLoseDetection(AActor* actor)
{

}

void AEnemyController::StopEscaping()
{
	Blackboard.Get()->SetValueAsBool("NeedToEscape", false);
}