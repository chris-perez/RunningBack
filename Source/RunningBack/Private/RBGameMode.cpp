// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "RBGameMode.h"
#include "RBPlayerState.h"
#include <iostream>
#include "RBPlayerController.h"
#include "RunningBackPawn.h"


ARBGameMode::ARBGameMode()
{
	/*PlayerControllerClass = ARBPlayerController::StaticClass();
	DefaultPawnClass = ARunningBackPawn::StaticClass();*/

}


void ARBGameMode::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("OnConstruction "));
	TSubclassOf<ARBPlayerStart> player_start_class = TSubclassOf<ARBPlayerStart>(ARBPlayerStart::StaticClass());
	TArray<AActor*> found_starts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), player_start_class, found_starts);
	UE_LOG(LogTemp, Warning, TEXT("Found Starts Length: %d"), found_starts.Num());

	for (AActor* actor : found_starts)
	{
		ARBPlayerStart* player_start = static_cast<ARBPlayerStart*>(actor);
		if (player_start->team == ETeam::VE_Red)
		{
			red_starts.Add(player_start);
		}
		else
		{
			blue_starts.Add(player_start);
		}
		
	}
}


AActor* ARBGameMode::ChoosePlayerStart_Implementation(AController* player)
{
	APlayerController* playerController = static_cast<APlayerController*>(player);
	return GetSpawn(playerController);
}

AActor* ARBGameMode::GetSpawn(APlayerController* player_controller)
{
	UE_LOG(LogTemp, Warning, TEXT("Getting Spawn "));
	ARBPlayerState* player_state = static_cast<ARBPlayerState*>(player_controller->PlayerState);

	if (player_state->Team == ETeam::VE_Blue && blue_starts.Num() > 0)
	{
		return blue_starts[rand() % blue_starts.Num()];
	}
	else if (player_state->Team == ETeam::VE_Red && red_starts.Num() > 0)
	{
		return red_starts[rand() % red_starts.Num()];
	}
	else
	{
		return player_controller;
	}

}


