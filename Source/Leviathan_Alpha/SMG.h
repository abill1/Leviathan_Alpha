// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "SMG.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API ASMG : public AGun
{
	GENERATED_BODY()
public:
	ASMG();
	void Attack() override;
	void PullTrigger() override;

public:
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

};
