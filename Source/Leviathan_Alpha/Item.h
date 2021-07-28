// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class LEVIATHAN_ALPHA_API AItem : public AActor
{
	GENERATED_BODY()

public:
	UENUM()
		enum class PrimaryType : uint8
	{
		WEAPON,
		AMMO,
		POTION,
		OTHER
	};

public:	
	// Sets default values for this actor's properties
	AItem();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE AItem::PrimaryType GetPrimaryType() const { return primary; }
	FORCEINLINE float GetWeight() const { return weight; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
		float weight;

	UPROPERTY(EditAnywhere)
		PrimaryType primary;

};
