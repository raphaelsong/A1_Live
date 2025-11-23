// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/A1CharacterStatComponent.h"
#include "A1CharacterStatComponent.h"

// Sets default values for this component's properties
UA1CharacterStatComponent::UA1CharacterStatComponent()
{
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UA1CharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UA1CharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetCurrentHp(MaxHp);
}

void UA1CharacterStatComponent::SetMaxHp(float NewMaxHp)
{
	MaxHp = FMath::Clamp<float>(NewMaxHp, 0.0f, NewMaxHp);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UA1CharacterStatComponent::SetCurrentHp(float NewCurrentHp)
{
	CurrentHp = FMath::Clamp<float>(NewCurrentHp, 0.0f, MaxHp);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

float UA1CharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	SetCurrentHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}


