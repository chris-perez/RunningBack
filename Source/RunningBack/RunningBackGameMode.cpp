// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "RunningBack.h"
#include "RunningBackGameMode.h"
#include "RunningBackPawn.h"
#include "RunningBackHud.h"

ARunningBackGameMode::ARunningBackGameMode()
{
	DefaultPawnClass = ARunningBackPawn::StaticClass();
	HUDClass = ARunningBackHud::StaticClass();
}
