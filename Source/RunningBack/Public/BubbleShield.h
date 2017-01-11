// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "BubbleShield.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ABubbleShield : public ASpell
{
	GENERATED_BODY()


public:
	float Duration() override;


	void BeginPlay() override;
};
