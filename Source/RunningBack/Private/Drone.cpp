// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "Drone.h"


void ADrone::Activate()
{
	SpawnDrone();
	Super::Activate();
}

void ADrone::SpawnDrone()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drone Spawn Called"));
	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation = this->GetActorLocation();
//		SpawnLocation.Z += 370.f;
		SpawnLocation.X -= 600.f;
		SpawnLocation.Y += 600.f;

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 360.f;
		SpawnRotation.Pitch = 360.f;
		SpawnRotation.Roll = 360.f;

		ADroneCharacter* Drone = World->SpawnActor<ADroneCharacter>(DroneCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drone Spawned"));


		UE_LOG(LogClass, Log, TEXT("Spell spawned succesfully "));

	}
}
