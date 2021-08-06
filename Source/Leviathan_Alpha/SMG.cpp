// Copyright 2021 Anthony Bill. All rights reserved.


#include "SMG.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

#define MY_LOG(x) UE_LOG(LogTemp, Warning, x);

ASMG::ASMG()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASMG::BeginPlay()
{
	Super::BeginPlay();

}

void ASMG::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ASMG::Attack()
{
	this->PullTrigger();
	MY_LOG(TEXT("SMG_ATTACK"));

}

void ASMG::PullTrigger()
{
	Super::PullTrigger();
	
}
