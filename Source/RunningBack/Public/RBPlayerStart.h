// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "Team.h"
#include "RBPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBPlayerStart : public APlayerStart
{
public:
	UPROPERTY()
		ETeam team; 
	GENERATED_BODY()
	
};
