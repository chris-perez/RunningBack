// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "TurretGun.h"


void ATurretGun::DelayedShoot()
{
	Super::Shoot();
}

void ATurretGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (ProjectileParticle) 
	{
//		ProjectileParticle->SetRelativeRotation(FRotator(90, 0, 0));
//		ProjectileParticle->AddLocalRotation(-GetActorRotation());
//		ProjectileParticle->SetWorldRotation(GetActorRotation());
//		ProjectileParticle->SetWorldRotation(.5*GetActorRotation());
//		ProjectileParticle->PartSysVelocity.Set(-100, 0, 0);
	}
}

void ATurretGun::Shoot()
{
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &ATurretGun::DelayedShoot, 0.1f, true, 0.1f);
	if (ProjectileParticle)
	{
		ProjectileParticle->Activate();
	}
	else
	{
		
		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
			ProjectileTemp, GetRootComponent(), TEXT("Barrel"), FVector(0, 0, 0), FRotator(0, 0, 0 /*Owner->GetActorRotation().Yaw * -1*/), EAttachLocation::SnapToTarget , false);

//		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
//			ProjectileTemp, GetRootComponent(), TEXT("Barrel"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
//		ProjectileParticle = UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileTemp, GetActorLocation());
//		ProjectileParticle->AttachTo(GetRootComponent(), TEXT("Barrel"));
	}
}

void ATurretGun::ShootStop()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
	ProjectileParticle->Deactivate();
}
