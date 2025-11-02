// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "A1AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class A1_API UA1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UA1AnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

#pragma region AnimationMontage
public:
	void PlayAttackMontage();
	void PlaySkillMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> SkillMontage;

protected:
	int32 AttackIndex = -1;

	UPROPERTY(EditAnywhere)
	int32 MaxAttackCount = 3;
#pragma endregion

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ACharacter> OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling = false;
};
