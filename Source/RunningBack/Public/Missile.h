// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Spell.h"
#include "Missile.generated.h"

/**
 * 
 */
UCLASS()
class RUNNINGBACK_API AMissile : public ASpell
{
	GENERATED_BODY()


public:

	UPROPERTY(Category="Homing", BlueprintReadWrite, EditAnywhere)
	AActor* Target;

	void Activate() override;


	float Duration() override;

	AActor* FindTarget();

	UFUNCTION(BlueprintNativeEvent)
	void SetHomingTarget();

	virtual void SetHomingTarget_Implementation();
};
