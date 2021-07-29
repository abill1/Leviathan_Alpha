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
 * Question: Who should own the total ammo? The player will technically own it 
 * in their inventory, however, it would be faster for the gun to own it. 
 * 
 * I think for games like Mass Effect, Uncharted, or Tomb Raider the ammo is 
 * tied to the weapon; whereas, for a game like Resident Evil, several guns 
 * can use the same ammo type so the ammo is stored in the player inventory.
 * This style makes ammo more of a resource to manage. I think the first method
 * is more like the style I am going for. 
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

	FORCEINLINE UFUNCTION(BlueprintPure)
		int32 GetLoadedCount() const { return LoadedAmmo; }
	FORCEINLINE UFUNCTION(BlueprintPure)
		int32 GetTotalCount() const { return TotalAmmo; }

	FORCEINLINE bool IsEmpty() const { return LoadedAmmo == AWeapon::EMPTY; }
	FORCEINLINE bool IsOutOfAmmo() const { return TotalAmmo == AWeapon::EMPTY; }

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
		int32 TotalAmmo;

	UPROPERTY()
		int32 LoadedAmmo;

	UPROPERTY(EditAnywhere)
		GunType GunSubType;

};
