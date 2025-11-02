// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_AttackHitCheck.h"
#include "Interface/A1NotifyInterface.h"

UAnimNotify_AttackHitCheck::UAnimNotify_AttackHitCheck()
{
}

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IA1NotifyInterface* AttackActor = Cast<IA1NotifyInterface>(MeshComp->GetOwner());
		if (AttackActor)
		{
			AttackActor->AttackHitCheck(AttackRange, AttackRadius);
		}
	}
}
