// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "FreezeRay.h"
#include "AICharacter.h"

#define COLLISION_WEAPON        ECC_GameTraceChannel1

void AFreezeRay::DelayedShoot()
{
	//if (Controller && Controller->IsLocalPlayerController()) { // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	FVector CamLoc;
	FRotator CamRot;

	//SpawnedWeapon->GetActorEyesViewPoint(CamLoc, CamRot);

	const FVector StartTrace = GetSoc(); // trace start is the camera location
	const FVector Direction = GetActorForwardVector();
	const FVector EndTrace = StartTrace + Direction * 10000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	APawn *ARB = Cast<APawn>(Hit.GetActor());
	AAICharacter *HitAI = Cast<AAICharacter>(Hit.GetActor());
	if (HitAI)
	{
//		DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 10);
		HitAI->SlowDown(10, 1.0f);
	} else if (ARB && ARB != Owner)
	{
//	DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 10);
	ARB->TakeDamage(10, FDamageEvent(), Owner->GetController(), this);

	}
	else {
//	DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true, 1.0f, 0, 10);
	}
	
//	Super::Shoot();
}

void AFreezeRay::Shoot()
{
	if (FireSoundStart != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSoundStart, GetActorLocation());
	}
	
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &AFreezeRay::DelayedShoot, 0.1f, true, 0.05f);
//	DelayedShoot();
	if(SnowParticle)
	{
		SnowParticle->Activate();
	} else
	{
//		SnowParticle = UGameplayStatics::SpawnEmitterAttached(
//			SnowTemp, GetRootComponent(), NAME_None, FVector(200, 0, 0), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, true);

		SnowParticle = UGameplayStatics::SpawnEmitterAttached(
			SnowTemp, Owner->GetRootComponent(), NAME_None, FVector(200, 0, 450), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, true);
	}
	
}

void AFreezeRay::ShootStop()
{
	GetWorld()->GetTimerManager().ClearTimer(FireDelayHandle);
	SnowParticle->Deactivate();
}

