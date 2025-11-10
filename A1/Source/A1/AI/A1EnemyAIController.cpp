// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/A1EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

AA1EnemyAIController::AA1EnemyAIController()
{
}

void AA1EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void AA1EnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}

void AA1EnemyAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);

		Blackboard->SetValueAsVector(TEXT("HomePos"), GetPawn()->GetActorLocation());
	}
}

void AA1EnemyAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}
