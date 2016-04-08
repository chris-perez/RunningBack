// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "RBGameState.h"

ARBGameState::ARBGameState()
{
	CountdownTimer = 300.f;
	LTime = 1.f;
}
void ARBGameState::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(LoopTime, this, &ARBGameState::DecreaseTime, LTime);

}
void ARBGameState::DecreaseTime()
{
	CountdownTimer = CountdownTimer - 1;
	if (CountdownTimer <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(LoopTime);
	}
}
void ARBGameState::Tick(float Delta)
{
	FString TheFloatStr = FString::SanitizeFloat(CountdownTimer);
	GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
}

