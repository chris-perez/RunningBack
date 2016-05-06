// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.4
//Seth's Updates:
//Added a timer! Can be adjusted in the GameMode cpp file. When timer hits 0, Game freezes.
//Added a win condition! If you destroy all the gold spheres before the timer runs out, game again freezes.
//Hopefully both the game win/lose condition are replaced with a matinee instead of freezing.
//Added a crosshair! For now, it's just a purple square. Will make better adjustments later.
//Created a class. It's called IsPlane, because originally I wanted to make it so you destroy the planes.
//But I'd have to create a BP class attached to the IsPlane cpp class and add a plane static mesh to each BP instance for
//it to work. Too much work. So instead, IsPlane is attached to the Gold Spheres and is used to register
//hits, so they can be destroyed.

//THINGS (FOR IGOR) TO DO:
//Add explosion sound for destruction.
//Add matinee for win/lose condition.
//Add water around AirCraft carrier. And a skybox. Make it pretty.
//(Try) and make the car/turret mesh look less ugly. God it's ugly.

//PS:
//Check the GameModeCPP file and look at the comments I made there. They're useful.

#include "RunningBack.h"
#include "RunningBackPawn.h"
#include "RunningBackWheelFront.h"
#include "RunningBackWheelRear.h"
#include "RunningBackHud.h"
#include "RunningBackGameMode.h"
#include "Attachable.h"
#include "IsPlane.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/InputComponent.h"
#include "Vehicles/WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine.h"

#ifdef HMD_INTGERATION
// Needed for VR Headset
#include "IHeadMountedDisplay.h"
#endif // HMD_INTGERATION

const FName ARunningBackPawn::LookUpBinding("LookUp");
const FName ARunningBackPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ARunningBackPawn::ARunningBackPawn()
{
	
	PrimaryActorTick.bCanEverTick = true;
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/RunningBack/Flying_Car.Flying_Car"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/RunningBack/FlyingCar_ABP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = URunningBackWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = URunningBackWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = URunningBackWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = URunningBackWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->AttachTo(RootComponent);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));

	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = true;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(8.0f, -40.0f, 130.0f);
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->AttachTo(SpringArm, USpringArmComponent::SocketName);
	InternalCamera->bUsePawnControlRotation = true;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetRelativeLocation(InternalCameraOrigin);
	InternalCamera->AttachTo(GetMesh());

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->AttachTo(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f, 0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->AttachTo(GetMesh());

	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;


	/*************************            Custom Code     *******************/

	MaxLife = 2000.f;
	LifePoints = 2000.f;
	lifeDecreaseRate = 100;
	TurnRate = 25.f;

	GunOffset = FVector(100.0f, 30.0f, 40.0f);
	fRate = 0.4f;

	//Collection SPhere stuff
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->AttachTo(RootComponent);
	CollectionSphere->SetSphereRadius(150.0f);


}

void ARunningBackPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ARunningBackPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARunningBackPawn::MoveRight);
	InputComponent->BindAxis("LookUp", this, &ARunningBackPawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookRight", this, &ARunningBackPawn::AddControllerYawInput);

	InputComponent->BindAction("Handbrake", IE_Pressed, this, &ARunningBackPawn::OnHandbrakePressed);
	InputComponent->BindAction("Handbrake", IE_Released, this, &ARunningBackPawn::OnHandbrakeReleased);
	InputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ARunningBackPawn::OnToggleCamera);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ARunningBackPawn::Shoot);
	InputComponent->BindAction("Shoot", IE_Released, this, &ARunningBackPawn::ShootStop);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ARunningBackPawn::OnResetVR);

	InputComponent->BindAction("Instructions", IE_Pressed, this, &ARunningBackPawn::ToggleVisibility);
}

#define COLLISION_WEAPON        ECC_GameTraceChannel1

void ARunningBackPawn::Shoot() {
	ServerShoot();
}

