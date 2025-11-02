// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/A1NotifyInterface.h"
#include "Interface/A1NotifySkillInterface.h"
#include "A1Character.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class A1_API AA1Character : public ACharacter, public IA1NotifyInterface, public IA1NotifySkillInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AA1Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void AttackHitCheck(float AttackRange, float AttackRadius) override;
	virtual void SkillHitCheck(float SkillRange) override;

public:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY()
	TObjectPtr<class UA1AnimInstance> A1AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	uint8 bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill)
	uint8 bUseSkill = true;

	float SkillCoolTime = 5.0f;
	FTimerHandle SkillCoolTimeHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

#pragma region EnhancedInputSystem
public:
	void Input_Attack(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Skill(const FInputActionValue& InputValue);

protected:
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> IMCShoulder;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SkillAction;
#pragma endregion
};
