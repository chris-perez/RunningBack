// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "TimerUp.h"


// Sets default values
ATimerUp::ATimerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TimerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	//Pickup start active
	bIsActive = true;
	
	RootComponent = TimerUpMesh;
	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere1"));
	SphereOverlap->SetSphereRadius(3.f);
	SphereOverlap->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ATimerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATimerUp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//SphereOverlap->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ATimerUp::OverlapWithCar)
	//TimerUpMesh->OnComponentBeginOverlap.AddD(this, ATimerUp::OverlapWithCar);

}

bool ATimerUp::IsActive() {
	return bIsActive;
}

void ATimerUp::setActive(bool NewPickupState) {
	bIsActive = NewPickupState;
}

void ATimerUp::WasCollected_Implementation() {
	FString TimerUpDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have Collected"));
}

void ATimerUp::OverlapWithCar() {

}

