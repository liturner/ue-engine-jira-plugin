// Copyright TD Technologies. All Rights Reserved.

#include "JiraAsyncFunctions.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"
#include "Math/NumericLimits.h"

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
	if (!JiraConnectionWeakPtr->ProcessRequest(NewRequest))
	{
		NewRequest->CancelRequest();
		return;
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
		// Generic "Something went wrong" code
		ErrorDetails.ResponseCode = 599;

		// If a better code was stored at a previous step, use it
		FString AbortCode = Request->GetHeader("AbortCode");
		if (!AbortCode.IsEmpty())
		{
			ErrorDetails.ResponseCode = FCString::Atoi(*AbortCode);
		}

		ErrorDetails.ErrorBrief = ErrorMap.FindRef(ErrorDetails.ResponseCode);

		OnFailure.Broadcast(ErrorDetails, Project);
	}
}