// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "RunningBackHud.generated.h"


UCLASS(config = Game)
class ARunningBackHud : public AHUD
{
	GENERATED_BODY()

public:
	ARunningBackHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
