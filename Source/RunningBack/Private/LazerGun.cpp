// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "LazerGun.h"

void ALazerGun::DelayedShoot()
{
	Super::Shoot();
	if (ProjectileParticle)
	{
		ProjectileParticle->Activate();
	}
	else
	{
//		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
//			ProjectileTemp, GetRootComponent(), TEXT("Barrel"), FVector(0, 0, 0), FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true);

		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
			ProjectileTemp, Owner->GetRootComponent(), NAME_None, FVector(0, 0, 450), FRotator(90, 0, 0), EAttachLocation::SnapToTarget, true);

		
		//		ProjectileParticle->AttachTo(GetRootComponent(), TEXT("Barrel"));
	}
}

void ALazerGun::Shoot()
{
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &ALazerGun::DelayedShoot, 1.0f, true, 0.5f);		
}

void ALazerGun::ShootStop()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
	ProjectileParticle->Deactivate();
}


