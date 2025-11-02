// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_AttackHitCheck();

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackRadius = 50.0f;
};
