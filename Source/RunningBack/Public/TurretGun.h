// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Attachable.h"
#include "TurretGun.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API ATurretGun : public AAttachable
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void Shoot() override;
	
	virtual void ShootStop() override;

	void ATurretGun::DelayedShoot();

	FTimerHandle FireDelayHandle;
};
