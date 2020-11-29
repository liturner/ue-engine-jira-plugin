// Copyright Epic Games, Inc. All Rights Reserved.

#include "Jira/GetProject.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"

UGetProjectAsync::UGetProjectAsync(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGetProjectAsync* UGetProjectAsync::GetProjectAsync(const FString projectIdOrKey, const AJiraConnection* JiraConnection)
{
	UGetProjectAsync* Proxy = NewObject<UGetProjectAsync>();

	return Proxy;
}

void UGetProjectAsync::Activate()
{

}
