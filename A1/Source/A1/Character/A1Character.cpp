// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/A1Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/A1AnimInstance.h"

// Sets default values
AA1Character::AA1Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 700.0f;
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AA1Character::BeginPlay()
{
	Super::BeginPlay();
	
	// IMC 등록 -> 중간에도 수정 가능
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(IMCShoulder, 0);
		}
	}

	// 애니메이션 클래스 캐싱
	A1AnimInstance = Cast<UA1AnimInstance>(GetMesh()->GetAnimInstance());
	if (A1AnimInstance)
	{
		A1AnimInstance->OnMontageEnded.AddDynamic(this, &AA1Character::OnAttackMontageEnded);
		A1AnimInstance->OnMontageEnded.AddDynamic(this, &AA1Character::OnSkillMontageEnded);
	}
}

// Called every frame
void AA1Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AA1Character::Input_Attack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AA1Character::Input_Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AA1Character::Input_Move);
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AA1Character::Input_Skill);
	}
}

void AA1Character::AttackHitCheck(float AttackRange, float AttackRadius)
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
		if (OutHitResult.GetActor())
		{
			OutHitResult.GetActor()->Destroy();
		}
	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleCenter = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange;
	FColor DrawColor = IsHit ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleCenter, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);
#endif
}

void AA1Character::SkillHitCheck(float SkillRange)
{
	TArray<FHitResult> OutHitResults;
	FVector Origin = GetActorLocation();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHitResult = GetWorld()->SweepMultiByChannel(OutHitResults, Origin, Origin, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(SkillRange), CollisionParams);

	if (bHitResult)
	{
		for (auto& OutHitResult : OutHitResults)
		{
			OutHitResult.GetActor()->Destroy();
		}
	}

#if ENABLE_DRAW_DEBUG
	FColor DrawColor = bHitResult ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), Origin, SkillRange, 16, DrawColor, false, 5.0f);
#endif
}

void AA1Character::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && Montage->GetFName() == TEXT("AM_Attack"))
	{
		bIsAttacking = false;
	}
}

void AA1Character::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage && Montage->GetFName() == TEXT("AM_Skill"))
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		GetWorld()->GetTimerManager().SetTimer(
			SkillCoolTimeHandle,
			FTimerDelegate::CreateLambda(
				[&]()
				{
					bUseSkill = true;
					GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Use Skill!"));
				}
			),
			SkillCoolTime,
			false
		);
	}
}

void AA1Character::Input_Attack(const FInputActionValue& InputValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Attack"));
	
	if (bIsAttacking)
		return;

	bIsAttacking = true;

	if (A1AnimInstance)
	{
		A1AnimInstance->PlayAttackMontage();
	}
}

void AA1Character::Input_Look(const FInputActionValue& InputValue)
{
	FVector2D LookAxisVector = InputValue.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
}

void AA1Character::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AA1Character::Input_Skill(const FInputActionValue& InputValue)
{
	if (bUseSkill == false)
		return;

	bUseSkill = false;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	if (A1AnimInstance)
	{
		A1AnimInstance->PlaySkillMontage();
	}
}

