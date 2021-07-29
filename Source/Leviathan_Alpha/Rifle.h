// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Rifle.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API ARifle : public AGun
{
	GENERATED_BODY()

public:
	ARifle();

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;


};
