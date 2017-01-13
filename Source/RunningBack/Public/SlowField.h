// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "SlowField.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ASlowField : public ASpell
{
	GENERATED_BODY()


public:
	float Duration() override;
};
