// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "RunningBackPawn.h"
#include "Ramp.h"

void ARamp::Activate()
{
//	SetActorLocation(Creator->GetActorLocation() + FVector(0, 0, 200));
	SetActorLocation(FVector(0, 0, 0));
	AttachRootComponentTo(Creator->GetMesh());
	SetActorRotation(Creator->GetActorRotation());
}