void ARunningBackPawn::ServerShoot_Implementation()
{
	//if (Controller && Controller->IsLocalPlayerController()) { // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
		FVector CamLoc;
		FRotator CamRot;

		//SpawnedWeapon->GetActorEyesViewPoint(CamLoc, CamRot);
		
		const FVector StartTrace = SpawnedWeapon->GetSoc(); // trace start is the camera location
		const FVector Direction = SpawnedWeapon->GetActorForwardVector();
		const FVector EndTrace = StartTrace + Direction * 10000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

																// Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult Hit(ForceInit);
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function


		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		ARunningBackPawn *ARB = Cast<ARunningBackPawn>(Hit.GetActor());
		if (ARB)
		{
			DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 12);
			ARB->TakeDamage(10, FDamageEvent(), GetController(), this);
			
		}
		else {
			DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true, 1.0f, 0, 12);
		}
		GetWorld()->GetTimerManager().SetTimer(FireRate, this, &ARunningBackPawn::Shoot, fRate);

}
	
bool ARunningBackPawn::ServerShoot_Validate() {
	return true;
}

void ARunningBackPawn::DisplayDebugLine_Implementation(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
	DrawDebugLine(InWorld, LineStart, LineEnd, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
}

void ARunningBackPawn::ServerTakeDamage_Implementation(APawn* p, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	p->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

bool ARunningBackPawn::ServerTakeDamage_Validate(APawn* p, float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
	return true;
}

void ARunningBackPawn::ShootStop() {
	ServerShootStop();
}

void ARunningBackPawn::ServerShootStop_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(FireRate);
}

bool ARunningBackPawn::ServerShootStop_Validate() {
	return true;
}

bool ARunningBackPawn::IsCar()
{
	return true;
}

void ARunningBackPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ARunningBackPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
	//AddControllerYawInput(Val * GetWorld()->GetDeltaSeconds() * TurnRate);
}

void ARunningBackPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ARunningBackPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ARunningBackPawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ARunningBackPawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || (bForce == true))
	{
		bInCarCameraActive = bState;

		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if ((PlayerController != nullptr) && (PlayerController->PlayerCameraManager != nullptr))
		{
			PlayerController->PlayerCameraManager->bFollowHmdOrientation = true;
		}

		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}

void ARunningBackPawn::Tick(float Delta)
{
	if (LifePoints <= 0)
	{
		PawnState = EPawnState::Inactive;
		SpawnedWeapon->Destroy();
		Destroy();
		if (Controller && Controller->IsLocalPlayerController())
		{
			UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
		}
	}

		// Setup the flag to say we are in reverse gear
		bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
		// Update the strings used in the hud (incar and onscreen)
		UpdateHUDStrings();

		// Set the string in the incar hud
		SetupInCarHUD();

		//OnActorHit.Add(&ARunningBackPawn::TakeDamage);
		

		bool bHMDActive = false;
#ifdef HMD_INTGERATION
		if ((GEngine->HMDDevice.IsValid() == true) && ((GEngine->HMDDevice->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
		{
			bHMDActive = true;
		}
#endif // HMD_INTGERATION
		if (bHMDActive == false)
		{
			if ((InputComponent) && (bInCarCameraActive == true))
			{
				FRotator HeadRotation = InternalCamera->RelativeRotation;
				HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
				HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
				InternalCamera->RelativeRotation = HeadRotation;
			}
		}

		
}

void ARunningBackPawn::BeginPlay()
{
	if (OnTest) FunctionOnTest();

	PawnState = EPawnState::Active;
	SpawnWeapon();
	if (SpawnedWeapon)
	{
		//SpringArm->AttachTo(SpawnedWeapon->WeaponSubObj);
	}
	bool bEnableInCar = false;
#ifdef HMD_INTGERATION
	bEnableInCar = GEngine->HMDDevice.IsValid();
#endif // HMD_INTGERATION
	EnableIncarView(bEnableInCar, true);
}

void ARunningBackPawn::OnResetVR()
{
#ifdef HMD_INTGERATION
	if (GEngine->HMDDevice.IsValid())
	{
		GEngine->HMDDevice->ResetOrientationAndPosition();
		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
		GetController()->SetControlRotation(FRotator());
	}
#endif // HMD_INTGERATION
}

void ARunningBackPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}
}

void ARunningBackPawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr))
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);

		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

