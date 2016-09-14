// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "RBGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBGameMode : public AGameMode
{
	GENERATED_BODY()

	AActor* ChoosePlayerStart_Implementation(AController* Player);
	
	
};
