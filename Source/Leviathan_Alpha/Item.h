// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM()
enum class PrimaryItemType : uint8
{
	WEAPON	UMETA(DisplayName="Weapon"),
	AMMO	UMETA(DisplayName = "Ammo"),
	POTION	UMETA(DisplayName = "Potion"),
	OTHER	UMETA(DisplayName = "Other")
};

/*
* This class serves as the basis for all items the player can 
* add to their inventory. With this base class we can later create 
* a tree to store and sort the player's inventory. 
* 
*/
UCLASS()
class LEVIATHAN_ALPHA_API AItem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AItem();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE PrimaryItemType GetPrimaryType() const { return primary; }
	FORCEINLINE float GetWeight() const { return weight; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
		float weight;

	UPROPERTY(EditAnywhere)
		PrimaryItemType primary;

};
