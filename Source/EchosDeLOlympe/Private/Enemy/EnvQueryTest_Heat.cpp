// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnvQueryTest_Heat.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "Systems/HeatSubSystem.h"

UEnvQueryTest_Heat::UEnvQueryTest_Heat()
{
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
}

void UEnvQueryTest_Heat::RunTest(FEnvQueryInstance& QueryInstance) const
{
	if (!QueryInstance.Owner.IsValid())
		return;

	UWorld* World = QueryInstance.Owner.Get()->GetWorld();
	if (!World)
		return;

	UHeatSubSystem* heatSubSystem = World->GetSubsystem<UHeatSubSystem>();

	if (!heatSubSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeatSubSystem not found!"));
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector position = GetItemLocation(QueryInstance, It.GetIndex());

		float temp = heatSubSystem->GetTemperatureAtLocation(position);

		It.SetScore(TestPurpose, FilterType, temp, MinTemperature, MaxTemperature);
	}
}
