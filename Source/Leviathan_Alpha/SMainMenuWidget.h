// Copyright 2021 Anthony Bill. All rights reserved.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMainMenuHUD>, OwningHUD)
	SLATE_END_ARGS()

	/** Needed for every widget. Initializes the widget. */
	void Construct(const FArguments& InArgs);

	FReply OnNewGameClicked() const;
	FReply OnQuitGameClicked() const;

	/** Add to appease compiler warnings */
	virtual bool SupportsKeyboardFocus() const override;

public:
	TWeakObjectPtr<class AMainMenuHUD> OwningHUD;				// This HUD creates the widget

};
