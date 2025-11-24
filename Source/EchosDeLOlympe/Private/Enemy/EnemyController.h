// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AEnemyController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	class UAISenseConfig_Sight* m_sightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);

};
