// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Attachable.h"
#include "FreezeRay.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API AFreezeRay : public AAttachable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystem* SnowTemp;

	UParticleSystemComponent* SnowParticle;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void Shoot() override;

	virtual void ShootStop() override;

	void DelayedShoot();

	FTimerHandle FireDelayHandle;
};
