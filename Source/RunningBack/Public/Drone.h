// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "DroneCharacter.h"
#include "Drone.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ADrone : public ASpell
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DroneClass")
		TSubclassOf<ADroneCharacter> DroneCharacterClass;

	ADroneCharacter* Drone;

	virtual void Activate() override;

	void Deactivate() override;

	void SpawnDrone();

	void Shoot();
};
