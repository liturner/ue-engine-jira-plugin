// Copyright Epic Games, Inc. All Rights Reserved.

#include "JiraPluginCommands.h"

#define LOCTEXT_NAMESPACE "FJiraPluginModule"

void FJiraPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Jira", "Bring up Jira window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
