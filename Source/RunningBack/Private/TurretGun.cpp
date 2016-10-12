// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "TurretGun.h"


void ATurretGun::DelayedShoot()
{
	Super::Shoot();
}

void ATurretGun::Shoot()
{
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &ATurretGun::DelayedShoot, 0.1f, true, 0.1f);
}

void ATurretGun::ShootStop()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
}