void ARunningBackPawn::LookUp()
{
	//CurrentWeapon->SetActorRotation(this->GetCamera()->GetComponentRotation());

}

void ARunningBackPawn::SetLifePoints(float NewLife)
{
	LifePoints += NewLife;
}

float ARunningBackPawn::GetLifePoints()
{
	return LifePoints;
}

float ARunningBackPawn::GetMaxLife()
{
	return MaxLife;
}

void ARunningBackPawn::SpawnWeapon() {
	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 20.f;
		//SpawnLocation.X += 400.f;

		FRotator SpawnRotation;
		SpawnRotation.Yaw = 360.f;
		SpawnRotation.Pitch = 360.f;
		SpawnRotation.Roll = 360.f;

		SpawnedWeapon = World->SpawnActor<AAttachable>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		SpawnedWeapon->AttachRootComponentTo(GetMesh());
		SpawnLocation = GetActorLocation();
		SpawnLocation.Z += 225.f;
		
		SpawnedWeapon->SetActorLocation(SpawnLocation);
		SpawnedWeapon->SetActorRotation(GetActorRotation());
		UE_LOG(LogClass, Log, TEXT("Gun Mesh Spawned succesfully "));

	}
}

void ARunningBackPawn::AddControllerPitchInput(float Val) {
	//Super::AddControllerPitchInput(Val);

	if (SpawnedWeapon != nullptr)
	{
		FRotator MeshRot = GetMesh()->GetComponentRotation();
		FRotator NewRot = SpringArm->GetComponentRotation();
		NewRot.Roll = MeshRot.Roll;
		NewRot += FRotator(-Val, 0, 0);
		NewRot = NewRot.Clamp();
		SpringArm->SetWorldRotation(NewRot);
		SpawnedWeapon->SetActorRotation(NewRot);
	}
}

void ARunningBackPawn::AddControllerYawInput(float Val) {
	//Super::AddControllerYawInput(Val);

	if (SpawnedWeapon != nullptr)
	{
		FRotator MeshRot = GetMesh()->GetComponentRotation();
		FRotator NewRot = SpringArm->GetComponentRotation();
		NewRot.Roll = MeshRot.Roll;
		NewRot += FRotator(0, Val, 0);
		NewRot = NewRot.Clamp();
		SpringArm->SetWorldRotation(NewRot);
		SpawnedWeapon->SetActorRotation(NewRot);

		//SpawnedWeapon->AddActorLocalRotation(FRotator(0, Val, 0));		
	}
}
void ARunningBackPawn::Hit(AActor *SelfActor, AActor *OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogClass, Log, TEXT("Hit Succesfully "));
}

EPawnState ARunningBackPawn::GetPawnState()
{
	return PawnState;
}

void ARunningBackPawn::FunctionOnTest()
{
	Shoot();
	GetWorldTimerManager().SetTimer(ShootTestTimer, this, &ARunningBackPawn::FunctionOnTest, 1, false);
}

bool ARunningBackPawn::GetVisibility()
{
	return  InstructionVisibility;
}
void ARunningBackPawn::SetVisibility(bool Visibility)
{
	InstructionVisibility = Visibility;
}
void ARunningBackPawn::ToggleVisibility()
{
	SetVisibility(!InstructionVisibility);
}

float ARunningBackPawn::GetAngleTestYaw()
{
	return  AngleTestYaw;
}

float ARunningBackPawn::GetAngleTestPitch()
{
	return  AngleTestPitch;
}


void ARunningBackPawn::changeGun(uint8 index, UStaticMesh* msh)
{
	SpawnedWeapon->ChangeMesh(msh);
}
AAttachable* ARunningBackPawn::GetCurrentWeapon()
{
	return SpawnedWeapon;
}


#undef LOCTEXT_NAMESPACE
