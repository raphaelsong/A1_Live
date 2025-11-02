// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/A1AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UA1AnimInstance::UA1AnimInstance()
{
}

void UA1AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UA1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();
	bShouldMove = (GroundSpeed > 3.0f);
	bIsFalling = MovementComponent->IsFalling();
}

void UA1AnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);

	AttackIndex = (AttackIndex + 1) % MaxAttackCount;
	FName SectionName = FName(*FString::Printf(TEXT("Attack%d"), AttackIndex));
	Montage_JumpToSection(SectionName, AttackMontage);
}

void UA1AnimInstance::PlaySkillMontage()
{
	Montage_Play(SkillMontage, 1.0f);
}
