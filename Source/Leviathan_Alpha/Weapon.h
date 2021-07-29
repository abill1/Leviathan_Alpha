// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM()
enum class WeaponType : uint8
{
	RANGED		UMETA(DisplayName = "Ranged"),
	MELEE		UMETA(DisplayName = "Melee"),
	EXPLOSIVE	UMETA(DisplayName = "Explosive"),
	OTHER		UMETA(DisplayName = "Other")

};

/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

public:
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE WeaponType GetWeaponType() const { return WeaponSubtype; }

protected:
	virtual void BeginPlay() override;

private:

protected:

	UPROPERTY(EditAnywhere)
		int32 Damage;

	UPROPERTY(EditAnywhere)
		WeaponType WeaponSubtype;

	static const int32 EMPTY;

};
