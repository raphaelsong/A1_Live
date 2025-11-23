// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/A1AIAttackInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControlligPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControlligPawn)
		return EBTNodeResult::Failed;

	IA1AIAttackInterface* AttackPawn = Cast<IA1AIAttackInterface>(ControlligPawn);
	if(nullptr == AttackPawn)
		return EBTNodeResult::Failed;

	FAIAttackFinished OnAIAttackFinished;
	OnAIAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AttackPawn->SetAIAttackFinishedDelegate(OnAIAttackFinished);
	AttackPawn->AttackByAI();

	return EBTNodeResult::InProgress;
}
