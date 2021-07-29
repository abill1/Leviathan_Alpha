// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Gun.generated.h"

UENUM()
enum class GunType : uint8
{
	RIFLE		UMETA(DisplayName = "Rifle"),
	HANDGUN		UMETA(DisplayName = "Handgun"),
	SHOTGUN		UMETA(DisplayName = "Shotgun"),
	OTHER		UMETA(DisplayName = "Other")

};


/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API AGun : public AWeapon
{
	GENERATED_BODY()
public:
	AGun();

public:
	virtual void Tick(float DeltaSeconds) override;

	void PullTrigger();
	void Reload();
	
	UFUNCTION(BlueprintPure)
		
	FORCEINLINE bool IsEmpty() const { return CurrentAmmo == AWeapon::EMPTY; }

protected:
	virtual void BeginPlay() override;

protected:

	FCollisionQueryParams mParams;
	FHitResult mHitResult;
	FVector mCamLocation;
	FRotator mCamRotator;
	FVector	mEndPoint;
	FVector mShotDirection;
	FTransform mSpawnTransform;
	FName mMuzzleSocket_Str;
	FName mReloadSocket_Str;

	UPROPERTY()
		APawn* pGunOwner;

	UPROPERTY()
		AController* pOwnerController;

	UPROPERTY()
		UWorld* pWorld;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
		UParticleSystem* BulletHitTarget;

	UPROPERTY(EditAnywhere)
		USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere)
		USoundBase* BulletHitSound;

	UPROPERTY(EditAnywhere)
		float MaxRange;

	UPROPERTY(EditAnywhere)
		int32 ClipSize;

	UPROPERTY()
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<GunType> GunSubType;

};
