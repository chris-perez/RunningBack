// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "RunningBackGameMode.generated.h"

UCLASS(minimalapi)
class ARunningBackGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ARunningBackGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	TSubclassOf <class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	class UUserWidget *CurrentWidget;

	virtual void BeginPlay() override;
};



