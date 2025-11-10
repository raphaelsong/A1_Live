// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/A1Enemy.h"

// Sets default values
AA1Enemy::AA1Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA1Enemy::BeginPlay()
{
	Super::BeginPlay();
	
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

