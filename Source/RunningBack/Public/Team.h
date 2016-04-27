// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/UserDefinedEnum.h"
#include "Team.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ETeam : uint8 {
	VE_Red		UMETA(DisplayName="Red Team"),
	VE_Blue		UMETA(DisplayName="Blue Team")
};


