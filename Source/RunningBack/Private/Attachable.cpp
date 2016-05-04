// Fill out your copyright notice in the Description page of Project Settings.

#include "RunningBack.h"
#include "Attachable.h"


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