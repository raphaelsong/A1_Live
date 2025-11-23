// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/A1Enemy.h"
#include "Animation/A1AnimInstance.h"
#include "AI/A1EnemyAIController.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/A1CharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/A1HpBarWidget.h"

// Sets default values
AA1Enemy::AA1Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Stat Component
	StatComponent = CreateDefaultSubobject<UA1CharacterStatComponent>(TEXT("Stat"));
	StatComponent->SetMaxHp(200.0f);

	// widget Component
	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBarWidgetComponent->SetupAttachment(GetMesh());
	HpBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));
	HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HpBarWidgetComponent->SetDrawSize(FVector2D(100.0f, 15.0f));
	HpBarWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AA1Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	A1AnimInstance = Cast<UA1AnimInstance>(GetMesh()->GetAnimInstance());
	if (A1AnimInstance)
	{
		A1AnimInstance->OnMontageEnded.AddDynamic(this, &AA1Enemy::OnEnemyAttackMontageEnded);
	}
}

void AA1Enemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (StatComponent)
	{
		StatComponent->OnHpZero.AddUObject(this, &AA1Enemy::SetEnemyDead);
	}

	if (HpBarWidgetComponent)
	{
		HpBarWidgetComponent->InitWidget();

		UA1HpBarWidget* HpBarWidget = Cast<UA1HpBarWidget>(HpBarWidgetComponent->GetUserWidgetObject());
		if (HpBarWidget)
		{
			HpBarWidget->UpdateHp(StatComponent->GetCurrentHP(), StatComponent->GetMaxHp());

			StatComponent->OnHpChanged.AddUObject(HpBarWidget, &UA1HpBarWidget::UpdateHp);
		}
	}
}

// Called every frame
void AA1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA1Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AA1Enemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatComponent)
	{
		StatComponent->ApplyDamage(DamageAmount);
	}

	return DamageAmount;
}

void AA1Enemy::SetEnemyDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(false);

	if (A1AnimInstance)
	{
		A1AnimInstance->PlayDeadMontage();
	}

	AA1EnemyAIController* AIController = Cast<AA1EnemyAIController>(GetController());
	if (AIController)
	{
		AIController->StopAI();
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 5.0f, false);
}

void AA1Enemy::AttackByAI()
{
	if (A1AnimInstance)
	{
		A1AnimInstance->PlayAttackMontage();
	}
}

void AA1Enemy::SetAIAttackFinishedDelegate(const FAIAttackFinished& InOnAttackFinished)
{
	OnAIAttackFinished = InOnAttackFinished;
}

void AA1Enemy::AttackHitCheck(float AttackRange, float AttackRadius)
{
	// 충돌 처리
	FHitResult OutHitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * AttackRange;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);


	bool IsHit = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius), CollisionParams);

	if (IsHit)
	{
		const float AttackDamage = 30.0f;
		if (OutHitResult.GetActor())
		{
			FDamageEvent DamageEvent;
			OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleCenter = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange;
	FColor DrawColor = IsHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleCenter, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif

}

void AA1Enemy::OnEnemyAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->GetFName() == FName("AM_EnemyAttack"))
	{
		OnAIAttackFinished.ExecuteIfBound();
	}
}
