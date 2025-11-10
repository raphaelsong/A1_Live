// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "A1EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class A1_API AA1EnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AA1EnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void RunAI();
	void StopAI();

protected:
	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<class UBehaviorTree> BTAsset;
};
