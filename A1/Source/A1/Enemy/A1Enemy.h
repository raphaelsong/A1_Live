// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/A1AIAttackInterface.h"
#include "Interface/A1NotifyInterface.h"
#include "A1Enemy.generated.h"

UCLASS()
class A1_API AA1Enemy : public ACharacter, public IA1AIAttackInterface, public IA1NotifyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AA1Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	void SetEnemyDead();

public:
	// Inherited via IA1AIAttackInterface
	void AttackByAI() override;
	void SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished) override;

public:
	// Inherited via IA1NotifyInterface
	void AttackHitCheck(float AttackRange, float AttackRadius) override;

public:
	UFUNCTION()
	void OnEnemyAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UA1CharacterStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HpBarWidgetComponent;

private:
	UPROPERTY()
	TObjectPtr<class UA1AnimInstance> A1AnimInstance;

	FAIAttackFinished OnAIAttackFinished;
};
