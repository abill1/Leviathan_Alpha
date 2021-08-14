// Copyright 2021 Anthony Bill. All rights reserved.


#include "SMainMenuWidget.h"

/* SNew is used to create new slate widgets */
#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	const FMargin ContentPadding = FMargin(500.0f, 300.0f);
	const FText MainTitle = LOCTEXT("GameTitle", "Leviathan: Alpha");

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
					+ SVerticalBox::Slot()
						[
							SNew(STextBlock).Text(MainTitle)
						]
				]
		];

}

#undef LOCTEXT_NAMESPACE

