// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "Missile.h"
#include "RunningBackPawn.h"

#define COLLISION_WEAPON        ECC_GameTraceChannel1

void AMissile::Activate()
{
	Target = FindTarget();
	if (Target)
	{
		SetHomingTarget();
		Super::Activate();
	} else
	{
		Kill();
	}
}

float AMissile::Duration()
{
	return 10.0f;
}

AActor* AMissile::FindTarget()
{
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MoveToLocation"));
	//if (Controller && Controller->IsLocalPlayerController()) { // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	FVector CamLoc;
	FRotator CamRot;

	//SpawnedWeapon->GetActorEyesViewPoint(CamLoc, CamRot);

	const FVector StartTrace = Creator->SpawnedWeapon->GetSoc(); // trace start is the camera location
	const FVector Direction = Creator->SpawnedWeapon->GetActorForwardVector();
	const FVector EndTrace = StartTrace + Direction * 10000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

																// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	
	if (Creator && Creator->CurrentSpell)
	{
		TraceParams.AddIgnoredActor(Creator->CurrentSpell);
	}

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function

	/*if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}*/

	APawn *ARB = Cast<APawn>(Hit.GetActor());

	if (ARB && ARB != Creator)
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 10);
//		ARB->TakeDamage(10, FDamageEvent(), Creator->GetController(), this);
		return ARB;
	}
	else {
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true, 1.0f, 0, 10);
		return nullptr;
	}

	//automatic fire
	//	GetWorld()->GetTimerManager().SetTimer(FireRate, this, &AAttachable::Shoot, fRate);
}

void AMissile::SetHomingTarget_Implementation()
{
	//do the rest in blueprint
}
