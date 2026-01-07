// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_Heat.generated.h"

/**
 * 
 */
UCLASS()
class UEnvQueryTest_Heat : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_Heat();

	virtual void RunTest(FEnvQueryInstance& QueryInstance)const override;
	
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MinTemperature = 0;
	UPROPERTY(EditAnywhere, Category = "Temperature")
	float MaxTemperature = 0;
};
