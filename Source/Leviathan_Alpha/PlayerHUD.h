// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "PlayerHUD.generated.h"


/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	APlayerHUD();

	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void DrawHUD() override;
	void MakeUV(struct FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);

protected:
	void DrawCrosshair();

private:
	void privGetPlayerPawn();

private:

	UPROPERTY()
		FCanvasIcon mCrosshair;

	UPROPERTY()
		FVector2D mViewPortSize;

	UPROPERTY()
		UTexture2D* pHUDMainTexture;

	UPROPERTY()
		class APlayerCharacter* pPlayer;

};
