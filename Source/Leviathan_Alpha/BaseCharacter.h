// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class LEVIATHAN_ALPHA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();																				// Sets default values for this character's properties

public:
	virtual void Tick(float DeltaTime) override;													// Called every frame	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input

protected:
	virtual void BeginPlay() override;																// Called when the game starts or when spawned

private:
	void privMoveFwd_Bwd(const float _axisValue);

private:


};
