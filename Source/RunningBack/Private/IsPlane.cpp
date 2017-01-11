// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "IsPlane.h"


// Sets default values
AIsPlane::AIsPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIsPlane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsPlane::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

bool isPlane()
{
	return true;
}
