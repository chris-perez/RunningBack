// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
//#include "RunningBackPawn.h"
#include "Spell.generated.h"

class ARunningBackPawn;

UCLASS()
class RUNNINGBACK_API ASpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ARunningBackPawn* Creator;
	FTimerHandle LifetimeHandle;
	FTimerHandle CooldownHandle;

	// Sets default values for this actor's properties
	ASpell();



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetCreator(ARunningBackPawn* Pawn);

	void Kill();

	virtual void Activate();

	virtual void Deactivate();

	virtual float Cooldown();

	void Countdown();
};
