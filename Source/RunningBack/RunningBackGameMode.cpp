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
}
