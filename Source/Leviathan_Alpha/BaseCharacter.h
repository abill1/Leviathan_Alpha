// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS(config=Game)
class LEVIATHAN_ALPHA_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();																				// Sets default values for this character's properties

public:
	virtual void Tick(float DeltaTime) override;													// Called every frame	
	
	class USpingArmComponent* GetSpringArmComponent() const;
	class UCameraComponent* GetCameraComponent() const;

protected:
	virtual void BeginPlay() override;																// Called when the game starts or when spawned
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input

	void MoveFwd_Bwd(const float _axisValue);
	void MoveLeft_Right(const float _axisValue);

private:


private:

	UPROPERTY()
		FVector2D MouseInput;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)												// Can be used for rotation using gamepad or arrow keys
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere)
		float CapsuleRadius;

	UPROPERTY(VisibleAnywhere)
		float CapsuleHalfHeight;

	UPROPERTY(EditAnywhere)
		float SpringArmLength;

};
