// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "JiraPluginStyle.h"

class FJiraPluginCommands : public TCommands<FJiraPluginCommands>
{
public:

	FJiraPluginCommands()
		: TCommands<FJiraPluginCommands>(TEXT("JiraPlugin"), NSLOCTEXT("Contexts", "JiraPlugin", "JiraPlugin Plugin"), NAME_None, FJiraPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};