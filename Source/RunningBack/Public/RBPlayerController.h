// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "UserWidget.h"
#include "RBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBPlayerController : public APlayerController
{
	
	GENERATED_BODY()
	
	public:
		UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Widgets")
			TSubclassOf<class UUserWidget> WidgetTemplate;

		UPROPERTY()
			UUserWidget* WidgetInstance;

		virtual void BeginPlay() override;
};
