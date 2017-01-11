// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Team.h"

#include "GameFramework/PlayerState.h"
#include "RBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARBPlayerState : public APlayerState
{
	GENERATED_BODY()

		/** Copy properties which need to be saved in inactive PlayerState */
		virtual void CopyProperties(class APlayerState* PlayerState) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Enum)
		ETeam Team;
	
	
	
};
