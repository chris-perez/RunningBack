// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "RBPlayerController.h"
#include "Blueprint/UserWidget.h"


void ARBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetTemplate)
	{
		WidgetInstance = CreateWidget<UUserWidget>(this, WidgetTemplate);

		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
	}
	
}

