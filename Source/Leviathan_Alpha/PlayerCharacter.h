// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class AWeapon;

// Current Size of Class: 1584
//int size = sizeof(APlayerCharacter);

/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	APlayerCharacter();

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsInBeginPlay() const { return InBeginPlay; }

	virtual void Tick(float DeltaTime) override;													// Called every frame	
	virtual void MoveFwd_Bwd(const float _axisValue);
	virtual void MoveLeft_Right(const float _axisValue);
	virtual void LookUp_Down(const float _axisValue);

	virtual void Jump() override;
	virtual void ZoomIn();

	void Attack();

protected:
	virtual void BeginPlay() override;																// Called when the game starts or when spawned
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	// Called to bind functionality to input

private:

protected:

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		class AWeapon* Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		class AWeapon* Primary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
		class UAnimMontage* BeginPlayMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
		class APlayerHUD* HUD;

	UPROPERTY()
		float IntroDuration;

	UPROPERTY()
		bool InBeginPlay;

};
