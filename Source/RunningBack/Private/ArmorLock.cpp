// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "ArmorLock.h"
#include "RunningBackPawn.h"


void AArmorLock::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Armor Lock Activate"));
	if (Creator == nullptr)
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Null Creator"));
		return;
	}
	Creator->SpellTimeToReady = Cooldown();
	
	static_cast<ARunningBackPawn*>(GetOwner())->GetVehicleMovementComponent()->RecreatePhysicsState();
	static_cast<ARunningBackPawn*>(GetOwner())->GetVehicleMovementComponent()->SetHandbrakeInput(true);

	Creator->SpellDurationLeft = Duration();

	float TimeDelay = .1;
	FTimerDelegate CountdownDelegate;
	CountdownDelegate.BindUFunction(this, FName("Countdown"), TimeDelay);
	GetWorldTimerManager().SetTimer(CooldownHandle, CountdownDelegate, TimeDelay, true);
}

void AArmorLock::Deactivate()
{
	static_cast<ARunningBackPawn*>(GetOwner())->GetVehicleMovementComponent()->SetHandbrakeInput(false);
	Creator->RechargeSpell();
	Super::Deactivate();
}

float AArmorLock::Cooldown()
{
	return 8.0f;
}

float AArmorLock::Duration()
{
	return 4.0f;
}
