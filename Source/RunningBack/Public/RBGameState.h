// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "RBGameState.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBGameState : public AGameState
{
	GENERATED_BODY()

public:
	ARBGameState();

	float CountdownTimer;
	float LTime;
	FTimerHandle LoopTime;
	virtual void BeginPlay() override;
	void DecreaseTime();
	virtual void Tick(float Delta) override;
	
	
	
	
};
