// Copyright 2021 Anthony Bill. All rights reserved.


#include "MainMenuHUD.h"
#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine && GEngine->GameViewport)
	{
		MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent( SAssignNew(MainMenuWidgetContainer, SWeakWidget).PossiblyNullContent(MainMenuWidget.ToSharedRef()) );

	}

}