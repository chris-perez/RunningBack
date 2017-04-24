// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "BubbleShield.h"


float ABubbleShield::Duration()
{
	return 30.0f;
}

void ABubbleShield::BeginPlay()
{
	Super::BeginPlay();
	FVector Location = GetActorLocation();
	
	Location.AddBounded(FVector(100, 0, 0));
	SetActorLocation(Location);
}
