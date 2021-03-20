// Copyright TD Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/CoreOnline.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "JiraConnection.h"
#include "JiraTypes.h"

#include "JiraAsyncFunctions.generated.h"


#define BEGIN_JIRA_ERROR_CODES \
private: \
	const TMap<int32, FString> SupportedErrorsInitialiser() \
	{ \
		TMap<int32, FString> SupportedErrorsMap; \
		SupportedErrorsMap.Add(200, TEXT("Success")); \
		SupportedErrorsMap.Add(400, TEXT("Something went wrong")); \
		SupportedErrorsMap.Add(498, TEXT("Cannot deserialise response body")); \
		SupportedErrorsMap.Add(499, TEXT("Cannot authenticate, JiraConnection cannot incomplete")); \
		SupportedErrorsMap.Add(503, TEXT("Service unavailable")); \
		SupportedErrorsMap.Add(599, TEXT("Request failed for an unknown reason"));
		
#define JIRA_ERROR_CODE(Code, Description) \
		SupportedErrorsMap.Add(Code, TEXT(Description));

#define END_JIRA_ERROR_CODES \
		return SupportedErrorsMap; \
	} \
	const TMap<int32, FString> ErrorMap = SupportedErrorsInitialiser();


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetProjectResult, FJiraError, Error, FJiraProject, JiraProject);

UCLASS(MinimalAPI)
class UGetProjectAsync : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	BEGIN_JIRA_ERROR_CODES
		JIRA_ERROR_CODE(401, "The authentication credentials are incorrect or missing");
		JIRA_ERROR_CODE(404, "The project is not found or the user does not have permission to view it");
	END_JIRA_ERROR_CODES

public:
	UGetProjectAsync(const FObjectInitializer& ObjectInitializer);

	// Called when the operation is successfull
	UPROPERTY(BlueprintAssignable)
		FGetProjectResult OnSuccess;

	// Called when Jira responds with anything other than "OK"
	UPROPERTY(BlueprintAssignable)
		FGetProjectResult OnFailure;

	// Returns the project details for a project.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Jira")
		static UGetProjectAsync* GetProjectAsync(const FString ProjectIdOrKey, UPARAM(ref)AJiraConnection* JiraConnection);

	virtual void Activate() override;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	TWeakObjectPtr<AJiraConnection> JiraConnectionWeakPtr;

	FString ProjectIdOrKey;
};
