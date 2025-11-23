// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "A1AIAttackInterface.generated.h"

DECLARE_DELEGATE(FAIAttackFinished)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UA1AIAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class A1_API IA1AIAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AttackByAI() = 0;
	virtual void SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished) = 0;
};
