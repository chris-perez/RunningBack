// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "Spell.h"
#include "RunningBackPawn.h"


// Sets default values
ASpell::ASpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpell::BeginPlay()
{
	Super::BeginPlay();
//	Activate();
	
	GetWorldTimerManager().SetTimer(LifetimeHandle, this, &ASpell::Kill, Cooldown(), false);
}

// Called every frame
void ASpell::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASpell::SetCreator(ARunningBackPawn* Pawn)
{
	Creator = Pawn;
}

void ASpell::Kill()
{
	Deactivate();
	Destroy();
}

void ASpell::Activate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Spell Activate"));
	if (Creator == nullptr)
	{
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Null Creator"));
		return;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Creator Not Null"));
	}
	Creator->SpellTimeToReady = Cooldown();
	GetWorldTimerManager().SetTimer(CooldownHandle, this, &ASpell::Countdown, 1, true);
}

void ASpell::Deactivate()
{
	GetWorldTimerManager().ClearTimer(LifetimeHandle);
}

float ASpell::Cooldown()
{
	return 5.0f;
}

void ASpell::Countdown()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Countdown"));
//	ARunningBackPawn* Pawn = static_cast<ARunningBackPawn*>(GetOwner());
	if (Creator == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Null Creator"));
		return;
	}
	if (Creator->GetSpellTimeToReady() <= 0)
	{
		Creator->SetSpellTimeToReady(0);
	} else
	{
		Creator->SetSpellTimeToReady(Creator->SpellTimeToReady-1);
	}
}
