// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "Attachable.h"
#include "RunningBackPawn.h"


// Sets default values
AAttachable::AAttachable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<UStaticMeshComponent> MeshComp(TEXT("'/Game/Assets/Meshes/SM_Statue.SM_Statue'"));
	// Weapon Comp

	WeaponSubObj = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	
	//WeaponSubObj = MeshComp.Object;
	
	RootComponent = WeaponSubObj;


}

// Called when the game starts or when spawned
void AAttachable::BeginPlay()
{
	Super::BeginPlay();

}

#define COLLISION_WEAPON        ECC_GameTraceChannel1

void AAttachable::Shoot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MoveToLocation"));
	//if (Controller && Controller->IsLocalPlayerController()) { // we check the controller becouse we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	FVector CamLoc;
	FRotator CamRot;

	//SpawnedWeapon->GetActorEyesViewPoint(CamLoc, CamRot);

	const FVector StartTrace = GetSoc(); // trace start is the camera location
	const FVector Direction = GetActorForwardVector();
	const FVector EndTrace = StartTrace + Direction * 10000; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;

	ARunningBackPawn* RunningBackPawn = Cast<ARunningBackPawn>(Owner); 
	if (RunningBackPawn)
	{
		TraceParams.AddIgnoredActor(RunningBackPawn);
		if (RunningBackPawn->CurrentSpell) 
		{
			TraceParams.AddIgnoredActor(RunningBackPawn->CurrentSpell);
		}
	}

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, COLLISION_WEAPON, TraceParams); // simple trace function

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	APawn *ARB = Cast<APawn>(Hit.GetActor());
	/*if (ARB) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, ARB->GetHumanReadableName());
	}*/

	if (ARB && ARB != Owner)
	{
//		DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(0, 255, 0), true, 1.0f, 0, 10);
		ARB->TakeDamage(10, FDamageEvent(), Owner->GetController(), this);
	}
	else {
//		DisplayDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), true, 1.0f, 0, 10);
	}

	//automatic fire
//	GetWorld()->GetTimerManager().SetTimer(FireRate, this, &AAttachable::Shoot, fRate);
}

void AAttachable::ShootStop() {
}

// Called every frame
void AAttachable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AAttachable::GetSoc()
{
	FVector soc = GetRootComponent()->GetChildComponent(0)->GetSocketLocation("Barrel");
	return soc;
}

void AAttachable::ChangeMesh(UStaticMesh* mesh)
{
	UStaticMeshComponent *msh = Cast<UStaticMeshComponent>(GetRootComponent()->GetChildComponent(0));
	msh->SetStaticMesh(mesh);

}

void AAttachable::DisplayDebugLine_Implementation(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
	DrawDebugLine(InWorld, LineStart, LineEnd, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
}