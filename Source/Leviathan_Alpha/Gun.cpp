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
	this->CurrentAmmo = 0;
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

