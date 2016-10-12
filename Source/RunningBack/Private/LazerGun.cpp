// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "LazerGun.h"

void ALazerGun::DelayedShoot()
{
	Super::Shoot();
}

void ALazerGun::Shoot()
{
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &ALazerGun::DelayedShoot, 1.0f, true, 0.5f);	
}

void ALazerGun::ShootStop()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
}


