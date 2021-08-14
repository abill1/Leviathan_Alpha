// Copyright 2021 Anthony Bill. All rights reserved.


#include "MainPlayerController.h"
#include "MainMenuHUD.h"

AMainPlayerController::AMainPlayerController()
{

}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMainPlayerController::OpenMenu);

	}
}

void AMainPlayerController::OpenMenu()
{
	AMainMenuHUD* pHUD = Cast<AMainMenuHUD>(GetHUD());
	if (pHUD)
	{
		pHUD->ShowMenu();

	}
}

