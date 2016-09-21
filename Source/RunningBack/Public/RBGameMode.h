// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"
#include "RBPlayerStart.h"
#include <vector>
#include <list>
#include "RBGameMode.generated.h"


using namespace std;

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBGameMode : public AGameMode
{
	UPROPERTY()
		TArray<ARBPlayerStart*> blue_starts = TArray<ARBPlayerStart*>();
		TArray<ARBPlayerStart*> red_starts = TArray<ARBPlayerStart*>();

	GENERATED_BODY()

	UFUNCTION()
	void OnConstruction(const FTransform& Transform);

	UFUNCTION()
	AActor* ChoosePlayerStart_Implementation(AController* player);
	
	UFUNCTION()
	AActor* GetSpawn(APlayerController* player_controller);


	
};
