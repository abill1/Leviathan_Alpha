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
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return Camera; }

protected:
	virtual void BeginPlay() override;																// Called when the game starts or when spawned
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input

	void MoveFwd_Bwd(const float _axisValue);
	void MoveLeft_Right(const float _axisValue);
	void LookUp_Down(const float _axisValue);

	void ZoomIn();																					// Methods for positioning the camera while aiming
	void ZoomOut();
	
	void EnableRotation();
	void DisableRotation();

private:
	void privDebugCamAndPlayerPosition() const;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Camera)												// Can be used for rotation using gamepad or arrow keys
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere)
		float CapsuleRadius;

	UPROPERTY(VisibleAnywhere)
		float CapsuleHalfHeight;

	UPROPERTY(EditAnywhere, Category = Camera)
		float MaxPitch;

	UPROPERTY(EditAnywhere, Category = Camera)
		float MinPitch;

	UPROPERTY(EditAnywhere, Category = Camera)
		float SpringArmFollowLength;

	UPROPERTY(EditAnywhere, Category = Camera)
		float SpringArmAimLength;

	UPROPERTY(EditAnywhere)
		float WalkingSpeed;

	UPROPERTY(EditAnywhere)
		float WalkAndAimSpeed;
	
	UPROPERTY(EditAnywhere)
		float ZoomRate;

	UPROPERTY(EditAnywhere)
		bool ZoomedIn;
	
	UPROPERTY(EditAnywhere)
		bool EnableRotateCamera;

};
