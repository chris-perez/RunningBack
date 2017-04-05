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

	APawn* Owner;

	FORCEINLINE class UStaticMesh* GetMesh() const { return WeaponMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent *WeaponSubObj;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons", meta = (AllowPrivateAccess = "true"))
	class UStaticMesh *WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float fRate = 0.4f;

	FTimerHandle FireRate;

	UFUNCTION(NetMulticast, Reliable)
		void DisplayDebugLine(const UWorld* InWorld, FVector const& LineStart, FVector const& LineEnd, FColor const& Color, bool bPersistentLines, float LifeTime, uint8 DepthPriority, float Thickness);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
		virtual void Shoot();

	virtual void ShootStop();

	FVector GetSoc();

	void ChangeMesh(UStaticMesh* mesh);

protected:


private:

};
