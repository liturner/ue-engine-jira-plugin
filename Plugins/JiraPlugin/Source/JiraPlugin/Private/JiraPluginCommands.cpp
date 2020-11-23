// Copyright Epic Games, Inc. All Rights Reserved.

#include "JiraPluginCommands.h"

#define LOCTEXT_NAMESPACE "FJiraPluginModule"

void FJiraPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "JiraPlugin", "Bring up JiraPlugin window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
