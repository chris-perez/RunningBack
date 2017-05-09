// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "ShootingAI.h"
#include "Kismet/KismetMathLibrary.h"

#define COLLISION_WEAPON        ECC_GameTraceChannel1

// Sets default values
AShootingAI::AShootingAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShootingAI::BeginPlay()
{
	Super::BeginPlay();
	DelayedShoot();
}

// Called every frame
void AShootingAI::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AShootingAI::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AShootingAI::Shoot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MoveToLocation"));
	//if (Controller && Controller->IsLocalPlayerController()) { // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	FVector CamLoc;
	FRotator CamRot;

	//SpawnedWeapon->GetActorEyesViewPoint(CamLoc, CamRot);
	ARunningBackPawn* Enemy = FindEnemy();
	if (Enemy)
	{
		UStaticMeshComponent* TurretGunMesh = static_cast<UStaticMeshComponent*>(GetMesh()->GetChildComponent(0));
		FVector StartTrace = GetActorLocation(); // trace start is the camera location
		if (TurretGunMesh)
		{
			StartTrace = TurretGunMesh->GetComponentLocation() + FVector(0, 0, 100); // trace start is the camera location
		}
		
		const FVector Direction = Enemy->GetActorLocation() + FVector(0, 0, 250);
		const FVector EndTrace = Direction; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

																 // Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(GetAttachParentActor());

		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function

		/*if (FireSound != nullptr)
		{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}*/

		ARunningBackPawn *ARB = Cast<ARunningBackPawn>(Hit.GetActor());

		if (ARB)
		{			
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ARB->GetActorLocation());
//			TurretGunMesh->SetWorldRotation(LookAtRotation + FRotator(-2 * LookAtRotation.Pitch, 180, 0));
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TurretGunMesh->GetName());

//			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 10);
			ARB->TakeDamage(10, FDamageEvent(), GetController(), this);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Player Shot"));

			if (ProjectileParticle)
			{
				ProjectileParticle->Deactivate();
				ProjectileParticle->SetWorldRotation(LookAtRotation + FRotator(90, 0, 0));
				ProjectileParticle->Activate();
				
			}
			else
			{
				FRotator Rotation = (GetActorLocation() - EndTrace).Rotation();
					ProjectileParticle = UGameplayStatics::SpawnEmitterAttached(
						ProjectileTemp, GetRootComponent(), NAME_None, GetActorLocation(), LookAtRotation + FRotator(90, 0, 0), EAttachLocation::KeepWorldPosition, true);
			}

		}
		else {
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true, 1.0f, 0, 10);
		}

		//automatic fire
		//	GetWorld()->GetTimerManager().SetTimer(FireRate, this, &AAttachable::Shoot, fRate);
	}
}

void AShootingAI::DelayedShoot()
{
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &AShootingAI::Shoot, 1.5f, true, 0.05f);
}

ARunningBackPawn* AShootingAI::FindEnemy()
{

	/*The Height of my Sphere starting from the location of the Actor*/
	float SphereHeight = 200;

	/*Sphere segments - used for visualization only*/
	int32 Segments = 100;
	/*TArray is the collection that contains all the HitResults*/
	TArray<FHitResult> HitResults;

	/*The Start location of the sphere*/
	FVector StartLocation = GetActorLocation();

	/*The End location of the sphere is equal to the default location of the actor plus the height we will enter from the editor
	To extend this functionality, you can replace the height variable with a FVector*/
	FVector EndLocation = GetActorLocation();
	EndLocation.Z += SphereHeight;

	/*Search for static objects only*/
	ECollisionChannel ECC = ECollisionChannel::ECC_WorldStatic;

	/*Declare the Collision Shape, assign a Sphere shape and set it's radius*/
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(SearchRadius);

	/*Perform the actual raycast. This method returns true if there is at least 1 hit.*/
	bool bHitSomething = GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::FQuat(), ECC, CollisionShape);


	/*In order to draw the sphere of the first image, I will use the DrawDebugSphere function which resides in the DrawDebugHelpers.h
	This function needs the center of the sphere which in this case is provided by the following equation*/
	FVector CenterOfSphere = ((EndLocation - StartLocation) / 2) + StartLocation;

	/*Draw the sphere in the viewport*/
	DrawDebugSphere(GetWorld(), CenterOfSphere, CollisionShape.GetSphereRadius(), Segments, FColor::Green, true, .5f);

	/*If the raycast hit a number of objects, iterate through them and print their name in the console*/
	if (bHitSomething)
	{
		for (auto It = HitResults.CreateIterator(); It; It++)
		{
//			GLog->Log((*It).Actor->GetName());
			AActor* HitActor = (*It).Actor.Get();
			ARunningBackPawn* RunningBackPawn = Cast<ARunningBackPawn>(HitActor);
			if (RunningBackPawn)
			{
				return RunningBackPawn;
			}
		}
	}
	return nullptr;
}
