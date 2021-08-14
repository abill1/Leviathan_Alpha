// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class LEVIATHAN_ALPHA_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

protected:
	TSharedPtr<class SMainMenuWidget> MainMenuWidget;
	TSharedPtr<class SWidget> MainMenuWidgetContainer;

};

