// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TimerUp.generated.h"

UCLASS()
class RUNNINGBACK_API ATimerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATimerUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	//return the mesh
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return TimerUpMesh; }

	//return if the actor is active
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void setActive(bool NewPickupState);

	UFUNCTION(BlueprintNativeEvent)
		void WasCollected();
	virtual void WasCollected_Implementation();

	//handles when the car overlaps the battery
	void OverlapWithCar();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereOverlap;

protected:
	//control the use of the pickup
	bool bIsActive;

private:
	// represent the mesh of the pickup
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *TimerUpMesh;
	
	
};
