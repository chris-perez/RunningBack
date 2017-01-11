// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "Ramp.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ARamp : public ASpell
{
	GENERATED_BODY()


public:
	void Activate() override;
};
