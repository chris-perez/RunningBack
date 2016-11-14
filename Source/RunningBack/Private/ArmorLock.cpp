// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "ArmorLock.h"
#include "RunningBackPawn.h"


void AArmorLock::Activate()
{
	Super::Activate();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Armor Lock Activate"));
	
	static_cast<ARunningBackPawn*>(GetOwner())->GetVehicleMovementComponent()->RecreatePhysicsState();
	static_cast<ARunningBackPawn*>(GetOwner())->GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AArmorLock::Deactivate()
{
	static_cast<ARunningBackPawn*>(GetOwner())->GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

float AArmorLock::Cooldown()
{
	return 4.0f;
}
