// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/WheeledVehicle.h"
#include "public/Attachable.h"
#include "public/Projectile.h"
#include "SpellComponent.h"
#include "Spell.h"
#include "RunningBackPawn.generated.h"

UENUM(BlueprintType)
enum class EPawnState {
	Active,
	Inactive,
	Unknown
};

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
UCLASS(config=Game)
class ARunningBackPawn : public AWheeledVehicle
{
	GENERATED_BODY()

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InternalCamera;

	/** Text component for the In-Car speed */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* InCarSpeed;

	/** Text component for the In-Car gear */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* InCarGear;

public:
	ARunningBackPawn();

	/** The current speed as a string eg 10 km/h */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FText SpeedDisplayString;

	/** The current gear as a string (R,N, 1,2 etc) */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FText GearDisplayString;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	/** The color of the incar gear text in forward gears */
	FColor	GearDisplayColor;

	/** The color of the incar gear text when in reverse */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FColor	GearDisplayReverseColor;

	/** Are we using incar camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
	bool bInCarCameraActive;

	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
	bool bInReverseGear;

	/** Initial offset of incar camera */
	FVector InternalCameraOrigin;
	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End Pawn interface

	// Begin Actor interface
	virtual void Tick(float Delta) override;
	virtual void BeginPlay() override;
	// End Actor interface

	/** Handle pressing forwards */
	void MoveForward(float Val);
	/** Setup the strings used on the hud */
	void SetupInCarHUD();

	/** Update the physics material used by the vehicle mesh */
	void UpdatePhysicsMaterial();
	/** Handle pressing right */
	void MoveRight(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
	/** Switch between cameras */
	void OnToggleCamera();
	/** Handle reset VR device */
	void OnResetVR();

	static const FName LookUpBinding;
	static const FName LookRightBinding;


	/***************************************************        custom code               *****************************/

	// handle the lookUp for the attachables and whatnot
	void LookUp();

	UPROPERTY(EditAnywhere, Category = "Health", BlueprintReadWrite)
		float LifePoints;

	UPROPERTY(EditAnywhere, Category = "Health", BlueprintReadWrite)
		float MaxLife;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetMaxLife();

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetLifePoints(float NewLife);

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetLifePoints();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Guns")
		AAttachable* SpawnedWeapon;

	UFUNCTION(BlueprintCallable, Category = "Guns")
		AAttachable* GetCurrentWeapon();

	UPROPERTY(EditAnywhere, Category = "Projectile")
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float fRate;

	//controlls the pawn state

	EPawnState PawnState;


	//Control the Weapon on the car
	//UPROPERTY(EditAnywhere, Category = "Weapons", BlueprintReadWrite)
	//AAttachable *CurrentWeapon;

	void AddControllerPitchInput(float Val) override;

	void AddControllerYawInput(float Val) override;

	UFUNCTION()
	void Shoot();

	UFUNCTION( Server, Reliable, WithValidation )
	void ServerShoot();

	UFUNCTION(NetMulticast, Reliable)
	void DisplayDebugLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTakeDamage(APawn* p, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category="Health")
	float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void ShootStop();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerShootStop();

	UFUNCTION(BlueprintCallable, Category="Spell")
	void CastSpell();

	UFUNCTION(Blueprintpure, Category="State")
	EPawnState GetPawnState();

	UFUNCTION(BlueprintCallable, Category = "Colision")
	void Hit(AActor *SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	bool IsCar();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AAttachable> WhatToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spells")
	TSubclassOf<ASpell> SpellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		FVector GunOffset;

	UPROPERTY(EditAnywhere, Category="Health")
	float lifeDecreaseRate;

	float TurnRate;

	void SpawnWeapon();

	void ChangeWeaponLazerGun();

	void ChangeWeaponTurretGun();

	void ChangeWeaponFreezeRay();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Projectile")
		UParticleSystemComponent *FreezeRay;*/
	UFUNCTION(BlueprintNativeEvent, Category="Projectile")
		void SpawnRay();
	virtual void SpawnRay_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FVector RayDest;

	FTimerHandle FreezeDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float FreezDelayTime;

	FTimerHandle FireRate;
	// to set up the collection of the timer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	bool OnTest = false;

	FTimerHandle ShootTestTimer;

	void FunctionOnTest();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interface")
		bool InstructionVisibility;

	UFUNCTION(BlueprintPure, Category = "Interface")
		bool GetVisibility();

	UFUNCTION(BlueprintCallable, Category = "Interface")
		void SetVisibility(bool Visibility);

	void ToggleVisibility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;
	//bool Destroy(bool bNetForce, bool bShouldModifyLevel) override;

	



	//Testing stuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	float AngleTestYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
		float AngleTestPitch;

	UFUNCTION(BlueprintCallable, Category = "Test")
	float GetAngleTestYaw();

	UFUNCTION(BlueprintCallable, Category = "Test")
	float GetAngleTestPitch();

	UFUNCTION(BlueprintCallable, Category = "Customization")
		void changeGun(uint8 index, UStaticMesh* msh);




private:
	/** 
	 * Activate In-Car camera. Enable camera and sets visibility of incar hud display
	 *
	 * @param	bState true will enable in car view and set visibility of various if its doesnt match new state
	 * @param	bForce true will force to always change state
	 */
	void EnableIncarView( const bool bState, const bool bForce = false );

	/** Update the gear and speed strings */
	void UpdateHUDStrings();

	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;


public:
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	/** Returns InternalCamera subobject **/
	FORCEINLINE UCameraComponent* GetInternalCamera() const { return InternalCamera; }
	/** Returns InCarSpeed subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarSpeed() const { return InCarSpeed; }
	/** Returns InCarGear subobject **/
	FORCEINLINE UTextRenderComponent* GetInCarGear() const { return InCarGear; }

	// Returns CollectionSphere
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
};
