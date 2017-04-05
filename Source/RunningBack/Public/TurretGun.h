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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	UParticleSystem* ProjectileTemp;

	UParticleSystemComponent* ProjectileParticle;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void Shoot() override;
	
	virtual void ShootStop() override;

	void DelayedShoot();

	FTimerHandle FireDelayHandle;


	void Tick(float DeltaSeconds) override;
};
