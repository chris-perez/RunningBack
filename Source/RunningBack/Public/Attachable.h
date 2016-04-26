#pragma once

#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Attachable.generated.h"
//#include "GameFramework/WheeledVehicle.h"

UCLASS()
class RUNNINGBACK_API AAttachable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAttachable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Weapons", BlueprintReadWrite)
		FString WeaponName;

	FORCEINLINE class UStaticMesh* GetMesh() const { return WeaponMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *WeaponSubObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	class UStaticMesh *WeaponMesh;

	FVector GetSoc();

protected:


private:

};
