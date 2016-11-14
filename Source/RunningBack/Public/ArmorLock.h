// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "ArmorLock.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API AArmorLock : public ASpell
{
	GENERATED_BODY()

public:
	static const float COOLDOWN;

	virtual void Activate() override;
	
	virtual void Deactivate() override;

	virtual float Cooldown() override;
};
