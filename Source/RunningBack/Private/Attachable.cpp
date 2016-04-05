// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "Attachable.h"


// Sets default values
AAttachable::AAttachable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_AW2(TEXT("StaticMesh'/Game/StarterContent/Architecture/Pillar_50x500.Pillar_50x500'"));
	// Weapon Comp
	WeaponMesh = StaticMeshOb_AW2.Object;


}

// Called when the game starts or when spawned
void AAttachable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAttachable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

