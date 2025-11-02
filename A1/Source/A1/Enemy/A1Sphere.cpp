// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/A1Sphere.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AA1Sphere::AA1Sphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Sphere->SetCollisionProfileName(FName("A1Enemy"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereAsset(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (SphereAsset.Succeeded())
	{
		Sphere->SetStaticMesh(SphereAsset.Object);
	}
}

// Called when the game starts or when spawned
void AA1Sphere::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sphere)
	{
		Sphere->OnComponentHit.AddDynamic(this, &AA1Sphere::OnSphereHit);
	}
}

// Called every frame
void AA1Sphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA1Sphere::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

