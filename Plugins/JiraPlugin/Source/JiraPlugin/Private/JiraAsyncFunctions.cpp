// Copyright TD Technologies. All Rights Reserved.

#include "JiraAsyncFunctions.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"

UGetProjectAsync::UGetProjectAsync(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UGetProjectAsync* UGetProjectAsync::GetProjectAsync(const FString ProjectIdOrKey, UPARAM(ref)AJiraConnection* JiraConnection)
{
	UGetProjectAsync* Proxy = NewObject<UGetProjectAsync>();
	Proxy->JiraConnectionWeakPtr = JiraConnection;
	Proxy->ProjectIdOrKey = ProjectIdOrKey;
	return Proxy;
}

void UGetProjectAsync::Activate()
{
	FHttpRequestRef NewRequest = JiraConnectionWeakPtr->CreateRequest();
	NewRequest->SetVerb("GET");
	NewRequest->SetURL("/rest/api/3/project/" + ProjectIdOrKey);
	NewRequest->OnProcessRequestComplete().BindUObject(this, &UGetProjectAsync::OnResponseReceived);
	JiraConnectionWeakPtr->ProcessRequest(NewRequest);
}

void UGetProjectAsync::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
{
	FJiraProject Project;
	FJiraError ErrorDetails;

	if (bWasSuccessful)
	{
		FString ResponseText = Response->GetContentAsString();

		
		if (!Project.FromJson(ResponseText))
		{
			// error
		}

		OnSuccess.Broadcast(ErrorDetails, Project);
	}
	else
	{
		OnFailure.Broadcast(ErrorDetails, Project);
	}
}