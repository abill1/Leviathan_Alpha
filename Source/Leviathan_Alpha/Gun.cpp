// Copyright 2021 Anthony Bill. All rights reserved.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

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
	this->TotalAmmo = 0;
	this->LoadedAmmo = 0;
	this->GunSubType = GunType::OTHER;

}

void AGun::BeginPlay()
{
	Super::BeginPlay();

}

void AGun::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

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

	}

}
