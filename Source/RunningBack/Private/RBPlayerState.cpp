// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "RBPlayerState.h"

void ARBPlayerState::CopyProperties(APlayerState* PlayerState) {
	APlayerState::CopyProperties(PlayerState);

	ARBPlayerState* newState = Cast<ARBPlayerState>(PlayerState);

	if (newState) {
		newState->Team = Team;
	}
}
