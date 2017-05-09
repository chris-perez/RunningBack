// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "DroneCharacter.h"
#include "Kismet/KismetMathLibrary.h"

#define COLLISION_WEAPON        ECC_GameTraceChannel1

// Sets default values
ADroneCharacter::ADroneCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	DelayedShoot();
	
}

// Called every frame
void ADroneCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ADroneCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}


void ADroneCharacter::Shoot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MoveToLocation"));
	//if (Controller && Controller->IsLocalPlayerController()) { // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	FVector CamLoc;
	FRotator CamRot;

	//SpawnedWeapon->GetActorEyesViewPoint(CamLoc, CamRot);
	AAICharacter* Enemy = FindEnemy();
	if (Enemy)
	{
		const FVector StartTrace = GetMesh()->GetComponentLocation(); // trace start is the camera location
		const FVector Direction = Enemy->GetActorLocation() + FVector(0, 0, 100);
//		const FVector EndTrace = StartTrace + Direction * 10000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks
		const FVector EndTrace = Direction;
																 // Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function

		/*if (FireSound != nullptr)
		{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}*/

//		APawn *ARB = Cast<APawn>(Hit.GetActor());
		APawn *ARB = (Enemy);

		if (ARB && ARB != Creator)
		{
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 10);

			ARB->TakeDamage(10, FDamageEvent(), Creator->GetController(), this);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StartTrace, Direction);
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
					ProjectileTemp, GetRootComponent(), NAME_None, StartTrace, LookAtRotation + FRotator(90, 0, 0), EAttachLocation::KeepWorldPosition, true);
			}
		}
		else {
			DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true, 1.0f, 0, 10);
		}

		//automatic fire
		//	GetWorld()->GetTimerManager().SetTimer(FireRate, this, &AAttachable::Shoot, fRate);
	}
}

void ADroneCharacter::DelayedShoot()
{
	GetWorldTimerManager().SetTimer(FireDelayHandle, this, &ADroneCharacter::Shoot, 0.5f, true, 0.05f);
}

AAICharacter* ADroneCharacter::FindEnemy()
{

	/*The Height of my Sphere starting from the location of the Actor*/
		float SphereHeight = 200;

	/*The Radius of the sphere trace*/
		float SphereRadius = 5000;

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
	CollisionShape.SetSphere(SphereRadius);

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
			if ((*It).Actor != nullptr) {
				GLog->Log((*It).Actor->GetName());
				AActor* HitActor = (*It).Actor.Get();
				AAICharacter* AICharacter = Cast<AAICharacter>(HitActor);
				if (AICharacter)
				{
					return AICharacter;
				}
			}
		}
	}
	return nullptr;
}

