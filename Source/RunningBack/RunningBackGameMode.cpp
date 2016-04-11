// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "RunningBack.h"
#include "RunningBackGameMode.h"
#include "RunningBackPawn.h"
#include "RunningBackHud.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ARunningBackGameMode::ARunningBackGameMode()
{
	DefaultPawnClass = ARunningBackPawn::StaticClass();
	HUDClass = ARunningBackHud::StaticClass();
	CountdownTimer = 60.f;
	MaxTime = 60;
	LTime = 1.f;
	//Score starts at 0.
	score = 0;
	//Max Score is the score you need to reach to end game.
	//There are 6 spheres on the map currently so max score is 6.
	//If you add a sphere to the map, you must add 1 to max score.
	maxScore = 6;
}

void ARunningBackGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass); 
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
		else
			UE_LOG(LogClass, Log, TEXT("Failed to create widget "));
	}
	else
		UE_LOG(LogClass, Log, TEXT("HUD widgetClass Is NULL "));*/
	GetWorld()->GetTimerManager().SetTimer(LoopTime, this, &ARunningBackGameMode::DecreaseTime, LTime, true, -1);
}
void ARunningBackGameMode::DecreaseTime()
{
	CountdownTimer = CountdownTimer - 1;
	if (CountdownTimer <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(LoopTime);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}
void ARunningBackGameMode::Tick(float Delta)
{
	//FString TheFloatStr = FString::SanitizeFloat(CountdownTimer);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
}

float ARunningBackGameMode::GetMaxTime()
{
	return MaxTime;
}

float ARunningBackGameMode::GetCountdownTime()
{
	return CountdownTimer;
}