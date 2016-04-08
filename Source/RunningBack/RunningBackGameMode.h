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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float CountdownTimer;

	float MaxTime;

	UFUNCTION(BlueprintPure, Category = "Time")
		float GetMaxTime();

	UFUNCTION(BlueprintPure, Category = "Time")
		float GetCountdownTime();

	float LTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	float score;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	float maxScore;
	
	FTimerHandle LoopTime;
	void DecreaseTime();
	virtual void Tick(float Delta) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TSubclassOf <class UUserWidget> PauseMenu;

};



