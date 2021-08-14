// Copyright 2021 Anthony Bill. All rights reserved.


#include "MainMenuHUD.h"
#include "SMainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	this->ShowMenu();
}

void AMainMenuHUD::ShowMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MainMenuWidgetContainer, SWeakWidget).PossiblyNullContent(MainMenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());		// Setting to only allow clicking on UI

		}

	}

}

void AMainMenuHUD::RemoveMenu()
{
	if (GEngine && GEngine->GameViewport && MainMenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MainMenuWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());		// Setting to allow player movement and interaction

		}

	}

}