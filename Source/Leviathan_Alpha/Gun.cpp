// Copyright 2021 Anthony Bill. All rights reserved.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

#define MY_LOG(x) UE_LOG(LogTemp, Warning, x);

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	// ----- Create the Root for this Object
	this->Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	check(this->Root);
	this->SetRootComponent(this->Root);

	// ----- Attach the Mesh to the Root
	this->Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	this->Mesh->SetupAttachment(this->Root);

	// ----- Initialize Characteristics 
	this->MaxRange = 100.0f;	// 1 meter == 100.0f
	this->ClipSize = 1;
	this->TotalAmmo = 100;
	this->LoadedAmmo = 0;
	this->MaxAmmo = 1;
	this->GunSubType = GunType::OTHER;
	this->mMuzzleSocket_Str = TEXT("SMG_Barrel");
	this->mReloadSocket_Str = TEXT("SMG_clipSocket");

}

void AGun::BeginPlay()
{
	Super::BeginPlay();

}

void AGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

/*
* Notes:
*	The ECollisionChannel enum is based on the project settings. Had to go in under Engine/Collsion and add new
* trace channel "bullet", then look up where the channel was assigned under DefaultEngine.ini to find its trace channel
*/
void AGun::PullTrigger()
{
	if (this->LoadedAmmo)									// Fire if ammo remains loaded
	{
		--this->LoadedAmmo;

		this->privSpawnMuzzleFlash();
		this->privUpdateGunOwner();
		this->privUpdateController();
		if (this->pOwnerController != nullptr)
		{
			this->privUpdateWorld();
			this->privUpdateCamLocationRotation();
			this->privUpdateEndPoint();						// Get the point the player is targeting
			this->privUpdateCollisionParams();				// Must be set each time the trigger function is called 

			if (this->privCheckHit())						// Hit Target
			{
				this->privSpawnHitTargetEmitter();
				this->privDealDamage();

			}
			else											// Missed Target
			{
				MY_LOG(TEXT("Missed target."));

			}

		}
	}
	else if (this->TotalAmmo)								// If clip is empty, then a tick must be used to reload and prevent firing
	{
		this->Reload();
		UE_LOG(LogTemp, Warning, TEXT("Reloaded Ammo."));

	}
	else													// Clip is empty and out of ammo
	{
		UE_LOG(LogTemp, Warning, TEXT("Out of Ammo."));

	}

}


void AGun::Reload()
{
	if ((this->TotalAmmo) && (this->LoadedAmmo < this->ClipSize))	// If Ammo remains and the gun is not fully loaded
	{
		int32 neededAmmo = this->ClipSize - this->LoadedAmmo;		// Find how many rounds are needed to refill

		if (neededAmmo > this->TotalAmmo)							// If the amount needed exceeds the total ammo remaining
		{
			this->LoadedAmmo += this->TotalAmmo;					// Just add whatever remains
			this->TotalAmmo = 0;									// Zero out the remaining ammo

		}
		else
		{
			this->TotalAmmo -= neededAmmo;							// Remove the amount needed to refill from total
			this->LoadedAmmo = this->ClipSize;						// The amount will be reset to the max
		
		}

		// ----- Trigger Reload Sound
		UGameplayStatics::SpawnSoundAttached(this->ReloadSound, this->Mesh, this->mReloadSocket_Str);

	}

}

bool AGun::AddAmmo(const int32 _AmmoPickedUp)
{
	bool bLoaded = false;								// Can be used later to determine whether to remove the ammo item from the world

	if (this->TotalAmmo < this->MaxAmmo)				// If the player does not have full ammo, then pick up the ammo
	{
		this->TotalAmmo += _AmmoPickedUp;
		if (this->TotalAmmo >= this->MaxAmmo)			// Only allow the player to pick up to the max
			this->TotalAmmo = this->MaxAmmo;
		bLoaded = true;									// Remove the ammo pickup even if it exceeded max
	
	}

	return bLoaded;
}

void AGun::privUpdateGunOwner()
{
	this->pGunOwner = (APawn*)GetOwner();

}

/**
* The pGunOwner cannot be nullptr.
* Otherwise, the pOwnerController will be set to nullptr.
*/
void AGun::privUpdateController()
{
	if (this->pGunOwner)
		this->pOwnerController = this->pGunOwner->GetController();
	else
		this->pOwnerController = nullptr;

}

void AGun::privUpdateWorld()
{
	this->pWorld = GetWorld();

}

void AGun::privUpdateCamLocationRotation()
{
	this->pOwnerController->GetPlayerViewPoint(this->mCamLocation, this->mCamRotator);			// Returns location and rotation of the player's camera
	this->mRotationVector = this->mCamRotator.Vector();

}

void AGun::privUpdateEndPoint()
{
	this->mEndPoint = this->mCamLocation + this->mRotationVector * this->MaxRange;		// Returns the End point of the player's sight

}

void AGun::privUpdateCollisionParams()
{
	this->mParams.AddIgnoredActor(this);					// Add this gun to ignore
	this->mParams.AddIgnoredActor(this->GetOwner());		// Add the owner of this Gun

}

void AGun::privSpawnMuzzleFlash()
{
	FVector loc = this->Mesh->GetRelativeLocation();
	loc += FVector(-5.0f, 12.0f, 0.0f);
	UGameplayStatics::SpawnEmitterAttached(this->MuzzleFlash, this->Mesh, this->mMuzzleSocket_Str, loc);	// Add the partical effect to the muzzle socket of the rifle --- This will make it look like the gun has fired when the condition is met
	UGameplayStatics::SpawnSoundAttached(this->MuzzleSound, this->Mesh, this->mMuzzleSocket_Str);

}

void AGun::privSpawnHitTargetEmitter()
{
	this->mShotDirection = -this->mCamRotator.Vector();	// Can be used later for debugging
	this->mSpawnTransform = FTransform(this->mShotDirection.Rotation(), this->mHitResult.Location);
	UGameplayStatics::SpawnEmitterAtLocation(this->pWorld, this->BulletHitTarget, this->mSpawnTransform);
	UGameplayStatics::SpawnSoundAtLocation(this->pWorld, this->BulletHitSound, this->mHitResult.Location);

}

void AGun::privDealDamage()
{
	AActor* hitActor = this->mHitResult.GetActor();
	if (hitActor)							// If an actor exists
	{
		FPointDamageEvent DamageEvent(this->Damage, this->mHitResult, this->mShotDirection, nullptr);	// Used by character class's take damage to apply damage; last arg is for added damage types created by user like fire, poision, etc. 
		hitActor->TakeDamage(this->Damage, DamageEvent, pOwnerController, this);

	}

}

bool AGun::privCheckHit()
{
	return this->pWorld->LineTraceSingleByChannel(this->mHitResult, this->mCamLocation, this->mEndPoint, ECollisionChannel::ECC_GameTraceChannel1, this->mParams);	// Get the trace from the player's camera to its endpoint
}
