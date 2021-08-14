// Copyright 2021 Anthony Bill. All rights reserved.


#include "SMainMenuWidget.h"
#include "MainMenuHUD.h"
#include "GameFramework/PlayerController.h"

/* SNew is used to create new slate widgets */
#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;												// Add to appease compiler warnings

	this->OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(500.0f, 300.0f);
	const FMargin ButtonPadding = FMargin(10.0f);
	const FText sMainTitle	= LOCTEXT("GameTitle", "Leviathan: Alpha");
	const FText sNewGame	= LOCTEXT("NewGame", "New Game");
	const FText sSettings	= LOCTEXT("Settings", "Settings");
	const FText sQuitGame	= LOCTEXT("QuitGame", "Quit Game");

	// ----- Set Up Font -----
	FSlateFontInfo ButtonFont = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonFont.Size = 40.0f;
	FSlateFontInfo TitleFont = ButtonFont;
	TitleFont.Size = 60.0f;

	// ----- Set Up Widget Slots -----
	ChildSlot
	[
		// ----- Background Image ----- 
		SNew(SOverlay) 
		+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		[
				SNew(SImage).ColorAndOpacity(FColor::Black)
		]
		+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(ContentPadding)
		[
			SNew(SVerticalBox)
			// ----- Title -----
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock).Font(TitleFont).Text(sMainTitle).Justification(ETextJustify::Center)
			]
			// ----- New Game Button -----
			+ SVerticalBox::Slot()
			[
				SNew(SButton).OnClicked(this, &SMainMenuWidget::OnNewGameClicked)[SNew(STextBlock).Font(ButtonFont).Text(sNewGame).Justification(ETextJustify::Center)]
			]
			// ----- Settings Button -----
			+ SVerticalBox::Slot()
			[
				SNew(SButton)[SNew(STextBlock).Font(ButtonFont).Text(sSettings).Justification(ETextJustify::Center)]
			]
			// ----- Quit Game Button -----
			+ SVerticalBox::Slot()
			[
				SNew(SButton).OnClicked(this, &SMainMenuWidget::OnQuitGameClicked)[SNew(STextBlock).Font(ButtonFont).Text(sMainTitle).Justification(ETextJustify::Center)]
			]
		]
	];

}

bool SMainMenuWidget::SupportsKeyboardFocus() const
{
	return true;
}

FReply SMainMenuWidget::OnNewGameClicked() const
{
	
	return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitGameClicked() const
{
	if (OwningHUD.IsValid())
	{
		APlayerController* pController = OwningHUD->PlayerOwner;
		if (pController)
			pController->ConsoleCommand("quit");
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE

