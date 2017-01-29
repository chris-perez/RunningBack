// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "AICharacter.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAICharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

float AAICharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	LifePoints -= ActualDamage;
	if (LifePoints <= 0.f)
	{
		LifePoints = 0;
		Destroy();
	}
	return ActualDamage;
}

void AAICharacter::SlowDown(float SpeedDecrease, float time)
{
	GetCharacterMovement()->MaxWalkSpeed -= SpeedDecrease;
	GetWorldTimerManager().SetTimer(SlowDelayHandle, this, &AAICharacter::SpeedUp, time);
}

void AAICharacter::Stun()
{
	SlowDown(GetCharacterMovement()->MaxWalkSpeed, 5.0f);
}

void AAICharacter::SpeedUp()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Speed Up"));
	GetCharacterMovement()->MaxWalkSpeed = 600;
}
