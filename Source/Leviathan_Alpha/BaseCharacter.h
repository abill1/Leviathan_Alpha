// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

// Current Size of Class: 1536
//int size = sizeof(ABaseCharacter);

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

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAiming() const { return ZoomedIn; }

	virtual void MoveFwd_Bwd(const float _axisValue);
	virtual void MoveLeft_Right(const float _axisValue);
	virtual void LookUp_Down(const float _axisValue);

	virtual void Jump() override;
	virtual void StopJumping() override;
	void UpdateInAirState();

	virtual void ZoomIn();																					// Methods for positioning the camera while aiming
	virtual void ZoomOut();

	virtual void EnableRotation();
	virtual void DisableRotation();
	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintPure)
		bool IsDead() const;


protected:
	virtual void BeginPlay() override;																// Called when the game starts or when spawned
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input

private:
	void privDebugCamAndPlayerPosition() const;
	void privSetDeadState();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		FVector AimingOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		FRotator MeshRotatorForAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY()
		class USkeletalMeshComponent* pMesh;

	UPROPERTY()
		class UCharacterMovementComponent* pCharacterMovement;

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
		float WalkSpeedWhileAiming;
	
	UPROPERTY(EditAnywhere)
		float ZoomRate;

	UPROPERTY(EditAnywhere)
		float MaxHealth;

	UPROPERTY(VisibleAnywhere)
		float CurrentHealth;

	UPROPERTY(EditAnywhere)
		bool ZoomedIn;

	UPROPERTY(EditAnywhere)
		bool EnableRotateCamera;

	UPROPERTY(EditAnywhere)
		bool IsInAir;
	
	// Pad 0

};
