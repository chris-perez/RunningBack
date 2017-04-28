// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "TurretGun.h"


void ATurretGun::DelayedShoot()
{
	Super::Shoot();
}

void ATurretGun::Tick(float DeltaSeconds)
{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, GetActorRotation().ToString());
	Super::Tick(DeltaSeconds);
	if (ProjectileParticle) 
	{
//		ProjectileParticle->SetWorldRotation(GetActorRotation());
		FRotator CurrentRotation = ProjectileParticle->GetComponentRotation();
//		ProjectileParticle->SetWorldRotation(FRotator(CurrentRotation.Pitch, 0, CurrentRotation.Roll));
//		float Yaw = GetActorRotation().Yaw / 2;
//		ProjectileParticle->SetWorldLocationAndRotation(GetActorLocation(), FRotator(GetActorRotation().Pitch/2, Yaw, GetActorRotation().Roll/2));
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, ProjectileParticle->GetComponentRotation().ToString());

//		ProjectileParticle->SetRelativeRotation(FRotator(0, 90, 0));
		
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
		
//		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
//			ProjectileTemp, GetRootComponent(), TEXT("Barrel"), FVector(0, 0, 0), FRotator(0, 0, 0 /*Owner->GetActorRotation().Yaw * -1*/), EAttachLocation::SnapToTarget , false);

//		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
//			ProjectileTemp, GetRootComponent(), TEXT("Barrel"), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
//		ProjectileParticle = UGameplayStatics::SpawnEmitterAtLocation(this, ProjectileTemp, GetActorLocation());
//		ProjectileParticle->AttachTo(GetRootComponent(), TEXT("Barrel"));

		ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
			ProjectileTemp, Owner->GetRootComponent(), NAME_None, FVector(0, 0, 450), FRotator(0, 0, 0 /*Owner->GetActorRotation().Yaw * -1*/), EAttachLocation::SnapToTarget , false);
	}
}

void ATurretGun::ShootStop()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
	if (ProjectileParticle) ProjectileParticle->Deactivate();
}
