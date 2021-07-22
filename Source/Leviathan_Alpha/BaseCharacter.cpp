// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ----- Set Movement key bindings 
	PlayerInputComponent->BindAxis(TEXT("MoveFwd_Bwd"), this, &ABaseCharacter::privMoveFwd_Bwd);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft_Right"), this, &ABaseCharacter::privMoveLeft_Right);

}

void ABaseCharacter::privMoveFwd_Bwd(const float _axisValue)
{
	this->AddMovementInput(this->GetActorForwardVector() * _axisValue);

}

void ABaseCharacter::privMoveLeft_Right(const float _axisValue)
{
	this->AddMovementInput(this->GetActorRightVector() * _axisValue);

}

