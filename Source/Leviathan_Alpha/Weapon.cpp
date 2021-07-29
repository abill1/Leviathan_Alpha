// Copyright 2021 Anthony Bill. All rights reserved.


#include "Weapon.h"

const uint32 AWeapon::EMPTY = 0;

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	this->primary = PrimaryItemType::WEAPON;
	this->WeaponSubtype = WeaponType::OTHER;
	this->Damage = 0;

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

