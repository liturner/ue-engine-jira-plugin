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
	if (!AJiraConnection::CanAuthenticate(JiraConnectionWeakPtr.Get()))
	{
		FJiraProject Project;
		FJiraError ErrorDetails;
		ErrorDetails.ResponseCode = 499;
		ErrorDetails.ErrorBrief = ErrorMap.FindRef(ErrorDetails.ResponseCode);
		OnFailure.Broadcast(ErrorDetails, Project);
		return;
	}

	FHttpRequestRef NewRequest = JiraConnectionWeakPtr->CreateRequest();
	NewRequest->SetVerb("GET");
	NewRequest->SetURL("/rest/api/3/project/" + ProjectIdOrKey);
	NewRequest->OnProcessRequestComplete().BindUObject(this, &UGetProjectAsync::OnResponseReceived);
	if (!JiraConnectionWeakPtr->ProcessRequest(NewRequest))
	{
		NewRequest->CancelRequest();
	}
}

void UGetProjectAsync::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) 
{
	FJiraProject Project;
	FJiraError ErrorDetails;

	if (bWasSuccessful)
	{
		int32 ResponseCode = Response->GetResponseCode();

		if (ResponseCode == 200) {
			FString ResponseText = Response->GetContentAsString();
			if (!Project.FromJson(ResponseText))
			{
				ErrorDetails.ResponseCode = 498;
				ErrorDetails.ErrorBrief = ErrorMap.FindRef(ErrorDetails.ResponseCode);

				OnFailure.Broadcast(ErrorDetails, Project);
			}
			else
			{
				OnSuccess.Broadcast(ErrorDetails, Project);
			}
		}
		else 
		{
			ErrorDetails.ResponseCode = ResponseCode;
			ErrorDetails.ErrorBrief = ErrorMap.FindRef(ErrorDetails.ResponseCode);

			OnFailure.Broadcast(ErrorDetails, Project);
		}
	}
	else
	{
		ErrorDetails.ResponseCode = 599;
		ErrorDetails.ErrorBrief = ErrorMap.FindRef(ErrorDetails.ResponseCode);

		OnFailure.Broadcast(ErrorDetails, Project);
	}
}