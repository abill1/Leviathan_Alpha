// Copyright 2021 Anthony Bill. All rights reserved.


#include "Rifle.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

}

void ARifle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ARifle::Attack()
{
	this->PullTrigger();

}

void ARifle::PullTrigger()
{
	Super::PullTrigger();

}
