// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy.generated.h"

class UHeatReactor;

UCLASS()
class AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehaviorTree();

	bool IsActive;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reactor")
	UHeatReactor* _reactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_tree;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FVector> _patrolPoint;
	
	int _currentPatrolPoint  = 0.f;

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool _isStatic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float _activationCd;

	UBlackboardComponent* _blackboard;

	UFUNCTION()
	void Activate();

	UFUNCTION()
	void Deactivate();
};
